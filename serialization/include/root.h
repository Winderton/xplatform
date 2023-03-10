#pragma once
#include <string>
#include <vector>
#include "lib.h"
#include "meta.h"


namespace ObjectModel
{

	class Root
	{
	public:
		uint8_t wrapper;
	protected:
		mutable int16_t nameLength;
		mutable std::string name;
		mutable int32_t size;
	public:
		Root()
			:
			name("unknown"),
			wrapper(0),
			nameLength(0),
			size(sizeof nameLength + sizeof wrapper + sizeof size) {}
	public:
		inline int32_t getSize() const { return size; }

		void setName(std::string name) const
		{
			this->name = name;
			nameLength = (int16_t)name.length();
			size += nameLength;
		}

		inline std::string getName() const { return name; }

		virtual void pack(std::vector<uint8_t>&, int16_t&) = 0;
	};
}

