#pragma once
#include "Defines.hpp"
#include <vector>

namespace cga
{

class VertexBuffer
{
public:
	VertexBuffer() = default;

	void LoadPoints(glm::vec3f* data, int count);
	void LoadIndices(int* data, int count);

	void AddPoint(const glm::vec3f& pos);
	void AddFace(int p1, int p2, int p3);

	const std::vector<glm::vec3f>& GetPoints() const;
	const std::vector<int>& GetIndices() const;

private:
	std::vector<glm::vec3f> m_points;
	std::vector<int> m_faces;
};

}
