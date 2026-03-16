#pragma once
#include "Observers/Observer.h"
#include <vector>
#include <algorithm>

namespace dae
{
	class Subject
	{
	public:
		explicit Subject(int observerAmount)
		{
			m_pObservers.reserve(observerAmount);
		}

		virtual ~Subject() {
			for (auto observer : m_pObservers) 
			{
				observer->OnSubjectDestroyed();
			}
		}

		void addObserver(Observer* observer)
		{
			m_pObservers.emplace_back(observer);
		}

		void removeObserver(Observer* observer)
		{
			std::erase(m_pObservers, observer);
		}

		void NotifyObservers(GameObject* gameObject, EventId event)
		{
			for (auto observer : m_pObservers)
			{
				observer->Notify(gameObject, event);
			}
		}

	private:
		std::vector<Observer*> m_pObservers;
	};
}