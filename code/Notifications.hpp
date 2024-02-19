#pragma once
#include "ImGui/imgui.h"

namespace Sentinel
{
	enum class eNotificationType 
	{
		INFO,
		SUCCESS,
		WARNING,
		DANGER,
	};

	struct Notification
	{
		eNotificationType m_Type;
		const std::string m_Title;
		const std::string m_Message;
		const std::chrono::time_point<std::chrono::system_clock> m_CreationDate;
		const float m_DestructionDate;
		float m_Alpha;
	};

	class Notifications final
	{
		std::unordered_map<std::size_t, Notification> m_Notifications;

	public:
		Notifications();
		virtual ~Notifications();

		void Push(Notification);
		void Push(std::string, std::string);
		void PushWarning(std::string, std::string);
		void PushError(std::string, std::string);
		std::vector<Notification> Get();
		void Tick();

		std::map<eNotificationType, Color> m_NotificationColors = 
		{
			{ eNotificationType::INFO, Color(193, 192, 193, 255) },
			{ eNotificationType::WARNING, Color(242, 255, 0, 255) },
			{ eNotificationType::DANGER, Color(255, 77, 77, 255) }
		};

		float m_PosX = 0.90f;
		float m_PosY = 0.05f;
		float m_Width = 0.2f;
		float m_Height = 0.05f;
		float m_LineWidth = 0.0025f;
		float m_Padding = 2.1f;
		Color m_NotificationBackground{ 17, 17, 17, 255 };
		Color m_NotificationText{ 193, 192, 193, 255 };
	private:
		float Render(float start, ImDrawList* drawlist, std::string title, std::string message, Color color);
	};

	inline Notifications* g_Notifications{};
}