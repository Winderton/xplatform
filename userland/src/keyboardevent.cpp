#include "keyboardevent.h"


namespace EventSystem
{
	KeyboardEvent::KeyboardEvent(int16_t keyCode, bool pressed, bool released)
		:
		Event(Event::KEYBOARD),
		keyCode(keyCode),
		pressed(pressed),
		released(released) {}


	void KeyboardEvent::serialize(ObjectModel::Object* o)
	{
		Event::serialize(o);
		ObjectModel::Primitive* keyCode = ObjectModel::Primitive::create("keyCode", ObjectModel::Type::I16, this->keyCode);
		ObjectModel::Primitive* pressed = ObjectModel::Primitive::create("pressed", ObjectModel::Type::BOOL, this->pressed);
		ObjectModel::Primitive* released = ObjectModel::Primitive::create("released", ObjectModel::Type::BOOL, this->released);
		o->addEntitie(keyCode);
		o->addEntitie(pressed);
		o->addEntitie(released);
	}
}