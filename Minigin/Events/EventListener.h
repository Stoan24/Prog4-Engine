#pragma once
#include "Event.h"

namespace dae
{
	class EventListener
	{
	public:

		virtual ~EventListener() = default;
		virtual void OnEvent(const Event& e) = 0;
	};
}