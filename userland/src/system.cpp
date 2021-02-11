#include "system.h"
#include "keyboardevent.h"



namespace EventSystem
{
	System::System(std::string name)
		:
		name(name),
		descriptor(123),
		index(1),
		active(true) {}

	System::~System()
	{
		//TODO::
	}


	void System::addEvent(Event* e)
	{
		e->bind(this, e);
	}


	Event* System::getEvent()
	{
		return events.front();
	}




	void System::serialize()
	{
		ObjectModel::Object system("SysInfo");
		ObjectModel::Array* name = ObjectModel::Array::createString("sysName", ObjectModel::Type::I8, this->name);
		ObjectModel::Primitive* desc = ObjectModel::Primitive::create("desc", ObjectModel::Type::I32, this->descriptor);
		ObjectModel::Primitive* index = ObjectModel::Primitive::create("index", ObjectModel::Type::I16, this->index);
		ObjectModel::Primitive* active = ObjectModel::Primitive::create("active", ObjectModel::Type::BOOL, this->active);
		system.addEntitie(name);
		system.addEntitie(desc);
		system.addEntitie(index);
		system.addEntitie(active);

		for (Event* e : events)
		{
			KeyboardEvent* kb = static_cast<KeyboardEvent*>(e);
			ObjectModel::Object* eventObject = new ObjectModel::Object("Event: " + std::to_string(e->getID()));
			kb->serialize(eventObject);
			system.addEntitie(eventObject);
		}

		Core::Util::retriveNsave(&system);
	}


}