#include "ProjectionRenderJob.hpp"
#include <core/Brezenheim.hpp>
#include <core/Utils.hpp>
#include <core/InputManager.hpp>
#include <Windows.h>
#include <core/App.hpp>

ProjectionRenderJob::ProjectionRenderJob(const glm::vec2i& center)
{
	// Construct geometry
	int n = 6;
	float l = 1.f;
	float h = l * 0.25f;
	const glm::vec3f up = glm::vec3f(0.f, 0.f, 1.f);

	int k = 0;
	float heightTotal = h * n;

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

		auto transformTransposed = transform.Transpose();
		for (int j = 0; j < 8; j++)
		{
			points[j] = cga::Utils::TransformPoint(points[j], transformTransposed);
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

		auto rotation = cga::Utils::QuatFromAxisAngle(up, glm::pi<float>() * 0.5f);
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
	m_projection = cga::Utils::ConstructPerspectiveProjection(fov, aspect, 0.25f, 100.f);

	// Create fixed camera matrix
	float offsetScalar = 100.f;
	float projectionAngle = 30.f;
	const glm::vec3f cameraTargetOffset(offsetScalar * glm::tan(glm::radians(projectionAngle)), -offsetScalar, -offsetScalar * glm::tan(glm::radians(projectionAngle)));
	m_cameraFirst = cga::Utils::ConstructLookAtMatrix(cameraTargetOffset, glm::vec3f(0.f), up);
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
		m_worldScale = glm::clamp(m_worldScale - 0.01f, 0.001f, 100.f);
	}
	if (cga::InputManager::GetInstance().GetKeyState(SDL_SCANCODE_E))
	{
		m_worldScale = glm::clamp(m_worldScale + 0.01f, 0.001f, 100.f);
	}
}

void ProjectionRenderJob::Render(SDL_Window* wnd, SDL_Renderer* renderer)
{
	UpdateInput();

	auto fixedAxisRotation = cga::Utils::ConstructAxisAngleRotation(m_fixedAxisStart, m_fixedAxisEnd, m_fixedAxisRotationAngle);

	auto worldTransform = cga::Utils::ConstructTransform(glm::vec3f(0.f), glm::quat(), glm::vec3f(m_worldScale));
	auto mvpMatrix = fixedAxisRotation * worldTransform * m_cameraFirst * m_projection;

	cga::Brezenheim::DrawVertexBuffer(renderer, &m_vb, mvpMatrix);

	// Draw fixed axis
	auto transposed = mvpMatrix.Transpose();
	glm::vec2i size;
	SDL_GetWindowSize(wnd, &size.x, &size.y);
	auto fixedAxisStartProjected = cga::Utils::ProjectPoint(cga::Utils::TransformPoint(m_fixedAxisStart, transposed), size);
	auto fixedAxisEndProjected = cga::Utils::ProjectPoint(cga::Utils::TransformPoint(m_fixedAxisEnd, transposed), size);

	cga::Brezenheim::DrawLine(renderer, fixedAxisStartProjected, fixedAxisEndProjected, glm::vec3i(0, 255, 0), true);
	cga::Brezenheim::DrawQuad(renderer, fixedAxisStartProjected, 5, glm::vec3i(0, 255, 0));
	cga::Brezenheim::DrawQuad(renderer, fixedAxisEndProjected, 5, glm::vec3i(0, 255, 0));
}
