#include "Pointers.hpp"
#include "ScriptHandler.hpp"
#include "ScriptManager.hpp"

#ifndef _GAME_HPP_
#define _GAME_HPP_
namespace Sentinel::Game
{
	template<typename T>
	inline constexpr void SetBit(T* value, T bit)
	{
		*value |= (1 << bit);
	}

	template<typename T, typename ...T2>
	inline constexpr void SetBits(T* value, T2... bit)
	{
		(SetBit<T>(value, std::forward<T>(bit)), ...);
	}

	template<typename T>
	inline constexpr void ClearBit(T* value, T bit)
	{
		*value &= ~(1 << bit);
	}

	template<typename T, typename ...T2>
	inline constexpr void ClearBits(T* value, T2... bit)
	{
		(ClearBits<T>(value, std::forward<T>(bit)), ...);
	}

	inline static GtaThread* FindScriptThread(rage::joaat_t hash)
	{
		for (auto thread : *g_Pointers->m_ScriptThreads)
		{
			if (thread
				&& thread->m_context.m_thread_id
				&& thread->m_handler
				&& thread->m_script_hash == hash)
			{
				return thread;
			}
		}

		return nullptr;
	}

	inline bool ForceHost(rage::joaat_t hash)
	{
		if (auto launcher = FindScriptThread(hash); launcher && launcher->m_net_component)
		{
			for (int i = 0; !((CGameScriptHandlerNetComponent*)launcher->m_net_component)->is_local_player_host(); i++)
			{
				if (i > 200)
					return false;

				((CGameScriptHandlerNetComponent*)launcher->m_net_component)->send_host_migration_event(g_Pointers->m_GetNetPlayer(PLAYER::PLAYER_ID()));
				Script::GetCurrent()->ScriptYield(10ms);

				if (!launcher->m_stack || !launcher->m_net_component)
					return false;
			}
		}

		return true;
	}

	inline CPed* GetLocalPed()
	{
		if (auto PedFactory = *g_Pointers->m_PedFactory)
			return PedFactory->m_local_ped;

		return nullptr;
	}

	inline CPlayerInfo* GetLocalPlayerInfo()
	{
		if (auto Ped = GetLocalPed())
			return Ped->m_player_info;

		return nullptr;
	}

	inline float* WaterCollision(CNavigation* nav)
	{
		auto nav_addr = (std::uint64_t)nav;
		return (float*)(*(std::uint64_t*)(nav_addr + 0x10) + 0x54);
	}

	inline void ResetOxygenLevel(CPed* ped)
	{
		auto Address = (std::uint64_t)ped;
		*(float*)(*(std::uint64_t*)(Address + 0x10A0) + 0x278) = 0.f;
	}

	inline CNetworkPlayerMgr* GetNetworkPlayerMgr()
	{
		if (auto NetworkPlayerMgr = *g_Pointers->m_NetworkPlayerManager)
			return NetworkPlayerMgr;

		return nullptr;
	}

	inline Network* GetNetwork()
	{
		return *g_Pointers->m_Network;
	}
}
#endif