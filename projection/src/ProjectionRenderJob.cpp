#include "ProjectionRenderJob.hpp"
#include <core/Brezenheim.hpp>
#include <core/Utils.hpp>
#include <core/InputManager.hpp>
#include <Windows.h>
#include <core/App.hpp>
#include <glm/gtc/matrix_transform.hpp>

ProjectionRenderJob::ProjectionRenderJob(const glm::vec2i& center)
{
	// Construct geometry
	int n = 4;
	float l = 100.f;
	float h = l * 0.25f;
	m_worldUp = glm::vec3f(0.f, 0.f, 1.f);

	int k = 0;
	float heightTotal = h * n;

	// Construct ground
	float groundHeight = -heightTotal * 0.5f - h / 2.f;
	glm::vec3f planePoints[] =
	{
		glm::vec3f(-100.f, -100.f, groundHeight),
		glm::vec3f(-100.f, +100.f, groundHeight),
		glm::vec3f(+100.f, +100.f, groundHeight),
		glm::vec3f(+100.f, -100.f, groundHeight),
	};

	int indices[] =
	{
		2, 1, 0,
		0, 3, 2,
	};

	m_groundVb.LoadPoints(planePoints, sizeof(planePoints) / sizeof(glm::vec3f));
	m_groundVb.LoadIndices(indices, sizeof(indices) / sizeof(int));

	// Construct task geometry
	cga::math::Matrix transform;
	transform.SetIdentity();
	transform.SetTranslation(glm::vec3f(0.f, 0.f, -heightTotal * 0.5f));

	for (int i = 0; i < n; i++)
	{
		glm::vec3f points[] =
		{
			glm::vec3f(-l / 2.f, -h / 2.f, -h / 2.f),
			glm::vec3f(-l / 2.f, +h / 2.f, -h / 2.f),
			glm::vec3f(+l / 2.f, +h / 2.f, -h / 2.f),
			glm::vec3f(+l / 2.f, -h / 2.f, -h / 2.f),
			glm::vec3f(-l / 2.f, -h / 2.f, +h / 2.f),
			glm::vec3f(-l / 2.f, +h / 2.f, +h / 2.f),
			glm::vec3f(+l / 2.f, +h / 2.f, +h / 2.f),
			glm::vec3f(+l / 2.f, -h / 2.f, +h / 2.f),
		};

		for (int j = 0; j < 8; j++)
		{
			points[j] = cga::Utils::TransformPoint(points[j], transform, false);
		}

		int indices[] =
		{
			k + 2, k + 1, k + 0,
			k + 0, k + 3, k + 2,
			k + 0, k + 4, k + 7,
			k + 7, k + 3, k + 0,
			k + 1, k + 4, k + 0,
			k + 1, k + 5, k + 4,
			k + 1, k + 6, k + 5,
			k + 1, k + 2, k + 6,
			k + 3, k + 7, k + 6,
			k + 3, k + 6, k + 2,
			k + 4, k + 5, k + 6,
			k + 4, k + 6, k + 7,
		};

		m_vb.LoadPoints(points, sizeof(points) / sizeof(glm::vec3f));
		m_vb.LoadIndices(indices, sizeof(indices) / sizeof(int));
		k += 8;

		auto rotation = cga::Utils::QuatFromAxisAngle(m_worldUp, glm::pi<float>() * 0.5f);
		glm::vec3f offset = cga::Utils::Rotate(rotation, glm::vec3f(-1.5f * h, -1.5f * h, h));
		auto newTransform = cga::Utils::ConstructTransform(offset, rotation, glm::vec3f(1.f));
		transform = transform * newTransform;
	}

	// Define fixed axis (given by task)
	m_fixedAxisStart = m_vb.GetPoints()[5];
	m_fixedAxisEnd = m_vb.GetPoints()[8 * (n - 1) + 5];

	// Create projection matrix
	auto wnd = cga::App::GetInstance()->GetWindow();
	glm::vec2i size;
	SDL_GetWindowSize(wnd, &size.x, &size.y);

	float aspect = float(size.x) / float(size.y);
	float fov = glm::radians(60.f);
	m_projection = cga::Utils::ConstructPerspectiveProjection(fov, aspect, 0.5f, 200.f);

	glm::vec3f testPoint(-5.f, 5.f, -25.f);
	auto testPoint2 = cga::Utils::TransformPoint(testPoint, m_projection);

	// Create fixed camera matrix
	m_cameraOffset = 125.f;
	m_cameraAngle = glm::radians(60.f);
	const glm::vec3f cameraTargetOffset(m_cameraOffset * glm::tan(m_cameraAngle), m_cameraOffset * glm::tan(m_cameraAngle), m_cameraOffset);
	m_cameraFirst = cga::Utils::ConstructLookAtMatrix(cameraTargetOffset, glm::vec3f(0.f), m_worldUp);
}

