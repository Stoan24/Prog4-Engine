#pragma once
#include "Observer.h"
#include "Events/Event.h"
#include <vector>

namespace dae
{
	class Subject
	{
	public:
		explicit Subject(int observerAmount)
		{
			m_pObservers.reserve(observerAmount);
		}

		virtual ~Subject() {}

		void AddObserver(Observer* observer)
		{
			m_pObservers.emplace_back(observer);
		}

		void RemoveObserver(Observer* observer)
		{
			if (!m_pObservers.empty())
			{
				std::erase(m_pObservers, observer);
			}
		}

		void NotifyObservers(GameObject* gameObject, EventId event)
		{
			Event e(event);
			e.nbArgs = 1;
			e.args[0].gameObject = gameObject;

			for (auto observer : m_pObservers)
			{
				observer->Notify(e);
			}
		}

	private:
		std::vector<Observer*> m_pObservers{};
	};
}