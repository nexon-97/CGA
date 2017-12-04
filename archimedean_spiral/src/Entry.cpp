#include <Windows.h>

#include "core/App.hpp"
#include "core/RenderQueue.hpp"
#include "core/SafeInput.hpp"

#include "ArchimedeanSpiralRenderJob.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	float linearSpeed = 25.f, radialSpeed = 3.f, accuracy = 2.f;
	/*float linearSpeed = cga::SafeInput<float>("Enter linear speed (px): ");
	float radialSpeed = cga::SafeInput<float>("Enter linear speed (rad): ");
	float accuracy = cga::RangeInput<float>("Enter accuracy", 0.5f, 4.f);*/

	cga::App app("Archimedean Spiral", false);
	if (!app.Init())
		return 1;

	auto wnd = app.GetWindow();
	auto renderQueue = app.GetRenderQueue();

	glm::vec2i size;
	SDL_GetWindowSize(wnd, &size.x, &size.y);
	glm::vec2i center = size / 2;
	
	auto renderJob = std::make_shared<ArchimedeanSpiralRenderJob>(center, linearSpeed, radialSpeed, accuracy);
	renderQueue->QAddJob(renderJob);

	app.Run();

	return 0;
}
