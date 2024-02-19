#include "Hooking.hpp"
#include "NativeHook.hpp"
#include "Pointers.hpp"
#include "ScriptManager.hpp"
#include "CustomText.hpp"
#include "Util.hpp"
#include "D3DRenderer.hpp"
#include "D3DScript.hpp"
#include "UI/UIManager.hpp"
#include "Natives.hpp"
#include "Game.hpp"
#include "Features/Functions.hpp"
#include "Features/Player.hpp"
#include "Features/Protections.hpp"
#include "Features/Network.hpp"
#include "netGameEvent.hpp"
#include "enums.hpp"
#include <MinHook/MinHook.h>

namespace Sentinel
{
	const char* Hooks::GetLabelText(void* unk, const char* label)
	{
		if (g_Running)
			if (auto text = g_CustomText->GetText(rage::joaat(label)))
				return text;

		return static_cast<decltype(&GetLabelText)>(g_Hooking->m_OriginalGetLabelText)(unk, label);
	}

	LRESULT Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		g_D3DRenderer->WndProc(hWnd, msg, wParam, lParam);
		return static_cast<decltype(&WndProc)>(g_Hooking->m_OriginalWndProc)(hWnd, msg, wParam, lParam);
	}

	bool IncrementStatEvent(rage::CNetworkIncrementStatEvent* net_event, CNetGamePlayer* sender)
	{
		switch (net_event->m_stat)
		{
		case RAGE_JOAAT("MPPLY_BAD_CREW_STATUS"):
		case RAGE_JOAAT("MPPLY_BAD_CREW_MOTTO"):
		case RAGE_JOAAT("MPPLY_BAD_CREW_NAME"):
		case RAGE_JOAAT("MPPLY_BAD_CREW_EMBLEM"):
		case RAGE_JOAAT("MPPLY_EXPLOITS"):
		case RAGE_JOAAT("MPPLY_GAME_EXPLOITS"):
		case RAGE_JOAAT("MPPLY_TC_ANNOYINGME"):
		case RAGE_JOAAT("MPPLY_TC_HATE"):
		case RAGE_JOAAT("MPPLY_VC_ANNOYINGME"):
		case RAGE_JOAAT("MPPLY_VC_HATE"):
			if (g_Protections->m_NetworkEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Network Events", std::format("{} attempted to send a report.", sender->get_name()));

			return true;
		}

		return false;
	}

	bool ScriptedGameEvent(rage::CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player)
	{
		const auto Args = scripted_game_event->m_args;

		const auto Hash = static_cast<eRemoteEvent>(Args[0]);
		const auto Name = player->get_name();

		switch (Hash)
		{
		case eRemoteEvent::Bounty:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send a bounty.", Name));

			return true;
			break;
		case eRemoteEvent::CeoBan:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send a CEO Ban.", Name));

			return true;
			break;
		case eRemoteEvent::CeoKick:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send a CEO Kick.", Name));

			return true;
			break;
		case eRemoteEvent::CeoMoney:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send CEO Money.", Name));

			return true;
			break;
		case eRemoteEvent::ClearWantedLevel:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to modify your wanted level.", Name));

			return true;
			break;
		case eRemoteEvent::BadThing1:
		case eRemoteEvent::BadThing2:
		case eRemoteEvent::BadThing3:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to edit your stats.", Name));

			return true;
			break;
		case eRemoteEvent::Notification:
			switch (static_cast<eRemoteEvent>(Args[2]))
			{
			case eRemoteEvent::NotificationMoneyBanked:
			case eRemoteEvent::NotificationMoneyRemoved:
			case eRemoteEvent::NotificationMoneyStolen:
				if (g_Protections->m_ScriptEventsPos == 2)
					g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send fake messages.", Name));

				return true;
				break;
			}
			break;
		case eRemoteEvent::ForceMission:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to force you to a job.", Name));

			return true;
			break;
		case eRemoteEvent::GiveCollectible:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send you a collectible.", Name));

			return true;
			break;
		case eRemoteEvent::GtaBanner:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send you a GTA Banner.", Name));

			return true;
			break;
		case eRemoteEvent::MCTeleport:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to teleport you.", Name));

			return true;
			break;
		case eRemoteEvent::PersonalVehicleDestroyed:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send fake messages.", Name));

			return true;
			break;
		case eRemoteEvent::RemoteOffradar:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to off radar you.", Name));

			return true;
			break;
		case eRemoteEvent::TSECommand:
			if (static_cast<eRemoteEvent>(Args[2]) == eRemoteEvent::TSECommandRotateCam)
			{
				if (g_Protections->m_ScriptEventsPos == 2)
					g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to rotate your cam.", Name));

				return true;
			}
			break;
		case eRemoteEvent::SendToCayoPerico:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send you to Cayo.", Name));

			return true;
			break;
		case eRemoteEvent::SendToCutscene:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send you to a cutscene.", Name));

			return true;
			break;
		case eRemoteEvent::SoundSpam:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send you a sound spam.", Name));

			return true;
			break;
		case eRemoteEvent::Spectate:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send fake messages.", Name));

			return true;
			break;
		case eRemoteEvent::Teleport:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to teleport you.", Name));

			return true;
			break;
		case eRemoteEvent::TransactionError:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to send you an error.", Name));

			return true;
			break;
		case eRemoteEvent::VehicleKick:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to vehicle kick you.", Name));

			return true;
			break;
		case eRemoteEvent::NetworkBail:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to bail kick you.", Name));

			return true;
			break;
		case eRemoteEvent::TeleportToWarehouse:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to teleport you to warehouse.", Name));

			return true;
			break;
		case eRemoteEvent::InteriorControl:
		{
			int Interior = (int)Args[2];
			if (Interior < 0 || Interior > 161) // The upper bound will change after an update
			{
				if (g_Protections->m_ScriptEventsPos == 2)
					g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to null kick you.", Name));

				return true;
			}
			break;
		}
		case eRemoteEvent::SMS:
			if (g_Protections->m_ScriptEventsPos == 2)
				g_Notifications->PushWarning("Protections -> Script Events", std::format("{} attempted to sms you.", Name));

			return true;
			break;
		}
		return false;
	}

	void Hooks::ReceivedEventHandler(std::uintptr_t* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, std::uint16_t event_id, int event_index, int event_handled_bitset, int buffer_size, rage::datBitBuffer* buffer)
	{
		if (event_id > 91u)
		{
			g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);
			return;
		}

		const auto Name = *(char**)((DWORD64)event_manager + 8i64 * event_id + 243376);
		if (Name == nullptr || source_player == nullptr || source_player->m_player_id < 0 || source_player->m_player_id >= 32)
		{
			g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);
			return;
		}

		switch (static_cast<eNetworkEvents>(event_id))
		{
			case eNetworkEvents::KICK_VOTES_EVENT:
			{
				if (g_Protections->m_VoteKickPos != 0) {
					std::uint32_t Bitfield = buffer->Read<std::uint32_t>(32);
					if (Bitfield & (1 << target_player->m_player_id))
					{
						if (g_Protections->m_VoteKickPos == 2)
							g_Notifications->PushWarning("Protections -> Network Events", std::format("{} attempted to vote kick you.", Name));
					}
					buffer->Seek(0);
				}
				break;
			}
			case eNetworkEvents::NETWORK_INCREMENT_STAT_EVENT:
			{
				if (g_Protections->m_StatEventsPos != 0) {
					const auto StatEvent = std::make_unique<rage::CNetworkIncrementStatEvent>();
					buffer->ReadDword(&StatEvent->m_stat, 0x20);
					buffer->ReadDword(&StatEvent->m_amount, 0x20);
					if (IncrementStatEvent(StatEvent.get(), source_player))
					{
						g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);
						return;
					}
					buffer->Seek(0);
				}
				break;
			}
			case eNetworkEvents::SCRIPTED_GAME_EVENT:
			{				
				if (g_Protections->m_StatEventsPos != 0) {
					const auto ScriptEvent = std::make_unique<rage::CScriptedGameEvent>();
					buffer->ReadDword(&ScriptEvent->m_args_size, 32);
					if (ScriptEvent->m_args_size - 1 <= 0x1AF)
						buffer->ReadArray(&ScriptEvent->m_args, 8 * ScriptEvent->m_args_size);

					if (ScriptedGameEvent(ScriptEvent.get(), source_player))
					{
						g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);

						return;
					}
					buffer->Seek(0);
				}

				break;
			}
			case eNetworkEvents::RAGDOLL_REQUEST_EVENT:
			{
				if (g_Protections->m_NetworkEventsPos != 0) {
					int NetworkID = buffer->Read<int>(13);

					if (Game::GetLocalPed() && Game::GetLocalPed()->m_net_object && Game::GetLocalPed()->m_net_object->m_object_id == NetworkID)
					{
						g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);

						if (g_Protections->m_NetworkEventsPos == 2)
							g_Notifications->PushWarning("Protections -> Network Events", std::format("{} attempted to ragdoll you.", Name));

						return;
					}

					buffer->Seek(0);
				}
				break;
			}
			case eNetworkEvents::REMOVE_WEAPON_EVENT:
			{
				int NetworkID = buffer->Read<int>(13);
				auto Hash = buffer->Read<std::uint32_t>(32);

				if (Hash == RAGE_JOAAT("WEAPON_UNARMED") && g_Protections->m_WeaponCrashList != 0)
				{
					if (g_Protections->m_WeaponCrashList == 2)
						g_Notifications->PushWarning("Protections -> Network Events", std::format("{} attempted to weapon crash you.", Name));
					g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);
					return;
				}

				if (Game::GetLocalPed() && Game::GetLocalPed()->m_net_object && Game::GetLocalPed()->m_net_object->m_object_id == NetworkID && g_Protections->m_NetworkEventsPos != 0)
				{
					if (g_Protections->m_NetworkEventsPos == 2)
						g_Notifications->PushWarning("Protections -> Network Events", std::format("{} attempted to remove a weapon.", Name));
					g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);
					return;
				}

				buffer->Seek(0);
				break;
			}
			case eNetworkEvents::NETWORK_CLEAR_PED_TASKS_EVENT:
			{
				if (g_Protections->m_NetworkEventsPos != 0) {
					int NetworkID = buffer->Read<int>(13);

					if (Game::GetLocalPed() && Game::GetLocalPed()->m_net_object && Game::GetLocalPed()->m_net_object->m_object_id == NetworkID)
					{
						g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);

						if (g_Protections->m_NetworkEventsPos == 2)
							g_Notifications->PushWarning("Protections -> Network Events", std::format("{} attempted to freeze you.", Name));

						return;
					}

					buffer->Seek(0);
				}
				break;
			}
			case eNetworkEvents::SCRIPT_ENTITY_STATE_CHANGE_EVENT:
			{
				if (g_Protections->m_TaskCrashList != 0) {
					std::uint16_t Entity = buffer->Read<std::uint16_t>(13);
					auto Type = buffer->Read<ScriptEntityChangeType>(4);
					std::uint32_t Unknown = buffer->Read<std::uint32_t>(32);
					if (Type == ScriptEntityChangeType::SettingOfTaskVehicleTempAction)
					{
						std::uint16_t ped_id = buffer->Read<std::uint16_t>(13);
						std::uint32_t action = buffer->Read<std::uint32_t>(8);

						if ((action >= 15 && action <= 18) || action == 33)
						{
							g_Pointers->m_SendEventAcknowledge(event_manager, source_player, target_player, event_index, event_handled_bitset);
							if (g_Protections->m_TaskCrashList == 2)
								g_Notifications->PushWarning("Protections -> Network Events", std::format("{} attempted to task crash you.", Name));
							return;
						}
					}
					buffer->Seek(0);
				}
				break;
			}
		}

		return static_cast<decltype(&ReceivedEventHandler)>(g_Hooking->m_OriginalReceivedEventHandler)(event_manager, source_player, target_player, event_id, event_index, event_handled_bitset, buffer_size, buffer);
	}

	CBaseModelInfo* Hooks::GetModelInfo(std::uint32_t hash, std::int32_t* index)
	{
		if (g_Protections->m_InvalidModelsPos != 0)
		{
			for (auto Iterator : eCrashPeds) 
			{
				if (Iterator == hash)
				{
					hash = 0xC3B52966;

					if (g_Protections->m_InvalidModelsPos == 2)
						g_Notifications->PushWarning("Protections -> Invalid Models", "Blocked an invalid model.");
				}
			}

			for (auto Iterator : eCrashObjects)
			{
				if (Iterator == hash)
				{
					hash = 0xC3B52966;

					if (g_Protections->m_InvalidModelsPos == 2)
						g_Notifications->PushWarning("Protections -> Invalid Objects", "Blocked an invalid object.");
				}
			}
		}

		if (g_Protections->m_NextGenVehiclesPos != 0 && !g_Functions->m_VehicleSpawnerOpen)
		{
			for (auto Iterator : eCrashVehicles)
			{
				if (Iterator == hash)
				{
					hash = 0xC3B52966;

					if (g_Protections->m_NextGenVehiclesPos == 2)
						g_Notifications->PushWarning("Protections -> Next Gen Crash", "Blocked an invalid vehicle.");
				}
			}
		}

		return static_cast<decltype(&GetModelInfo)>(g_Hooking->m_OriginalGetModelInfo)(hash, index);
	}

	bool Hooks::OverworkedRendererCrash(std::uintptr_t a1, std::uint32_t a2, std::uintptr_t a3, std::uintptr_t a4, std::uintptr_t a5)
	{
		if (g_Protections->m_FragmentsPos != 0) 
		{
			if (auto Pointer = *reinterpret_cast<std::uintptr_t*>(a5 + 0x70); Pointer)
			{
				if (auto Pointer2 = *reinterpret_cast<std::uintptr_t*>(Pointer + 8 * a2); !Pointer2)
				{
					if (g_Protections->m_FragmentsPos == 2) {
						g_Notifications->PushWarning("Protections -> Overworked Renderer", "Patched overworked renderer crash.");
					}
					return false;
				}
			}
		}

		return static_cast<decltype(&OverworkedRendererCrash)>(g_Hooking->m_OriginalOverworkedRendererCrash)(a1, a2, a3, a4, a5);
	}

	bool Hooks::OverworkedRendererCrash2(float* a1, float* a2)
	{
		if (g_Protections->m_FragmentsPos != 0)
		{
			if (!a1 || !a2) 
			{
				if (g_Protections->m_FragmentsPos == 2) {
					g_Notifications->PushWarning("Protections -> Overworked Renderer", "Patched overworked renderer crash.");
				}
				return false;
			}
		}

		return static_cast<decltype(&OverworkedRendererCrash2)>(g_Hooking->m_OriginalOverworkedRendererCrash2)(a1, a2);
	}

	void Hooks::SendCloneSync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, rage::netObject* netObject, rage::datBitBuffer* buffer, std::uint16_t* object_id, bool a6)
	{
		if (*g_Pointers->m_OnlineMode && g_NetworkOptions->m_SyncFuzzer)
		{
			buffer->WriteInt64(0x94828921, 64);
			buffer->WriteInt64(0x784, 1);
			buffer->WriteArray(nullptr, 0);
		}
		return static_cast<decltype(&SendCloneSync)>(g_Hooking->m_OriginalSendCloneSync)(mgr, src, netObject, buffer, object_id, a6);
	}

	std::uint64_t* Hooks::FallTaskConstructor(std::uint64_t* _this, std::uint32_t flags)
	{
		if (g_PlayerOptions->m_GracefulLanding)
			Game::SetBits<std::uint32_t>(&flags, 10, 11, 12, 16);
		return static_cast<decltype(&FallTaskConstructor)>(g_Hooking->m_OriginalFallTaskConstructor)(_this, flags);
	}

	std::uint64_t Hooks::TaskJumpConstructor(std::uint64_t a1, int a2)
	{
		if (g_PlayerOptions->m_SuperJump || g_PlayerOptions->m_SuperMan)
			a2 |= 1 << 15;
		if (g_PlayerOptions->m_BeastJump)
			a2 |= (1 << 15) | (1 << 17);
		return static_cast<decltype(&TaskJumpConstructor)>(g_Hooking->m_OriginalTaskJumpConstructor)(a1, a2);
	}

	namespace { std::uint32_t g_HookFrameCount{}; }

	// By using this to hook natives, we are sacrificing performance for safety 
	// Detouring it is faster, but cockstar detects that
	void Hooks::StatGetInt(rage::scrNativeCallContext* src)
	{
		const auto statHash = src->get_arg<Hash>(0);
		const auto outValue = src->get_arg<int*>(1);
		const auto p2 = src->get_arg<int>(2);

		if (g_Running && g_HookFrameCount != *g_Pointers->m_FrameCount)
		{
			g_HookFrameCount = *g_Pointers->m_FrameCount;
			ScriptManager::Tick();
		}

		src->set_return_value(STATS::STAT_GET_INT(statHash, outValue, p2));
	}

	HRESULT Hooks::Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags)
	{
		if (g_Running)
		{
			g_D3DRenderer->BeginFrame();
			g_D3DScript->Tick();
			g_D3DRenderer->EndFrame();
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&Present)>(PresentIndex)(dis, syncInterval, flags);
	}

	HRESULT Hooks::ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	{
		if (g_Running)
		{
			g_D3DRenderer->PreResize();
			auto hr = g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
			if (SUCCEEDED(hr))
			{
				g_D3DRenderer->PostResize();
			}

			return hr;
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
	}

	int Hooks::VirtualMemory(void* _this, HANDLE handle, PVOID base_addr, int info_class, MEMORY_BASIC_INFORMATION* info, int size, size_t* return_len)
	{
		return 1;
	}

	Hooking::Hooking() :
		m_D3DHook(g_Pointers->m_Swapchain, 18)
	{

		MH_Initialize();
		MH_CreateHook(g_Pointers->m_GetLabelText, &Hooks::GetLabelText, &m_OriginalGetLabelText);
		g_Logger->Info("Hooked: GLT");
		MH_CreateHook(g_Pointers->m_WndProc, &Hooks::WndProc, &m_OriginalWndProc);
		g_Logger->Info("Hooked: WD");		
		MH_CreateHook(g_Pointers->m_ReceivedEventHandler, &Hooks::ReceivedEventHandler, &m_OriginalReceivedEventHandler);
		g_Logger->Info("Hooked: REH");
		MH_CreateHook(g_Pointers->m_GetModelInfo, &Hooks::GetModelInfo, &m_OriginalGetModelInfo);
		g_Logger->Info("Hooked: GMI");
		MH_CreateHook(g_Pointers->m_OverworkedRendererCrash, &Hooks::OverworkedRendererCrash, &m_OriginalOverworkedRendererCrash);
		g_Logger->Info("Hooked: ORC");
		MH_CreateHook(g_Pointers->m_OverworkedRendererCrash2, &Hooks::OverworkedRendererCrash2, &m_OriginalOverworkedRendererCrash2);
		g_Logger->Info("Hooked: ORC2");
		MH_CreateHook(g_Pointers->m_SendCloneSync, &Hooks::SendCloneSync, &m_OriginalSendCloneSync);
		g_Logger->Info("Hooked: SCS");
		MH_CreateHook(g_Pointers->m_FallTaskConstructor, &Hooks::FallTaskConstructor, &m_OriginalFallTaskConstructor);
		g_Logger->Info("Hooked: FTC");
		MH_CreateHook(g_Pointers->m_TaskJumpConstructor, &Hooks::TaskJumpConstructor, &m_OriginalTaskJumpConstructor);
		g_Logger->Info("Hooked: TJC");

		m_D3DHook.Hook(&Hooks::Present, Hooks::PresentIndex);
		m_D3DHook.Hook(&Hooks::ResizeBuffers, Hooks::ResizeBuffersIndex);
	}

	Hooking::~Hooking() noexcept
	{
		MH_RemoveHook(g_Pointers->m_TaskJumpConstructor);
		g_Logger->Info("Unhooked: TJC");
		MH_RemoveHook(g_Pointers->m_FallTaskConstructor);
		g_Logger->Info("Unhooked: FTC");
		MH_RemoveHook(g_Pointers->m_SendCloneSync);
		g_Logger->Info("Unhooked: SCS");
		MH_RemoveHook(g_Pointers->m_OverworkedRendererCrash2);
		g_Logger->Info("Unhooked: ORC2");
		MH_RemoveHook(g_Pointers->m_OverworkedRendererCrash);
		g_Logger->Info("Unhooked: ORC");
		MH_RemoveHook(g_Pointers->m_GetModelInfo);
		g_Logger->Info("Unhooked: GMI");
		MH_RemoveHook(g_Pointers->m_ReceivedEventHandler);
		g_Logger->Info("Unhooked: REH");
		MH_RemoveHook(g_Pointers->m_WndProc);
		g_Logger->Info("Unhooked: WD");
		MH_RemoveHook(g_Pointers->m_GetLabelText);
		g_Logger->Info("Unhooked: GLT");
		MH_Uninitialize();
	}

	void Hooking::Hook()
	{
		m_D3DHook.Enable();
		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		m_D3DHook.Disable();
		MH_DisableHook(MH_ALL_HOOKS);
	}
}
