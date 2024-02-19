#pragma once

namespace Sentinel
{
	class MainScript
	{
	public:
		explicit MainScript() = default;
		~MainScript() noexcept = default;

		void Initialize();
		static void Tick();
	private:
		inline static int m_TextureID;
	};

	inline std::shared_ptr<MainScript> g_MainScript;
}