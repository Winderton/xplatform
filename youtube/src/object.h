#pragma once
#include <iostream>
#include "root.h"


namespace ObjectModel
{
	class LIB Object : public Root
	{
	private:
		std::vector<Root*> entities;
		int16_t count = 0;
	public:
		Object(std::string);
		void addEntitie(Root* r);
		Root* findByName(std::string);
		void pack(std::vector<int8_t>*, int16_t*);
	};

}