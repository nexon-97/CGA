#pragma once
#include <vector>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace cga
{
	class RenderJob;
	using RenderJobPtr = std::shared_ptr<RenderJob>;
	using JobsVector = std::vector<RenderJobPtr>;

	class RenderQueue
	{
	public:
		explicit RenderQueue(SDL_Window* wnd, SDL_Renderer* renderer);
		~RenderQueue() = default;

		void QAddJob(const RenderJobPtr& job);
		void Clear();

		const JobsVector& GetJobs() const;

		void Render();

	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
		JobsVector m_jobs;
	};
}
