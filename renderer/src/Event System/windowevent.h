#pragma once


#include "event.h"



namespace CoreNative
{
	class WindowResizedEvent : public Event
	{
	private:
		int width, height;
	public:
		WindowResizedEvent(int width, int height)
			:
			Event("Window resize event", Event::EventType::WINDOW_RESIZED_EVENT),
			width(width),
			height(height) {}
	public:
		std::string format() const override
		{
			std::string formatted = name + ": " + std::to_string(width) + ", " + std::to_string(height);
			return formatted;
		}
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent()
			:
			Event("Window Close Event", Event::EventType::WINDOW_CLOSED_EVENT) {}
	public:
		std::string format() const override
		{
			return name;
		}
	};
}
