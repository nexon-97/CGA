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

glm::vec3f Utils::TransformPoint(glm::vec3f& point, math::Matrix& transform, bool normalize)
{
	auto pt = glm::vec4f(point, 1.f);
	glm::vec4f transformed;

	transformed.x = pt.x * transform.m[0][0] + pt.y * transform.m[1][0] + pt.z * transform.m[2][0] + pt.w * transform.m[3][0];
	transformed.y = pt.x * transform.m[0][1] + pt.y * transform.m[1][1] + pt.z * transform.m[2][1] + pt.w * transform.m[3][1];
	transformed.z = pt.x * transform.m[0][2] + pt.y * transform.m[1][2] + pt.z * transform.m[2][2] + pt.w * transform.m[3][2];
	transformed.w = pt.x * transform.m[0][3] + pt.y * transform.m[1][3] + pt.z * transform.m[2][3] + pt.w * transform.m[3][3];

	if (normalize)
	{
		transformed.x /= transformed.w;
		transformed.y /= transformed.w;
		transformed.z /= transformed.w;
	}

	return glm::vec3f(transformed.x, transformed.y, transformed.z);
}

glm::vec2i Utils::ProjectPoint(const glm::vec3f& point, const glm::vec2i& windowSize)
{
	glm::vec2f t = (point + 1.f) / 2.f;
	auto projected = glm::vec2i(glm::vec2f(windowSize) * t);
	projected.y = windowSize.y - projected.y;
	return projected;
}

math::Matrix Utils::ConstructTransform(const glm::vec3f& position, const glm::quat& rotation, const glm::vec3f& scale)
{
	auto translationM = ConstructTranslationMatrix(position);
	auto rotationM = QuatToMatrix(rotation);
	auto scaleM = ConstructScaleMatrix(scale);

	return scaleM * rotationM * translationM;
}

math::Matrix Utils::ConstructTranslationMatrix(const glm::vec3f& position)
{
	math::Matrix result;
	result.SetTranslation(const_cast<glm::vec3f&>(position));
	return result;
}

math::Matrix Utils::ConstructRotationMatrix(const glm::quat& rotation)
{
	math::Matrix result(0.f);

	float sqX = rotation.x * rotation.x;
	float sqY = rotation.y * rotation.y;
	float sqZ = rotation.z * rotation.z;

	result.m[0][0] = 1.f - 2.f * (sqY - sqZ);
	result.m[0][1] = 2.f * (rotation.x * rotation.y + rotation.w * rotation.z);
	result.m[0][2] = 2.f * (rotation.x * rotation.z - rotation.w * rotation.y);
	result.m[1][0] = 2.f * (rotation.x * rotation.y - rotation.w * rotation.z);
	result.m[1][1] = 1.f - 2.f * (sqX - sqZ);
	result.m[1][2] = 2.f * (rotation.y * rotation.z + rotation.w * rotation.x);
	result.m[2][0] = 2.f * (rotation.x * rotation.z + rotation.w * rotation.y);
	result.m[2][1] = 2.f * (rotation.y * rotation.z - rotation.w * rotation.x);
	result.m[2][2] = 1.f - 2.f * (sqX - sqY);
	result.m[3][3] = 1.f;

	return result;
}

math::Matrix Utils::ConstructScaleMatrix(const glm::vec3f& scale)
{
	math::Matrix result;
	result.SetScale(const_cast<glm::vec3f&>(scale));
	return result;
}

math::Matrix Utils::ConstructOrthoProjection(float width, float height, float zNear, float zFar)
{
	math::Matrix result;

	result.m[0][0] = 2.f / width;
	result.m[0][1] = 0.f;
	result.m[0][2] = 0.f;
	result.m[0][3] = 0.f;
	result.m[1][0] = 0.f;
	result.m[1][1] = 2.f / height;
	result.m[1][2] = 0.f;
	result.m[1][3] = 0.f;
	result.m[2][0] = 0.f;
	result.m[2][1] = 0.f;
	result.m[2][2] = -2.f / (zFar - zNear);
	result.m[2][3] = 0.f;
	result.m[3][0] = 0.f;
	result.m[3][1] = 0.f;
	result.m[3][2] = -(zFar + zNear) / (zFar - zNear);
	result.m[3][3] = 1.f;

	return result;
}

