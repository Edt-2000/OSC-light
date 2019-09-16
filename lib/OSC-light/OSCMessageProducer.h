#pragma once

#include "OSCMessage.h"

namespace OSC {
	template <class MessageType>
	class MessageProducer
	{
	public:
		virtual void loop() = 0;
		virtual MessageType * generateMessage() = 0;
	};
}