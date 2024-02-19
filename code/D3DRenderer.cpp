#include "D3DRenderer.hpp"
#include "Pointers.hpp"
#include "UI/UIManager.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <STB/stb_image_write.h>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Sentinel
{
    D3DRenderer::D3DRenderer() :
        m_Swapchain(g_Pointers->m_Swapchain)
    {
        if (FAILED(m_Swapchain->GetDevice(__uuidof(ID3D11Device), (void**)(&m_Device))))
            g_Logger->Error("Failed to get D3D Device");
        m_Device->GetImmediateContext(&m_Context);

        ImGui::CreateContext();
        ImGui_ImplDX11_Init(m_Device, m_Context);
        ImGui_ImplWin32_Init(g_Pointers->m_GameWindow);

        ImGui::StyleColorsDark();
        auto& style = ImGui::GetStyle();
        style.WindowRounding = 10.f;
        style.PopupRounding = 10.f;
        style.FrameRounding = 10.f;
        style.ChildBorderSize = 0.f;
        style.FrameBorderSize = 0.f;
        style.PopupBorderSize = 0.f;
        style.TabBorderSize = 0.f;
        style.WindowBorderSize = 0.f;

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr; // Crucial for not leaving the ImGui.ini file

        auto ImGui = ImGui::GetIO().Fonts;
        ImFontConfig RobotoFamily{};
        RobotoFamily.FontDataOwnedByAtlas = false;
        std::strcpy(RobotoFamily.Name, "Roboto-Family");

        m_RobotoRegular = ImGui->AddFontFromFileTTF(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Fonts\\Roboto-Regular.ttf", GetDocumentsPath()).c_str(), ScaleFont(22.f), &RobotoFamily, ImGui->GetGlyphRangesDefault());
        m_RobotoBold = ImGui->AddFontFromFileTTF(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Fonts\\Roboto-Bold.ttf", GetDocumentsPath()).c_str(), ScaleFont(22.f), &RobotoFamily, ImGui->GetGlyphRangesDefault());
        m_RobotoItalic = ImGui->AddFontFromFileTTF(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Fonts\\Roboto-BoldItalic.ttf", GetDocumentsPath()).c_str(), ScaleFont(22.f), &RobotoFamily, ImGui->GetGlyphRangesDefault());
        m_RobotoRegularNotify = ImGui->AddFontFromFileTTF(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Fonts\\Roboto-Regular.ttf", GetDocumentsPath()).c_str(), ScaleFont(20.f), &RobotoFamily, ImGui->GetGlyphRangesDefault());
        m_RobotoBoldNotify = ImGui->AddFontFromFileTTF(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Fonts\\Roboto-Bold.ttf", GetDocumentsPath()).c_str(), ScaleFont(18.f), &RobotoFamily, ImGui->GetGlyphRangesDefault());

        m_SubmenuResource = CreateTexture(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Sprites\\ArrowRight.png", GetDocumentsPath()));
        m_ArrowsResource = CreateTexture(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Sprites\\Arrows.png", GetDocumentsPath()));
        m_ToggleOnResource = CreateTexture(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Sprites\\ToggleOn.png", GetDocumentsPath()));
        m_ToggleOffResource = CreateTexture(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Sprites\\ToggleOff.png", GetDocumentsPath()));

        g_UiManager->m_HeaderLoaded = false;
        g_UiManager->m_Header.clear();
        g_UiManager->m_HeaderFrame = 0;
        g_UiManager->m_Header = CreateGIFTexture(std::format("{}\\Sentinel\\Sentinel Lite V2 DX\\Header.gif", GetDocumentsPath()));
        g_UiManager->m_HeaderLoaded = true;
    }

    D3DRenderer::~D3DRenderer() noexcept
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void D3DRenderer::BeginFrame()
    {
        if (g_UiManager->m_Opened && g_Settings.m_LockMouse)
        {
            ImGui::GetIO().MouseDrawCursor = true;
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            ImGui::GetIO().MouseDrawCursor = false;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void D3DRenderer::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void D3DRenderer::PreResize()
    {
        ImGui_ImplDX11_InvalidateDeviceObjects();
    }

    void D3DRenderer::PostResize()
    {
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    void D3DRenderer::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (g_UiManager->m_Opened && msg == WM_KEYUP && wParam == 'X')
            g_Settings.m_LockMouse = !g_Settings.m_LockMouse;

        if (ImGui::GetCurrentContext())
            ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    }

    float D3DRenderer::ScaleFont(float size)
    {
        return size;
    }

    ImageData D3DRenderer::WriteImageToMemory(int x, int y, int comp, const void* data, int bytes, int delay)
    {
        int length;
        unsigned char* png = stbi_write_png_to_mem((const unsigned char*)data, bytes, x, y, comp, &length);
        if (png == NULL)
            return {};
        return { png, length, delay };
    }

    Image D3DRenderer::ReadImageFromMemory(ImageData data)
    {
        ImageVector vector{};
        auto image = stbi_load_from_memory(data.image_bytes, data.image_length, &vector.x, &vector.y, NULL, NULL);
        if (image == nullptr)
            return {};
        return { image, vector };
    }

    std::map<int, ImageData> D3DRenderer::LoadGIF(std::string path)
    {
        std::map<int, ImageData> frames;
        return frames;
    }

    ID3D11ShaderResourceView* D3DRenderer::CreateTexture(std::string path)
    {
        ID3D11ShaderResourceView* tmp{};
        return tmp;
    }

    std::map<int, FrameData> D3DRenderer::CreateGIFTexture(std::string path)
    {
        std::map<int, FrameData> array;
        return array;
    }

    ID3D11ShaderResourceView* D3DRenderer::CreateResourceView(unsigned char* data, ImageVector size)
    {
        ID3D11ShaderResourceView* tmpRes{};
        return tmpRes;
    }
}