#pragma once
#include <vector>
#include <core/Defines.hpp>
#include <unordered_map>
#include <SDL.h>

namespace cga
{

class InputManager
{
public:
	static InputManager& GetInstance();

	bool IsMouseDown(int idx) const;
	void SetMouseDown(int idx, bool down);
	void SetMousePosition(const glm::vec2i& position);
	const glm::vec2i& GetMousePosition() const;
	void SetKeyState(SDL_Scancode scancode, bool state);
	bool GetKeyState(SDL_Scancode scancode) const;

private:
	glm::vec2i m_mousePos;
	std::unordered_map<SDL_Scancode, bool> m_buttonStates;
	bool m_isMouseDown[4] { false };
};

}