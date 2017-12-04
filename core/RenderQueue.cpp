#include "RenderQueue.hpp"
#include "RenderJob.hpp"

namespace cga
{

RenderQueue::RenderQueue(SDL_Window* wnd, SDL_Renderer* renderer)
	: m_window(wnd)
	, m_renderer(renderer)
{
}

void RenderQueue::QAddJob(const RenderJobPtr& job)
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
		job->Render(m_window, m_renderer);
	}
}

}
