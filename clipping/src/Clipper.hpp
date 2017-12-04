#include "core/RenderJob.hpp"
#include <vector>
#include <core/Defines.hpp>

class Clipper
{
public:
	Clipper() = default;

	struct ClippingResult
	{
		std::vector<glm::linei> remain;
		std::vector<glm::linei> clipped;
	};

	ClippingResult Clip(const std::vector<glm::vec2i>& clipper, const std::vector<glm::vec2i>& clippee, bool booleanMode);

private:
	float Scalar(const glm::vec2f& a, const glm::vec2f& b);
	glm::vec2f FindNormal(const glm::vec2f& a, const glm::vec2f& b, const glm::vec2f& c);
	void AddLine(std::vector<glm::linei>& result, const glm::linei& line);
};
