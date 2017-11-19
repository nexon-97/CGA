#include "RenderQueue.hpp"
#include "RenderJob.hpp"

namespace cga
{

RenderQueue::RenderQueue(sf::RenderWindow* wnd)
	: m_window(wnd)
{
}

void RenderQueue::AddJob(const RenderJobPtr& job)
{
	m_jobs.push_back(job);
}

void RenderQueue::Clear()
{
	m_jobs.clear();
}

const JobsVector& RenderQueue::GetJobs() const
{
	return m_jobs;
}

void RenderQueue::Render()
{
	for (const auto& job : GetJobs())
	{
		job->Render(m_window);
	}
}

}
