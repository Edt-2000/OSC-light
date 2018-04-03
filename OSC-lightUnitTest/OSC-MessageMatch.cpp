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

struct Data {
	uint32_t int1;
	uint32_t int2;
	uint32_t int3;
	uint32_t int4;
	uint32_t int5;
};

struct Data2 {
	uint32_t int1;
	float float1;
	uint32_t int2;
	float float2;
};

struct DataType1 {
	uint32_t int1;
	uint32_t int2;
	uint32_t int3;
	uint32_t int4;
};
struct DataType2 {
	uint32_t int1;
	uint32_t int2;
	uint32_t int3;
	uint32_t int4;
};
struct DataType3 {
	uint32_t int1;
	uint32_t int2;
	uint32_t int3;
	uint32_t int4;
};
struct DataType4 {
	uint32_t int1;
	uint32_t int2;
	uint32_t int3;
	uint32_t int4;
};
struct ShortStruct {
	uint32_t int1;
};
struct LongStruct {
	uint32_t int1;
	uint32_t int2;
	uint32_t int3;
	uint32_t int4;
	uint32_t int5;
	uint32_t int6;
	uint32_t int7;
	uint32_t int8;
};

struct DataType1_8bit {
	uint8_t int1;
	uint8_t int2;
	uint8_t int3;
	uint8_t int4;
};

struct MessageData {
	uint32_t int0; uint32_t int1; uint32_t int2; uint32_t int3; uint32_t int4; uint32_t int5; uint32_t int6; uint32_t int7; uint32_t int8; uint32_t int9;
	float float10; float float11; float float12; float float13; float float14; float float15; float float16; float float17; float float18; float float19;
};

enum class DataTypes
{
	DataType1 = 0,
	DataType2 = 1,
	DataType3 = 2,
	DataType4 = 3,

	ShortStruct = 4,
	LongStruct = 5
};

enum ColorCommands : uint8_t
{
	SingleSolid = 0,
	SinglePulse = 1,
	SingleSpark = 7,
	RainbowSolid = 2,
	RainbowPulse = 3,
	RainbowSpark = 8,
	VuMeter = 4,
	Twinkle = 5,
	Strobo = 6,
	DualSolid = 9,
	DualPulse = 10,
	DualSparkle = 11,
	Kitt = 12
};

struct SingleColorCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t saturation : 8;
	uint8_t value : 8;
	uint8_t duration : 8;
};

struct DualColorCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue1 : 8;
	uint8_t hue2 : 8;
	uint8_t percentage : 8;
	uint8_t duration : 8;
};

struct RainbowCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t deltaHue : 8;
	uint8_t duration : 8;
};

struct VuMeterCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;
	uint8_t center : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t deltaHue : 8;
	uint8_t intensity : 8;
};

struct TwinkleCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t intensity : 8;
};

struct KittCommand
{
	// 0 - 127
	uint8_t position : 8;
	uint8_t length : 8;

	// 0 - 255
	uint8_t hue : 8;
};

struct StroboCommand
{
	// 0 - 255
	uint8_t hue : 8;
	uint8_t intensity : 8;
};

struct EdtMessage
{
	ColorCommands command : 8;
	union Commands {
		SingleColorCommand singleColor;
		DualColorCommand dualColor;
		RainbowCommand rainbow;
		VuMeterCommand vuMeter;
		TwinkleCommand twinkle;
		KittCommand kitt;
		StroboCommand strobo;
	};

	Commands commands;
};

namespace OSClightUnitTest
{
	class OSCProducerConsumer : public OSC::MessageConsumer<OSC::Message>, public OSC::MessageProducer<OSC::Message>
	{
	public:
		OSC::Message message;

		OSCProducerConsumer() {
			message = OSC::Message();
			message.setAddress("/Some/Test/Address");
			message.reserveAtLeast(2);
			message.addInt(32676);
			message.addFloat(123.456f);
		}

		void loop() {
			message.setValidData(true);
		}

		OSC::Message * generateMessage() {
			message.setValidData(true);
			return &message;
		}

		const char * pattern() {
			return message.address;
		}

		void callbackMessage(OSC::Message * msg) {

		}
	};

	class OSCStructProducerConsumer : public OSC::MessageConsumer<OSC::StructMessage<Data, uint32_t>>, public OSC::MessageProducer<OSC::StructMessage<Data, uint32_t>>
	{
	public:
		OSC::StructMessage<Data, uint32_t> message;

		OSCStructProducerConsumer() {
			message = OSC::StructMessage<Data, uint32_t>();
			message.setAddress("/Some/Test/Address");
		}

		void loop() {
			message.setValidData(true);
		}

		OSC::StructMessage<Data, uint32_t> * generateMessage() {
			message.setValidData(true);
			return &message;
		}

		const char * pattern() {
			return message.address;
		}

		void callbackMessage(OSC::StructMessage<Data, uint32_t> * msg) {

		}
	};

	class OSCStruct8bitProducerConsumer : public OSC::MessageConsumer<OSC::StructMessage<DataType1_8bit, uint8_t>>, public OSC::MessageProducer<OSC::StructMessage<DataType1_8bit, uint8_t>>
	{
	public:
		OSC::StructMessage<DataType1_8bit, uint8_t> message;

		OSCStruct8bitProducerConsumer() {
			message = OSC::StructMessage<DataType1_8bit, uint8_t>();
			message.setAddress("/Some/Test/Address");
		}

		void loop() {
			message.setValidData(true);
		}

		OSC::StructMessage<DataType1_8bit, uint8_t> * generateMessage() {
			message.setValidData(true);
			return &message;
		}

		const char * pattern() {
			return message.address;
		}

		void callbackMessage(OSC::StructMessage<DataType1_8bit, uint8_t> * msg) {

		}
	};

	/*class OSCDataConsumer : public OSC::StructMessageConsumer<DataTypes, uint32_t>
	{
	public:
		OSC::Message message;

		OSCDataConsumer(int mappingNumber) : StructMessageConsumer(mappingNumber) {
		}

		const char * pattern() {
			return message.address;
		}

		DataTypes calledbackEnum;
		int callbackCallCount = 0;

		void callbackEnum(DataTypes dataTypeEnum) {
			calledbackEnum = dataTypeEnum;
			callbackCallCount++;
		}
	};*/

	/*class OSCDataConsumer_8bit : public OSC::StructMessageConsumer<DataTypes, uint8_t>
	{
	public:
		OSC::Message message;

		OSCDataConsumer_8bit(int mappingNumber) : StructMessageConsumer(mappingNumber) {
		}

		const char * pattern() {
			return message.address;
		}

		DataTypes calledbackEnum;
		int callbackCallCount = 0;

		void callbackEnum(DataTypes dataTypeEnum) {
			calledbackEnum = dataTypeEnum;
			callbackCallCount++;
		}
	};*/

	class OSCDataProducer : public OSC::MessageProducer<OSC::Message>
	{
	public:
		OSC::Message message;

