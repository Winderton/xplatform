#include <serialization.h>
#include <cassert>
#include "primitive.h"



using namespace ObjectModel;


namespace TestFrame
{
	void testPack()
	{
		int32_t foo = 5;
		Primitive* p = Primitive::create("int32", Type::I32, foo);
		Primitive* winPrim = Primitive::create("int32", Type::I32, foo);

		std::vector<int32_t> data{ 1,2,3,4 };
		Array* arr = Array::createArray("array", Type::I32, data);

		std::string name = "name";
		Array* str = Array::createString("string", Type::I8, name);


		Object Test("Foo");
		Test.addEntity(p);
		Test.addEntity(arr);
		Test.addEntity(str);

		Object Test2("Bar");
		Test2.addEntity(p);
		Test.addEntity(&Test2);

		Object win("Bazz");
		win.addEntity(winPrim);
		Test.addEntity(&win);

		Core::Util::retriveNsave(&Test);
	}

	void testUnpack()
	{
		std::vector<int8_t> objectFromFile = Core::Util::load("Foo.abc");

		int16_t it = 0;
		Object toPrintObject = Object::unpack(objectFromFile, it);
		std::cout << "Object: " + toPrintObject.getName() << std::endl;
		std::cout << "Last object in " << toPrintObject.getName() << ": " << toPrintObject.objects.back().getName() << std::endl;
		std::cout << "Size of 'Bar' object: " << toPrintObject.findByName("Bar")->getSize() << std::endl;
	}
}

int main(int argc, char** argv)
{
	assert(Core::Util::isLittleEndian());

	TestFrame::testPack();
	TestFrame::testUnpack();

	(void)argc;
	(void)argv;
	return 0;
}
