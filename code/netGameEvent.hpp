#pragma once 
#include "Pointers.hpp"

namespace rage
{
#pragma pack(push, 1)
	class datBitBuffer
	{
	public:
		datBitBuffer(void* data, std::uint32_t size) {
			m_data = data;
			m_bitOffset = 0;
			m_maxBit = size * 8;
			m_bitsRead = 0;
			m_curBit = 0;
			m_highestBitsRead = 0;
			m_flagBits = 0;
		}

		std::uint32_t GetPosition() 
		{
			return m_bitsRead;
		}

		bool Seek(std::uint32_t bits)
		{
			if (bits >= 0) {
				std::uint32_t length = (m_flagBits & 1) ? m_maxBit : m_curBit;
				if (bits <= length)
					m_bitsRead = bits;
			}
			return false;
		}

		bool WriteBool(bool integer) 
		{
			return Sentinel::g_Pointers->m_WriteBitbufBool(this, integer, 1);
		}

		bool ReadBool(bool* integer) 
		{
			return Sentinel::g_Pointers->m_ReadBitbufBool(this, integer, 1);
		}

		bool ReadPeerId(std::uint64_t* integer) 
		{
			return this->ReadQWord(integer, 0x40);
		}

		std::uint64_t ReadBits(std::size_t numBits)
		{
			auto const totalBits = (m_flagBits & 1) ? m_maxBit : m_curBit;
			if ((m_flagBits & 2) || m_bitsRead + numBits > totalBits)
				return 0;
			auto const bufPos = m_bitsRead + m_bitOffset;
			auto const initialBitOffset = bufPos & 0b111;
			auto const start = &((std::uint8_t*)m_data)[bufPos / 8];
			auto const next = &start[1];
			auto result = (start[0] << initialBitOffset) & 0xff;
			for (auto i = 0; i < ((numBits - 1) / 8); i++) {
				result <<= 8;
				result |= next[i] << initialBitOffset;
			}
			if (initialBitOffset)
				result |= next[0] >> (8 - initialBitOffset);
			m_bitsRead += static_cast<std::uint32_t>(numBits);
			if (m_bitsRead > m_highestBitsRead)
				m_highestBitsRead = m_bitsRead;
			return result >> ((8 - numBits) % 8);
		}

		int GetDataLength() 
		{
			int leftoverBit = (m_curBit % 8) ? 1 : 0;
			return (m_curBit / 8) + leftoverBit;
		}

		bool ReadString(char* string, int bits) 
		{
			return Sentinel::g_Pointers->m_ReadBitbufString(this, string, bits);
		}

		bool WriteByte(std::uint8_t integer, int bits)
		{
			return Sentinel::g_Pointers->m_WriteBitbufDword(this, integer, bits);
		}

		bool ReadByte(std::uint8_t* integer, int bits) {
			std::uint32_t read;
			if (Sentinel::g_Pointers->m_ReadBitbufDword(this, &read, bits)) {
				*integer = read;
				return true;
			}
			return false;
		}

		bool WriteWord(std::uint16_t integer, int bits)
		{
			return Sentinel::g_Pointers->m_WriteBitbufDword(this, integer, bits);
		}

		bool ReadWord(std::uint16_t* integer, int bits) 
		{
			std::uint32_t read;
			if (Sentinel::g_Pointers->m_ReadBitbufDword(this, &read, bits)) {
				*integer = read;
				return true;
			}
			return false;
		}

		bool WriteDword(std::uint32_t integer, int bits)
		{
			return Sentinel::g_Pointers->m_WriteBitbufDword(this, integer, bits);
		}

		bool ReadDword(std::uint32_t* integer, int bits)
		{
			return Sentinel::g_Pointers->m_ReadBitbufDword(this, integer, bits);
		}

		bool WriteInt32(std::int32_t integer, int bits) 
		{
			return Sentinel::g_Pointers->m_WriteBitbufInt32(this, integer, bits);
		}

		bool ReadInt32(std::int32_t* integer, int bits) 
		{
			std::int32_t v8;
			std::int32_t v9;
			if (ReadDword((std::uint32_t*)&v8, 1u) && ReadDword((std::uint32_t*)&v9, bits - 1)) {
				*integer = v8 + (v9 ^ -v8);
				return true;
			}
			return false;
		}

		bool WriteQWord(std::uint64_t integer, int bits) 
		{
			return Sentinel::g_Pointers->m_WriteBitbufQword(this, integer, bits);
		}

		bool ReadQWord(std::uint64_t* integer, int bits)
		{
			if (bits <= 32) {
				std::uint32_t v10;
				if (ReadDword(&v10, bits)) {
					*integer = v10;
					return true;
				}
			}
			else {
				std::uint32_t v10, v11;
				if (ReadDword(&v11, 32u) && ReadDword(&v10, bits - 32u)) {
					*integer = v11 | ((std::uint64_t)v10 << 32);
					return true;
				}
			}
			return false;
		}

		bool WriteInt64(std::int64_t integer, int bits)
		{
			return Sentinel::g_Pointers->m_WriteBitbufInt64(this, integer, bits);
		}

