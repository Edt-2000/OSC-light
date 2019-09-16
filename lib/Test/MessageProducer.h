#pragma once

#include <OSCStructMessage.h>
#include <OSCMessageProducer.h>
#include <Data.h>

class MessageProducer : public OSC::MessageProducer<OSC::StructMessage<Data, uint32_t>>
{
public:
	OSC::StructMessage<Data, uint32_t> message;

	MessageProducer()
	{
	}

	void loop()
	{
		message.setValidData(true);
	}

	OSC::StructMessage<Data, uint32_t> *generateMessage()
	{
		message.setValidData(true);
		message.setAddress("/TEST");
		return &message;
	}
};