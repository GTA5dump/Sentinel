#include "Common.hpp"
#include "Pointers.hpp"
#include "Hooking.hpp"
#include "Memory/Include.hpp"

namespace Sentinel
{
	Pointers::Pointers()
	{
		Memory::Batch MainBatch;

		MainBatch.Add("GS", "48 85 C9 74 4B 83 3D", [this](Memory::Handle Pointer)
		{
			m_GameState = Pointer.Add(7).Rip().As<decltype(m_GameState)>();
		});

		MainBatch.Add("ISS", "40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE", [this](Memory::Handle Pointer)
		{
			m_OnlineMode = Pointer.Add(3).Rip().As<decltype(m_OnlineMode)>();
		});

		MainBatch.Add("FC", "8B 15 ? ? ? ? 41 FF CF", [this](Memory::Handle Pointer)
		{
			m_FrameCount = Pointer.Add(2).Rip().As<decltype(m_FrameCount)>();
		});

		MainBatch.Add("SC", "44 38 35 ? ? ? ? 48 8B 01", [this](Memory::Handle Pointer)
		{
			m_Swapchain = Pointer.Sub(13).Rip().As<decltype(m_Swapchain)&>();
		});

		MainBatch.Add("SPT", "44 8B 0D ? ? ? ? 4C 8B 1D ? ? ? ? 48 8B 1D ? ? ? ? 41 83 F8 FF 74 3F 49 63 C0 42 0F B6 0C 18 81 E1", [this](Memory::Handle Pointer)
		{
			m_ScriptProgramTable = Pointer.Add(17).Rip().As<decltype(m_ScriptProgramTable)>();
		});

		MainBatch.Add("ST", "45 33 F6 8B E9 85 C9 B8", [this](Memory::Handle Pointer)
		{
			m_ScriptThreads = Pointer.Sub(4).Rip().Sub(8).As<decltype(m_ScriptThreads)>();
		});

		MainBatch.Add("NIP", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", [this](Memory::Handle Pointer)
		{
			m_NativeRegistrations = Pointer.Add(3).Rip().As<decltype(m_NativeRegistrations)>();
			m_GetNativeHandler = Pointer.Add(12).Rip().As<decltype(m_GetNativeHandler)>();
		});

		MainBatch.Add("NS", "FF E3", [this](Memory::Handle Pointer)
		{
			m_NativeSpoofer = Pointer.As<decltype(m_NativeSpoofer)>();
		});

		MainBatch.Add("PF", "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81", [this](Memory::Handle Pointer)
		{
			m_PedFactory = Pointer.Add(3).Rip().As<decltype(m_PedFactory)>();
		});

		MainBatch.Add("NPM", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF", [this](Memory::Handle Pointer)
		{
			m_NetworkPlayerManager = Pointer.Add(3).Rip().As<decltype(m_NetworkPlayerManager)>();
		});

		MainBatch.Add("N", "48 8B 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 75 17 48 8B 0D ? ? ? ? 48 8B D7", [this](Memory::Handle Pointer)
		{
			m_Network = Pointer.Add(3).Rip().As<decltype(m_Network)>();
		});

		MainBatch.Add("GCP & SRGC", "8D 42 FF 83 F8 FD 77 3D", [this](Memory::Handle Pointer)
		{
			m_GetConnectionPeer = Pointer.Add(23).Rip().As<decltype(m_GetConnectionPeer)>();
			m_SendRemoveGamerCommand = Pointer.Add(65).Rip().As<decltype(m_SendRemoveGamerCommand)>();
		});

		MainBatch.Add("HRGC", "41 FF C6 FF C7", [this](Memory::Handle Pointer)
		{
			m_HandleRemoveGamerCommand = Pointer.Sub(0x6E).As<decltype(m_HandleRemoveGamerCommand)>();
		});

		MainBatch.Add("TSE", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41 8B F9", [this](Memory::Handle Pointer)
		{
			m_TriggerScriptEvent = Pointer.As<decltype(m_TriggerScriptEvent)>();
		});

		MainBatch.Add("MSB", "48 8B C8 FF 52 30 84 C0 74 05 48", [this](Memory::Handle Pointer)
		{
			m_ModelSpawnBypass = Pointer.Add(8).As<decltype(m_ModelSpawnBypass)>();
		});

		MainBatch.Add("BE", "0F 85 ? ? ? ? 48 8B 05 ? ? ? ? 48 8B 48 08 E8", [this](Memory::Handle Pointer)
		{
			m_BlameExplodeBypass = Memory::BytePatch::Make(Pointer.As<std::uint16_t*>(), 0xE990).get();
		});

		MainBatch.Add("GB", "48 85 FF 48 89 1D", [this](Memory::Handle Pointer)
		{
			m_GlobalBase = Pointer.Sub(12).Rip().As<decltype(m_GlobalBase)>();
		});

		MainBatch.Add("GLT", "48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44 E8", [this](Memory::Handle Pointer)
		{
			m_GetLabelText = Pointer.As<decltype(m_GetLabelText)>();
		});

		MainBatch.Add("WDP", "44 8B E2 4C 8B E9 3B D0", [this](Memory::Handle Pointer)
		{
			m_WndProc = Pointer.Sub(52).As<decltype(m_WndProc)>();
		});	

		MainBatch.Add("SEA", "48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 7A", [this](Memory::Handle Pointer)
		{
			m_SendEventAcknowledge = Pointer.Sub(5).As<decltype(m_SendEventAcknowledge)>();
		});

		MainBatch.Add("REH", "66 41 83 F9 ? 0F 83", [this](Memory::Handle Pointer)
		{
			m_ReceivedEventHandler = Pointer.As<decltype(m_ReceivedEventHandler)>();
		});

		MainBatch.Add("GMI", "0F B7 05 ? ? ? ? 45 33 C9 4C 8B DA 66 85 C0 0F 84 ? ? ? ? 44 0F B7 C0 33 D2 8B C1 41 F7 F0 48 8B 05 ? ? ? ? 4C 8B 14 D0 EB 09 41 3B 0A 74 54", [this](Memory::Handle Pointer)
		{
			m_GetModelInfo = Pointer.As<decltype(m_GetModelInfo)>();
		});

		MainBatch.Add("ORC", "E8 ? ? ? ? 44 8B 4D 1C", [this](Memory::Handle Pointer)
		{
			m_OverworkedRendererCrash = Pointer.Add(1).Rip().As<decltype(m_OverworkedRendererCrash)>();
		});

		MainBatch.Add("ORC2", "E8 ? ? ? ? 84 C0 75 0B 41 FF CF", [this](Memory::Handle Pointer)
		{
			m_OverworkedRendererCrash2 = Pointer.Add(1).Rip().As<decltype(m_OverworkedRendererCrash2)>();
		});

		MainBatch.Add("SCS", "E8 ? ? ? ? 8B 44 24 5C 44 0F B7 4C 24 ?", [this](Memory::Handle Pointer)
		{
			m_SendCloneSync = Pointer.Add(1).Rip().As<decltype(m_SendCloneSync)>();
		});

		MainBatch.Add("FTC", "E8 ? ? ? ? B3 04 08 98 A0 00 00 00", [this](Memory::Handle Pointer)
		{
			m_FallTaskConstructor = Pointer.Add(1).Rip().As<decltype(m_FallTaskConstructor)>();
		});

		MainBatch.Add("TJC", "48 89 5C 24 ? 89 54 24 10 57 48 83 EC 30 0F 29 74 24", [this](Memory::Handle Pointer)
		{
			m_TaskJumpConstructor = Pointer.As<decltype(m_TaskJumpConstructor)>();
		});

		MainBatch.Add("FV", "83 79 18 ? 48 8B D1 74 4A FF 4A 18", [this](Memory::Handle Pointer)
		{
			m_FixVectors = Pointer.As<decltype(m_FixVectors)>();
		});

		MainBatch.Add("MWL", "8B 43 6C 89 05", [this](Memory::Handle Pointer)
		{
			// Forces your wanted level to zero, even in wanted zones like fort zancudo
			m_MaxWantedLevel = Memory::BytePatch::Make(Pointer.Add(5).Rip().As<std::uint32_t*>(), 0).get();
			m_MaxWantedLevel2 = Memory::BytePatch::Make(Pointer.Add(14).Rip().As<std::uint32_t*>(), 0).get();
		});

		MainBatch.Add("GNP", "48 83 EC 28 33 C0 38 05 ? ? ? ? 74 0A", [this](Memory::Handle Pointer)
		{
			m_GetNetPlayer = Pointer.As<decltype(m_GetNetPlayer)>();
		});

		MainBatch.Add("GPN", "85 D2 7E 0E 48 8B C8", [this](Memory::Handle Pointer)
		{
			m_GetPlayerName = Pointer.Add(28).As<decltype(m_GetPlayerName)>();
		});

		MainBatch.Add("LP", "4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B", [this](Memory::Handle Pointer)
		{
			m_ListPool = Pointer.Add(3).Rip().As<decltype(m_ListPool)>();
		});

		MainBatch.Add("RBWD", "48 89 74 24 ? 57 48 83 EC 20 48 8B D9 33 C9 41 8B F0 8A", [this](Memory::Handle Pointer)
		{
			m_ReadBitbufDword = Pointer.Sub(5).As<decltype(m_ReadBitbufDword)>();
		});

		MainBatch.Add("RBS", "E8 ? ? ? ? 48 8D 4F 3C", [this](Memory::Handle Pointer)
		{
			m_ReadBitbufString = Pointer.Add(1).Rip().As<decltype(m_ReadBitbufString)>();
		});

		MainBatch.Add("RBB", "E8 ? ? ? ? 84 C0 74 2D 48 8D 57 20", [this](Memory::Handle Pointer)
		{
			m_ReadBitbufBool = Pointer.Add(1).Rip().As<decltype(m_ReadBitbufBool)>();
		});

		MainBatch.Add("RBA", "48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C", [this](Memory::Handle Pointer)
		{
			m_ReadBitbufArray = Pointer.As<decltype(m_ReadBitbufArray)>();
		});

		MainBatch.Add("WBQ", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 41 8B F0 48 8B EA 48 8B D9 41 83 F8 20", [this](Memory::Handle Pointer)
		{
			m_WriteBitbufQword = Pointer.As<decltype(m_WriteBitbufQword)>();
		});

		MainBatch.Add("WBD", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 8B EA BF 01", [this](Memory::Handle Pointer)
		{
			m_WriteBitbufDword = Pointer.As<decltype(m_WriteBitbufDword)>();
		});

		MainBatch.Add("WBI64", "E8 ? ? ? ? 8A 53 39 48 8B CF", [this](Memory::Handle Pointer)
		{
			m_WriteBitbufInt64 = Pointer.Add(1).Rip().As<decltype(m_WriteBitbufInt64)>();
		});

		MainBatch.Add("WBI32", "E8 ? ? ? ? 8A 53 74", [this](Memory::Handle Pointer)
		{
			m_WriteBitbufInt32 = Pointer.Add(1).Rip().As<decltype(m_WriteBitbufInt32)>();
		});

		MainBatch.Add("WBB", "E8 ? ? ? ? 8A 57 39", [this](Memory::Handle Pointer)
		{
			m_WriteBitbufBool = Pointer.Add(1).Rip().As<decltype(m_WriteBitbufBool)>();
		});

		MainBatch.Add("WBA", "E8 ? ? ? ? 01 7E 08", [this](Memory::Handle Pointer)
		{
			m_WriteBitbufArray = Pointer.Add(1).Rip().As<decltype(m_WriteBitbufArray)>();
		});

		MainBatch.Add("GEFS", "E8 ? ? ? ? 8D 53 01 33 DB", [this](Memory::Handle Pointer)
		{
			m_GetEntityFromScript = Pointer.Add(1).Rip().As<decltype(m_GetEntityFromScript)>();
		});

		MainBatch.Add("CNOO", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 81 EC ? ? ? ? 44 8A 62 4B", [this](Memory::Handle Pointer)
		{
			m_ChangeNetworkObjectOwner = Pointer.As<decltype(m_ChangeNetworkObjectOwner)>();
		});

		MainBatch.Add("WIHL & WIHLC", "44 0F B7 05 ? ? ? ? 44 8B DD 41 FF C8 78 28 48 8B 1D", [this](Memory::Handle Pointer)
		{
			m_WeaponInfoHashList = Pointer.Add(0x10).Add(3).Rip().As<decltype(m_WeaponInfoHashList)>();
			m_WeaponInfoHashListCount = Pointer.Add(4).Rip().As<decltype(m_WeaponInfoHashListCount)>();
		});

		MainBatch.Add("NOMGI", "48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 33 FF 4C 8B F0", [this](Memory::Handle Pointer)
		{
			m_NetworkObjectMgrInterface = Pointer.Add(3).Rip().As<decltype(m_NetworkObjectMgrInterface)>();
		});

		// Sigscan Disabler
		MainBatch.Add("VM", "66 0F 6F 0D ? ? ? ? 66 0F 6F 05 ? ? ? ? 66 0F 66 C4", [this](Memory::Handle Pointer)
		{
			Memory::BytePatch::Make(Pointer.Add(4).Rip().Sub(32).As<std::uint64_t*>(), (std::uint64_t)&Hooks::VirtualMemory)->Apply();
		});

		if (!MainBatch.Run(Memory::Module("GTA5.exe")))
			throw std::runtime_error("Failed to find some pointers");

		m_GameWindow = FindWindowA("grcWindow", "Grand Theft Auto V");

		if (!m_GameWindow)
			throw std::runtime_error("Failed to find the game's window.");

		g_Pointers = this;
	}

	Pointers::~Pointers()
	{
		// Make sure we can reinject
		Memory::BytePatch::RestoreAll();

		g_Pointers = nullptr;
	}
}