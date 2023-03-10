#include "../include/object.h"
#include "../include/core.h"


namespace ObjectModel
{
	Object::Object(std::string name = "default")
	{
		setName(name);
		wrapper = static_cast<uint8_t>(Wrapper::OBJECT);
		size += (sizeof(int16_t)) * 4;
	}

	void Object::addEntity(Root* r)
	{
		switch (r->wrapper)
		{
		case 1: primitives.push_back(*dynamic_cast<Primitive*>(r)); primitiveCount += 1; break;
		case 2: arrays.push_back(*dynamic_cast<Array*>(r)); arrayCount += 1; break;
		case 3: strings.push_back(*dynamic_cast<Array*>(r)); stringCount += 1; break;
		case 4: objects.push_back(*dynamic_cast<Object*>(r)); objectCount += 1; break;
		}

		size += r->getSize();
	}




	void Object::pack(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Core::encode<uint8_t>(buffer, it, wrapper);
		Core::encode<int16_t>(buffer, it, nameLength);
		Core::encode<std::string>(buffer, it, name);

		// refactor this into std::vector<Entities*> entities;
		// for (auto e : entities) {e.pack(b,i};}
		Core::encode<int16_t>(buffer, it, primitiveCount);
		for (auto p : primitives)
		{
			p.pack(buffer, it);
		}

		Core::encode<int16_t>(buffer, it, arrayCount);
		for (auto arr : arrays)
		{
			arr.pack(buffer, it);
		}

		Core::encode<int16_t>(buffer, it, stringCount);
		for (auto str : strings)
		{
			str.pack(buffer, it);
		}

		Core::encode<int16_t>(buffer, it, objectCount);
		for (auto o : objects)
		{
			o.pack(buffer, it);
		}


		Core::encode<int32_t>(buffer, it, size);

	}

	Object Object::unpack(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Object obj;
		obj.wrapper = Core::decode<uint8_t>(buffer, it);
		obj.nameLength = Core::decode<int16_t>(buffer, it);
		obj.name = Core::decode<std::string>(buffer, it);

		// refactor this into:
		// for (auto e : entities) {e.entities.push_back(e.unpack(b,i));}
		obj.primitiveCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.primitiveCount; i++)
		{
			obj.primitives.push_back(Primitive::unpack(buffer, it));
		}

		obj.arrayCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.arrayCount; i++)
		{
			obj.arrays.push_back(Array::unpack(buffer, it));
		}

		obj.stringCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.stringCount; i++)
		{
			obj.strings.push_back(Array::unpackS(buffer, it));
		}

		obj.objectCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.objectCount; i++)
		{
			obj.objects.push_back(unpack(buffer, it));
		}

		obj.size = Core::decode<int32_t>(buffer, it);


		return obj;
	}
}