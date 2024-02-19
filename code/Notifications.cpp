#include "Auth/Implementation.hpp"
#include "Notifications.hpp"
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"

namespace Sentinel
{
	Notifications::Notifications()
	{
		Push(std::format("Welcome, skid"), "Loaded Sentinel Lite V2 DX, press insert to open.");

		g_Notifications = this;
	}

	Notifications::~Notifications()
	{
		g_Notifications = nullptr;
	}

	void Notifications::Push(Notification n)
	{
		this->m_Notifications.emplace(std::hash<std::string>{}(n.m_Message + n.m_Title), n);
	}

	void Notifications::Push(std::string title, std::string message)
	{
		this->Push({ eNotificationType::INFO, title, message, std::chrono::system_clock::now(), 5000.f , 1.f });
	}

	void Notifications::PushWarning(std::string title, std::string message)
	{
		this->Push({ eNotificationType::WARNING, title, message, std::chrono::system_clock::now(), 7000.f , 1.f });
	}

	void Notifications::PushError(std::string title, std::string message)
	{
		this->Push({ eNotificationType::DANGER, title, message, std::chrono::system_clock::now(), 7000.f , 1.f });
	}

	std::vector<Notification> Notifications::Get()
	{
		std::vector<Notification> ToSend;
		std::vector<std::size_t> ToRemove;
		for (auto& n : this->m_Notifications) 
		{
			std::chrono::time_point<std::chrono::system_clock> CurrentTime = std::chrono::system_clock::now();
			const float TimeDifference = (float)std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - n.second.m_CreationDate).count();
			n.second.m_Alpha = 1;
			if (n.second.m_DestructionDate <= TimeDifference) 
			{
				n.second.m_Alpha = 1.f - ((TimeDifference - n.second.m_DestructionDate) / 600.f);
				n.second.m_Alpha = n.second.m_Alpha < 0.f ? 0.f : n.second.m_Alpha;
			}

			if (n.second.m_Alpha > 0.f)
				ToSend.push_back(n.second);
			else ToRemove.push_back(n.first);
		}
		for (std::size_t k : ToRemove)
			this->m_Notifications.erase(k);

		return ToSend;
	}

	void Notifications::Tick()
	{
		std::vector<Notification> Notifications = Get();

		float PreviousPos = 0.f;
		for (int i = 0; i < Notifications.size(); i++)
		{
			Notification& n = Notifications[i];

			PreviousPos = Render(PreviousPos, ImGui::GetBackgroundDrawList(), n.m_Title, n.m_Message, m_NotificationColors.at(n.m_Type));
		}
	}

	bool g_ShortEnough = false;
	float g_YOffset = 0.005f;
	float Notifications::Render(float start, ImDrawList* drawlist, std::string title, std::string message, Color color)
	{
		int j = 0;
		int PreviousSpace = 0;
		float TotalSize = 0.f;
		std::vector<std::string> SplitPoints;
		for (int i = 0; i <= message.size(); i++)
		{
			std::string CurrentMessage = message.substr(j, i - j);
			auto Size = g_UiManager->GetTextScale(CurrentMessage.c_str());

			if (message.substr(i, 1) == " ")
			{
				PreviousSpace = i;
			}

			if (i == message.size())
			{
				TotalSize = TotalSize + Size.y;
				SplitPoints.push_back(message.substr(j, i));
			}
			else if (Size.x >= m_Width)
			{
				TotalSize = TotalSize + Size.y;
				SplitPoints.push_back(message.substr(j, PreviousSpace - j));
				j = PreviousSpace + 1;
				i = PreviousSpace;
			}
		}

		if (g_UiManager->GetTextScale(message.c_str()).x >= m_Width)
			g_YOffset = 0.005f;
		else
			g_YOffset = 0.f;

		g_UiManager->DrawRect(
			m_PosX,
			m_PosY + start,
			m_Width,
			m_Height + (TotalSize / 2.f),
			m_NotificationBackground);

		g_UiManager->DrawRect(
			m_PosX - (m_Width / 2.f) + (m_LineWidth / 2.f),
			m_PosY + start,
			m_LineWidth,
			m_Height + (TotalSize / 2.f),
			color);

		g_UiManager->DrawLeftText(
			title.c_str(),
			m_PosX - (m_Width / m_Padding) + m_LineWidth,
			m_PosY + start - (g_UiManager->GetTextScale(title.c_str()).y / 2.f) - (m_Height / 5.f) - (TotalSize / 4.f),
			g_D3DRenderer->m_RobotoBoldNotify,
			m_NotificationText);

		int i = 0;
		for (std::string Text : SplitPoints)
		{
			g_UiManager->DrawLeftText(
				Text.c_str(),
				m_PosX - (m_Width / m_Padding) + m_LineWidth,
				m_PosY + start - (g_UiManager->GetTextScale(Text.c_str()).y / 2.f) + (m_Height / 5.f) + (i * 0.015f) - g_YOffset,
				g_D3DRenderer->m_RobotoRegularNotify,
				m_NotificationText);
			i++;
		}

		return start + 0.003f + m_Height + TotalSize;
	}
}