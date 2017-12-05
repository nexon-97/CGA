#include "Utils.hpp"

namespace cga
{

glm::mat3f Utils::ConstructTransform(const Pose& pose)
{
	return ConstructTransform(pose.position, pose.rotation);
}

glm::mat3f Utils::ConstructTransform(const glm::vec2i& position, float rotation)
{
	glm::mat3f transform;
	float sin = glm::sin(rotation);
	float cos = glm::cos(rotation);

	transform[0].x = cos;
	transform[0].y = -sin;
	transform[0].z = float(position.x);
	transform[1].x = sin;
	transform[1].y = cos;
	transform[1].z = float(position.y);
	transform[2].x = 0.f;
	transform[2].y = 0.f;
	transform[2].z = 1.f;

	return transform;
}

glm::vec2f Utils::TransformPoint(const glm::vec2f& point, const glm::mat3f& transform)
{
	glm::vec3f transformed = glm::vec3f(point, 1.f) * transform;
	return glm::vec2f(transformed.x, transformed.y);
}

glm::recti Utils::ComputeBoundingBox(const std::vector<glm::vec2i>& shape)
{
	glm::recti result = glm::recti(glm::vec2i(1e+6f, 1e+6f), glm::vec2i(-1e+6f, -1e+6f));

	for (const auto& pt : shape)
	{
		if (result.topLeft.x > pt.x) result.topLeft.x = pt.x;
		if (result.topLeft.y > pt.y) result.topLeft.y = pt.y;
		if (result.bottomRight.x < pt.x) result.bottomRight.x = pt.x;
		if (result.bottomRight.y < pt.y) result.bottomRight.y = pt.y;
	}

	return result;
}

bool Utils::BoundingBoxOverlap(const glm::recti& a, const glm::recti& b)
{
	bool xOverlap = ((a.topLeft.x >= b.topLeft.x) && (a.topLeft.x <= b.bottomRight.x)) ||
					((b.topLeft.x >= a.topLeft.x) && (b.topLeft.x <= a.bottomRight.x));
	bool yOverlap = ((a.topLeft.y >= b.topLeft.y) && (a.topLeft.y <= b.bottomRight.y)) ||
					((b.topLeft.y >= a.topLeft.y) && (b.topLeft.y <= a.bottomRight.y));

	return xOverlap && yOverlap;
}

glm::vec3f Utils::TransformPoint(const glm::vec3f& point, const glm::mat4f& transform)
{
	return glm::vec4f(point, 1.f) * transform;
}

glm::mat4f Utils::ConstructTransform(const glm::vec3f& position, const glm::quat& rotation, const glm::vec3f& scale)
{
	auto translationM = ConstructTranslationMatrix(position);
	auto rotationM = ConstructRotationMatrix(rotation);
	auto scaleM = ConstructScaleMatrix(scale);

	return scaleM * rotationM * translationM;
}

glm::mat4f Utils::ConstructTranslationMatrix(const glm::vec3f& position)
{
	glm::mat4f result(0.f);

	result[0][0] = 1.f;
	result[1][1] = 1.f;
	result[2][2] = 1.f;
	result[3][3] = 1.f;
	result[3][0] = position.x;
	result[3][1] = position.y;
	result[3][2] = position.z;

	return result;
}

glm::mat4f Utils::ConstructRotationMatrix(const glm::quat& rotation)
{
	glm::mat4f result(0.f);

	float sqX = rotation.x * rotation.x;
	float sqY = rotation.y * rotation.y;
	float sqZ = rotation.z * rotation.z;

	result[0][0] = 1.f - 2.f * (sqY - sqZ);
	result[0][1] = 2.f * (rotation.x * rotation.y + rotation.w * rotation.z);
	result[0][2] = 2.f * (rotation.x * rotation.z - rotation.w * rotation.y);
	result[1][0] = 2.f * (rotation.x * rotation.y - rotation.w * rotation.z);
	result[1][1] = 1.f - 2.f * (sqX - sqZ);
	result[1][2] = 2.f * (rotation.y * rotation.z + rotation.w * rotation.x);
	result[2][0] = 2.f * (rotation.x * rotation.z + rotation.w * rotation.y);
	result[2][1] = 2.f * (rotation.y * rotation.z - rotation.w * rotation.x);
	result[2][2] = 1.f - 2.f * (sqX - sqY);
	result[3][3] = 1.f;

	return result;
}

glm::mat4f Utils::ConstructScaleMatrix(const glm::vec3f& scale)
{
	glm::mat4f result(0.f);

	result[0][0] = scale.x;
	result[1][1] = scale.y;
	result[2][2] = scale.z;
	result[3][3] = 1.f;

	return result;
}

glm::mat4f Utils::ConstructOrthoProjection(float width, float height, float zNear, float zFar)
{
	glm::mat4f m(0.f);

	m[0] = glm::vec4f(2.f / width, 0.f, 0.f, 0.f);
	m[1] = glm::vec4f(0.f, 2.f / height, 0.f, 0.f);
	m[2] = glm::vec4f(0.f, 0.f, -2.f / (zFar - zNear), 0.f);
	m[3] = glm::vec4f(0.f, 0.f, -(zFar + zNear) / (zFar - zNear), 1.f);

	return m;
}

glm::quat Utils::QuatFromAxisAngle(const glm::vec3f& axis, float angle)
{
	float halfAngle = angle * 0.5f;
	float sin = glm::sin(halfAngle);
	float cos = glm::cos(halfAngle);
	
	return glm::quat(axis.x * sin, axis.y * sin, axis.z * sin, cos);
}

glm::mat4f Utils::ConstructLookAtMatrix(const glm::vec3f& eye, const glm::vec3f& target, const glm::vec3f& up)
{
	glm::vec3f zaxis = glm::normalize(eye - target);
	glm::vec3f xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3f yaxis = glm::cross(zaxis, xaxis);

	glm::mat4f viewMatrix = {
		glm::vec4f(xaxis.x,            yaxis.x,            zaxis.x,       0),
		glm::vec4f(xaxis.y,            yaxis.y,            zaxis.y,       0),
		glm::vec4f(xaxis.z,            yaxis.z,            zaxis.z,       0),
		glm::vec4f(-glm::dot(xaxis, eye), -glm::dot(yaxis, eye), -glm::dot(zaxis, eye),  1)
	};

	return viewMatrix;
}

glm::quat Utils::QuatFromMatrix(const glm::mat3f& mat)
{
	glm::quat result;

	float trace = mat[0][0] + mat[1][1] + mat[2][2];
	if (trace > 0)
	{
		float fRoot = glm::sqrt(trace + 1);
		result.w = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		result.x = (mat[2][1] - mat[1][2])*fRoot;
		result.y = (mat[0][2] - mat[2][0])*fRoot;
		result.z = (mat[1][0] - mat[0][1])*fRoot;
	}
	else
	{
		int i = 0;
		if (mat[1][1] > mat[0][0])
			i = 1;
		if (mat[2][2] > mat[i][i])
			i = 2;
		int j = (i + 1) % 3;
		int k = (j + 1) % 3;

		float fRoot = glm::sqrt(mat[i][i] - mat[j][j] - mat[k][k] + 1);
		float* apfQuat[3] = { &result.x, &result.y, &result.z };
		*apfQuat[i] = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		result.w = (mat[k][j] - mat[j][k]) * fRoot;
		*apfQuat[j] = (mat[j][i] + mat[i][j]) * fRoot;
		*apfQuat[k] = (mat[k][i] + mat[i][k]) * fRoot;
	}

	return result;
}

}
