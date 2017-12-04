#include "core/RenderJob.hpp"
#include "StarMovement.hpp"
#include "Clipper.hpp"
#include <vector>
#include <core/Defines.hpp>
#include <core/Utils.hpp>

class ClippingRenderJob : public cga::RenderJob
{
public:
	ClippingRenderJob(const glm::vec2i& size);

	virtual void Render(SDL_Window* wnd, SDL_Renderer* renderer) override;

private:
	void GenerateStar(const glm::vec2i& center, float radius);
	std::vector<glm::vec2i> TransformShape(const std::vector<glm::vec2i>& shape, const glm::mat3f& transform);
	void CleanupClipResults(Clipper::ClippingResult& result);
	
	std::vector<glm::vec2i> m_backShape;
	std::vector<glm::vec2i> m_starShape;
	std::vector<glm::vec2i> m_boxShape;
	cga::Pose m_boxPose;
	StarMovement m_starMovement;
	Clipper m_clipper;
};
