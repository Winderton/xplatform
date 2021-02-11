#include "object.h"
#include "core.h"




namespace ObjectModel
{
	Object::Object(std::string name)
	{
		setName(name);
		wrapper = static_cast<int8_t>(Wrapper::OBJECT);
		size += sizeof count;
	}

	void Object::addEntitie(Root* r)
	{
		this->entities.push_back(r);
		count += 1;
		size += r->getSize();
	}


	Root* Object::findByName(std::string name)
	{
		for (auto r : entities)
		{
			if (r->getName() == name)
			{
				return r;
			}

		}
		std::cout << "no as such" << std::endl;
		return new Object("ninja");
	}

	void Object::pack(std::vector<int8_t>* buffer, int16_t* iterator)
	{
		Core::encode<std::string>(buffer, iterator, name);
		Core::encode<int16_t>(buffer, iterator, nameLength);
		Core::encode<int8_t>(buffer, iterator, wrapper);
		Core::encode<int16_t>(buffer, iterator, count);

		for (auto r : entities)
		{
			r->pack(buffer, iterator);
		}

		Core::encode<int32_t>(buffer, iterator, size);

	}
}