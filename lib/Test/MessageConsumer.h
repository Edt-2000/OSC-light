#pragma once

#include <IMessage.h>
#include <OSCStructMessage.h>
#include <OSCMessageConsumer.h>
#include <Data.h>

class MessageConsumer : public OSC::MessageConsumer
{
public:
	const char * _address;
	bool callbackCalled = false;
	Data messageStruct;

	MessageConsumer(const char *pattern = "/TEST")
	{
		_address = pattern;
	}

	const char *address()
	{
		return _address;
	}

	void callbackMessage(OSC::IMessage * message)
	{
		auto structMessage = (OSC::StructMessage<Data, uint32_t> *)message;

		messageStruct = structMessage->messageStruct;

		callbackCalled = true;
	}
};