#pragma once

#include "OSCMessage.h"

namespace OSC {
	class IMessageProducer
	{
	public:
		virtual void loop() = 0;
		virtual Message * generateMessage() = 0;
	};
}