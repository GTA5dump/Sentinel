#pragma once
#include "ImGui/imgui.h"

namespace Sentinel
{
#pragma pack(push, 1)
	struct ImageVector
	{
		int x{};
		int y{};
	};

	struct Image
	{
		unsigned char* data;
		ImageVector vector;
	};

	struct ImageData
	{
		unsigned char* image_bytes;
		int image_length;
		int frame_delay;
	};

	struct FrameData
	{
		int frame_delay;
		ID3D11ShaderResourceView* resource_view;
	};
#pragma pack(pop)

	class D3DRenderer
	{
	public:
		explicit D3DRenderer();
		~D3DRenderer() noexcept;
		D3DRenderer(D3DRenderer const&) = delete;
		D3DRenderer(D3DRenderer&&) = delete;
		D3DRenderer& operator=(D3DRenderer const&) = delete;
		D3DRenderer& operator=(D3DRenderer&&) = delete;

		/**
		 * \brief Prepares the frame.
		 */
		void BeginFrame();

		/**
		 * \brief Finishes the frame and renders.
		 */
		void EndFrame();

		/**
		 * \brief Invalidates rendering resources
		 */
		void PreResize();

		/**
		 * \brief Finishes the frame and renders.
		 */
		void PostResize();

		/**
		 * \brief Processes input messages.
		 */
		void WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/**
		 * \brief Convert font scale to floating point
		 * \param size The floating point size
		 * \return float
		 */
		float ScaleFont(float size);

	private:
		ImageData WriteImageToMemory(int x, int y, int comp, const void* data, int bytes, int delay);

		Image ReadImageFromMemory(ImageData data);

		std::map<int, ImageData> LoadGIF(std::string path);

		ID3D11ShaderResourceView* CreateTexture(std::string path);

		std::map<int, FrameData> CreateGIFTexture(std::string path);

		ID3D11ShaderResourceView* CreateResourceView(unsigned char* data, ImageVector size);

	public:
		ImFont* m_RobotoRegular;
		ImFont* m_RobotoBold;
		ImFont* m_RobotoItalic;
		ImFont* m_RobotoRegularNotify;
		ImFont* m_RobotoBoldNotify;

		ID3D11ShaderResourceView* m_SubmenuResource;
		ID3D11ShaderResourceView* m_ArrowsResource;
		ID3D11ShaderResourceView* m_ToggleOnResource;
		ID3D11ShaderResourceView* m_ToggleOffResource;
	private:
		IDXGISwapChain* m_Swapchain{};
		ID3D11Device* m_Device{};
		ID3D11DeviceContext* m_Context{};

	};

	inline std::unique_ptr<D3DRenderer> g_D3DRenderer;
}