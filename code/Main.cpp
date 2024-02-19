#include "Pointers.hpp"
#include "Hooking.hpp"
#include "NativeHook.hpp"
#include "ScriptFiber.hpp"
#include "ScriptManager.hpp"
#include "D3DScript.hpp"
#include "MainScript.hpp"
#include "Invoker.hpp"
#include "CustomText.hpp"
#include "D3DRenderer.hpp"
#include "Debugger.hpp"
#include "UI/UIManager.hpp"
#include "Features/FeatureManager.hpp"
#include "Auth/Credentials.hpp"
#include "Auth/Implementation.hpp"
#include "Auth/Utility.hpp"

/**
 * \brief The entry point of the library
 * \param hInstance The base address of the DLL
 * \param reason The reason for this function being called
 */
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID)
{
	using namespace Sentinel;

	if (reason == DLL_PROCESS_ATTACH)
	{
		g_Module = hInstance;
		CreateThread(nullptr, 0, [](LPVOID) -> DWORD
		{
			g_Logger = std::make_unique<Logger>();
			g_Logger->Info("Sentinel Lite V2 DX | 1.5");

			auto g_Pointer = std::make_unique<Pointers>();

			// Wait for the game to load
			while (*g_Pointers->m_GameState != 0)
			{
				std::this_thread::sleep_for(3s);
				std::this_thread::yield();
			}

			g_CustomText = std::make_unique<CustomText>();
			g_UiManager = std::make_unique<UserInterface::UIManager>();
			g_D3DRenderer = std::make_unique<D3DRenderer>();

			g_MainScript = std::make_shared<MainScript>();
			g_D3DScript = std::make_shared<D3DScript>();

			auto g_ScriptFibers = std::make_unique<ScriptFiber>(10);

			ScriptManager::Init(std::make_unique<Script>(&MainScript::Tick));

			g_FeatureManager = std::make_unique<FeatureManager>();
			g_FeatureManager->Init();

			auto g_Notification = std::make_unique<Notifications>();

			g_Hooking = std::make_unique<Hooking>();
			g_Hooking->Hook();

			auto g_NativeHook = std::make_unique<NativeHooks>();

			g_Logger->Info("Sentinel Lite V2 DX injected");
			while (g_Running)
			{
				std::this_thread::yield();
			}

			std::this_thread::sleep_for(500ms);

			g_NativeHook.reset();

			g_Hooking->Unhook();

			g_Notification.reset();

			g_FeatureManager->Destroy();
			g_FeatureManager.reset();

			ScriptManager::Destroy();

			g_ScriptFibers.reset();

			g_MainScript.reset();
			g_D3DScript.reset();

			g_UiManager.reset();
			g_D3DRenderer.reset();
			g_CustomText.reset();

			std::this_thread::sleep_for(500ms);

			g_Hooking.reset();

			g_Pointer.reset();
			
			g_Logger->Info("Sentinel Lite V2 DX ejected");
			g_Logger.reset();

			FreeLibraryAndExitThread(g_Module, 0);
		}, nullptr, 0, nullptr);
	}

	return true;
}
