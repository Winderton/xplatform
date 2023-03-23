#pragma once
#include <bitset>
#include <fstream>
#include <vector>
#include "root.h"


namespace Core
{
	namespace Util
	{
		bool isLittleEndian(uint8_t);
		void save(const char*, std::vector<uint8_t>& vector);
		std::vector<uint8_t> load(const char*);
		void retriveNsave(ObjectModel::Root* r);

	}

	// 0 1 2 3
	// 0x00 0x00 0x01 0xd3
	template<typename T>
	void encode(std::vector<uint8_t>& buffer, int16_t& iterator, T value)
	{
		for (unsigned i = 0, j = 0; i < sizeof (T); i++)
		{
			buffer[(iterator)++] = (value >> ((sizeof (T) * 8) - 8) - ((i == 0) ? j : j += 8));
		}
	}


	template<>
	inline void encode<float>(std::vector<uint8_t>& buffer, int16_t& iterator, float value)
	{
		int32_t result = *reinterpret_cast<int32_t*>(&value);
		encode<int32_t>(buffer, iterator, result);
	}

	template<>
	inline void encode<double>(std::vector<uint8_t>& buffer, int16_t& iterator, double value)
	{
		int64_t result = *reinterpret_cast<int64_t*>(&value);
		encode<int64_t>(buffer, iterator, result);
	}

	template<>
	inline void encode<std::string>(std::vector<uint8_t>& buffer, int16_t& iterator, std::string value)
	{
		for (unsigned i = 0; i < value.size(); i++)
		{
			encode<uint8_t>(buffer, iterator, value[i]);
		}
	}

	template<typename T>
	void encode(std::vector<uint8_t>& buffer, int16_t& iterator, std::vector<T> value)
	{
		for (unsigned i = 0; i < value.size(); i++)
		{
			encode<T>(buffer, iterator, value[i]);
		}
	}


	//deserialize


	template<typename T>
	T decode(const std::vector<uint8_t>& buffer, int16_t& it)
	{
		T result = 0;

		
		for (unsigned i = 0; i < sizeof(T); i++)
		{
			T temp = (T)buffer[it++] << (((sizeof(T) * 8) - 8) - (i * 8));
			result = result | temp;
		}


		return result;
	}
	

	template<>
	inline std::string decode<std::string>(const std::vector<uint8_t>& buffer, int16_t& it)
	{
		it -= 2;
		int16_t size = decode<int16_t>(buffer, it);

		std::string result((buffer.begin() + it), (buffer.begin() + (it + size)));
		it += size;

		return result;

	}


	template<typename ...>
	void decode(const std::vector<uint8_t>& buffer, int16_t& it, std::vector<uint8_t>& dest)
	{
		for (unsigned i = 0; i < dest.size(); i++)
		{
			dest[i] = buffer[it++];
		}
	}


} 