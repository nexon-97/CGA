#include "ClippingRenderJob.hpp"
#include "core/InputManager.hpp"
#include "core/Brezenheim.hpp"
#include <SDL.h>
#include <algorithm>

ClippingRenderJob::ClippingRenderJob(const glm::vec2i& size)
	: m_starMovement(glm::recti(glm::vec2i(150, 150), size - glm::vec2i(150, 150)))
{
	m_backShape = 
	{
		glm::vec2i(50, 50),
		glm::vec2i(size.x - 50, 50),
		glm::vec2i(size.x - 50, size.y - 50),
		glm::vec2i(50, size.y - 50),
		glm::vec2i(50, 50),
	};

	m_boxShape =
	{
		glm::vec2i(-75, -75),
		glm::vec2i(+75, -75),
		glm::vec2i(+75, +75),
		glm::vec2i(-75, +75),
		glm::vec2i(-75, -75),
	};

	m_boxPose = cga::Pose(glm::vec2i(200, 200), 0.f);

	glm::vec2i center(300, 300);
	GenerateStar(center, 150.f);
}

void ClippingRenderJob::Render(SDL_Window* wnd, SDL_Renderer* renderer)
{
	// Manual input processing
	auto inputManager = cga::InputManager::GetInstance();

	if (inputManager.IsMouseDown(SDL_BUTTON_LEFT))
	{
		m_starMovement.Pause();
		m_starMovement.SetPosition(inputManager.GetMousePosition());
	}
	else
	{
		m_starMovement.Resume();
	}

	if (inputManager.IsMouseDown(SDL_BUTTON_RIGHT))
	{
		m_boxPose.position = inputManager.GetMousePosition();
	}

	m_starMovement.Update();

	// Transform box
	auto boxTransform = cga::Utils::ConstructTransform(m_boxPose);
	auto boxShape = TransformShape(m_boxShape, boxTransform);
	auto star = TransformShape(m_starShape, m_starMovement.GetTransform());

	// Perform clipping
	auto backVsBoxClipResult = m_clipper.Clip(m_backShape, boxShape, true);
	auto backVsStarClipResult = m_clipper.Clip(m_backShape, star, true);
	auto boxVsStarClipResult = m_clipper.Clip(boxShape, star, false);

	auto concatenateResults = [](const std::vector<glm::linei>& a, const std::vector<glm::linei>& b)
	{
		std::vector<glm::linei> result;
		result.reserve(a.size() + b.size());
		result.insert(result.end(), a.begin(), a.end());
		result.insert(result.end(), b.begin(), b.end());

		return result;
	};
	
	backVsStarClipResult.remain = concatenateResults(backVsStarClipResult.remain, boxVsStarClipResult.remain);
	backVsStarClipResult.clipped = concatenateResults(backVsStarClipResult.clipped, boxVsStarClipResult.clipped);
	CleanupClipResults(backVsStarClipResult);

	// Draw results
	cga::Brezenheim::DrawPolygon(renderer, m_backShape.data(), m_backShape.size(), glm::vec3i(255, 255, 255), false);

	cga::Brezenheim::DrawLines(renderer, backVsBoxClipResult.remain.data(), backVsBoxClipResult.remain.size(), glm::vec3i(255, 255, 255), false);
	cga::Brezenheim::DrawLines(renderer, backVsBoxClipResult.clipped.data(), backVsBoxClipResult.clipped.size(), glm::vec3i(255, 0, 0), true);
	cga::Brezenheim::DrawLines(renderer, backVsStarClipResult.remain.data(), backVsStarClipResult.remain.size(), glm::vec3i(255, 255, 255), false);
	cga::Brezenheim::DrawLines(renderer, backVsStarClipResult.clipped.data(), backVsStarClipResult.clipped.size(), glm::vec3i(255, 0, 0), true);
}

void ClippingRenderJob::GenerateStar(const glm::vec2i& center, float radius)
{
	static const float PI = 3.1415f;
	static const float PI_DOUBLE = 2.f * PI;

	float sector = PI_DOUBLE / 5.f;
	glm::vec2i starPoints[5];
	for (int i = 0; i < 5; i++)
	{
		float phi = sector * float(i);
		starPoints[i] = glm::vec2f(sin(phi), -cos(phi)) * radius;
	}

	m_starShape.push_back(starPoints[0]);
	m_starShape.push_back(starPoints[2]);
	m_starShape.push_back(starPoints[4]);
	m_starShape.push_back(starPoints[1]);
	m_starShape.push_back(starPoints[3]);
	m_starShape.push_back(starPoints[0]);
}

std::vector<glm::vec2i> ClippingRenderJob::TransformShape(const std::vector<glm::vec2i>& shape, const glm::mat3f& transform)
{
	std::vector<glm::vec2i> result;
	auto transformer = [transform](const glm::vec2i& point)
	{
		return glm::vec2i(cga::Utils::TransformPoint(point, transform));
	};
	std::transform(shape.begin(), shape.end(), std::back_inserter(result), transformer);

	return result;
}

void ClippingRenderJob::CleanupClipResults(Clipper::ClippingResult& result)
{
	result.clipped.erase(std::unique(result.clipped.begin(), result.clipped.end()), result.clipped.end());
	result.remain.erase(std::unique(result.remain.begin(), result.remain.end()), result.remain.end());

	for (const auto& line : result.clipped)
	{
		auto it = std::find(result.remain.begin(), result.remain.end(), line);
		if (it != result.remain.end())
		{
			result.remain.erase(it, it + 1);
		}
	}

	for (const auto& line : result.remain)
	{
		auto it = std::find(result.clipped.begin(), result.clipped.end(), line);
		if (it != result.clipped.end())
		{
			result.clipped.erase(it, it + 1);
		}
	}
}