void ProjectionRenderJob::UpdateInput()
{
	const glm::vec3f right = glm::vec3f(1.f, 0.f, 0.f);
	const glm::vec3f up = glm::vec3f(0.f, 0.f, 1.f);

	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_LEFT))
	{
		m_fixedAxisRotationAngle -= 0.05f;
	}
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_RIGHT))
	{
		m_fixedAxisRotationAngle += 0.05f;
	}
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_Q))
	{
		m_worldScale = glm::clamp(m_worldScale - 0.05f, 0.001f, 100.f);
	}
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_E))
	{
		m_worldScale = glm::clamp(m_worldScale + 0.05f, 0.001f, 100.f);
	}
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_A))
	{
		m_cameraYaw -= 0.05f;
	}
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_D))
	{
		m_cameraYaw += 0.05f;
	}
	
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_1))
	{
		m_renderMode = RenderMode::Wireframe;
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_2))
	{
		m_renderMode = RenderMode::Solid;
	}
	else if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_5))
	{
		m_cameraDynamic = !m_cameraDynamic;
	}
}

void ProjectionRenderJob::Render(SDL_Window* wnd, SDL_Renderer* renderer)
{
	UpdateInput();

	glm::vec2i size;
	SDL_GetWindowSize(wnd, &size.x, &size.y);

	if (m_cameraDynamic)
	{
		glm::vec3f cameraTargetOffset(m_cameraOffset * glm::tan(m_cameraAngle), m_cameraOffset * glm::tan(m_cameraAngle), m_cameraOffset);
		cameraTargetOffset.x *= glm::cos(m_cameraYaw);
		cameraTargetOffset.y *= glm::sin(m_cameraYaw);
		m_cameraMatrix = cga::Utils::ConstructLookAtMatrix(cameraTargetOffset, glm::vec3f(0.f), m_worldUp);
	}

	auto fixedAxisRotation = cga::Utils::ConstructAxisAngleRotation(m_fixedAxisStart, m_fixedAxisEnd, m_fixedAxisRotationAngle);

	auto worldTransform = cga::Utils::ConstructTransform(glm::vec3f(-0.5f, 0.f, 0.f), glm::quat(1.f, 0.f, 0.f, 0.f), glm::vec3f(m_worldScale));
	auto viewProjection = (m_cameraDynamic ? m_cameraMatrix : m_cameraFirst) * m_projection;
	auto mvpMatrix = (fixedAxisRotation * worldTransform) * viewProjection;

	if (m_renderMode == RenderMode::Wireframe)
	{
		auto groundMvp = worldTransform * viewProjection;
		cga::Brezenheim::DrawVertexBuffer(renderer, &m_groundVb, groundMvp, false);

		cga::Brezenheim::DrawVertexBuffer(renderer, &m_vb, mvpMatrix, false);
	}
	else if (m_renderMode == RenderMode::Solid)
	{
		auto groundMvp = worldTransform * viewProjection;
		cga::Brezenheim::DrawVertexBuffer(renderer, &m_groundVb, groundMvp, true);

		cga::Brezenheim::DrawVertexBuffer(renderer, &m_vb, mvpMatrix, true);
	}

	// Draw fixed axis
	auto fixedAxisStartProjected = cga::Utils::ProjectPoint(cga::Utils::TransformPoint(m_fixedAxisStart, mvpMatrix), size);
	auto fixedAxisEndProjected = cga::Utils::ProjectPoint(cga::Utils::TransformPoint(m_fixedAxisEnd, mvpMatrix), size);

	cga::Brezenheim::DrawLine(renderer, fixedAxisStartProjected, fixedAxisEndProjected, glm::vec3i(0, 255, 0), true, false, nullptr);
	cga::Brezenheim::DrawFilledQuad(renderer, fixedAxisStartProjected, 5, glm::vec3i(0, 255, 0));
	cga::Brezenheim::DrawFilledQuad(renderer, fixedAxisEndProjected, 5, glm::vec3i(0, 255, 0));
}
