#pragma once
#include "FunctionTypes.hpp"
#include "Memory/BytePatch.hpp"
#include <GTAV-Classes/script/GtaThread.hpp>

namespace Sentinel
{
	class Pointers
	{
	public:
		explicit Pointers();
		~Pointers();
	public:
		std::uint32_t* m_GameState;
		bool* m_OnlineMode;
		std::uint32_t* m_FrameCount;
		IDXGISwapChain* m_Swapchain;
		rage::scrProgramTable* m_ScriptProgramTable;
		rage::atArray<GtaThread*>* m_ScriptThreads;
		rage::scrNativeRegistrationTable* m_NativeRegistrations;
		FunctionTypes::GetNativeHandler* m_GetNativeHandler;
		void* m_NativeSpoofer;
		CPedFactory** m_PedFactory;
		CNetworkPlayerMgr** m_NetworkPlayerManager;
		Network** m_Network;
		FunctionTypes::GetConnectionPeer* m_GetConnectionPeer;
		FunctionTypes::SendRemoveGamerCommand* m_SendRemoveGamerCommand;
		FunctionTypes::HandleRemoveGamerCommand* m_HandleRemoveGamerCommand;
		FunctionTypes::TriggerScriptEvent* m_TriggerScriptEvent;
		PVOID m_ModelSpawnBypass;
		Memory::BytePatch* m_BlameExplodeBypass;
		std::uint64_t** m_GlobalBase;
		FunctionTypes::GetLabelText* m_GetLabelText;
		FunctionTypes::WndProc* m_WndProc;
		FunctionTypes::SendEventAcknowledge* m_SendEventAcknowledge;
		PVOID m_ReceivedEventHandler;
		FunctionTypes::GetModelInfo* m_GetModelInfo;
		PVOID m_OverworkedRendererCrash;
		PVOID m_OverworkedRendererCrash2;
		PVOID m_SendCloneSync;
		PVOID m_FallTaskConstructor;
		PVOID m_TaskJumpConstructor;
		FunctionTypes::FixVectors* m_FixVectors;
		Memory::BytePatch* m_MaxWantedLevel;
		Memory::BytePatch* m_MaxWantedLevel2;
		FunctionTypes::GetNetPlayer* m_GetNetPlayer;
		FunctionTypes::GetPlayerName* m_GetPlayerName;
		FunctionTypes::ListPool* m_ListPool;
		FunctionTypes::ReadBitbufDword* m_ReadBitbufDword;
		FunctionTypes::ReadBitbufString* m_ReadBitbufString;
		FunctionTypes::ReadBitbufBool* m_ReadBitbufBool;
		FunctionTypes::ReadBitbufArray* m_ReadBitbufArray;
		FunctionTypes::WriteBitbufQword* m_WriteBitbufQword;
		FunctionTypes::WriteBitbufDword* m_WriteBitbufDword;
		FunctionTypes::WriteBitbufInt64* m_WriteBitbufInt64;
		FunctionTypes::WriteBitbufInt32* m_WriteBitbufInt32;
		FunctionTypes::WriteBitbufBool* m_WriteBitbufBool;
		FunctionTypes::WriteBitbufArray* m_WriteBitbufArray;
		FunctionTypes::GetEntityFromScript* m_GetEntityFromScript;
		FunctionTypes::ChangeNetworkObjectOwner* m_ChangeNetworkObjectOwner;
		std::uint64_t m_WeaponInfoHashList;
		std::uint64_t m_WeaponInfoHashListCount;
		std::uintptr_t m_NetworkObjectMgrInterface;

		HWND m_GameWindow;
	};

	inline Pointers* g_Pointers{};
}