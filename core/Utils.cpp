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

}
