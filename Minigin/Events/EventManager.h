#pragma once
#include "Singleton.h"
#include <queue>
#include <map>
#include "Event.h"
#include "EventListener.h"

namespace dae
{

	class EventManager final : public Singleton<EventManager>
	{
	public:

		void AddEvent(EventId id, EventListener* listener);
		void HandleEvent(Event e);

		void Update();

	private:
		friend class Singleton<EventManager>;
		EventManager() = default;

		std::queue<Event> m_EventQueue;

		//Link events to multiple listeners
		std::map<EventId, std::vector<EventListener*>> m_Listeners;
	};
}