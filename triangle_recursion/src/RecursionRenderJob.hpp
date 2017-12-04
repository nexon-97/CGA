#include "core/RenderJob.hpp"
#include <vector>
#include "core/Defines.hpp"

class RecursionRenderJob : public cga::RenderJob
{
public:
	RecursionRenderJob(const glm::vec2i& center, int recursionDepth, float rotationStep);

	virtual void Render(SDL_Window* wnd, SDL_Renderer* renderer) override;

private:
	void AddTriangle(glm::vec2i triangle[3]);

	std::vector<glm::vec2i> m_points;
	int m_recursionDepth;
};