		OSCDataProducer() {
		}

		void loop() {

		}

		OSC::Message * generateMessage() {


			return &message;
		}
	};

	class OSCStructDataConsumer : public OSC::MessageConsumer<OSC::StructMessage<Data, uint32_t>> {
	public:
		OSC::StructMessage<Data, uint32_t> * structMessage;

		const char * pattern()
		{
			return "/Some/Address";
		}
		void callbackMessage(OSC::StructMessage<Data, uint32_t> * message)
		{
			structMessage = message;
		}
	};

	class OSCEdtMessageConsumer : public OSC::MessageConsumer<OSC::StructMessage<EdtMessage, uint8_t>> {
	public:
		OSC::StructMessage<EdtMessage, uint8_t> * structMessage;

		const char * pattern()
		{
			return "/Some/Address";
		}
		void callbackMessage(OSC::StructMessage<EdtMessage, uint8_t> * message)
		{
			structMessage = message;
		}
	};

	TEST_CLASS(OSClightUnitTest)
	{
	public:


		TEST_METHOD(OSCEqualMatchesTest) {
			auto tester = OSC::Match();

			Assert::IsTrue(tester.isMatch("/Unit1", "/Unit1"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit1/Preset"));
			Assert::IsFalse(tester.isMatch("/Unit1", "/Unit2"));
			Assert::IsFalse(tester.isMatch("/Unit1/Preset", "/Unit2"));
		}

		TEST_METHOD(OSCSimplePathMatchesTest) {
			auto tester = OSC::Match();

			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit1"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/Deeper", "/Unit1"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset/Deeper"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset/Deeper/MoreDeeper"));
			Assert::IsFalse(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset/MoreDeeper"));
			Assert::IsFalse(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Deeper/MoreDeeper"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset/a", "/Unit1/Preset/a"));
		}

		TEST_METHOD(OSCSimpleWildcardMatchesTest) {
			auto tester = OSC::Match();

			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit?2345"));
			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit?2?45"));
			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit?2?4?"));
			Assert::IsFalse(tester.isMatch("/Unit12345", "/Unit?2?3?"));
			Assert::IsFalse(tester.isMatch("/Unit12345", "/Unit?2?1?"));
			Assert::IsTrue(tester.isMatch("/Unit12345", "/Unit?2???"));
			Assert::IsTrue(tester.isMatch("/MegaUnitMegaUnitMegaUnitMegaUnit", "/MegaUnit?egaUnitMegaUnitMeg?Unit"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit?/Preset"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/?n?t?/Preset"));
			Assert::IsTrue(tester.isMatch("/Unit1/Preset", "/Unit?/Preset"));
			Assert::IsTrue(tester.isMatch("/Unit2/Preset/a", "/Unit?/Preset/a"));
		}

		TEST_METHOD(OSCComplexWildcardMatchesTest) {
			auto tester = OSC::Match();

			Assert::IsTrue(tester.isMatch("/Unit1", "/*"));
			Assert::IsFalse(tester.isMatch("/Unit1", "/"));
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
			auto message = OSC::Message();

			message.setAddress("/M");

			Assert::IsTrue(message.isValidRoute("/M"));

			message.setAddress("/New/Address");

			Assert::IsFalse(message.isValidRoute("/M"));
			Assert::IsTrue(message.isValidRoute("/New/Address"));

			message.setAddress("/M");

			Assert::IsTrue(message.isValidRoute("/M"));
			Assert::IsFalse(message.isValidRoute("/New/Address"));
		}


		// MESSAGE
		TEST_METHOD(OSCMessageContentTest) {
			auto newMessage = OSC::Message();
			auto message = OSC::Message();
			auto print = Print();

			message.setAddress("/M");
			message.empty();
			message.reserveAtLeast(16);
			message.addInt(127);
			message.addFloat(2.3f);
			message.addFloat(3.4f);
			message.addFloat(4.5f);
			message.addFloat(5.6f);
			message.addInt(4301);
			message.addInt(-1);
			message.addFloat(123.123f);
			message.addFloat(1234.1234f);
			message.addFloat(12345.12345f);
			message.addFloat(123456.123f);
			message.addInt(-2);
			message.addInt(32767);
			message.addFloat(-2.1f);
			message.addFloat(-2.0001f);
			message.addFloat(-10.0001f);

			message.send(&print);

			newMessage.reserveBuffer(print.bufferSize);
			newMessage.processBuffer = print.buffer;
			newMessage.process();

			for (int i = 0; i < 16; i++) {
				if (message.getDataType(i) == OSC::DataType::Float) {
					Assert::AreEqual(message.getFloat(i), newMessage.getFloat(i), 0.0f, L"Floats not equal", LINE_INFO());
				}
				else if (message.getDataType(i) == OSC::DataType::Integer) {
					Assert::AreEqual(message.getInt(i), newMessage.getInt(i), L"Integers not equal", LINE_INFO());
				}
				else {
					Assert::Fail(L"Unknown data type");
				}
			}

			Assert::IsTrue(strcmp(message.address, newMessage.address) == 0);

			int bufferLength = newMessage.bufferLength;

			for (int i = 0; i < 1000; i++) {

				newMessage.reserveBuffer(print.bufferSize);
				newMessage.processBuffer = print.buffer;
				newMessage.process();

			}

			Assert::IsTrue(bufferLength == newMessage.bufferLength);
		}

		TEST_METHOD(OSCMessageContentTestUsingSets) {
			auto newMessage = OSC::Message();
			auto message = OSC::Message();
			auto print = Print();

			message.setAddress("/M");
			message.empty();
			message.reserveAtLeast(16);
			message.setInt(0, 127);
			message.setFloat(1, 2.3f);
			message.setFloat(2, 3.4f);
			message.setFloat(3, 4.5f);
			message.setFloat(4, 5.6f);
			message.setInt(5, 4301);
			message.setInt(6, -1);
			message.setFloat(7, 123.123f);
			message.setFloat(8, 1234.1234f);
			message.setFloat(9, 12345.12345f);
			message.setFloat(10, 123456.123f);
			message.setInt(11, -2);
			message.setInt(12, 32767);
			message.setFloat(13, -2.1f);
			message.setFloat(14, -2.0001f);
			message.setFloat(15, -10.0001f);

			message.send(&print);

			newMessage.reserveBuffer(print.bufferSize);
			newMessage.processBuffer = print.buffer;
			newMessage.process();

			for (int i = 0; i < 16; i++) {
				if (message.getDataType(i) == OSC::DataType::Float) {
					Assert::AreEqual(message.getFloat(i), newMessage.getFloat(i), 0.0f, L"Floats not equal", LINE_INFO());
				}
				else if (message.getDataType(i) == OSC::DataType::Integer) {
					Assert::AreEqual(message.getInt(i), newMessage.getInt(i), L"Integers not equal", LINE_INFO());
				}
				else {
					Assert::Fail(L"Unknown data type");
				}
			}

			Assert::IsTrue(strcmp(message.address, newMessage.address) == 0);

			int bufferLength = newMessage.bufferLength;

			for (int i = 0; i < 1000; i++) {

				newMessage.reserveBuffer(print.bufferSize);
				newMessage.processBuffer = print.buffer;
				newMessage.process();

			}

			Assert::IsTrue(bufferLength == newMessage.bufferLength);
		}

		TEST_METHOD(OSCSerializationBigEndian) {
			auto message = OSC::Message();
			auto print = Print();

			message.setAddress("/Some/Address");
			message.empty();
			message.reserveAtLeast(5);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);
			message.addInt(5);

			message.send(&print, false);

			std::string stringFromSerialization(print.fullBuffer);

			Assert::AreEqual(
				"/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5",
				stringFromSerialization.c_str(),
				L"Serialized data not conform spec", LINE_INFO());
		}

