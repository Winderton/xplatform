#include "array.h"
#include "core.h"


namespace ObjectModel
{
	Array::Array()
	{
		size += sizeof type + sizeof count;
	}

	void Array::pack(std::vector<int8_t>* buffer, int16_t* iterator)
	{
		Core::encode<std::string>(buffer, iterator, name);
		Core::encode<int16_t>(buffer, iterator, nameLength);
		Core::encode<int8_t>(buffer, iterator, wrapper);
		Core::encode<int8_t>(buffer, iterator, type);
		Core::encode<int32_t>(buffer, iterator, count);
		Core::encode<int8_t>(buffer, iterator, *data);
		Core::encode<int32_t>(buffer, iterator, size);
	}
}