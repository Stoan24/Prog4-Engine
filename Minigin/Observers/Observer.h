#pragma once
#include "GameObject.h"

namespace dae
{
	using EventId = unsigned int;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(GameObject* gameObject, EventId event) = 0;
		virtual void OnSubjectDestroyed() {}
	};
}