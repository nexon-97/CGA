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
	if (idx < 4)
		return m_isMouseDown[idx];

	return false;
}

void InputManager::SetMouseDown(int idx, bool down)
{
	if (idx < 4)
		m_isMouseDown[idx] = down;
}

void InputManager::SetMousePosition(const glm::vec2i& position)
{
	m_mousePos = position;
}

const glm::vec2i& InputManager::GetMousePosition() const
{
	return m_mousePos;
}

void InputManager::SetKeyState(SDL_Scancode scancode, bool state)
{
	m_buttonStates[scancode] = state;
}

bool InputManager::GetKeyState(SDL_Scancode scancode) const
{
	auto it = m_buttonStates.find(scancode);
	return it != m_buttonStates.end() ? it->second : false;
}

}
