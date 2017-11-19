#pragma once
#include <vector>
#include <memory>

namespace sf
{
class RenderWindow;
}

namespace cga
{
	class RenderJob;
	using RenderJobPtr = std::shared_ptr<RenderJob>;
	using JobsVector = std::vector<RenderJobPtr>;

	class RenderQueue
	{
	public:
		RenderQueue(sf::RenderWindow* wnd);
		~RenderQueue() = default;

		void AddJob(const RenderJobPtr& job);
		void Clear();

		const JobsVector& GetJobs() const;

		void Render();

	private:
		sf::RenderWindow* m_window = nullptr;
		JobsVector m_jobs;
	};
}
