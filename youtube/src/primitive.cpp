#include "primitive.h"
#include "core.h"


namespace ObjectModel
{

	Primitive::Primitive()
	{
		size += sizeof type;
	}


	void Primitive::pack(std::vector<int8_t>* buffer, int16_t* iterator)
	{
		Core::encode<int8_t>(buffer, iterator, wrapper);
		Core::encode<int16_t>(buffer, iterator, nameLength);
		Core::encode<std::string>(buffer, iterator, name);
		Core::encode<int8_t>(buffer, iterator, type);
		Core::encode<int8_t>(buffer, iterator, *data);
		Core::encode<int32_t>(buffer, iterator, size);
	}


	Primitive Primitive::unpack(const std::vector<int8_t>& buffer)
	{
		Primitive p;
		int16_t it = 0;
		p.wrapper =		Core::decode<int8_t>(buffer, it);
		p.nameLength =  Core::decode<int16_t>(buffer, it);
		p.name =		Core::decode<std::string>(buffer, it);
		p.type =		Core::decode<int8_t>(buffer, it);
		p.data = new std::vector<int8_t>(getTypeSize((Type)p.type));
						Core::decode(buffer, it, *p.data);
		p.size =		Core::decode<int32_t>(buffer, it);

		return p;
	}

	std::vector<int8_t> Primitive::getData()
	{
		return *data;
	}

}