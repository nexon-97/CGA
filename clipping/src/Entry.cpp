#include "core/App.hpp"
#include "core/RenderQueue.hpp"
#include "core/SafeInput.hpp"

#include "ClippingRenderJob.hpp"

int main()
{
	cga::App app("Clipping shapes", false);
	auto wnd = app.GetWindow();
	auto renderQueue = app.GetRenderQueue();

	auto wndSize = wnd->getSize();
	auto renderJob = std::make_shared<ClippingRenderJob>(sf::Vector2f(wndSize));
	renderQueue->AddJob(renderJob);

	app.Run();

	return 0;
}
