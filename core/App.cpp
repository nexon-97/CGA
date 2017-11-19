#include "App.hpp"

namespace cga
{

App::App(const char* wndTitle)
{
	int wndStyle = sf::Style::Titlebar | sf::Style::Close;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	m_window.reset(new sf::RenderWindow(sf::VideoMode(1300, 600), wndTitle, wndStyle, settings));
	m_renderQueue.reset(new cga::RenderQueue(m_window.get()));
}

App::~App()
{

}

sf::RenderWindow* App::GetWindow() const
{
	return m_window.get();
}

cga::RenderQueue* App::GetRenderQueue() const
{
	return m_renderQueue.get();
}

void App::Run()
{
	while (m_window->isOpen())
	{
		sf::Event event;
		while (m_window->pollEvent(event))
        {
			switch (event.type)
			{
				case sf::Event::Closed:
					m_window->close();
					break;
			}

			m_window->clear(sf::Color::White);
			m_renderQueue->Render();
			m_window->display();
		}
	}
}

}