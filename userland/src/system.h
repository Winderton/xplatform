#pragma once
#include <string>
#include <vector>
#include <serialization.h>

namespace EventSystem
{

	class Event;

	class System
	{
	private:
		friend class Event;
		std::string name;
		int32_t descriptor;
		int16_t index;
		bool active;
		std::vector<Event*> events;
	public:
		System(std::string);
		~System();
	public:
		void addEvent(Event*);
		Event* getEvent();
		void serialize();
	};
}