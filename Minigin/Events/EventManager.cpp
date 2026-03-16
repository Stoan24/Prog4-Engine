#include "EventManager.h"

void dae::EventManager::AddEvent(EventId id, EventListener* listener)
{
	m_Listeners[id].push_back(listener);
}

void dae::EventManager::HandleEvent(Event e)
{
	m_EventQueue.push(e);
}

void dae::EventManager::Update()
{
	while (!m_EventQueue.empty())
	{
		Event e = m_EventQueue.front();


		//Find the listeners of that specific Event, call their OnEvent
		auto eventIt = m_Listeners.find(e.id);

		if (eventIt != m_Listeners.end())
		{
			for (auto listener : eventIt->second)
			{
				listener->OnEvent(e);
			}
		}
		
		m_EventQueue.pop();
	}
}
