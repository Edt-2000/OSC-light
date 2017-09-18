#pragma once

#include "OSCMessage.h"

// TODO: implement same struct based message sending for producer

namespace OSC {
	class MessageProducer
	{
	public:
		virtual void loop() = 0;
		virtual Message * generateMessage() = 0;
	};
}