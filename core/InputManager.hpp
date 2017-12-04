#pragma once
#include <vector>
#include <core/Defines.hpp>

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

private:
	glm::vec2i m_mousePos;
	bool m_isMouseDown[4] { false };
};

}