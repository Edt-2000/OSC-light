#include "stdafx.h"
#include "CppUnitTest.h"

#include "Print.h"

#include "..\OSC-light\IMessage.h"
#include "..\OSC-light\OSCMessage.h"
#include "..\OSC-light\OSCStructMessage.h"
#include "..\OSC-light\OSCMatch.h"
#include "..\OSC-light\OSCArduino.h"

#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OSClightUnitTest
{
	struct Simple
	{
		uint8_t int0 : 8;
	};

	class TestDevice : public OSC::MessageConsumer<OSC::StructMessage<Simple, uint8_t>>
	{
	private:
		const char *_pattern;
		bool _state = false;

	public:
		TestDevice(const char *pattern) : MessageConsumer()
		{
			_pattern = pattern;
		}

		const char *pattern()
		{
			return _pattern;
		}

		void callbackMessage(OSC::StructMessage<Simple, uint8_t> *message)
		{
			int0 = message->messageStruct.int0;
		}

		uint8_t int0;
	};

	TEST_CLASS(OSCSimpleMessageTests)
	{
	public:


		TEST_METHOD(OSCSimleMessageTest) {

			Stream stream = Stream();
			Stream print = Stream();

			TestDevice device = TestDevice("/TD");

			auto osc = OSC::Arduino<OSC::StructMessage<Simple, uint8_t>>(1, 0);
			//osc.bindUDP(&udp, IPAddress(10, 0, 0, 1), 12345);
			osc.bindStream(&print);
			osc.addConsumer(&device);

			std::string testString = "/TD_,i_____\x9";
			int length = testString.length();

			stream.write(testString.c_str(), length);
			print.write(stream.reversedBuffer, length);

			osc.loop();

			Assert::AreEqual((uint8_t)9u, device.int0, L"Integers not equal", LINE_INFO());

		}
	};
}