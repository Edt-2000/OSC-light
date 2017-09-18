#pragma once

#include "OSCMessage.h"

namespace OSC {
	class MessageProducer
	{
	public:
		virtual void loop() = 0;
		virtual Message * generateMessage() = 0;
	};
}