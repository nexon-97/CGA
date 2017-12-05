#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

namespace glm
{

using vec2i = vec<2, int>;
using vec3i = vec<3, int>;
using vec4i = vec<4, int>;
using vec2f = vec<2, float>;
using vec3f = vec<3, float>;
using vec4f = vec<4, float>;

template <typename T>
struct rect
{
	vec<2, T> topLeft;
	vec<2, T> bottomRight;

	rect() = default;
	rect(const vec<2, T>& topLeft, const vec<2, T>& bottomRight)
		: topLeft(topLeft)
		, bottomRight(bottomRight)
	{}

	bool operator==(const rect& b) const
	{
		return topLeft == b.topLeft && bottomRight == b.bottomRight;
	}

	bool operator !=(const rect& b) const
	{
		return topLeft != b.topLeft || bottomRight != b.bottomRight;
	}
};

using recti = rect<int>;
using rectf = rect<float>;
using linef = rectf;
using linei = recti;

using mat3f = glm::tmat3x3<float>;
using mat4f = glm::tmat4x4<float>;

}
