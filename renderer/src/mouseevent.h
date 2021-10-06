#pragma once
#include "event.h"



namespace CoreNative
{
	class MouseMovedEvent : public Event
	{
	private:
		int x, y;
	public:
		MouseMovedEvent(int x, int y)
			:
			Event("Mouse moved Event", Event::EventType::MOUSE_MOVED_EVENT),
			x(x),
			y(y) {}


	public:
		std::string format() const override
		{
			std::string formatted = name + ": " + std::to_string(x) + ", " + std::to_string(y);
			return formatted;
		}
	};



	class MouseScrolledEvent : public Event
	{
	private:
		int offset;
	public:
		MouseScrolledEvent(int offset)
			:
			Event("Mouse Scrolled Event", Event::EventType::MOUSE_SCROLLED_EVENT),
			offset(offset) {}
	public:
		std::string format() const override
		{
			std::string formatted = "";
			return (offset < 0) ? formatted = name + ": " + "to bottom" : formatted = name + ": " + "to top";
		}
	};



	class MouseButtonPressed : public Event
	{
	private:
		int button;
	public:
		MouseButtonPressed(int button)
			:
			Event("Mouse button pressed", Event::EventType::MOUSE_BUTTON_PRESSED_EVENT),
			button(button) {}
	public:
		std::string format() const override
		{
			std::string formatted = name + ": " + std::to_string(button);
			return formatted;
		}

	};


	class MouseButtonReleased : public Event
	{
	private:
		int button;
	public:
		MouseButtonReleased(int button)
			:
			Event("Mouse button released", Event::EventType::MOUSE_BUTTON_RELEASED_EVENT),
			button(button) {}
	public:
		std::string format() const override
		{
			std::string formatted = name + ": " + std::to_string(button);
			return formatted;
		}

	};


}