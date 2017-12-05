#include "VertexBuffer.hpp"

namespace cga
{

void VertexBuffer::LoadPoints(glm::vec3f* data, int count)
{
	m_points.reserve(count);
	for (int i = 0; i < count; i++)
	{
		m_points.push_back(data[i]);
	}
}

void VertexBuffer::LoadIndices(int* data, int count)
{
	m_faces.reserve(count / 3);
	for (int i = 0; i < count; i += 3)
	{
		m_faces.push_back(data[i]);
		m_faces.push_back(data[i + 1]);
		m_faces.push_back(data[i + 2]);
	}
}

void VertexBuffer::AddPoint(const glm::vec3f& pos)
{

}

void VertexBuffer::AddFace(int p1, int p2, int p3)
{

}

const std::vector<glm::vec3f>& VertexBuffer::GetPoints() const
{
	return m_points;
}

const std::vector<int>& VertexBuffer::GetIndices() const
{
	return m_faces;
}

}