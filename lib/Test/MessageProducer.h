#pragma once

#include <IMessage.h>
#include <OSCStructMessage.h>
#include <OSCMessageProducer.h>
#include <Data.h>

class MessageProducer : public OSC::MessageProducer
{
public:
	OSC::StructMessage<Data, uint32_t> _message;

	MessageProducer()
	{
	}

	void loop()
	{
		_message.setValidData(true);
	}

	OSC::IMessage *generateMessage()
	{
		_message.setValidData(true);
		_message.setAddress("/TEST");
		return &_message;
	}
};