#include <Windows.h>

#include "core/App.hpp"
#include "core/RenderQueue.hpp"
#include "core/SafeInput.hpp"

#include "RecursionRenderJob.hpp"
#include "core/Defines.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	float rotationStep = 0.1f;
	int recursionDepth = 25;
	/*float rotationStep = cga::RangeInput<float>("Enter rotation step", 0.f, 3.14f * 2.f);
	int recursionDepth = cga::RangeInput<int>("Enter recursion depth", 3, 20);*/

	cga::App app("Triangle recursion", false);
	if (!app.Init())
		return 1;

	auto wnd = app.GetWindow();
	auto renderQueue = app.GetRenderQueue();

	glm::vec2i size;
	SDL_GetWindowSize(wnd, &size.x, &size.y);
	glm::vec2i center = size / 2;
	
	auto renderJob = std::make_shared<RecursionRenderJob>(center, recursionDepth, rotationStep);
	renderQueue->QAddJob(renderJob);

	app.Run();

	return 0;
}
