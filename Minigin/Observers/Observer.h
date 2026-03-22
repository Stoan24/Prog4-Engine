#pragma once
#include "GameObject.h"
#include <Events/Event.h>

namespace dae
{
	using EventId = unsigned int;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const Event& e) = 0;
	};
}