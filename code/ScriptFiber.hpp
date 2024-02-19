#pragma once
#include "Common.hpp"

namespace Sentinel
{
	class ScriptFiber
	{
	public:
		explicit ScriptFiber(std::size_t num_fibers);
		~ScriptFiber();

		void Queue(std::function<void()> func);

		void Tick();
		static void FiberTick();
	private:
		std::recursive_mutex m_Mutex;
		std::stack<std::function<void()>> m_Jobs;
	};

	inline ScriptFiber* g_ScriptFiber{};

#define QUEUE(...) g_ScriptFiber->Queue([__VA_ARGS__] {
#define STOP {} });
}