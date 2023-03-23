#pragma once
#include "root.h"
#include <memory>
#include "core.h"


namespace ObjectModel
{
	class Primitive : public Root
	{
	private:
		uint8_t type = 0;
		std::vector<uint8_t>* data = nullptr;
	private:
		Primitive();
	public:
		template<typename T>
		static std::unique_ptr<Primitive> create(std::string name, Type type, T value)
		{
			std::unique_ptr<Primitive> p(new Primitive());
			p->setName(name);
			p->wrapper = static_cast<uint8_t>(Wrapper::PRIMITIVE);
			p->type = static_cast<uint8_t>(type);
			p->data = new std::vector<uint8_t>(sizeof value);
			p->size += (int32_t)p->data->size();
			int16_t iterator = 0;
			Core::encode<T>(*p->data, iterator, value);

			return p;
		}

		void pack(std::vector<uint8_t>&, int16_t&);
		static Primitive unpack(const std::vector<uint8_t>&, int16_t&);

		std::vector<uint8_t> getData();
		std::vector<uint8_t>* getPtrData() {return data;}

	};



}