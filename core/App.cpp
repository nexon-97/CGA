#include "App.hpp"
#include "InputManager.hpp"
#include "Brezenheim.hpp"
#include <iostream>

namespace cga
{

App* App::s_app = nullptr;

App* App::GetInstance()
{
	return s_app;
}

App::App(const char* wndTitle, bool antialiasing)
	: m_wndTitle(wndTitle)
{
	s_app = this;
}

App::~App()
{
	if (m_sdlInitialized)
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_sdlWindow);
		SDL_Quit();
	}

	if (!!m_zbufferTex)
	{
		SDL_DestroyTexture(m_zbufferTex);
		delete[] m_zbufferContent;
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
	if (SDL_CreateWindowAndRenderer(1300, 600, SDL_WINDOW_SHOWN | SDL_RENDERER_ACCELERATED, &m_sdlWindow, &m_renderer) < 0)
		return false;
	
	m_renderQueue = std::make_unique<cga::RenderQueue>(m_sdlWindow, m_renderer);

	return true;
}

void App::EnableZBuffer()
{
	glm::vec2i size;
	SDL_GetWindowSize(m_sdlWindow, &size.x, &size.y);

	m_zbufferTex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
	if (!m_zbufferTex)
	{
		SDL_ShowSimpleMessageBox(0, "Error", "Can't create zbuffer texture!", m_sdlWindow);
	}

	m_zbufferContent = new float[size.x * size.y];
}

void App::ClearZBuffer()
{
	if (!!m_zbufferTex)
	{
		SDL_SetRenderTarget(m_renderer, m_zbufferTex);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
		SDL_RenderClear(m_renderer);
		SDL_SetRenderTarget(m_renderer, NULL);

		glm::vec2i size;
		SDL_GetWindowSize(m_sdlWindow, &size.x, &size.y);
		for (int i = 0, sz = size.x * size.y; i < sz; i++)
		{
			m_zbufferContent[i] = 1.f;
		}
	}	
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
			case SDL_MOUSEBUTTONDOWN:
				InputManager::GetInstance().SetMouseDown(e.button.button, true);
				break;
			case SDL_MOUSEBUTTONUP:
				InputManager::GetInstance().SetMouseDown(e.button.button, false);
				break;
			case SDL_MOUSEMOTION:
				InputManager::GetInstance().SetMousePosition(glm::vec2i(e.motion.x, e.motion.y));
				break;
			case SDL_KEYDOWN:
				InputManager::GetInstance().SetKeyState(e.key.keysym.scancode, true);
				break;
			case SDL_KEYUP:
				InputManager::GetInstance().SetKeyState(e.key.keysym.scancode, false);
				break;
			}
		}

		SDL_SetRenderTarget(m_renderer, NULL);
		SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
		SDL_RenderClear(m_renderer);
		ClearZBuffer();

		Brezenheim::ResetDashFactor();

		m_renderQueue->Render();

		SDL_RenderPresent(m_renderer);
		SDL_Delay(25);
	}
}

}
