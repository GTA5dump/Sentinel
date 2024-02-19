#pragma once 
#include "Natives.hpp"
#include "Game.hpp"
#include "Pointers.hpp"

namespace Sentinel::Removals
{
	// Lists
	inline const char* m_BreakupKick[2]
	{
		"Regular",
		"Cheater"
	};

	inline std::size_t m_BreakupKickPos = 0;

	inline rage::snPlayer* GetSessionPlayer(CNetGamePlayer* target)
	{
		for (std::uint32_t i = 0; i < Game::GetNetwork()->m_game_session_ptr->m_player_count; i++)
		{
			if (Game::GetNetwork()->m_game_session_ptr->m_players[i]->m_player_data.m_host_token == target->get_net_data()->m_host_token)
			{
				return Game::GetNetwork()->m_game_session_ptr->m_players[i];
			}
		}

		if (Game::GetNetwork()->m_game_session_ptr->m_local_player.m_player_data.m_host_token == target->get_net_data()->m_host_token)
			return &Game::GetNetwork()->m_game_session_ptr->m_local_player;

		return nullptr;
	}

	inline rage::snPeer* GetSessionPeer(CNetGamePlayer* target)
	{
		for (std::uint32_t i = 0; i < Game::GetNetwork()->m_game_session_ptr->m_peer_count; i++)
		{
			if (Game::GetNetwork()->m_game_session_ptr->m_peers[i]->m_peer_data.m_gamer_handle_2.m_rockstar_id == target->get_net_data()->m_gamer_handle_2.m_rockstar_id)
			{
				return Game::GetNetwork()->m_game_session_ptr->m_peers[i];
			}
		}

		return nullptr;
	}	

	inline bool BreakupKick(CNetGamePlayer* target) // From maybegreat48
	{
		auto PlayerManager = Game::GetNetworkPlayerMgr();
		if (PlayerManager == nullptr) {
			g_Notifications->PushError("Removals -> Breakup Kick", "Network Player Manager is invalid!");
			return false; 
		}
		auto LocalPlayer = PlayerManager->m_local_net_player;
		if (LocalPlayer == nullptr) { 
			g_Notifications->PushError("Removals -> Breakup Kick", "Local Player is invalid!");
			return false;
		}
		if (target == nullptr) { 
			g_Notifications->PushError("Removals -> Breakup Kick", "Target is invalid!");
			return false;
		}
		if (target == LocalPlayer) { 
			g_Notifications->PushError("Removals -> Breakup Kick", "You may not Breakup Kick yourself!");
			return false;
		}

		rage::snMsgRemoveGamersFromSessionCmd cmd{};
		cmd.m_session_id = Game::GetNetwork()->m_game_session_ptr->m_rline_session.m_session_id;
		cmd.m_num_peers = 1;
		cmd.m_peer_ids[0] = GetSessionPeer(target)->m_peer_data.m_peer_id_2;

		if (m_BreakupKickPos == 1)
			cmd.m_unk = 19;

		if (Game::GetNetwork()->m_game_session.is_host())
		{
			g_Pointers->m_HandleRemoveGamerCommand(Game::GetNetwork()->m_game_session_ptr, GetSessionPlayer(target), &cmd);
		}
		else if (target->is_host())
		{
			for (auto Player : PlayerManager->m_player_list)
			{
				if (Player != nullptr)
				{
					if (Player->is_valid())
					{
						if (Player->m_player_id != target->m_player_id)
						{
							g_Pointers->m_SendRemoveGamerCommand(Game::GetNetwork()->m_game_session_ptr->m_net_connection_mgr,
								g_Pointers->m_GetConnectionPeer(Game::GetNetwork()->m_game_session_ptr->m_net_connection_mgr, (int)GetSessionPlayer(Player)->m_player_data.m_peer_id_2),
								Game::GetNetwork()->m_game_session_ptr->m_connection_identifier, &cmd, 0x1000000);
						}
					}
				}
			}

			g_Pointers->m_HandleRemoveGamerCommand(Game::GetNetwork()->m_game_session_ptr, GetSessionPlayer(target), &cmd);
		}
		else
		{
			for (auto Player : PlayerManager->m_player_list)
			{
				if (Player != nullptr)
				{
					if (Player->is_host())
					{
						g_Pointers->m_SendRemoveGamerCommand(Game::GetNetwork()->m_game_session_ptr->m_net_connection_mgr,
							g_Pointers->m_GetConnectionPeer(Game::GetNetwork()->m_game_session_ptr->m_net_connection_mgr, (int)GetSessionPlayer(Player)->m_player_data.m_peer_id_2),
							Game::GetNetwork()->m_game_session_ptr->m_connection_identifier, &cmd, 0x1000000);
					}
				}
			}
		}

		g_Notifications->Push("Removals -> Breakup Kick", std::format("Kicked {}, they're not coming back from this :P", target->get_name()));

		return true;
	}

