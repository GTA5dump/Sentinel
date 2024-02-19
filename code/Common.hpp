#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <Urlmon.h>
#include <winternl.h>
#include <d3d11.h>

#pragma comment(lib, "Urlmon.lib")

#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <set>
#include <signal.h>

// JSON
#include <JSON/json.hpp>
#include <JSON/json_fwd.hpp>

// GTA-V
#include <GTAV-Classes/script/scrNativeHandler.hpp>
#include <GTAV-Classes/script/scrNativeRegistration.hpp>
#include <GTAV-Classes/script/scrNativeRegistrationTable.hpp>
#include <GTAV-Classes/network/CNetGamePlayer.hpp>
#include <GTAV-Classes/rage/joaat.hpp>

#define SENTINEL_WIDE_IMPL(str) L ## str
#define SENTINEL_WIDE(str) SENTINEL_WIDE_IMPL(str)

#define SENTINEL_STRINGIZE_IMPL(x) #x
#define SENTINEL_STRINGIZE(x) SENTINEL_STRINGIZE_IMPL(x)

#define SENTINEL_NAME "Sentinel Lite V2 DX"

namespace Sentinel
{
	using namespace std::chrono_literals;
	namespace fs = std::filesystem;

	inline HMODULE g_Module{};
	inline std::atomic_bool g_Running = true;

	inline std::uint32_t g_SelectedPlayer{};
	inline CNetGamePlayer* g_SelectedNetPlayer{};
	inline bool g_PlayerKicked = false;
}

class CNetworkObjectMgr;

#include "Settings.hpp"
#include "Logger.hpp"
#include "Util.hpp"
#include "Pointers.hpp"
#include "ScriptGlobal.hpp"
#include "Obfuscate.hpp"
#include "Notifications.hpp"