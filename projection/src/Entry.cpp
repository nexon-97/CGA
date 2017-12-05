#include <Windows.h>

#include "core/App.hpp"
#include "core/RenderQueue.hpp"
#include "core/SafeInput.hpp"

#include "ProjectionRenderJob.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	cga::App app("Projection", false);
	if (!app.Init())
		return 1;

	auto wnd = app.GetWindow();
	auto renderQueue = app.GetRenderQueue();

	glm::vec2i size;
	SDL_GetWindowSize(wnd, &size.x, &size.y);
	glm::vec2i center = size / 2;
	
	auto renderJob = std::make_shared<ProjectionRenderJob>(center);
	renderQueue->QAddJob(renderJob);

	app.Run();

	return 0;
}
