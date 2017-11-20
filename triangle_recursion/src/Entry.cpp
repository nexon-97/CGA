#include "core/App.hpp"
#include "core/RenderQueue.hpp"
#include "core/SafeInput.hpp"

#include "RecursionRenderJob.hpp"

int main()
{
	float rotationStep = cga::RangeInput<float>("Enter rotation step", 0.f, 3.14f * 2.f);
	int recursionDepth = cga::RangeInput<int>("Enter recursion depth", 3, 20);

	cga::App app("Triangle recursion", false);
	auto wnd = app.GetWindow();
	auto renderQueue = app.GetRenderQueue();

	auto wndSize = wnd->getSize();
	sf::Vector2f center(float(wndSize.x) / 2.f, float(wndSize.y) / 2.f);
	
	auto renderJob = std::make_shared<RecursionRenderJob>(center, recursionDepth, rotationStep);
	renderQueue->AddJob(renderJob);

	app.Run();

	return 0;
}
