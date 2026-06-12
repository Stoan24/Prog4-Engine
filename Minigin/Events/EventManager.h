#pragma once
#include "Singleton.h"
#include "Observers/Observer.h"
#include "Event.h"
#include <queue>
#include <vector>
#include <map>

namespace dae
{

	class EventManager final : public Singleton<EventManager>
	{
	public:

		void AddEvent(EventId id, IObserver* listener);
		void RemoveObserver(EventId id, IObserver* observer);
		void HandleEvent(Event e);

		void Update();

	private:
		friend class Singleton<EventManager>;
		EventManager() = default;

		std::queue<Event> m_EventQueue;

		//Link events to multiple listeners
		std::map<EventId, std::vector<IObserver*>> m_Observers;
	};
}