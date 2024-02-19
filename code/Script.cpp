#include "Script.hpp"

namespace Sentinel
{
	Script::Script(Helper function, std::optional<std::size_t> stack) :
		m_Function(function),
		m_ScriptFiber(nullptr),
		m_MainFiber(nullptr)
	{
		m_ScriptFiber = CreateFiber(stack.has_value() ? stack.value() : 0, [](void* param)
		{
			auto ThisScript = static_cast<Script*>(param);
			ThisScript->FiberFunction();
		}, this);
	}

	Script::~Script()
	{
		if (m_ScriptFiber)
			DeleteFiber(m_ScriptFiber);
	}

	void Script::Tick()
	{
		m_MainFiber = GetCurrentFiber();
		if (!m_WakeTime.has_value() || m_WakeTime.value() <= std::chrono::high_resolution_clock::now())
			SwitchToFiber(m_ScriptFiber);
	}

	void Script::ScriptYield(std::optional<std::chrono::high_resolution_clock::duration> time)
	{
		if (time.has_value())
			m_WakeTime = std::chrono::high_resolution_clock::now() + time.value();
		else
			m_WakeTime = std::nullopt;

		SwitchToFiber(m_MainFiber);
	}

	Script* Script::GetCurrent()
	{
		return static_cast<Script*>(GetFiberData());
	}

	void Script::FiberFunction()
	{
		try
		{
			m_Function();
		}
		catch (...) {}
		[]() {
		}();
		while (true)
		{
			ScriptYield(0ms);
		}
	}
}