		bool ReadInt64(std::int64_t* integer, int bits)
		{
			std::uint32_t v8;
			std::uint64_t v9;
			if (ReadDword(&v8, 1u) && ReadQWord(&v9, bits - 1)) {
				*integer = v8 + (v9 ^ -(std::int64_t)v8);
				return true;
			}
			return false;
		}

		bool WriteArray(void* array, int size) 
		{
			return Sentinel::g_Pointers->m_WriteBitbufArray(this, array, size, 0);
		}

		bool ReadArray(PVOID array, int size) 
		{
			return Sentinel::g_Pointers->m_ReadBitbufArray(this, array, size, 0);
		}

		template<typename T>
		inline T Read(int length)
		{
			static_assert(sizeof(T) <= 4, "maximum of 32 bit read");

			std::uint32_t val = 0;
			ReadDword(&val, length);

			return T(val);
		}

		template<typename T>
		inline T ReadSigned(int length)
		{
			static_assert(sizeof(T) <= 4, "maximum of 32 bit read");

			int val = 0;
			ReadInt32(&val, length);

			return T(val);
		}

		template<typename T>
		inline void Write(T data, int length)
		{
			static_assert(sizeof(T) <= 8, "maximum of 64 bit write");

			WriteQWord((uint64_t)data, length);
		}

		template<typename T>
		inline void WriteSigned(int length, T data)
		{
			int sign = data < 0;
			int signEx = (data < 0) ? 0xFFFFFFFF : 0;
			int d = (data ^ signEx);

			Write<int>(1, sign);
			Write<int>(length - 1, d);
		}

		inline float ReadFloat(int length, float divisor)
		{
			auto integer = Read<int>(length);

			float max = (1 << length) - 1;
			return ((float)integer / max) * divisor;
		}

		inline void WriteFloat(int length, float divisor, float value)
		{
			float max = (1 << length) - 1;
			int integer = (int)((value / divisor) * max);

			Write<int>(length, integer);
		}

		inline float ReadSignedFloat(int length, float divisor)
		{
			auto integer = ReadSigned<int>(length);

			float max = (1 << (length - 1)) - 1;
			return ((float)integer / max) * divisor;
		}

		inline void WriteSignedFloat(int length, float divisor, float value)
		{
			float max = (1 << (length - 1)) - 1;
			int integer = (int)((value / divisor) * max);

			WriteSigned<int>(length, integer);
		}
	public:
		void* m_data; //0x0000
		std::uint32_t m_bitOffset; //0x0008
		std::uint32_t m_maxBit; //0x000C
		std::uint32_t m_bitsRead; //0x0010
		std::uint32_t m_curBit; //0x0014
		std::uint32_t m_highestBitsRead; //0x0018
		std::uint8_t m_flagBits; //0x001C
	};

	class netGameEvent
	{
	public:
		virtual ~netGameEvent() = default;

		virtual const char* get_name() { return 0; };
		virtual bool is_in_scope(netPlayer* player) { return 0; };
		virtual bool time_to_resend(std::uint32_t time) { return 0; };
		virtual bool can_change_scope() { return 0; };

		virtual void prepare_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player) {};
		virtual void handle_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player) {};

		virtual bool decide(netPlayer* source_player, netPlayer* target_player) { return 0; };

		virtual void prepare_reply(datBitBuffer* buffer, netPlayer* reply_player) {};
		virtual void handle_reply(datBitBuffer* buffer, netPlayer* souce_player) {};

		virtual void prepare_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2) {};
		virtual void handle_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2) {};

	private:
		virtual void unk_0x60() {};
		virtual void unk_0x68() {};
		virtual void unk_0x70() {};
		virtual void unk_0x78() {};
	public:
		virtual bool operator==(netGameEvent const& other) { return 0; };
		virtual bool operator!=(netGameEvent const& other) { return 0; };

		virtual bool must_persist() { return 0; };
		virtual bool must_persist_when_out_of_scope() { return 0; };
		virtual bool has_timed_out() { return 0; };
	public:
		std::uint16_t m_id;          // 0x08
		bool m_requires_reply;       // 0x0A
	private:
		char m_padding1[0x05];       // 0x0B
	public:
		netPlayer* m_source_player;  // 0x10
		netPlayer* m_target_player;  // 0x18
		std::uint32_t m_resend_time; // 0x20
	private:
		std::uint16_t m_0x24;        // 0x24
		std::uint8_t m_0x26;         // 0x26
		std::uint8_t m_0x27;         // 0x27
		std::uint32_t m_0x28;        // 0x28
		char m_padding2[0x04];
	};
#pragma pack(pop)

#pragma pack(push, 1)
	class CScriptedGameEvent : public netGameEvent
	{
	public:
		char m_padding[0x40];      // 0x30
		std::int64_t m_args[54];   // 0x70
		std::uint32_t m_bitset;    // 0x220
		std::uint32_t m_args_size; // 0x224
	};

	class CNetworkIncrementStatEvent : public netGameEvent
	{
	public:
		std::uint32_t m_stat;    // 0x30
		std::uint32_t m_amount; // 0x34
	};
#pragma pack(pop)
}