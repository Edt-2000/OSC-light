#include "stdafx.h"
#include "CppUnitTest.h"

#include "Print.h"

#include "..\OSC-light\OSCMessage.h"
#include "..\OSC-light\OSCMessage.cpp"
#include "..\OSC-light\OSCMatch.h"

#include "..\OSC-light\OSCArduino.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OSClightTest
{
	class OSCProducerConsumer : public OSC::IMessageConsumer, public OSC::IMessageProducer
	{
	public:
		OSC::Message message;

		OSCProducerConsumer() {
			message = OSC::Message();
			message.setAddress("/Some/Test/Address");
			message.reserveAtLeast(2);
			message.add(32676);
			message.add(123.456f);
		}

		void loop() {

		}

		OSC::Message * generateMessage() {
			return &message;
		}

		const char * address() {
			return message.address;
		}

		void callback(OSC::Message * msg) {
			
		}
	};

	TEST_CLASS(OSClightTest)
	{
	public:

		Print print = Print();
		OSC::Match tester = OSC::Match();
		OSC::Message message = OSC::Message();
		OSC::Message newMessage = OSC::Message();
		OSCProducerConsumer prodCons = OSCProducerConsumer();

		OSC::Arduino OSC;
		UDP Udp;

		TEST_METHOD(OSCMatchTest) {
			Assert::IsTrue(tester.isMatch("/Unit1", "/Unit1"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit1/Preset"));
			Assert::IsFalse(tester.isMatch("/Unit1", "/Unit2"));
			Assert::IsFalse(tester.isMatch("/Unit1/Preset", "/Unit2"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/a", "/Unit1/Preset/a"));
			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit_2345"));
			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit_2_45"));
			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit_2_4_"));
			Assert::IsFalse(tester.isMatch("/Unit12345", "/Unit_2_3_"));
			Assert::IsFalse(tester.isMatch("/Unit12345", "/Unit_2_1_"));
			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit_2___"));
			Assert::IsTrue(tester.isMatch("/MegaUnitMegaUnitMegaUnitMegaUnit", "/MegaUnit_egaUnitMegaUnitMeg_Unit"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit_/Preset"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/_n_t_/Preset"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit_/Preset"));
			Assert::IsTrue(tester.isMatch("/Unit2/Preset/a", "/Unit_/Preset/a"));
			Assert::IsTrue(tester.isMatch("/Unit1", "/*"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit1/*"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/a", "/Unit1/*/a"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/a", "/Unit1/Preset/*"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/a", "/*/Preset/a"));
			Assert::IsFalse(tester.isMatch("/Unit1/Preset", "/Unit1/*/a"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/a/b/c", "/Unit1/*/a/*/c"));
			Assert::IsFalse(tester.isMatch("/Unit1/Preset/a/b/d", "/Unit1/*/a/*/c"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/a/c/a", "/*/*/a/*/*"));
		}

		TEST_METHOD(OSCMessageMatchTest) {
			message.setAddress("/M");

			Assert::IsTrue(message.isValidRoute("/M"));

			message.setAddress("/New/Address");

			Assert::IsFalse(message.isValidRoute("/M"));
			Assert::IsTrue(message.isValidRoute("/New/Address"));

			message.setAddress("/M");

			Assert::IsTrue(message.isValidRoute("/M"));
			Assert::IsFalse(message.isValidRoute("/New/Address"));
		}

		TEST_METHOD(OSCMessageContentTest) {
			message.setAddress("/M");
			message.empty();
			message.reserveAtLeast(16);
			message.add<int>(127);
			message.add<float>(2.3f);
			message.add<float>(3.4f);
			message.add<float>(4.5f);
			message.add<float>(5.6f);
			message.add<int>(4301);
			message.add<int>(-1);
			message.add<float>(123.123f);
			message.add<float>(1234.1234f);
			message.add<float>(12345.12345f);
			message.add<float>(123456.123456f);
			message.add<int>(-2);
			message.add<int>(32767);
			message.add<float>(-2.1f);
			message.add<float>(-2.0001f);
			message.add<float>(-10.0001f);

			message.send(&print);

			newMessage.reserveForProcess(print.bufferSize);
			newMessage.processBuffer = print.buffer;
			newMessage.process();

			for (int i = 0; i < 16; i++) {
				Assert::AreEqual(message.getFloat(i), newMessage.getFloat(i), 0.0f, L"Floats not equal", LINE_INFO());
				Assert::AreEqual(message.getInt(i), newMessage.getInt(i), L"Integers not equal", LINE_INFO());
			}
			
			Assert::IsTrue(strcmp(message.address, newMessage.address) == 0);

			int bufferLength = newMessage.bufferLength;

			for (int i = 0; i < 1000; i++) {

				newMessage.reserveForProcess(print.bufferSize);
				newMessage.processBuffer = print.buffer;
				newMessage.process();

			}

			Assert::IsTrue(bufferLength == newMessage.bufferLength);
		}

		TEST_METHOD(OSCLoop) {
			OSC = OSC::Arduino(1, 1);
			Udp = UDP();

			OSC.bindUDP(&Udp, 1, 1);

			OSC.addConsumer(&prodCons);
			OSC.addProducer(&prodCons);

			// complete a full write and a full read write and then store buffer sizes
			OSC.loop(true);
			OSC.loop(true);

			int bufferSizePre = OSC.bufferMessage.bufferLength;
			
			for (int i = 0; i < 1000; i++) {
				OSC.loop(true);
			}

			int bufferSizePost = OSC.bufferMessage.bufferLength;

			Assert::AreEqual(bufferSizePre, bufferSizePost, L"Buffer message buffers not equal after 1000 loops", LINE_INFO());
		}

	};
}