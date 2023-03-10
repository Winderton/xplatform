#include "../include/array.h"
#include "core.h"


namespace ObjectModel
{

	int Array::count = 0;

	Array::Array()
	{
		size += sizeof type + sizeof count;
	}

	void Array::pack(std::vector<uint8_t>& buffer, int16_t& iterator)
	{
		Core::encode<uint8_t>(buffer, iterator, wrapper);
		Core::encode<int16_t>(buffer, iterator, nameLength);
		Core::encode<std::string>(buffer, iterator, name);
		Core::encode<uint8_t>(buffer, iterator, type);
		Core::encode<int32_t>(buffer, iterator, count);
		Core::encode<uint8_t>(buffer, iterator, *data);
		Core::encode<int32_t>(buffer, iterator, size);
	}


	Array Array::unpack(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Array arr;
		arr.wrapper = Core::decode<uint8_t>(buffer, it);
		arr.nameLength = Core::decode<int16_t>(buffer, it);
		arr.name = Core::decode<std::string>(buffer, it);
		arr.type = Core::decode<uint8_t>(buffer, it);
		arr.count = Core::decode<int32_t>(buffer, it);
		arr.data = new std::vector<uint8_t>(getTypeSize((Type)arr.type) * count);
		Core::decode(buffer, it, *arr.data);
		arr.size = Core::decode<int32_t>(buffer, it);


		return arr;
	}


	Array Array::unpackS(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Array str;
		str.wrapper = Core::decode<uint8_t>(buffer, it);
		str.nameLength = Core::decode<int16_t>(buffer, it);
		str.name = Core::decode<std::string>(buffer, it);
		str.type = Core::decode<uint8_t>(buffer, it);
		str.count = Core::decode<int32_t>(buffer, it);
		str.data = new std::vector<uint8_t>(str.count);
		Core::decode(buffer, it, *str.data);
		str.size = Core::decode<int32_t>(buffer, it);


		return str;
	}
}

