#include "../include/core.h"


namespace Core
{

	namespace Util
	{

		bool isLittleEndian(uint8_t a)
		{
			// 0x00 0x00 0x00 0b0000 0101
			std::string result = std::bitset<8>(a).to_string();
			return (result.back() == '1') ? true : false;
		}

		void save(const char* file, std::vector<uint8_t>& buffer)
		{
			std::ofstream out;
			out.open(file);

			for (unsigned i = 0; i < buffer.size(); i++)
			{
				out << buffer[i];
			}

			out.close();
		}

		std::vector<uint8_t> load(const char* path)
		{
			std::ifstream in(path, std::ios::binary);
			std::vector<uint8_t> result((std::istreambuf_iterator<char>(in)),(std::istreambuf_iterator<char>()));
			return result;
		}


		void retriveNsave(ObjectModel::Root* r)
		{
			int16_t iterator = 0;
			std::vector<uint8_t> buffer(r->getSize());
			std::string name = r->getName().substr(0, r->getName().length()).append(".abc");
			r->pack(buffer, iterator);
			save(name.c_str(), buffer);
		}
	}
}