#pragma once
#include "Events/Event.h"

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const Event& e) = 0;
	};
}