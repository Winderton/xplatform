#pragma once
#include "root.h"
#include <memory>
#include "core.h"

namespace ObjectModel
{
	class Array : public Root
	{
	private:
		uint8_t type = 0;
		static int32_t count;
		std::vector<uint8_t>* data = nullptr;
	public:
		Array();
	public:
		template<typename T>
		static std::unique_ptr<Array> createArray(std::string name, Type type, std::vector<T> value)
		{
			std::unique_ptr<Array> arr = std::make_unique<Array>();
			arr->setName(name);
			arr->wrapper = static_cast<uint8_t>(Wrapper::ARRAY);
			arr->type = static_cast<uint8_t>(type);
			arr->count = (int32_t)value.size();
			arr->data = new std::vector<uint8_t>(sizeof(T) * arr->count);
			arr->size += (int32_t)(value.size()) * sizeof(T);
			int16_t iterator = 0;
			Core::encode<T>(*arr->data, iterator, value);

			return arr;
		}


		template<typename T>
		static std::unique_ptr<Array> createString(std::string name, Type type, T value)
		{
			std::unique_ptr<Array>str = std::make_unique<Array>();
			str->setName(name);
			str->wrapper = static_cast<uint8_t>(Wrapper::STRING); 
			str->type = static_cast<uint8_t>(type);
			str->count = (int32_t)value.size();
			str->data = new std::vector<uint8_t>(value.size());
			str->size += (int32_t)value.size();
			int16_t iterator = 0;
			Core::encode<T>(*str->data, iterator, value);


			return str;
		}
		void pack(std::vector<uint8_t>&, int16_t&);
		static Array unpack(std::vector<uint8_t>& buffer, int16_t&);
		static Array unpackS(std::vector<uint8_t>& buffer, int16_t&);
	};
}