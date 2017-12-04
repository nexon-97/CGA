#pragma once

struct SDL_Window;
struct SDL_Renderer;

namespace cga
{

class RenderJob
{
public:
	virtual void Render(SDL_Window* wnd, SDL_Renderer* renderer) = 0;
};

}
