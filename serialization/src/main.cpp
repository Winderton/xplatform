#include "../include/serialization.h"
#include <cassert>
#include "../include/primitive.h"



using namespace ObjectModel;


namespace TestFrame
{
	void testPack()
	{
		int32_t foo = 231;
		std::unique_ptr<Primitive> p = Primitive::create("int32", Type::I32, foo);	

		Object Test("Foo");
		Test.addEntity(p.get());

		Core::Util::retriveNsave(&Test);
	}

	void testUnpack()
	{
		std::vector<uint8_t> objectFromFile = Core::Util::load("Foo.abc");

		[[maybe_unused]]int16_t it = 0;
		Object toPrintObject = Object::unpack(objectFromFile, it);

		[[maybe_unused]]int16_t it2 = 0;
		int w = Core::decode<int32_t>(toPrintObject.findPrimitiveByName("int32").getData(), it2);
		std::cout << w << std::endl;
	}
}

int main(int argc, char** argv)
{
	assert(Core::Util::isLittleEndian(5));

	TestFrame::testPack();
	TestFrame::testUnpack();

	(void)argc;
	(void)argv;
	return 0;
}
