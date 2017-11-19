#pragma once

namespace sf
{
class RenderWindow;
}

namespace cga
{

class RenderJob
{
public:
	virtual void Render(sf::RenderWindow* wnd) = 0;
};

}
