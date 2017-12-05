#pragma once
#include "RenderQueue.hpp"
#include <memory>
#include <SDL.h>

namespace cga
{

class App
{
public:
	static App* GetInstance();

	App(const char* wndTitle, bool antialiasing = true);
	~App();

	bool Init();
	void Run();

	SDL_Window* GetWindow() const;
	SDL_Renderer* GetRenderer() const;
	cga::RenderQueue* GetRenderQueue() const;

private:
	static App* s_app;

	std::unique_ptr<cga::RenderQueue> m_renderQueue;

	const char* m_wndTitle = nullptr;
	SDL_Window* m_sdlWindow = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	SDL_Surface* m_sdlScreenSurface = nullptr;
	bool m_sdlInitialized = false;
};

}
