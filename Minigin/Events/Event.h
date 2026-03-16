#pragma once
#include "GameObject.h"

namespace dae
{

	struct EventArg 
	{
		//union --> shares the same space
		union 
		{
			GameObject* gameObject;
			int value;
		};
		
	};

	using EventId = unsigned int;

	struct Event 
	{
		const EventId id;
		static const uint8_t MAX_ARGS = 8;
		uint8_t nbArgs;
		EventArg args[MAX_ARGS]{};

		explicit Event(EventId _id) : id{ _id } {}
	};
}