	inline bool HostKick(CNetGamePlayer* target)
	{
		auto PlayerManager = Game::GetNetworkPlayerMgr();
		if (PlayerManager == nullptr) {
			g_Notifications->PushError("Removals -> Host Kick", "Network Player Manager is invalid!");
			return false;
		}
		auto LocalPlayer = PlayerManager->m_local_net_player;
		if (LocalPlayer == nullptr) {
			g_Notifications->PushError("Removals -> Host Kick", "Local Player is invalid!");
			return false;
		}
		if (target == nullptr) {
			g_Notifications->PushError("Removals -> Host Kick", "Target is invalid!");
			return false;
		}
		if (target == LocalPlayer) {
			g_Notifications->PushError("Removals -> Host Kick", "You may not Breakup Kick yourself!");
			return false;
		}
		if (!LocalPlayer->is_host()) {
			g_Notifications->PushError("Removals -> Host Kick", "You must be the Session Host in order to use this kick!");
			return false;
		}

		NETWORK::NETWORK_SESSION_KICK_PLAYER(target->m_player_id);

		g_Notifications->Push("Removals -> Host Kick", std::format("Kicked {}, they're not coming back from this :P", target->get_name()));
		return true;
	}

	inline std::chrono::system_clock::time_point LastDesyncKick{};
	inline bool DesyncKick(CNetGamePlayer* target)
	{
		if (std::chrono::system_clock::now() - LastDesyncKick < 10s)
		{
			g_Notifications->PushError("Removals -> Desync Kick", "Don't spam this, it will backfire!");
			return false;
		}

		LastDesyncKick = std::chrono::system_clock::now();

		auto PlayerManager = Game::GetNetworkPlayerMgr();
		if (PlayerManager == nullptr) {
			g_Notifications->PushError("Removals -> Desync Kick", "Network Player Manager is invalid!");
			return false;
		}
		auto LocalPlayer = PlayerManager->m_local_net_player;
		if (LocalPlayer == nullptr) {
			g_Notifications->PushError("Removals -> Desync Kick", "Local Player is invalid!");
			return false;
		}
		if (target == nullptr) {
			g_Notifications->PushError("Removals -> Desync Kick", "Target is invalid!");
			return false;
		}
		if (target == LocalPlayer) {
			g_Notifications->PushError("Removals -> Desync Kick", "You may not Breakup Kick yourself!");
			return false;
		}
		if (target->is_host()) {
			g_Notifications->PushError("Removals -> Desync Kick", "Desyncing the Session Host will result in a backfire!");
			return false;
		}

		// The actual desyncing (three fucking lines)
		PlayerManager->RemovePlayer(target);
		// We do this to create one last sync before our target gets yeeted out of the lobby
		WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->m_player_id), FALSE);
		PlayerManager->UpdatePlayerListsForPlayer(LocalPlayer);

		g_Notifications->Push("Removals -> Desync Kick", std::format("Kicked {}, they're not coming back from this :P", target->get_name()));

		return false;
	}

	inline bool ScriptHostKick(Player target)
	{
		if (!g_Functions->IsScriptHost(PLAYER::PLAYER_ID()))
		{
			g_Notifications->PushWarning("Removals -> Script Host Kick", "You're not the Script Host, forcing it now.");
			if (Game::ForceHost(RAGE_JOAAT("freemode")))
			{
				g_Notifications->Push("Removals -> Script Host Kick", "You're now the Script Host.");
			}
			else
			{
				g_Notifications->PushError("Removals -> Script Host Kick", "Failed, couldn't force Script Host.");
				return false;
			}
		}

		*ScriptGlobal(1885209).Add(target, 1).As<bool*>() = true;
		g_Notifications->Push("Removals -> Script Host Kick", std::format("Kicked {}, it may take a few seconds.", g_Pointers->m_GetPlayerName(target)));

		return true;
	}

	void FragmentCrash(Player target)
	{
		auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target);
		auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
		auto CrashModel = RAGE_JOAAT("prop_fragtest_cnst_04");

		// Freeze them
		g_Functions->ClearPedTasks(Ped);

		while (!STREAMING::HAS_MODEL_LOADED(CrashModel)) {
			STREAMING::REQUEST_MODEL(CrashModel);
			Script::GetCurrent()->ScriptYield(100ms);
		}

		auto Object = OBJECT::CREATE_OBJECT(CrashModel, Coords.x, Coords.y, Coords.z, TRUE, TRUE, FALSE);
		OBJECT::BREAK_OBJECT_FRAGMENT_CHILD(Object, NULL, NULL);

		Script::GetCurrent()->ScriptYield(1s);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(CrashModel);
		g_Functions->DeleteEntity(Object);

		g_Notifications->Push("Removals -> Fragment Crash", std::format("Sent crash fragment to {}, it may take a few minutes.", g_Pointers->m_GetPlayerName(target)));
	}

	void ScriptCrash(Player target)
	{
		std::int64_t Args1[] = { (std::int64_t)eRemoteEvent::Crash, (std::int64_t)PLAYER::PLAYER_ID() };
		g_Pointers->m_TriggerScriptEvent(1, Args1, sizeof(Args1) / sizeof(Args1[0]), 1 << target);

		std::int64_t Args2[] = { (std::int64_t)eRemoteEvent::Crash2, (std::int64_t)PLAYER::PLAYER_ID(), rand() % INT64_MAX };
		g_Pointers->m_TriggerScriptEvent(1, Args2, sizeof(Args2) / sizeof(Args2[0]), 1 << target);

		std::int64_t Args3[] = { (std::int64_t)eRemoteEvent::Crash3, (std::int64_t)PLAYER::PLAYER_ID(), rand() % INT64_MAX, rand() % INT64_MAX };
		g_Pointers->m_TriggerScriptEvent(1, Args3, sizeof(Args3) / sizeof(Args3[0]), 1 << target);

		g_Notifications->Push("Removals -> Script Event Crash", std::format("Sent crash events to {}, it may take a few minutes.", g_Pointers->m_GetPlayerName(target)));
	}
}