		TEST_METHOD(OSCSerializationLittleEndian) {
			auto message = OSC::Message();
			auto print = Print();

			message.setAddress("/Some/Address");
			message.empty();
			message.reserveAtLeast(5);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);
			message.addInt(5);

			message.send(&print, true);

			std::string stringFromSerialization(print.fullBuffer);

			Assert::AreEqual(
				"/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___",
				stringFromSerialization.c_str(),
				L"Serialized data not conform spec", LINE_INFO());
		}

		TEST_METHOD(OSCDeserializationBigEndian) {
			auto message = OSC::Message();
			auto print = Print();

			std::string testString = "/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5";
			int length = testString.length();

			print.write(testString.c_str(), length);

			message.setAddress("/Some/Address");
			message.empty();
			message.reserveBuffer(length);
			memcpy(message.processBuffer, print.reversedBuffer, length);

			message.process(false);

			Assert::AreEqual(1u, message.getInt(0), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(2u, message.getInt(1), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(3u, message.getInt(2), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(4u, message.getInt(3), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(5u, message.getInt(4), L"Integers not equal", LINE_INFO());
		}

		TEST_METHOD(OSCDeserializationLittleEndian) {
			auto message = OSC::Message();
			auto print = Print();

			std::string testString = "/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___";
			int length = testString.length();

			print.write(testString.c_str(), length);

			message.setAddress("/Some/Address");
			message.empty();
			message.reserveBuffer(length);
			memcpy(message.processBuffer, print.reversedBuffer, length);

			message.process(true);

			Assert::AreEqual(1u, message.getInt(0), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(2u, message.getInt(1), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(3u, message.getInt(2), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(4u, message.getInt(3), L"Integers not equal", LINE_INFO());
			Assert::AreEqual(5u, message.getInt(4), L"Integers not equal", LINE_INFO());
		}

		TEST_METHOD(OSCLoop) {
			auto OSC = OSC::Arduino<OSC::Message>(1, 1);
			auto Udp = UDP();

			auto prodCons = OSCProducerConsumer();

			OSC.bindUDP(&Udp, 1, 1);

			OSC.addConsumer(&prodCons);
			OSC.addProducer(&prodCons);

			// complete a full write and a full read write and then store buffer sizes
			OSC.loop(true);
			OSC.loop(true);

			int bufferSizePre = OSC.bufferMessage.bufferLength;

			for (int i = 0; i < 1000000; i++) {
				OSC.loop(true);
			}

			int bufferSizePost = OSC.bufferMessage.bufferLength;

			Assert::AreEqual(bufferSizePre, bufferSizePost, L"Buffer message buffers not equal after 1000 loops", LINE_INFO());
		}

		TEST_METHOD(OSCLoopStream) {
			auto OSC = OSC::Arduino<OSC::Message>(1, 1);
			auto stream = Stream();

			auto prodCons = OSCProducerConsumer();

			OSC.bindStream(&stream);

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


		// MESSAGE STRUCT
		TEST_METHOD(OSCStructMessageContentTest) {
			auto newMessage = OSC::StructMessage<Data, uint32_t>();
			auto message = OSC::StructMessage<Data, uint32_t>();
			auto print = Print();

			message.setAddress("/M");
			message.messageStruct.int1 = 12;
			message.messageStruct.int2 = 123;
			message.messageStruct.int3 = 1234;
			message.messageStruct.int4 = 12345;
			message.messageStruct.int5 = 123456;

			message.send(&print);

			newMessage.reserveBuffer(print.bufferSize);
			newMessage.processBuffer = print.buffer;
			newMessage.process();

			Assert::IsTrue(message.messageStruct.int1 == newMessage.messageStruct.int1, L"Int 1 not the same", LINE_INFO());
			Assert::IsTrue(message.messageStruct.int2 == newMessage.messageStruct.int2, L"Int 2 not the same", LINE_INFO());
			Assert::IsTrue(message.messageStruct.int3 == newMessage.messageStruct.int3, L"Int 3 not the same", LINE_INFO());
			Assert::IsTrue(message.messageStruct.int4 == newMessage.messageStruct.int4, L"Int 4 not the same", LINE_INFO());
			Assert::IsTrue(message.messageStruct.int5 == newMessage.messageStruct.int5, L"Int 5 not the same", LINE_INFO());

			Assert::IsTrue(strcmp(message.address, newMessage.address) == 0);

			int bufferLength = newMessage.bufferLength;

			for (int i = 0; i < 1000; i++) {

				newMessage.reserveBuffer(print.bufferSize);
				newMessage.processBuffer = print.buffer;
				newMessage.process();

			}

			Assert::IsTrue(bufferLength == newMessage.bufferLength);
		}

		TEST_METHOD(OSCStructSerializationBigEndian) {
			auto message = OSC::StructMessage<Data, uint32_t>();
			auto print = Print();

			message.setAddress("/Some/Address");

			message.messageStruct.int1 = 1;
			message.messageStruct.int2 = 2;
			message.messageStruct.int3 = 3;
			message.messageStruct.int4 = 4;
			message.messageStruct.int5 = 5;

			message.send(&print, false);

			std::string stringFromSerialization(print.fullBuffer);

			Assert::AreEqual(
				"/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5",
				stringFromSerialization.c_str(),
				L"Serialized data not conform spec", LINE_INFO());
		}

		TEST_METHOD(OSCStructSerializationLittleEndian) {
			auto message = OSC::StructMessage<Data, uint32_t>();
			auto print = Print();

			message.setAddress("/Some/Address");

			message.messageStruct.int1 = 1;
			message.messageStruct.int2 = 2;
			message.messageStruct.int3 = 3;
			message.messageStruct.int4 = 4;
			message.messageStruct.int5 = 5;

			message.send(&print, true);

			std::string stringFromSerialization(print.fullBuffer);

			Assert::AreEqual(
				"/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___",
				stringFromSerialization.c_str(),
				L"Serialized data not conform spec", LINE_INFO());
		}

		TEST_METHOD(OSCStructDeserializationBigEndian) {
			auto message = OSC::StructMessage<Data, uint32_t>();
			auto print = Print();

			std::string testString = "/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5";
			int length = testString.length();

			print.write(testString.c_str(), length);

			message.setAddress("/Some/Address");
			message.reserveBuffer(length);
			memcpy(message.processBuffer, print.reversedBuffer, length);

			message.process(false);

			Assert::AreEqual(1u, message.messageStruct.int1, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(2u, message.messageStruct.int2, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(3u, message.messageStruct.int3, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(4u, message.messageStruct.int4, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(5u, message.messageStruct.int5, L"Integers not equal", LINE_INFO());
		}

		TEST_METHOD(OSCStructDeserializationLittleEndian) {
			auto message = OSC::StructMessage<Data, uint32_t>();
			auto print = Print();

			std::string testString = "/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___";
			int length = testString.length();

			print.write(testString.c_str(), length);

			message.setAddress("/Some/Address");
			message.reserveBuffer(length);
			memcpy(message.processBuffer, print.reversedBuffer, length);

			message.process(true);

			Assert::AreEqual(1u, message.messageStruct.int1, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(2u, message.messageStruct.int2, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(3u, message.messageStruct.int3, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(4u, message.messageStruct.int4, L"Integers not equal", LINE_INFO());
			Assert::AreEqual(5u, message.messageStruct.int5, L"Integers not equal", LINE_INFO());
		}

		TEST_METHOD(OSCStruct8bitMessageContentTest) {
			auto newMessage = OSC::StructMessage<DataType1_8bit, uint8_t>();
			auto message = OSC::StructMessage<DataType1_8bit, uint8_t>();
			auto print = Print();

			message.setAddress("/M");
			message.messageStruct.int1 = 12;
			message.messageStruct.int2 = 123;
			message.messageStruct.int3 = 255;
			message.messageStruct.int4 = 128;

			message.send(&print);

			newMessage.reserveBuffer(print.bufferSize);
			newMessage.processBuffer = print.buffer;
			newMessage.process();

			Assert::IsTrue(message.messageStruct.int1 == newMessage.messageStruct.int1, L"Int 1 not the same", LINE_INFO());
			Assert::IsTrue(message.messageStruct.int2 == newMessage.messageStruct.int2, L"Int 2 not the same", LINE_INFO());
			Assert::IsTrue(message.messageStruct.int3 == newMessage.messageStruct.int3, L"Int 3 not the same", LINE_INFO());
			Assert::IsTrue(message.messageStruct.int4 == newMessage.messageStruct.int4, L"Int 4 not the same", LINE_INFO());

			Assert::IsTrue(strcmp(message.address, newMessage.address) == 0);

			int bufferLength = newMessage.bufferLength;

			for (int i = 0; i < 1000; i++) {

				newMessage.reserveBuffer(print.bufferSize);
				newMessage.processBuffer = print.buffer;
				newMessage.process();

			}

			Assert::IsTrue(bufferLength == newMessage.bufferLength);
		}

		TEST_METHOD(OSCStruct8bitSerializationBigEndian) {
			auto message = OSC::StructMessage<DataType1_8bit, uint8_t>();
			auto print = Print();

			message.setAddress("/Some/Address");

			message.messageStruct.int1 = 1;
			message.messageStruct.int2 = 2;
			message.messageStruct.int3 = 3;
			message.messageStruct.int4 = 4;

			message.send(&print, false);

			std::string stringFromSerialization(print.fullBuffer);

			Assert::AreEqual(
				"/Some/Address___,iiii______\x1___\x2___\x3___\x4",
				stringFromSerialization.c_str(),
				L"Serialized data not conform spec", LINE_INFO());
		}

		TEST_METHOD(OSCStruct8bitSerializationLittleEndian) {
			auto message = OSC::StructMessage<DataType1_8bit, uint8_t>();
			auto print = Print();

			message.setAddress("/Some/Address");

			message.messageStruct.int1 = 1;
			message.messageStruct.int2 = 2;
			message.messageStruct.int3 = 3;
			message.messageStruct.int4 = 4;

			message.send(&print, true);

			std::string stringFromSerialization(print.fullBuffer);

			Assert::AreEqual(
				"/Some/Address___,iiii___\x1___\x2___\x3___\x4___",
				stringFromSerialization.c_str(),
				L"Serialized data not conform spec", LINE_INFO());
		}

		TEST_METHOD(OSCStruct8bitDeserializationBigEndian) {
			auto message = OSC::StructMessage<DataType1_8bit, uint8_t>();
			auto print = Print();

			std::string testString = "/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5";
			int length = testString.length();

			print.write(testString.c_str(), length);

			message.setAddress("/Some/Address");
			message.reserveBuffer(length);
			memcpy(message.processBuffer, print.reversedBuffer, length);

			message.process(false);

			Assert::AreEqual((uint8_t)1u, message.messageStruct.int1, L"Integers not equal", LINE_INFO());
			Assert::AreEqual((uint8_t)2u, message.messageStruct.int2, L"Integers not equal", LINE_INFO());
			Assert::AreEqual((uint8_t)3u, message.messageStruct.int3, L"Integers not equal", LINE_INFO());
			Assert::AreEqual((uint8_t)4u, message.messageStruct.int4, L"Integers not equal", LINE_INFO());
		}

		TEST_METHOD(OSCStruct8bitDeserializationLittleEndian) {
			auto message = OSC::StructMessage<DataType1_8bit, uint8_t>();
			auto print = Print();

			std::string testString = "/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___";
			int length = testString.length();

			print.write(testString.c_str(), length);

			message.setAddress("/Some/Address");
			message.reserveBuffer(length);
			memcpy(message.processBuffer, print.reversedBuffer, length);

			message.process(true);

			Assert::AreEqual((uint8_t)1u, message.messageStruct.int1, L"Integers not equal", LINE_INFO());
			Assert::AreEqual((uint8_t)2u, message.messageStruct.int2, L"Integers not equal", LINE_INFO());
			Assert::AreEqual((uint8_t)3u, message.messageStruct.int3, L"Integers not equal", LINE_INFO());
			Assert::AreEqual((uint8_t)4u, message.messageStruct.int4, L"Integers not equal", LINE_INFO());
		}

		TEST_METHOD(OSCStructLoop) {
			auto OSC = OSC::Arduino<OSC::StructMessage<Data, uint32_t>>(1, 1);
			auto Udp = UDP();

			auto prodCons = OSCStructProducerConsumer();

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

		TEST_METHOD(OSCStructLoopStream) {
			auto OSC = OSC::Arduino<OSC::StructMessage<Data, uint32_t>>(1, 1);
			auto stream = Stream();

			auto prodCons = OSCStructProducerConsumer();

			OSC.bindStream(&stream);

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

		TEST_METHOD(OSCStruct8bitLoop) {
			auto OSC = OSC::Arduino<OSC::StructMessage<DataType1_8bit, uint8_t>>(1, 1);
			auto Udp = UDP();

			auto prodCons = OSCStruct8bitProducerConsumer();

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


		TEST_METHOD(OSCStruct8bitLoopStream) {
			auto OSC = OSC::Arduino<OSC::StructMessage<DataType1_8bit, uint8_t>>(1, 1);
			auto stream = Stream();

			auto prodCons = OSCStruct8bitProducerConsumer();

			OSC.bindStream(&stream);

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

		// COMPARE MESSAGES

		TEST_METHOD(OSCSerializationSpeed) {
			auto messageRegular = OSC::Message();
			auto print = Print();

			int n = 1000000;

			auto startRegular = std::chrono::system_clock::now();

			messageRegular.setAddress("/M");
			messageRegular.empty();
			messageRegular.reserveAtLeast(5);
			messageRegular.addInt(12);
			messageRegular.addInt(123);
			messageRegular.addInt(1234);
			messageRegular.addInt(12345);
			messageRegular.addInt(123456);

			for (int i = 0; i < n; i++) {
				messageRegular.send(&print);
				messageRegular.processBuffer = print.buffer;
				messageRegular.process();
			}

			auto endRegular = std::chrono::system_clock::now();

			std::chrono::duration<double> diffRegular = endRegular - startRegular;

			auto valueRegular = diffRegular.count();

			auto messageStruct = OSC::StructMessage<Data, uint32_t>();

			auto startStruct = std::chrono::system_clock::now();

			messageStruct.setAddress("/M");
			messageStruct.messageStruct.int1 = 12;
			messageStruct.messageStruct.int2 = 123;
			messageStruct.messageStruct.int3 = 1234;
			messageStruct.messageStruct.int4 = 12345;
			messageStruct.messageStruct.int5 = 123456;

			for (int i = 0; i < n; i++) {
				messageStruct.send(&print);
				messageStruct.processBuffer = print.buffer;
				messageStruct.process();
			}

			auto endStruct = std::chrono::system_clock::now();

			std::chrono::duration<double> diffStruct = endStruct - startStruct;

			auto valueStruct = diffStruct.count();

			Assert::IsTrue(valueStruct < valueRegular, L"Better method faster", LINE_INFO());
		}

		// TODO: speed test with complete setup

		TEST_METHOD(OSCFullCycleSpeed) {
			int n = 1000;

			auto Udp = UDP();

			auto rOSC = OSC::Arduino<OSC::Message>(4, 1);

			auto rprodCons = OSCProducerConsumer();

			rOSC.bindUDP(&Udp, 1, 1);

			rOSC.addConsumer(&rprodCons);
			rOSC.addConsumer(&rprodCons);
			rOSC.addConsumer(&rprodCons);
			rOSC.addConsumer(&rprodCons);
			rOSC.addProducer(&rprodCons);

			// complete a full write and a full read write and then store buffer sizes
			rOSC.loop(true);
			rOSC.loop(true);

			int rbufferSizePre = rOSC.bufferMessage.bufferLength;
			
			auto startRegular = std::chrono::system_clock::now();

			for (int i = 0; i < n; i++) {
				rOSC.loop(true);
			}
			
			auto endRegular = std::chrono::system_clock::now();

			int rbufferSizePost = rOSC.bufferMessage.bufferLength;

			Assert::AreEqual(rbufferSizePre, rbufferSizePost, L"Buffer message buffers not equal after n loops", LINE_INFO());


			std::chrono::duration<double> diffRegular = endRegular - startRegular;

			auto valueRegular = diffRegular.count();

			auto messageStruct = OSC::StructMessage<Data, uint32_t>();


			auto sOSC = OSC::Arduino<OSC::StructMessage<Data, uint32_t>>(1, 1);
			
			auto sprodCons = OSCStructProducerConsumer();

			sOSC.bindUDP(&Udp, 4, 1);

			sOSC.addConsumer(&sprodCons);
			sOSC.addConsumer(&sprodCons);
			sOSC.addConsumer(&sprodCons);
			sOSC.addConsumer(&sprodCons);
			sOSC.addProducer(&sprodCons);

			// complete a full write and a full read write and then store buffer sizes
			sOSC.loop(true);
			sOSC.loop(true);

			int sbufferSizePre = sOSC.bufferMessage.bufferLength;

			auto startStruct = std::chrono::system_clock::now();
			
			for (int i = 0; i < n; i++) {
				sOSC.loop(true);
			}

			auto endStruct = std::chrono::system_clock::now();

			int sbufferSizePost = sOSC.bufferMessage.bufferLength;

			Assert::AreEqual(sbufferSizePre, sbufferSizePost, L"Buffer message buffers not equal after n loops", LINE_INFO());


			std::chrono::duration<double> diffStruct = endStruct - startStruct;

			auto valueStruct = diffStruct.count();

			Assert::IsTrue(valueStruct < valueRegular, L"Better method faster", LINE_INFO());
		}


		//// STRUCT Message Consumer
		//TEST_METHOD(OSCReadAsStruct) {
		//	auto message = OSC::Message();

		//	auto structCons = OSCDataConsumer(5);

		//	message.setAddress("/Some/Address");
		//	message.empty();
		//	message.addInt(1);
		//	message.addInt(2);
		//	message.addInt(3);
		//	message.addInt(4);
		//	message.addInt(5);

		//	auto data = Data();

		//	structCons.readToStruct(&message, (unsigned char*)&data, sizeof(Data));

		//	Assert::AreEqual(1u, data.int1, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(2u, data.int2, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(3u, data.int3, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(4u, data.int4, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(5u, data.int5, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCReadAsStruct2) {
		//	auto message = OSC::Message();

		//	auto structCons = OSCDataConsumer(5);

		//	message.setAddress("/Some/Address");
		//	message.empty();
		//	message.addInt(1);
		//	message.addFloat(2.345f);
		//	message.addInt(3);
		//	message.addFloat(441910.191f);

		//	auto data = Data2();

		//	structCons.readToStruct(&message, (unsigned char*)&data, sizeof(Data2));

		//	Assert::AreEqual(1u, data.int1, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(2.345f, data.float1, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(3u, data.int2, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(441910.191f, data.float2, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCStagedStructs) {

		//	auto type1 = DataType1();
		//	auto type2 = DataType2();
		//	auto type3 = DataType3();
		//	auto type4 = DataType4();

		//	auto structCons = OSCDataConsumer(4);

		//	structCons.addEnumToStructMapping<DataType1>(DataTypes::DataType1, &type1);
		//	structCons.addEnumToStructMapping<DataType2>(DataTypes::DataType2, &type2);
		//	structCons.addEnumToStructMapping<DataType3>(DataTypes::DataType3, &type3);
		//	structCons.addEnumToStructMapping<DataType4>(DataTypes::DataType4, &type4);

		//	auto message = OSC::Message();

		//	for (int i = 1; i <= 4; ++i) {
		//		message.empty();
		//		message.reserveAtLeast(5);
		//		message.addInt((i * 1) - 1);
		//		message.addInt(i * 1);
		//		message.addInt(i * 2);
		//		message.addInt(i * 3);
		//		message.addInt(i * 4);

		//		structCons.callbackMessage(&message);

		//		if (i == 1) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType1, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(1u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(2u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(3u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(4u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(1u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(2u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(3u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(1u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(2u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(3u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type3.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(1u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(2u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(3u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type4.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 2) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType2, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(2u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(4u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(6u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(8u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(2u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(2u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type3.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(2u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type4.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 3) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType3, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(3u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(6u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(9u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(12u, type3.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(3u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(9u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(3u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(9u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(3u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(9u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type4.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 4) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType4, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(4u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(8u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(12u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(16u, type4.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(4u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(16u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(4u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(16u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(4u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(16u, type3.int4, L"Struct does not contain same value", LINE_INFO());
		//		}
		//	}

		//}

		//TEST_METHOD(OSCStagedStructs_8bit) {

		//	auto type1 = DataType1_8bit();

		//	auto structCons = OSCDataConsumer_8bit(4);

		//	structCons.addEnumToStructMapping<DataType1_8bit>(DataTypes::DataType1, &type1);

		//	auto message = OSC::Message();

		//	for (int i = 1; i <= 4; ++i) {
		//		message.empty();
		//		message.reserveAtLeast(5);
		//		message.addInt((i * 1) - 1);
		//		message.addInt(i * 1);
		//		message.addInt(i * 2);
		//		message.addInt(i * 3);
		//		message.addInt(i * 4);

		//		structCons.callbackMessage(&message);

		//		if (i == 1) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType1, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual((uint8_t)1u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual((uint8_t)2u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual((uint8_t)3u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual((uint8_t)4u, type1.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 2) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType2, L"Wrong enum response", LINE_INFO());

		//			Assert::AreNotEqual((uint8_t)2u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)4u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)6u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)8u, type1.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 3) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType3, L"Wrong enum response", LINE_INFO());

		//			Assert::AreNotEqual((uint8_t)3u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)6u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)9u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)12u, type1.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 4) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType4, L"Wrong enum response", LINE_INFO());

		//			Assert::AreNotEqual((uint8_t)4u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)8u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)12u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual((uint8_t)16u, type1.int4, L"Struct does not contain same value", LINE_INFO());
		//		}
		//	}

		//}

		//TEST_METHOD(OSCStagedStructsNoReservation) {

		//	auto type1 = DataType1();
		//	auto type2 = DataType2();
		//	auto type3 = DataType3();
		//	auto type4 = DataType4();

		//	auto structCons = OSCDataConsumer(0);

		//	structCons.addEnumToStructMapping<DataType1>(DataTypes::DataType1, &type1);
		//	structCons.addEnumToStructMapping<DataType2>(DataTypes::DataType2, &type2);
		//	structCons.addEnumToStructMapping<DataType3>(DataTypes::DataType3, &type3);
		//	structCons.addEnumToStructMapping<DataType4>(DataTypes::DataType4, &type4);

		//	auto message = OSC::Message();

		//	for (int i = 1; i <= 4; ++i) {
		//		message.empty();
		//		message.reserveAtLeast(5);
		//		message.addInt((i * 1) - 1);
		//		message.addInt(i * 1);
		//		message.addInt(i * 2);
		//		message.addInt(i * 3);
		//		message.addInt(i * 4);

		//		structCons.callbackMessage(&message);

		//		if (i == 1) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType1, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(1u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(2u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(3u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(4u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(1u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(2u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(3u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(1u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(2u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(3u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type3.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(1u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(2u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(3u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type4.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 2) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType2, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(2u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(4u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(6u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(8u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(2u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(2u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type3.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(2u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(4u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type4.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 3) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType3, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(3u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(6u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(9u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(12u, type3.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(3u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(9u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(3u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(9u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(3u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(6u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(9u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type4.int4, L"Struct does not contain same value", LINE_INFO());
		//		}

		//		if (i == 4) {
		//			Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType4, L"Wrong enum response", LINE_INFO());

		//			Assert::AreEqual(4u, type4.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(8u, type4.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(12u, type4.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreEqual(16u, type4.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(4u, type1.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type1.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type1.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(16u, type1.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(4u, type2.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type2.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type2.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(16u, type2.int4, L"Struct does not contain same value", LINE_INFO());

		//			Assert::AreNotEqual(4u, type3.int1, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(8u, type3.int2, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(12u, type3.int3, L"Struct does not contain same value", LINE_INFO());
		//			Assert::AreNotEqual(16u, type3.int4, L"Struct does not contain same value", LINE_INFO());
		//		}
		//	}

		//}

		//TEST_METHOD(OSCStagedShortStructs) {

		//	auto shortStruct = ShortStruct();

		//	shortStruct.int1 = 255;

		//	auto structCons = OSCDataConsumer(10);

		//	structCons.addEnumToStructMapping<ShortStruct>(DataTypes::ShortStruct, &shortStruct);

		//	auto message = OSC::Message();
		//	message.empty();
		//	message.reserveAtLeast(5);
		//	message.addInt(4);
		//	message.addInt(1);
		//	message.addInt(2);
		//	message.addInt(3);
		//	message.addInt(4);

		//	structCons.callbackMessage(&message);

		//	Assert::IsTrue(structCons.calledbackEnum == DataTypes::ShortStruct, L"Wrong enum response", LINE_INFO());
		//	Assert::AreEqual(1u, shortStruct.int1, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCCorrectStagedShortStructs) {

		//	auto shortStruct = ShortStruct();

		//	shortStruct.int1 = 255;

		//	auto structCons = OSCDataConsumer(10);

		//	structCons.addEnumToStructMapping<ShortStruct>(DataTypes::DataType1, &shortStruct);
		//	structCons.addEnumToStructMapping<ShortStruct>(DataTypes::ShortStruct, &shortStruct);
		//	structCons.addEnumToStructMapping<ShortStruct>(DataTypes::DataType2, &shortStruct);
		//	structCons.addEnumToStructMapping<ShortStruct>(DataTypes::DataType4, &shortStruct);

		//	auto message = OSC::Message();
		//	message.empty();
		//	message.reserveAtLeast(5);
		//	message.addInt(4);
		//	message.addInt(1);
		//	message.addInt(2);
		//	message.addInt(3);
		//	message.addInt(4);

		//	structCons.callbackMessage(&message);

		//	Assert::IsTrue(structCons.calledbackEnum == DataTypes::ShortStruct, L"Wrong enum response", LINE_INFO());
		//	Assert::IsTrue(structCons.callbackCallCount == 1, L"Too many callbacks", LINE_INFO());
		//	Assert::AreEqual(1u, shortStruct.int1, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCStagedLongStructs) {

		//	auto longStruct = LongStruct();

		//	longStruct.int1 = 255;
		//	longStruct.int2 = 255;
		//	longStruct.int3 = 255;
		//	longStruct.int4 = 255;
		//	longStruct.int5 = 255;
		//	longStruct.int6 = 255;
		//	longStruct.int7 = 255;
		//	longStruct.int8 = 255;

		//	auto structCons = OSCDataConsumer(10);

		//	structCons.addEnumToStructMapping<LongStruct>(DataTypes::LongStruct, &longStruct);

		//	auto message = OSC::Message();
		//	message.empty();
		//	message.reserveAtLeast(5);
		//	message.addInt(5);
		//	message.addInt(1);
		//	message.addInt(2);
		//	message.addInt(3);
		//	message.addInt(4);

		//	structCons.callbackMessage(&message);

		//	Assert::IsTrue(structCons.calledbackEnum == DataTypes::LongStruct, L"Wrong enum response", LINE_INFO());
		//	Assert::AreEqual(1u, longStruct.int1, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(2u, longStruct.int2, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(3u, longStruct.int3, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(4u, longStruct.int4, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(0u, longStruct.int5, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(0u, longStruct.int6, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(0u, longStruct.int7, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(0u, longStruct.int8, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCSendAsIntStruct) {
		//	auto message = OSC::Message();

		//	message.setAddress("/Struct/Message");

		//	auto data = Data();
		//	data.int1 = 32;
		//	data.int2 = 64;
		//	data.int3 = 128;
		//	data.int4 = 256;
		//	data.int5 = 512;

		//	OSC::DataType types[5]
		//	{
		//		OSC::DataType::Integer, OSC::DataType::Integer, OSC::DataType::Integer, OSC::DataType::Integer, OSC::DataType::Integer
		//	};

		//	message.set<Data>(0, &data, types);

		//	Assert::AreEqual(32u, message.getInt(0), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(64u, message.getInt(1), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(128u, message.getInt(2), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(256u, message.getInt(3), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(512u, message.getInt(4), L"Struct does not contain same value", LINE_INFO());

		//	Assert::IsTrue(message.getDataType(0) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(1) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(2) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(3) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(4) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//}

		//TEST_METHOD(OSCSendAsFloatStruct) {
		//	auto message = OSC::Message();

		//	message.setAddress("/Struct/Message");

		//	auto data = Data2();
		//	data.int1 = 32;
		//	data.int2 = 64;
		//	data.float1 = 31124.12351f;
		//	data.float2 = 0.000198178f;

		//	OSC::DataType types[4]
		//	{
		//		OSC::DataType::Integer, OSC::DataType::Float, OSC::DataType::Integer, OSC::DataType::Float
		//	};

		//	message.set<Data2>(0, &data, types);

		//	Assert::AreEqual(32u, message.getInt(0), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(31124.12351f, message.getFloat(1), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(64u, message.getInt(2), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(0.000198178f, message.getFloat(3), L"Struct does not contain same value", LINE_INFO());

		//	Assert::IsTrue(message.getDataType(0) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(1) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(2) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(3) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//}

		//TEST_METHOD(OSCSendAsStructAutoDetect) {
		//	auto message = OSC::Message();

		//	message.setAddress("/Struct/Message");

		//	auto data = Data2();
		//	data.int1 = 32;
		//	data.int2 = 64;
		//	data.float1 = 31124.12351f;
		//	data.float2 = 0.000198178f;

		//	message.set<Data2>(0, &data);

		//	Assert::AreEqual(32u, message.getInt(0), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(31124.12351f, message.getFloat(1), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(64u, message.getInt(2), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(0.000198178f, message.getFloat(3), L"Struct does not contain same value", LINE_INFO());

		//	Assert::IsTrue(message.getDataType(0) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(1) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(2) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(3) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//}

		//TEST_METHOD(OSCSendAsStructAutoDetectBig) {
		//	auto message = OSC::Message();
		//	auto print = Print();

		//	message.setAddress("/Struct/Message");

		//	MessageData messageData;
		//	messageData.int0 = 0;
		//	messageData.int1 = 1;
		//	messageData.int2 = 2;
		//	messageData.int3 = 3;
		//	messageData.int4 = 4;
		//	messageData.int5 = 5;
		//	messageData.int6 = 6;
		//	messageData.int7 = 7;
		//	messageData.int8 = 8;
		//	messageData.int9 = 9;
		//	messageData.float10 = 10.10f;
		//	messageData.float11 = 11.11f;
		//	messageData.float12 = 12.12f;
		//	messageData.float13 = 13.13f;
		//	messageData.float14 = 14.14f;
		//	messageData.float15 = 15.15f;
		//	messageData.float16 = 16.16f;
		//	messageData.float17 = 17.17f;
		//	messageData.float18 = 18.18f;
		//	messageData.float19 = 19.19f;

		//	message.set<MessageData>(0, &messageData);

		//	message.send(&print);

		//	Assert::AreEqual(0u, message.getInt(0), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(1u, message.getInt(1), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(2u, message.getInt(2), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(3u, message.getInt(3), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(4u, message.getInt(4), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(5u, message.getInt(5), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(6u, message.getInt(6), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(7u, message.getInt(7), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(8u, message.getInt(8), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(9u, message.getInt(9), L"Struct does not contain same value", LINE_INFO());

		//	Assert::IsTrue(message.getDataType(0) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(1) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(2) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(3) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(4) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(5) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(6) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(7) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(8) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(9) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());

		//	Assert::AreEqual(10.10f, message.getFloat(10), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(11.11f, message.getFloat(11), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(12.12f, message.getFloat(12), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(13.13f, message.getFloat(13), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(14.14f, message.getFloat(14), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(15.15f, message.getFloat(15), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(16.16f, message.getFloat(16), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(17.17f, message.getFloat(17), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(18.18f, message.getFloat(18), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(19.19f, message.getFloat(19), L"Struct does not contain same value", LINE_INFO());

		//	Assert::IsTrue(message.getDataType(10) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(11) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(12) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(13) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(14) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(15) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(16) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(17) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(18) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(19) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//}

		//TEST_METHOD(OSCSendAsStructAutoDetectBig2) {
		//	auto message = OSC::Message();
		//	auto print = Print();

		//	message.setAddress("/Struct/Message");

		//	MessageData messageData;
		//	messageData.int0 = 0;
		//	messageData.int1 = 1;
		//	messageData.int2 = 2;
		//	messageData.int3 = 3;
		//	messageData.int4 = 4;
		//	messageData.int5 = 5;
		//	messageData.int6 = 6;
		//	messageData.int7 = 7;
		//	messageData.int8 = 8;
		//	messageData.int9 = 9;
		//	messageData.float10 = 10.10f;
		//	messageData.float11 = 11.11f;
		//	messageData.float12 = 12.12f;
		//	messageData.float13 = 13.13f;
		//	messageData.float14 = 14.14f;
		//	messageData.float15 = 15.15f;
		//	messageData.float16 = 16.16f;
		//	messageData.float17 = 17.17f;
		//	messageData.float18 = 18.18f;
		//	messageData.float19 = 19.19f;

		//	message.addInt(0);
		//	message.add<MessageData>(&messageData);
		//	message.addInt(12345);

		//	message.send(&print);

		//	Assert::AreEqual(0u, message.getInt(0), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(0u, message.getInt(1), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(1u, message.getInt(2), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(2u, message.getInt(3), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(3u, message.getInt(4), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(4u, message.getInt(5), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(5u, message.getInt(6), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(6u, message.getInt(7), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(7u, message.getInt(8), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(8u, message.getInt(9), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(9u, message.getInt(10), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(12345u, message.getInt(21), L"Struct does not contain same value", LINE_INFO());

		//	Assert::IsTrue(message.getDataType(0) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(1) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(2) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(3) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(4) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(5) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(6) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(7) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(8) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(9) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(10) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(21) == OSC::DataType::Integer, L"Datatype does not match", LINE_INFO());

		//	Assert::AreEqual(10.10f, message.getFloat(11), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(11.11f, message.getFloat(12), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(12.12f, message.getFloat(13), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(13.13f, message.getFloat(14), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(14.14f, message.getFloat(15), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(15.15f, message.getFloat(16), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(16.16f, message.getFloat(17), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(17.17f, message.getFloat(18), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(18.18f, message.getFloat(19), L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(19.19f, message.getFloat(20), L"Struct does not contain same value", LINE_INFO());

		//	Assert::IsTrue(message.getDataType(11) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(12) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(13) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(14) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(15) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(16) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(17) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(18) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(19) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//	Assert::IsTrue(message.getDataType(20) == OSC::DataType::Float, L"Datatype does not match", LINE_INFO());
		//}


		//TEST_METHOD(OSCReadMessageStruct) {
		//	auto message = OSC::StructMessage<Data, uint32_t>();

		//	auto structCons = OSCStructDataConsumer();

		//	message.setAddress(structCons.pattern());
		//	message.messageStruct.int1 = 1;
		//	message.messageStruct.int2 = 2;
		//	message.messageStruct.int3 = 3;
		//	message.messageStruct.int4 = 4;
		//	message.messageStruct.int5 = 5;

		//	auto OSC = OSC::Arduino<OSC::StructMessage<Data, uint32_t>>(1, 0);
		//	auto Udp = UDP();

		//	OSC.bindUDP(&Udp, 1, 1);
		//	OSC.addConsumer(&structCons);

		//	message.send(&Udp);

		//	OSC.loop(false);

		//	Assert::AreEqual(1u, structCons.structMessage->messageStruct.int1, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(2u, structCons.structMessage->messageStruct.int2, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(3u, structCons.structMessage->messageStruct.int3, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(4u, structCons.structMessage->messageStruct.int4, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual(5u, structCons.structMessage->messageStruct.int5, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCReadEdtMessageStruct) {
		//	auto message = OSC::StructMessage<OSC::EdtMessage, uint8_t>();

		//	auto structCons = OSCEdtMessageConsumer();

		//	message.setAddress(structCons.pattern());
		//	message.messageStruct.command = OSC::ColorCommands::SinglePulse;
		//	message.messageStruct.commands.singleColor.duration = 1;
		//	message.messageStruct.commands.singleColor.end = 2;
		//	message.messageStruct.commands.singleColor.hue = 3;
		//	message.messageStruct.commands.singleColor.saturation = 4;
		//	message.messageStruct.commands.singleColor.start = 5;
		//	message.messageStruct.commands.singleColor.value = 6;

		//	auto OSC = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);
		//	auto Udp = UDP();

		//	OSC.bindUDP(&Udp, 1, 1);
		//	OSC.addConsumer(&structCons);

		//	message.send(&Udp);

		//	OSC.loop(false);

		//	Assert::AreEqual((int)OSC::ColorCommands::SinglePulse, (int)structCons.structMessage->messageStruct.command, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)1, structCons.structMessage->messageStruct.commands.singleColor.duration, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)2, structCons.structMessage->messageStruct.commands.singleColor.end, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)3, structCons.structMessage->messageStruct.commands.singleColor.hue, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)4, structCons.structMessage->messageStruct.commands.singleColor.saturation, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)5, structCons.structMessage->messageStruct.commands.singleColor.start, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)6, structCons.structMessage->messageStruct.commands.singleColor.value, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCReadEdtMessageFromRawStruct) {
		//	auto message = OSC::StructMessage<OSC::EdtMessage, uint8_t>();
		//	auto structCons = OSCEdtMessageConsumer();
		//	auto print = Print();

		//	std::string testString = "/Some/Address___,iiiiiii_______\x1___\x2___\x3___\x4___\x5___\x6___\x7";
		//	int length = testString.length();

		//	print.write(testString.c_str(), length);

		//	message.setAddress("/Some/Address");
		//	message.reserveBuffer(length);
		//	memcpy(message.processBuffer, print.reversedBuffer, length);

		//	message.process(false);

		//	auto OSC = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);
		//	auto Udp = UDP();

		//	OSC.bindUDP(&Udp, 1, 1);
		//	OSC.addConsumer(&structCons);

		//	message.send(&Udp, false);

		//	OSC.loop(false);

		//	Assert::AreEqual((int)OSC::ColorCommands::SinglePulse, (int)structCons.structMessage->messageStruct.command, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)7, structCons.structMessage->messageStruct.commands.singleColor.duration, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)3, structCons.structMessage->messageStruct.commands.singleColor.end, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)4, structCons.structMessage->messageStruct.commands.singleColor.hue, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)5, structCons.structMessage->messageStruct.commands.singleColor.saturation, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)2, structCons.structMessage->messageStruct.commands.singleColor.start, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)6, structCons.structMessage->messageStruct.commands.singleColor.value, L"Struct does not contain same value", LINE_INFO());
		//}

		//TEST_METHOD(OSCReadEdtMessageFromRaw2Struct) {
		//	auto message = OSC::StructMessage<OSC::EdtMessage, uint8_t>();
		//	auto structCons = OSCEdtMessageConsumer();
		//	auto print = Print();

		//	std::string testString = "/Some/Address___,iii_______\x6___\x2___\x3";
		//	int length = testString.length();

		//	print.write(testString.c_str(), length);

		//	message.setAddress("/Some/Address");
		//	message.reserveBuffer(length);
		//	memcpy(message.processBuffer, print.reversedBuffer, length);

		//	message.process(false);

		//	auto OSC = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);
		//	auto Udp = UDP();

		//	OSC.bindUDP(&Udp, 1, 1);
		//	OSC.addConsumer(&structCons);

		//	message.send(&Udp, false);

		//	OSC.loop(false);

		//	Assert::AreEqual((int)OSC::ColorCommands::Strobo, (int)structCons.structMessage->messageStruct.command, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)2, structCons.structMessage->messageStruct.commands.strobo.hue, L"Struct does not contain same value", LINE_INFO());
		//	Assert::AreEqual((uint8_t)3, structCons.structMessage->messageStruct.commands.strobo.intensity, L"Struct does not contain same value", LINE_INFO());
		//}

	};
}