#pragma once
#include <string>
#include <vector>
#include "lib.h"
#include "meta.h"


namespace ObjectModel
{

	class LIB Root
	{
	protected:
		std::string name;
		int16_t nameLength;
		int8_t wrapper;
		int32_t size;
	public:
		Root();
	public:
		int32_t getSize();
		void setName(std::string);
		std::string getName();
		virtual void pack(std::vector<int8_t>*, int16_t*);
	};
}

