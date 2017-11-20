#include <iostream>

#include "core/App.hpp"
#include "core/RenderQueue.hpp"
#include "core/SafeInput.hpp"

#include "ArchimedeanSpiralRenderJob.hpp"

int main()
{
	float linearSpeed = cga::SafeInput<float>("Enter linear speed (px): ");
	float radialSpeed = cga::SafeInput<float>("Enter linear speed (rad): ");
	float accuracy = cga::RangeInput<float>("Enter accuracy", 0.5f, 4.f);

	cga::App app("Archimedean Spiral", false);
	auto wnd = app.GetWindow();
	auto renderQueue = app.GetRenderQueue();

	auto wndSize = wnd->getSize();
	sf::Vector2f center(float(wndSize.x) / 2.f, float(wndSize.y) / 2.f);
	
	auto renderJob = std::make_shared<ArchimedeanSpiralRenderJob>(center, linearSpeed, radialSpeed, accuracy);
	renderQueue->AddJob(renderJob);

	app.Run();

	return 0;
}
