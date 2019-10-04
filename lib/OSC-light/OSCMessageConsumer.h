#pragma once

#include "IMessage.h"

namespace OSC
{
class MessageConsumer
{
public:
	virtual const char *pattern() = 0;
	virtual IMessage *message() = 0;
	virtual void callbackMessage() = 0;
};
} // namespace OSC