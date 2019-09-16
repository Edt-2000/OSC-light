#pragma once

#include <OSCStructMessage.h>
#include <OSCMessageConsumer.h>
#include <Data.h>

class MessageConsumer : public OSC::MessageConsumer<OSC::StructMessage<Data, uint32_t>>
{
public:
	Data data;

	MessageConsumer()
	{
	}

	const char *pattern()
	{
		return "/TEST";
	}

	void callbackMessage(OSC::StructMessage<Data, uint32_t> *msg)
	{
		data = Data(msg->messageStruct);
	}
};