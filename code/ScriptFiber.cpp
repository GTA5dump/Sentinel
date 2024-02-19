#include "Common.hpp"
#include "ScriptFiber.hpp"
#include "Script.hpp"
#include "ScriptManager.hpp"

namespace Sentinel
{
	ScriptFiber::ScriptFiber(std::size_t num_fibers)
	{
		for (std::size_t i = 0; i < num_fibers; ++i)
		{
			ScriptManager::Init(std::make_unique<Script>(&FiberTick));
		}

		g_ScriptFiber = this;
	}

	ScriptFiber::~ScriptFiber()
	{
		g_ScriptFiber = nullptr;
	}

	void ScriptFiber::Queue(std::function<void()> func)
	{
		if (func)
		{
			std::lock_guard lock(m_Mutex);
			m_Jobs.push(std::move(func));
		}
	}

	void ScriptFiber::Tick()
	{
		std::unique_lock lock(m_Mutex);
		if (!m_Jobs.empty())
		{
			auto job = std::move(m_Jobs.top());
			m_Jobs.pop();
			lock.unlock();

			std::invoke(std::move(job));
		}
	}

	void ScriptFiber::FiberTick()
	{
		while (true)
		{
			g_ScriptFiber->Tick();
			Script::GetCurrent()->ScriptYield();
		}
	}
}