#pragma once
#include <iostream>
#include "primitive.h"
#include "array.h"
#include "object.h"

namespace ObjectModel
{
	class LIB Object : public Root
	{
	public:
		int16_t primitiveCount = 0, arrayCount = 0, stringCount = 0, objectCount = 0;
		std::vector<Primitive> primitives;
		std::vector<Array> arrays;
		std::vector<Array> strings;
		std::vector<Object> objects;
	public:
		Object(std::string);
		void addEntity(Root*);
		void pack(std::vector<int8_t>&, int16_t&);
		static Object unpack(std::vector<int8_t>&, int16_t&);
		Root* findByName(std::string name)
		{
			for (Object o : objects)
			{
				if (o.getName() == name)
				{
					return dynamic_cast<Object*>(&o);
				}
			}

			for (Primitive p : primitives)
			{
				if (p.getName() == name)
				{
					return dynamic_cast<Primitive*>(&p);
				}
			}

			for (Array arr : arrays)
			{
				if (arr.getName() == name)
				{
					return dynamic_cast<Array*>(&arr);
				}
			}

			for (Array str : strings)
			{
				if (str.getName() == name)
				{
					return dynamic_cast<Array*>(&str);
				}
			}

			std::cout << "no as such" << std::endl;
			return new Object("SYSTEM:empty");

		}
	};

}