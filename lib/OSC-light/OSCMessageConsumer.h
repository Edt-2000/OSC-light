#pragma once

#include "IMessage.h"

namespace OSC
{
class MessageConsumer
{
public:
	virtual const char *address() = 0;
	virtual void callbackMessage(IMessage *message) = 0;
};
} // namespace OSC