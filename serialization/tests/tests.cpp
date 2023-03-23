#include "../include/serialization.h"
#include <gtest/gtest.h>
#include "gmock/gmock.h"


using ::testing::StartsWith;


TEST(Core, primitive)
{
  using namespace ObjectModel;

  uint8_t a = 5;
  int32_t foo = 150;
  std::unique_ptr<Primitive> p = Primitive::create("int32", Type::I32, foo);

  int16_t it = 0;
  std::vector<uint8_t> buffer(p->getSize());
  p->pack(buffer, it);
  std::string str(buffer.begin(), buffer.end());


  EXPECT_TRUE(Core::Util::isLittleEndian(a)) << "Something bad with endianess";

  EXPECT_NE(p->getPtrData(), nullptr);
  EXPECT_THAT(str, StartsWith("\x1\0\x5"));
  EXPECT_STREQ("int32", p->getName().c_str());
  EXPECT_EQ(17, p->getSize());
}

TEST(Core, object)
{
  using namespace ObjectModel;

  int32_t foo = 231;
	std::unique_ptr<Primitive> p = Primitive::create("int32", Type::I32, foo);

  int64_t bar = 1;
	std::unique_ptr<Primitive> p2 = Primitive::create("int64", Type::I64, bar);

  std::vector<int16_t> data{5,10,15,20};
	std::unique_ptr<Array> arr = Array::createArray("ArrayOfInt16", Type::I16, data);

  std::string name = "wndtn";
	std::unique_ptr<Array> str = Array::createString("String", Type::I8, name);

  Object obj("Foo");

  obj.addEntity(p.get());
  obj.addEntity(p2.get());
  obj.addEntity(arr.get());
  obj.addEntity(str.get());

  Object obj2("Bar");
  obj2.addEntity(&obj);

  Core::Util::retriveNsave(&obj2);

  EXPECT_EQ((int16_t)2, obj.getPrimitiveCount());
  EXPECT_EQ((int16_t)1, obj.getArrayCount());
  EXPECT_EQ((int16_t)1, obj.getStringCount());
  EXPECT_EQ((int16_t)1, obj2.getObjectCount());


}

