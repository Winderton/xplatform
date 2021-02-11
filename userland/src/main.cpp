#include "eventsystem.h"
#include <serialization.h>
#include <cassert>



using namespace EventSystem;
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

#if 0

	System Foo("Foo");
	Event* e = new KeyboardEvent('a', true, false);

	Foo.addEvent(e);
	KeyboardEvent* kb = static_cast<KeyboardEvent*>(Foo.getEvent());


	Foo.serialize();


#endif

	int16_t foo = 23;
	Primitive* p = Primitive::create("int16", Type::I16, foo);
	Core::Util::retriveNsave(p);


	std::vector<int8_t> result = Core::Util::load("int16.abc");

	Primitive pp = Primitive::unpack(result);


	(void)argc;
	(void)argv;
	return 0;
}