math::Matrix Utils::ConstructPerspectiveProjection(float fovRadians, float aspectRatio, float zNear, float zFar)
{
	const float yFac = glm::tan(fovRadians / 2.f);
	const float xFac = yFac * aspectRatio;

	math::Matrix result;

	result.m[0][0] = 1.f / xFac;
	result.m[1][0] = 0.f;
	result.m[2][0] = 0.f;
	result.m[3][0] = 0.f;
	result.m[0][1] = 0.f;
	result.m[1][1] = 1.f / yFac;
	result.m[2][1] = 0.f;
	result.m[3][1] = 0.f;
	result.m[0][2] = 0.f;
	result.m[1][2] = 0.f;
	result.m[2][2] = -(zFar + zNear) / (zFar - zNear);
	result.m[3][2] = -(2.f * zFar * zNear) / (zFar - zNear);
	result.m[0][3] = 0.f;
	result.m[1][3] = 0.f;
	result.m[2][3] = -1.f;
	result.m[3][3] = 0.f;

	return result;
}

glm::quat Utils::QuatFromAxisAngle(const glm::vec3f& axis, float angle)
{
	const float fHalfAngle = 0.5f * angle;
	const float fSin = glm::sin(fHalfAngle);

	glm::quat result;
	result.x = fSin * axis.x;
	result.y = fSin * axis.y;
	result.z = fSin * axis.z;
	result.w = glm::cos(fHalfAngle);

	return result;
}

math::Matrix Utils::ConstructLookAtMatrix(const glm::vec3f& eye, const glm::vec3f& target, const glm::vec3f& up)
{
	glm::vec3f zaxis = glm::normalize(eye - target);
	glm::vec3f xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3f yaxis = glm::cross(zaxis, xaxis);

	math::Matrix viewMatrix;
	viewMatrix.m[0][0] = xaxis.x;
	viewMatrix.m[0][1] = yaxis.x;
	viewMatrix.m[0][2] = zaxis.x;
	viewMatrix.m[0][3] = 0.f;
	viewMatrix.m[1][0] = xaxis.y;
	viewMatrix.m[1][1] = yaxis.y;
	viewMatrix.m[1][2] = zaxis.y;
	viewMatrix.m[1][3] = 0.f;
	viewMatrix.m[2][0] = xaxis.z;
	viewMatrix.m[2][1] = yaxis.z;
	viewMatrix.m[2][2] = zaxis.z;
	viewMatrix.m[2][3] = 0.f;
	viewMatrix.m[3][0] = -glm::dot(xaxis, eye);
	viewMatrix.m[3][1] = -glm::dot(yaxis, eye);
	viewMatrix.m[3][2] = -glm::dot(zaxis, eye);
	viewMatrix.m[3][3] = 1.f;

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

math::Matrix Utils::QuatToMatrix(const glm::quat& quat)
{
	float x2 = quat.x * 2.f;
	float y2 = quat.y * 2.f;
	float z2 = quat.z * 2.f;

	const float _2xx = x2 * quat.x;
	const float _2yy = y2 * quat.y;
	const float _2zz = z2 * quat.z;
	const float _2xy = x2 * quat.y;
	const float _2xz = x2 * quat.z;
	const float _2xw = x2 * quat.w;
	const float _2yz = y2 * quat.z;
	const float _2yw = y2 * quat.w;
	const float _2zw = z2 * quat.w;

	math::Matrix result;
	result.m[0][0] = 1 - _2yy - _2zz;
	result.m[0][1] = _2xy - _2zw;
	result.m[0][2] = _2xz + _2yw;
	result.m[0][3] = 0.f;
	result.m[1][0] = _2xy + _2zw;
	result.m[1][1] = 1 - _2xx - _2zz;
	result.m[1][2] = _2yz - _2xw;
	result.m[1][3] = 0.f;
	result.m[2][0] = _2xz - _2yw;
	result.m[2][1] = _2yz + _2xw;
	result.m[2][2] = 1 - _2yy - _2xx;
	result.m[2][3] = 0.f;
	result.m[3][0] = 0.f;
	result.m[3][1] = 0.f;
	result.m[3][2] = 0.f;
	result.m[3][3] = 1.f;

	return result;
}

glm::vec3f Utils::Rotate(const glm::quat& quat, const glm::vec3f& v)
{
	return quat * v;
}

math::Matrix Utils::ConstructAxisAngleRotation(const glm::vec3f& start, const glm::vec3f& end, float angle)
{
	glm::vec3f dir = glm::normalize(end - start);
	auto rot = QuatFromAxisAngle(dir, angle);
	math::Matrix rotMatrix = QuatToMatrix(rot);

	math::Matrix translation;
	translation.SetTranslation(-start);
	math::Matrix translationInv;
	translationInv.SetTranslation(+start);

	return translation * rotMatrix * translationInv;
}

}
