#include "Clipper.hpp"
#include <core/Utils.hpp>

float Clipper::Scalar(const glm::vec2f& a, const glm::vec2f& b)
{
	return a.x * b.x + a.y * b.y;
}

glm::vec2f Clipper::FindNormal(const glm::vec2f& a, const glm::vec2f& b, const glm::vec2f& z)
{
	glm::vec2f d = b - a;
	glm::vec2f n(-d.y, d.x);
	glm::vec2f v(z.x - a.x, z.y - a.y);

	float dot = Scalar(v, n);
	if (!dot)
	{
		return glm::vec2f();
	}
	else if (dot < 0) 
	{
		n *= -1.f;
	}

	return n;
}

Clipper::ClippingResult Clipper::Clip(const std::vector<glm::vec2i>& clipper, const std::vector<glm::vec2i>& clippee, bool booleanMode)
{
	Clipper::ClippingResult result;

	glm::recti clipperAabb = cga::Utils::ComputeBoundingBox(clipper);
	glm::recti clippeeAabb = cga::Utils::ComputeBoundingBox(clippee);

	int lineCount = clippee.size() - 1;
	int step = 1;

	for (int ptIdx = 0; ptIdx < lineCount; ptIdx += step)
	{
		glm::vec2f b = clippee[ptIdx + 1];
		glm::vec2f a = clippee[ptIdx];
		glm::recti lineAabb = cga::Utils::ComputeBoundingBox(std::vector<glm::vec2i> { a, b });

		bool overlap = cga::Utils::BoundingBoxOverlap(clipperAabb, lineAabb);
		if (!overlap)
		{
			if (booleanMode)
			{
				AddLine(result.clipped, glm::linei(a, b));
			}
			else
			{
				AddLine(result.remain, glm::linei(a, b));
			}

			continue;
		}

		glm::vec2f d = b - a;
		glm::vec2f dir(d.x, d.y);

		float tEnter = 0.0f;
		float tLeave = 1.0f;

		glm::vec2f boundary = clipper[clipper.size() - 2];
		for (int i = 0, count = clipper.size() - 1; i < count; i++)
		{
			glm::vec2f edge_p0 = clipper[i + 0];
			glm::vec2f edge_p1 = clipper[i + 1];

			glm::vec2f norm(FindNormal(edge_p0, edge_p1, boundary));
			glm::vec2f w(a.x - edge_p0.x, a.y - edge_p0.y);

			float num = Scalar(w, norm);
			float den = Scalar(dir, norm);

			if (!den) 
			{
				if (num < 0) 
				{
					break;
				} else {
					continue;
				}
			}

			float t = -num / den;
			if (den > 0)
			{
				tEnter = glm::max(tEnter, t);
			} 
			else if (den < 0) 
			{
				tLeave = glm::min(tLeave, t);
			}

			boundary = edge_p0;
		}

		if (tEnter > tLeave) 
		{
			if (booleanMode)
			{
				AddLine(result.clipped, glm::linei(a, b));
			}
			else
			{
				AddLine(result.remain, glm::linei(a, b));
			}
		}
		else
		{
			glm::vec2f clipped_p0;
			clipped_p0.x = a.x + (d.x * tEnter);
			clipped_p0.y = a.y + (d.y * tEnter);

			glm::vec2f clipped_p1;
			clipped_p1.x = a.x + (d.x * tLeave);
			clipped_p1.y = a.y + (d.y * tLeave);

			if (booleanMode)
			{
				AddLine(result.clipped, glm::linei(a, clipped_p0));
				AddLine(result.remain, glm::linei(clipped_p0, clipped_p1));
				AddLine(result.clipped, glm::linei(clipped_p1, b));
			}
			else
			{
				AddLine(result.remain, glm::linei(a, clipped_p0));
				AddLine(result.clipped, glm::linei(clipped_p0, clipped_p1));
				AddLine(result.remain, glm::linei(clipped_p1, b));
			}
		}
	}

	return result;
}

void Clipper::AddLine(std::vector<glm::linei>& result, const glm::linei& line)
{
	if (line.bottomRight != line.topLeft)
	{
		result.push_back(line);
	}
}
