#pragma once
#include "Defines.hpp"
#include <vector>
#include <glm/ext.hpp>
#include <core/Math.h>

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
	static glm::vec3f TransformPoint(glm::vec3f& point, math::Matrix& transform, bool normalize = true);
	static glm::vec2i ProjectPoint(const glm::vec3f& point, const glm::vec2i& windowSize);
	static glm::recti ComputeBoundingBox(const std::vector<glm::vec2i>& shape);
	static bool BoundingBoxOverlap(const glm::recti& a, const glm::recti& b);

	static glm::quat QuatFromAxisAngle(const glm::vec3f& axis, float angle);

	static math::Matrix ConstructTransform(const glm::vec3f& position, const glm::quat& rotation, const glm::vec3f& scale);
	static math::Matrix ConstructTranslationMatrix(const glm::vec3f& position);
	static math::Matrix ConstructRotationMatrix(const glm::quat& rotation);
	static math::Matrix ConstructScaleMatrix(const glm::vec3f& scale);

	static math::Matrix ConstructOrthoProjection(float width, float height, float zNear, float zFar);
	static math::Matrix ConstructPerspectiveProjection(float fovRadians, float aspectRatio, float zNear, float zFar);
	static math::Matrix ConstructLookAtMatrix(const glm::vec3f& eye, const glm::vec3f& target, const glm::vec3f& up);
	static math::Matrix ConstructAxisAngleRotation(const glm::vec3f& start, const glm::vec3f& end, float angle);
	static glm::quat QuatFromMatrix(const glm::mat3f& mat);
	static math::Matrix QuatToMatrix(const glm::quat& quat);
	static glm::vec3f Rotate(const glm::quat& quat, const glm::vec3f& v);
};

}
