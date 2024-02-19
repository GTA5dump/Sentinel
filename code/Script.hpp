#pragma once
#include "Common.hpp"

namespace Sentinel
{
	class Script
	{
	public:
		using Helper = void(*)();
	public:
		explicit Script(Helper function, std::optional<std::size_t> stack = std::nullopt);
		~Script();

		void Tick();
		void ScriptYield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);
		static Script* GetCurrent();
	private:
		void FiberFunction();
	private:
		void* m_ScriptFiber;
		void* m_MainFiber;
		Helper m_Function;
		std::optional<std::chrono::high_resolution_clock::time_point> m_WakeTime;
	};
}