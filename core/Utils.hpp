#pragma once
#include "Defines.hpp"
#include <vector>
#include <glm/ext.hpp>

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
	static glm::vec3f TransformPoint(const glm::vec3f& point, const glm::mat4f& transform);
	static glm::recti ComputeBoundingBox(const std::vector<glm::vec2i>& shape);
	static bool BoundingBoxOverlap(const glm::recti& a, const glm::recti& b);

	static glm::quat QuatFromAxisAngle(const glm::vec3f& axis, float angle);

	static glm::mat4f ConstructTransform(const glm::vec3f& position, const glm::quat& rotation, const glm::vec3f& scale);
	static glm::mat4f ConstructTranslationMatrix(const glm::vec3f& position);
	static glm::mat4f ConstructRotationMatrix(const glm::quat& rotation);
	static glm::mat4f ConstructScaleMatrix(const glm::vec3f& scale);

	static glm::mat4f ConstructOrthoProjection(float width, float height, float zNear, float zFar);
	static glm::mat4f ConstructLookAtMatrix(const glm::vec3f& eye, const glm::vec3f& target, const glm::vec3f& up);
	static glm::quat QuatFromMatrix(const glm::mat3f& mat);
};

}
