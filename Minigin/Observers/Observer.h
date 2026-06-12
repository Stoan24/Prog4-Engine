#pragma once
#include "Events/Event.h"

namespace dae
{
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void Notify(const Event& e) = 0;
	};
}