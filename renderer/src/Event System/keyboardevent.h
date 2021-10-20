#pragma once
#include "event.h"


namespace CoreNative
{
	class KeyPressedEvent : public Event
	{
	private:
		int key;
	public:
		KeyPressedEvent(int key)
			:
			Event("Key Pressed Event", Event::EventType::KEY_PRESSED_EVENT),
			key(key) {}
	public:
		std::string format() const override
		{
			std::string formatted = name + ": " + (char)key;
			return formatted;
		}
	};



	class KeyReleasedEvent : public Event
	{
	private:
		int key;
	public:
		KeyReleasedEvent(int key)
			:
			Event("Key Released Event", Event::EventType::KEY_RELEASED_EVENT),
			key(key) {}
	public:
		std::string format() const override
		{
			std::string formatted = name + ": " + (char)key;
			return formatted;
		}
	};

}