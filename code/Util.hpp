#pragma once
#include "Pointers.hpp"
#include "Timer.hpp"
#include "Obfuscate.hpp"
#include <shlobj.h>
#include <curl/curl.h>

namespace Sentinel
{
	/**
	 * \brief Checks if a key is held
	 * \param value Returns true or false
	 * \param key Mapped virtual key code
	 * \param delay Original keypress delay
	 */
	inline void IsKeyHeld(bool& value, std::uint16_t key, std::size_t delay)
	{
		static SimpleTimer timer;
		if (GetForegroundWindow() == g_Pointers->m_GameWindow)
		{
			if (GetAsyncKeyState(key) & 1)
				value = true;
			else if (GetAsyncKeyState(key) & 0x8000)
			{
				timer.Initialize(delay * 100);
				if (timer.IsReady())
					value = true;
			}
			else
				timer.Destroy();
		}
	}

	inline std::string GetDocumentsPath()
	{
		wchar_t Folder[1024];
		HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
		if (SUCCEEDED(hr))
		{
			char str[1024];
			wcstombs(str, Folder, 1023);
			return str;
		}
		else return "";
	}

	/**
	 * \brief A stream buffer that uses a char[] buffer
	 */
	class MemoryStreamBuf : public std::streambuf
	{
	public:
		MemoryStreamBuf(char* array, std::size_t size)
		{
			std::fill_n(array, size, '\0');
			std::streambuf::setp(array, array + size - 1);
		}

		template <std::size_t size>
		MemoryStreamBuf(char(&array)[size]) :
			MemoryStreamBuf(&array[0], size)
		{
		}
	};

	/**
	 * \brief An input/output stream that uses a char[] buffer
	 */
	class MemoryStringStream : virtual MemoryStreamBuf, public std::iostream
	{
	public:
		MemoryStringStream(char* array, std::size_t size) :
			MemoryStreamBuf(array, size),
			std::iostream(static_cast<MemoryStreamBuf*>(this))
		{
		}

		template <std::size_t size>
		MemoryStringStream(char(&array)[size]) :
			MemoryStreamBuf(array),
			std::iostream(static_cast<MemoryStreamBuf*>(this))
		{
		}
	};

	/**
	 * \brief A MemoryStringStream with a built in buffer
	 */
	template <std::size_t N>
	class MemoryStringStreamWithBuffer : public MemoryStringStream
	{
	public:
		MemoryStringStreamWithBuffer():
			MemoryStringStream(m_Buffer)
		{}
	private:
		char m_Buffer[N];
	};
}
