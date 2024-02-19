#include "D3DScript.hpp"
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "Notifications.hpp"

namespace Sentinel
{
	void D3DScript::Tick()
	{
		// UI
		g_UiManager->OnTick();

		// Notifications
		g_Notifications->Tick();
	}
}
