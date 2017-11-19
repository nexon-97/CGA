#pragma once
#include "RenderQueue.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace cga
{

class App
{
public:
	App(const char* wndTitle, bool antialiasing = true);
	~App();

	void Run();

	sf::RenderWindow* GetWindow() const;
	cga::RenderQueue* GetRenderQueue() const;

private:
	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<cga::RenderQueue> m_renderQueue;
};

}
