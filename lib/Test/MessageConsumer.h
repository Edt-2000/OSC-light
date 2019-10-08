#pragma once

#include <IMessage.h>
#include <OSCStructMessage.h>
#include <OSCMessageConsumer.h>
#include <Data.h>

class MessageConsumer : public OSC::MessageConsumer
{
public:
	OSC::StructMessage<Data, uint32_t> _message;
	bool callbackCalled = false;

	MessageConsumer(const char *pattern = "/TEST")
	{
		_message.setAddress(pattern);
	}

	const char *address()
	{
		return _message.address;
	}

	OSC::IMessage *message()
	{
		return &_message;
	}

	void callbackMessage()
	{
		callbackCalled = true;
	}
};