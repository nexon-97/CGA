#pragma once
#include "Defines.hpp"

namespace cga
{
	
struct Vertex
{
	glm::vec2i position;
	glm::vec3i color;

	Vertex() = default;
	Vertex(const glm::vec2i& position, const glm::vec3i& color)
		: position(position)
		, color(color)
	{}
};

}
