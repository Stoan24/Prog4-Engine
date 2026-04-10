#include "EventManager.h"

void dae::EventManager::AddEvent(EventId id, Observer* observer)
{
	m_Observers[id].push_back(observer);
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

        auto eventIt = m_Observers.find(e.id);
        if (eventIt != m_Observers.end())
        {
            for (auto observer : eventIt->second)
            {
                observer->Notify(e);
            }
        }

        m_EventQueue.pop();
    }
}
