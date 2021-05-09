#include <serialization.h>
#include <cassert>
#include "primitive.h"



using namespace ObjectModel;



int main(int argc, char** argv)
{
	assert(Core::Util::isLittleEndian());

#if 0
	int32_t foo = 5;
	Primitive* p = Primitive::create("int32", Type::I32, foo);
	Core::Util::retriveNsave(p);

	std::vector<int64_t> data{ 1,2,3,4 };
	Array* arr = Array::createArray("array", Type::I64, data);
	Core::Util::retriveNsave(arr);

	std::string name = "name";
	Array* str = Array::createString("string", Type::I8, name);
	Core::Util::retriveNsave(str);


	Object Test("Test");
	Test.addEntitie(p);
	Test.addEntitie(arr);
	Test.addEntitie(str);

	Object Test2("Test2");
	Test2.addEntitie(p);
	Core::Util::retriveNsave(&Test2);

	Test.addEntitie(&Test2);
	Core::Util::retriveNsave(&Test);

#endif



	/*int16_t foo = 23;
	Primitive* p = Primitive::create("int16", Type::I16, foo);
	Core::Util::retriveNsave(p);




	std::vector<int8_t> result = Core::Util::load("int16.abc");
	int16_t it = 0;
	Primitive prim = Primitive::unpack(result, it);*/




	int32_t foo = 5;
	Primitive* p = Primitive::create("int32", Type::I32, foo);
	//Core::Util::retriveNsave(p);

	std::vector<int32_t> data{ 1,2,3,4 };
	Array* arr = Array::createArray("array", Type::I32, data);
	//Core::Util::retriveNsave(arr);

	std::string name = "name";
	Array* str = Array::createString("string", Type::I8, name);
	//Core::Util::retriveNsave(str);


	Object Test("Foo");
	Test.addEntity(p);
	Test.addEntity(arr);
	Test.addEntity(str);
	Core::Util::retriveNsave(&Test);

	Object Test2("Bar");
	Test2.addEntity(p);

	Test.addEntity(&Test2);

	Object win("Bazz");
	Primitive* winPrim = Primitive::create("int32", Type::I32, foo);
	win.addEntity(winPrim);
	Test.addEntity(&win);

	Core::Util::retriveNsave(&Test);


	int size = Test.getSize();
	std::vector<int8_t> objectFromFile = Core::Util::load("Foo.abc");

	int16_t it = 0;
	Object toPrintObject = Object::unpack(objectFromFile, it);
	std::cout << "Object: " + toPrintObject.getName() << std::endl;
	std::cout << "Last object in " << toPrintObject.getName() << ": " << toPrintObject.objects.back().getName() << std::endl;



	(void)argc;
	(void)argv;
	return 0;
}
