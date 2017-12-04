#pragma once
#include "Defines.hpp"

struct SDL_Renderer;

namespace cga
{

class Brezenheim
{
public:
	static void DrawLine(SDL_Renderer* renderer, glm::vec2i from, glm::vec2i to, const glm::vec3i& color, bool dashed);
	static void ResetDashFactor();

private:
	static int s_dashFactor;
};

}
