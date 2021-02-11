#pragma once
#include "event.h"



namespace EventSystem
{
	class KeyboardEvent : public Event
	{
	private:
		int16_t keyCode;
		bool pressed;
		bool released;
	public:
		KeyboardEvent(int16_t, bool, bool);
		void serialize(ObjectModel::Object* o);
	};
}