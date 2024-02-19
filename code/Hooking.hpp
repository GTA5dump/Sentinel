#pragma once
#include "VMTHook.hpp"
#include "Invoker.hpp"

namespace Sentinel
{
	struct Hooks
	{
		// Detour Hooks
		static const char* GetLabelText(void* unk, const char* label);
		static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static void ReceivedEventHandler(std::uintptr_t* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, std::uint16_t event_id, int event_index, int event_handled_bitset, int buffer_size, rage::datBitBuffer* buffer);
		static CBaseModelInfo* GetModelInfo(std::uint32_t hash, std::int32_t* index);
		static bool OverworkedRendererCrash(std::uintptr_t a1, std::uint32_t a2, std::uintptr_t a3, std::uintptr_t a4, std::uintptr_t a5);
		static bool OverworkedRendererCrash2(float* a1, float* a2);
		static void SendCloneSync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, rage::netObject* netObject, rage::datBitBuffer* buffer, std::uint16_t* object_id, bool a6);
		static std::uint64_t* FallTaskConstructor(std::uint64_t* _this, std::uint32_t flags);
		static std::uint64_t TaskJumpConstructor(std::uint64_t a1, int a2);

		// VMT Hooks
		static void StatGetInt(rage::scrNativeCallContext* src);

		static constexpr auto PresentIndex = 8;
		static HRESULT Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags);

		static constexpr auto ResizeBuffersIndex = 13;
		static HRESULT ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);

		static int VirtualMemory(void* _this, HANDLE handle, PVOID base_addr, int info_class, MEMORY_BASIC_INFORMATION* info, int size, size_t* return_len);
	};

	class Hooking
	{
	public:
		explicit Hooking();
		~Hooking() noexcept;
		Hooking(Hooking const&) = delete;
		Hooking(Hooking&&) = delete;
		Hooking& operator=(Hooking const&) = delete;
		Hooking& operator=(Hooking&&) = delete;

		void Hook();
		void Unhook();

		friend struct Hooks;
	private:
		void* m_OriginalGetLabelText{};
		void* m_OriginalWndProc{};
		void* m_OriginalReceivedEventHandler{};
		void* m_OriginalGetModelInfo{};
		void* m_OriginalOverworkedRendererCrash{};
		void* m_OriginalOverworkedRendererCrash2{};
		void* m_OriginalSendCloneSync{};
		void* m_OriginalFallTaskConstructor{};
		void* m_OriginalTaskJumpConstructor{};

		VMTHook m_D3DHook;
		void* m_NtQueryVirtualMemory{};
	};

	inline std::unique_ptr<Hooking> g_Hooking;
}
