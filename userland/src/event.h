#pragma once
#include <iostream>
#include "system.h"



namespace EventSystem
{
	class Event
	{
	private:
		int32_t ID;
	public:
		enum DeviceType : int8_t
		{
			KEYBOARD = 1,
			MOUSE,
			TOUCHPAD,
			JOYSTICK
		};
		DeviceType dType;
		System* system = nullptr;
	public:
		Event(DeviceType);
		DeviceType getdType();
		int32_t getID();
		friend std::ostream& operator<<(std::ostream& stream, const DeviceType dType)
		{
			std::string result;
#define PRINT(a) result = #a;
			switch (dType)
			{
			case KEYBOARD: PRINT(KEYBOARD); break;
			case MOUSE: PRINT(MOUSE); break;
			case TOUCHPAD: PRINT(TOUCHPAD); break;
			case JOYSTICK: PRINT(JOYSTICK); break;
			}

			return stream << result;
		}
		void bind(System*, Event*);
		void serialize(ObjectModel::Object* o);
	};
}