#pragma once

// OSC light implementation for Arduino
#include "IMessage.h"
#include "OSCMessage.h"
#include "OSCStructMessage.h"
#include "OSCMessageConsumer.h"
#include "OSCMessageProducer.h"
#include "EndianMemcpy.h"

#include <iostream>
#include <memory>

#ifdef UNIT_TEST
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <UdpForTest.h>
#include <StreamForTest.h>
#endif
#else
#include <Udp.h>
#endif

namespace OSC
{
template <int numberOfConsumers, int numberOfProducers>
class Arduino
{
private:
	UDP *_udpHandle;
	Stream *_serialHandle;

	Match _matchHelper = Match();

	MessageConsumer *_oscConsumers[numberOfConsumers];
	MessageProducer *_oscProducers[numberOfProducers];

	IPAddress _remoteIP;
	int _remotePort;

	int _producers = 0;
	int _consumers = 0;

	bool _useUdp = false;
	bool _useSerial = false;

	static bool _isBigEndian()
	{
		const int one = 1;
		const char sig = *(char *)&one;

		return (sig == 0);
	}

	void _handleProducers(const bool send)
	{
		for (auto &producer : _oscProducers)
		{
			producer->loop();

			if (send)
			{
				IMessage *message = producer->generateMessage();

				if (message->isSendableMessage())
				{
					if (_useUdp)
					{
						_udpHandle->beginPacket(_remoteIP, _remotePort);
						message->send(_udpHandle);
						_udpHandle->endPacket();
					}
					if (_useSerial)
					{
						message->send(_serialHandle);
					}
				}
			}
		}
	}

	void _handleConsumers()
	{
		// then process all the messages in
		if (numberOfConsumers > 0)
		{
			if (_useUdp)
			{
				const auto size = _udpHandle->parsePacket();

				if (size > 0)
				{
					char buffer[size];

					// write udp data to buffer
					_udpHandle->read(buffer, size);

					_processMessage(buffer, size);
				}
			}

			if (_useSerial)
			{
				const auto size = _serialHandle->available();

				if (size > 0)
				{
					char buffer[size];

					// write serial data to buffer
					_serialHandle->readBytes(buffer, size);

					_processMessage(buffer, size);
				}
			}
		}
	}

	void _processMessage(const char *buffer, const int size, const bool platformIsBigEndian = _isBigEndian())
	{
		constexpr auto bundleFlag = "#bundle";

		if (strncmp(buffer, bundleFlag, 7) == 0)
		{
			uint32_t elementSize = 0;
			uint32_t offset = strlen(bundleFlag) + 1 + sizeof(uint64_t);

			while (size > offset + sizeof(uint32_t))
			{
				memcpy(((char *)&elementSize), buffer + offset, sizeof(uint32_t), platformIsBigEndian);

				if (size >= offset + sizeof(uint32_t) + elementSize)
				{
					_sendMessageToConsumers(buffer + offset + sizeof(uint32_t), elementSize);
				}

				offset += elementSize + sizeof(uint32_t);
			}
		}
		else
		{
			_sendMessageToConsumers(buffer, size);
		}
	}

	void _sendMessageToConsumers(const char *buffer, const int size)
	{
		char workBuffer[size];

		for (auto &consumer : _oscConsumers)
		{
			if (_matchHelper.isMatch(buffer, consumer->pattern()))
			{
				const auto message = consumer->message();

				if (message->process(size, buffer, workBuffer))
				{
					consumer->callbackMessage();
				}
			}
		}
	}

public:
	// binds UDP handle to receive and send OSC messages
	void bindUDP(UDP *udp, IPAddress remoteIP, int remotePort)
	{
		_udpHandle = udp;
		_remoteIP = remoteIP;
		_remotePort = remotePort;
		_useUdp = true;
	}

	void bindStream(Stream *serial)
	{
		_serialHandle = serial;
		_useSerial = true;
	}

	// adds an OSC message consumer
	void addConsumer(MessageConsumer *consumer)
	{
		_oscConsumers[_consumers++] = consumer;
	}

	// adds an OSC message producer
	void addProducer(MessageProducer *producer)
	{
		_oscProducers[_producers++] = producer;
	}

	// main loop which uses UDP to read messages and feeds the UDP data to each consumer and sends messages generated by producers
	void loop(const bool send = true)
	{
		_handleProducers(send);
		_handleConsumers();
	}
};
} // namespace OSC