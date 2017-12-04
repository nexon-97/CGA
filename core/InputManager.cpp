#include "InputManager.hpp"

namespace cga
{

InputManager& InputManager::GetInstance()
{
	static InputManager inputManager;
	return inputManager;
}

bool InputManager::IsMouseDown(int idx) const
{
	return m_isMouseDown[idx];
}

void InputManager::SetMouseDown(int idx, bool down)
{
	m_isMouseDown[idx] = down;
}

/*void InputManager::SetMousePosition(const sf::Vector2f& position)
{
	m_mousePos = position;
}

const sf::Vector2f& InputManager::GetMousePosition() const
{
	return m_mousePos;
}*/

}
