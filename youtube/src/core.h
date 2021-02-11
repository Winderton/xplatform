#pragma once
#include <bitset>
#include <fstream>
#include <vector>
#include "root.h"


namespace Core
{
	namespace Util
	{
		bool LIB isLittleEndian();
		void save(const char*, std::vector<int8_t> vector);
		void LIB retriveNsave(ObjectModel::Root* r);

	}

	// 0 1 2 3
	// 0x00 0x00 0x00 0x5
	template<typename T>
	void encode(std::vector<int8_t>* buffer, int16_t* iterator, T value)
	{
		for (unsigned i = 0, j = 0; i < sizeof T; i++)
		{
			(*buffer)[(*iterator)++] = (value >> ((sizeof T * 8) - 8) - ((i == 0) ? j : j += 8));
		}
	}


	template<>
	inline void encode<float>(std::vector<int8_t>* buffer, int16_t* iterator, float value)
	{
		int32_t result = *reinterpret_cast<int32_t*>(&value);
		encode<int32_t>(buffer, iterator, result);
	}

	template<>
	inline void encode<double>(std::vector<int8_t>* buffer, int16_t* iterator, double value)
	{
		int64_t result = *reinterpret_cast<int64_t*>(&value);
		encode<int64_t>(buffer, iterator, result);
	}

	template<>
	inline void encode<std::string>(std::vector<int8_t>* buffer, int16_t* iterator, std::string value)
	{
		for (unsigned i = 0; i < value.size(); i++)
		{
			encode<int8_t>(buffer, iterator, value[i]);
		}
	}

	template<typename T>
	void encode(std::vector<int8_t>* buffer, int16_t* iterator, std::vector<T> value)
	{
		for (unsigned i = 0; i < value.size(); i++)
		{
			encode<T>(buffer, iterator, value[i]);
		}
	}

}