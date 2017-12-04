#include "core/RenderJob.hpp"
#include <vector>
#include "core/Vertex.hpp"

class ArchimedeanSpiralRenderJob : public cga::RenderJob
{
public:
	ArchimedeanSpiralRenderJob(const glm::vec2i& center, float linearSpeed, float radialSpeed, float accuracy);

	virtual void Render(SDL_Window* wnd, SDL_Renderer* renderer) override;

private:
	std::vector<glm::vec2i> m_points;
	std::vector<glm::vec2i> m_pointsReverse;
};
