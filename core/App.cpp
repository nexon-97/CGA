#include "App.hpp"
#include "InputManager.hpp"
#include "Brezenheim.hpp"
#include <iostream>

namespace cga
{

App::App(const char* wndTitle, bool antialiasing)
	: m_wndTitle(wndTitle)
{
	
}

App::~App()
{
	if (m_sdlInitialized)
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_sdlWindow);
		SDL_Quit();
	}
}

SDL_Window* App::GetWindow() const
{
	return m_sdlWindow;
}

cga::RenderQueue* App::GetRenderQueue() const
{
	return m_renderQueue.get();
}

SDL_Renderer* App::GetRenderer() const
{
	return m_renderer;
}

bool App::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return false;

	m_sdlInitialized = true;
	if (SDL_CreateWindowAndRenderer(1300, 600, SDL_WINDOW_SHOWN, &m_sdlWindow, &m_renderer) < 0)
		return false;
	
	m_renderQueue = std::make_unique<cga::RenderQueue>(m_sdlWindow, m_renderer);
	m_sdlScreenSurface = SDL_GetWindowSurface(m_sdlWindow);

	return true;
}

void App::Run()
{
	bool quit = false;
	
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		SDL_FillRect(m_sdlScreenSurface, nullptr, SDL_MapRGB(m_sdlScreenSurface->format, 50, 50, 50));		
		Brezenheim::ResetDashFactor();

		m_renderQueue->Render();

		SDL_RenderPresent(m_renderer);
		SDL_Delay(25);
	}
}

}
