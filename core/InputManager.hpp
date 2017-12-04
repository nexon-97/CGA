#pragma once
#include <vector>

namespace cga
{

class InputManager
{
public:
	static InputManager& GetInstance();

	bool IsMouseDown(int idx) const;
	void SetMouseDown(int idx, bool down);
	//void SetMousePosition(const sf::Vector2f& position);
	//const sf::Vector2f& GetMousePosition() const;

private:
	//sf::Vector2f m_mousePos;
	bool m_isMouseDown[2] { false };
};

}