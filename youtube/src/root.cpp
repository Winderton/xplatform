#include "root.h"


namespace ObjectModel
{
	Root::Root()
		:
		name("unknown"),
		wrapper(0),
		nameLength(0),
		size(sizeof nameLength + sizeof wrapper + sizeof size) {}


	void Root::setName(std::string name)
	{
		this->name = name;
		nameLength = (int16_t)name.length();
		size += nameLength;
	}


	int32_t Root::getSize()
	{
		return size;
	}

	void Root::pack(std::vector<int8_t>*, int16_t*)
	{
		//maybe pure virtual??
	}




	std::string Root::getName()
	{
		return name;
	}
}