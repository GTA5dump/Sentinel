#pragma once

using Any = int;
using Hash = std::uint32_t;
using Entity = std::int32_t;
using Player = std::int32_t;
using FireId = std::int32_t;
using Interior = std::int32_t;
using Ped = Entity;
using Vehicle = Entity;
using Cam = std::int32_t;
using Object = Entity;
using Pickup = Object;
using Blip = std::int32_t;
using Camera = Entity;
using ScrHandle = Entity;

#pragma pack(push, 1)
	struct Vector2
	{
		float x{};
		float y{};
	};
#pragma pack(pop)

#pragma pack(push, 1)
	class NativeVector3
	{
	public:
		NativeVector3() = default;

		NativeVector3(float x, float y, float z) :
			x(x), y(y), z(z)
		{}
	public:
		float x{};
	private:
		char m_padding1[0x04];
	public:
		float y{};
	private:
		char m_padding2[0x04];
	public:
		float z{};
	private:
		char m_padding3[0x04];
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct Vector3
	{
		float x{};
		float y{};
		float z{};
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct Color
	{
		std::uint8_t r{ 255 };
		std::uint8_t g{ 255 };
		std::uint8_t b{ 255 };
		std::uint8_t a{ 255 };
	};
#pragma pack(pop)