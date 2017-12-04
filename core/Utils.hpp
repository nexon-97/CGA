#pragma once
#include "Defines.hpp"
#include <vector>

namespace cga
{

struct Pose
{
	glm::vec2i position;
	float rotation;

	Pose()
		: rotation(0.f)
	{}

	Pose(const glm::vec2i& pos, float rot)
		: position(pos)
		, rotation(rot)
	{}
};

class Utils
{
public:
	static glm::mat3f ConstructTransform(const Pose& pose);
	static glm::mat3f ConstructTransform(const glm::vec2i& position, float rotation);
	static glm::vec2f TransformPoint(const glm::vec2f& point, const glm::mat3f& transform);
	static glm::recti ComputeBoundingBox(const std::vector<glm::vec2i>& shape);
	static bool BoundingBoxOverlap(const glm::recti& a, const glm::recti& b);
};

}
