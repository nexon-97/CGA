#include "ProjectionRenderJob.hpp"
#include <core/Brezenheim.hpp>
#include <core/Utils.hpp>
#include <core/InputManager.hpp>
#include <Windows.h>

ProjectionRenderJob::ProjectionRenderJob(const glm::vec2i& center)
{
	glm::vec3f points[] =
	{
		glm::vec3f(-1.f, -1.f, 0.f),
		glm::vec3f(-1.f, 1.f, 0.f),
		glm::vec3f(1.f, 1.f, 0.f),
		glm::vec3f(1.f, -1.f, 0.f),
		glm::vec3f(-1.f, -1.f, 1.f),
		glm::vec3f(-1.f, 1.f, 1.f),
		glm::vec3f(1.f, 1.f, 1.f),
		glm::vec3f(1.f, -1.f, 1.f),
	};

	int indices[] =
	{
		0, 1, 2,
		2, 3, 0,
		0, 4, 7,
		7, 3, 0,
		1, 4, 0,
		1, 5, 4,
		1, 6, 5,
		1, 2, 6,
		3, 7, 6,
		3, 6, 2,
		4, 5, 6,
		4, 6, 7,
	};

	m_vb.LoadPoints(points, sizeof(points) / sizeof(glm::vec3f));
	m_vb.LoadIndices(indices, sizeof(indices) / sizeof(int));

	m_cameraPosition = glm::vec3f(0.f, -5.f, 3.f);
	m_cameraTarget = glm::vec3f(0.f, 0.f, 0.f);
	m_worldUp = glm::vec3f(0.f, 0.f, 1.f);
	UpdateCameraState();

	m_projection = cga::Utils::ConstructOrthoProjection(10.f, 10.f, 0.1f, 10.f);
}

void ProjectionRenderJob::UpdateCameraState()
{
	m_cameraMatrix = cga::Utils::ConstructLookAtMatrix(m_cameraPosition, m_cameraTarget, m_worldUp);

	//std::string output = "Camera position: " + std::to_string(m_cameraPosition.x) + "; " + std::to_string(m_cameraPosition.y) + "; " + std::to_string(m_cameraPosition.z) + "\n";
	//OutputDebugStringA(output.c_str());
}

void ProjectionRenderJob::UpdateInput()
{
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_LEFT))
	{
		m_cameraPosition -= glm::vec3f(0.1f, 0.f, 0.f);
		UpdateCameraState();
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_RIGHT))
	{
		m_cameraPosition += glm::vec3f(0.1f, 0.f, 0.f);
		UpdateCameraState();
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_UP))
	{
		m_cameraPosition += glm::vec3f(0.f, 0.f, 0.1f);
		UpdateCameraState();
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_DOWN))
	{
		m_cameraPosition -= glm::vec3f(0.f, 0.f, 0.1f);
		UpdateCameraState();
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_W))
	{
		m_cameraPosition += glm::vec3f(0.f, 0.1f, 0.f);
		UpdateCameraState();
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_S))
	{
		m_cameraPosition -= glm::vec3f(0.f, 0.1f, 0.f);
		UpdateCameraState();
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_DOWN))
	{
		m_cameraPosition -= glm::vec3f(0.f, 0.f, 0.1f);
		UpdateCameraState();
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_Q))
	{
		m_worldScale -= 0.1f;
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_E))
	{
		m_worldScale += 0.1f;
	}
}

void ProjectionRenderJob::Render(SDL_Window* wnd, SDL_Renderer* renderer)
{
	UpdateInput();

	glm::vec3f pos(0.f, 0.f, 0.f);
	glm::quat rot(1.f, 0.f, 0.f, 0.f);

	auto worldTransform = cga::Utils::ConstructTransform(pos, rot, glm::vec3f(m_worldScale));

	auto mvpMatrix = worldTransform * m_cameraMatrix * m_projection;
	mvpMatrix = glm::transpose(mvpMatrix);
	cga::Brezenheim::DrawVertexBuffer(renderer, &m_vb, mvpMatrix);
}
