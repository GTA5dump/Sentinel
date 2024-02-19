#pragma once
#include "Common.hpp"
#include "Types.hpp"
#include <GTAV-Classes/ped/CPedFactory.hpp>
#include <GTAV-Classes/network/CNetworkPlayerMgr.hpp>
#include <GTAV-Classes/script/scrProgramTable.hpp>
#include <GTAV-Classes/script/scrNativeHandler.hpp>
#include <GTAV-Classes/script/scrNativeRegistration.hpp>
#include <GTAV-Classes/script/scrNativeRegistrationTable.hpp>
#include <GTAV-Classes/network/Network.hpp>
#include <GTAV-Classes/base/CBaseModelInfo.hpp>

namespace rage
{
	class datBitBuffer;
}

namespace Sentinel::FunctionTypes
{
	using GetLabelText = const char* (void* unk, const char* label);
	using WndProc = LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	using SendEventAcknowledge = void(std::uintptr_t* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, int event_index, int event_handled_bitset);
	using GetModelInfo = CBaseModelInfo*(std::uint32_t hash, std::int32_t* index);
	using GetNativeHandler = rage::scrNativeHandler(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
	using FixVectors = void(rage::scrNativeCallContext*);
	using GetNetPlayer = CNetGamePlayer*(std::uint32_t id);
	using GetPlayerName = const char*(std::uint32_t id);
	using GetConnectionPeer = __int64(rage::netConnectionManager* manager, std::uint64_t peer_id);
	using SendRemoveGamerCommand = void(rage::netConnectionManager* net_connection_mgr, __int64 player, int connection_id, rage::snMsgRemoveGamersFromSessionCmd* cmd, int flags);
	using HandleRemoveGamerCommand = void*(rage::snSession* session, rage::snPlayer* origin, rage::snMsgRemoveGamersFromSessionCmd* cmd);
	using TriggerScriptEvent = void(int event_group, std::int64_t* args, int arg_count, int player_bits);
	using ReadBitbufDword = bool(rage::datBitBuffer* buffer, PVOID read, int bits);
	using ReadBitbufString = bool(rage::datBitBuffer* buffer, char* read, int bits);
	using ReadBitbufBool = bool(rage::datBitBuffer* buffer, bool* read, int bits);
	using ReadBitbufArray = bool(rage::datBitBuffer* buffer, PVOID read, int bits, int unk);
	using WriteBitbufQword = bool(rage::datBitBuffer* buffer, uint64_t val, int bits);
	using WriteBitbufDword = bool(rage::datBitBuffer* buffer, uint32_t val, int bits);
	using WriteBitbufInt64 = bool(rage::datBitBuffer* buffer, int64_t val, int bits);
	using WriteBitbufInt32 = bool(rage::datBitBuffer* buffer, int32_t val, int bits);
	using WriteBitbufBool = bool(rage::datBitBuffer* buffer, bool val, int bits);
	using WriteBitbufArray = bool(rage::datBitBuffer* buffer, void* val, int bits, int unk);
	using GetEntityFromScript = std::uint64_t(std::int32_t script_index);
	using ChangeNetworkObjectOwner = std::uint64_t(std::uint64_t obj_mgr, std::uint64_t net_obj, CNetGamePlayer* player, std::uint64_t unk);

#pragma pack(push, 1)
	class ListPool
	{
	public:
		std::uint64_t* list; //0x0000
		std::int32_t capacity; //0x0008
		char pad_000C[4]; //0x000C
		std::int32_t free; //0x0010
		char pad_0014[4]; //0x0014
		std::uint64_t item_size; //0x0018
		char pad_0020[16]; //0x0020
		std::uint32_t* data; //0x0030
		char pad_0038[16]; //0x0038
		std::int32_t occupied; //0x0048
		const std::uint64_t get(std::int32_t index) const {
			if ((this->data[index >> 5] >> (index & 0x1F)) & 1)
				return this->list[index];
			return NULL;
		}
	}; // Size: 0x004C
#pragma pack(pop)
}