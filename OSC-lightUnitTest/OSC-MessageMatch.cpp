#include "stdafx.h"
#include "CppUnitTest.h"

#include "Print.h"

#include "..\OSC-light\OSCMessage.h"
#include "..\OSC-light\OSCMatch.h"

#include "..\OSC-light\OSCArduino.h"

#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct Data {
	int int1;
	int int2;
	int int3;
	int int4;
	int int5;
};

struct Data2 {
	int int1;
	float float1;
	int int2;
	float float2;
};

struct DataType1 {
	int int1;
	int int2;
	int int3;
	int int4;
};
struct DataType2 {
	int int1;
	int int2;
	int int3;
	int int4;
};
struct DataType3 {
	int int1;
	int int2;
	int int3;
	int int4;
};
struct DataType4 {
	int int1;
	int int2;
	int int3;
	int int4;
};

enum class DataTypes
{
	DataType1 = 0,
	DataType2 = 1,
	DataType3 = 2,
	DataType4 = 3
};

namespace OSClightUnitTest
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

		const char * pattern() {
			return message.address;
		}

		void callbackMessage(OSC::Message * msg) {

		}
	};

	class OSCDataConsumer : public OSC::AMessageConsumerUsingStructs<DataTypes>
	{
	public:
		OSC::Message message;

		OSCDataConsumer() {
		}

		void loop() {

		}

		OSC::Message * generateMessage() {
			return &message;
		}

		const char * pattern() {
			return message.address;
		}

		DataTypes calledbackEnum;

		void callbackEnum(DataTypes dataTypeEnum) {
			calledbackEnum = dataTypeEnum;
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

		TEST_METHOD(OSCMessageContentTest) {
			auto newMessage = OSC::Message();
			auto message = OSC::Message();
			auto print = Print();

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

		TEST_METHOD(OSCSerialization) {
			auto message = OSC::Message();
			auto print = Print();

			message.setAddress("/Some/Address");
			message.empty();
			message.reserveAtLeast(5);
			message.add<int>(1);
			message.add<int>(2);
			message.add<int>(3);
			message.add<int>(4);
			message.add<int>(5);

			message.send(&print);

			std::string stringFromSerialization(print.fullBuffer);

			Assert::AreEqual(
				stringFromSerialization.c_str(),
				"/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5",
				L"Serialized data not conform spec", LINE_INFO());
		}

		TEST_METHOD(OSCLoop) {
			auto OSC = OSC::Arduino(1, 1);
			auto Udp = UDP();

			auto prodCons = OSCProducerConsumer();

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

		TEST_METHOD(OSCSerializationSpeed) {
			auto message = OSC::Message();
			auto print = Print();

			auto start = std::chrono::system_clock::now();

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

			for (int i = 0; i < 1000; i++) {
				message.send(&print);
			}

			auto end = std::chrono::system_clock::now();

			std::chrono::duration<double> diff = end - start;

			auto value = diff.count();

			Assert::IsTrue(value < 1.0, L"Speed", LINE_INFO());
		}

		TEST_METHOD(OSCReadAsStruct) {
			auto message = OSC::Message();

			auto structCons = OSCDataConsumer();

			message.setAddress("/Some/Address");
			message.empty();
			message.reserveAtLeast(5);
			message.add<int>(1);
			message.add<int>(2);
			message.add<int>(3);
			message.add<int>(4);
			message.add<int>(5);

			auto data = Data();

			structCons.readToStruct(&message, (unsigned char*)&data, sizeof(Data));

			Assert::AreEqual(data.int1, 1, L"Struct does not contain same value", LINE_INFO());
			Assert::AreEqual(data.int2, 2, L"Struct does not contain same value", LINE_INFO());
			Assert::AreEqual(data.int3, 3, L"Struct does not contain same value", LINE_INFO());
			Assert::AreEqual(data.int4, 4, L"Struct does not contain same value", LINE_INFO());
			Assert::AreEqual(data.int5, 5, L"Struct does not contain same value", LINE_INFO());
		}

		TEST_METHOD(OSCReadAsStruct2) {
			auto message = OSC::Message();

			auto structCons = OSCDataConsumer();

			message.setAddress("/Some/Address");
			message.empty();
			message.reserveAtLeast(5);
			message.add<int>(1);
			message.add<float>(2.345f);
			message.add<int>(3);
			message.add<float>(441910.191f);

			auto data = Data2();

			structCons.readToStruct(&message, (unsigned char*)&data, sizeof(Data2));

			Assert::AreEqual(data.int1, 1, L"Struct does not contain same value", LINE_INFO());
			Assert::AreEqual(data.float1, 2.345f, L"Struct does not contain same value", LINE_INFO());
			Assert::AreEqual(data.int2, 3, L"Struct does not contain same value", LINE_INFO());
			Assert::AreEqual(data.float2, 441910.191f, L"Struct does not contain same value", LINE_INFO());
		}

		TEST_METHOD(OSCStagedStructs) {

			auto type1 = DataType1();
			auto type2 = DataType2();
			auto type3 = DataType3();
			auto type4 = DataType4();

			auto structCons = OSCDataConsumer();

			structCons.reserveAtLeast(4);
			structCons.stageStruct(DataTypes::DataType1, &type1, sizeof(DataType1));
			structCons.stageStruct(DataTypes::DataType2, &type2, sizeof(DataType1));
			structCons.stageStruct(DataTypes::DataType3, &type3, sizeof(DataType1));
			structCons.stageStruct(DataTypes::DataType4, &type4, sizeof(DataType1));

			for (int i = 1; i <= 4; ++i) {
				auto message = OSC::Message();
				message.empty();
				message.reserveAtLeast(5);
				message.add<int>((i * 1) - 1);
				message.add<int>(i * 1);
				message.add<int>(i * 2);
				message.add<int>(i * 3);
				message.add<int>(i * 4);

				structCons.callbackMessage(&message);

				if (i == 1) {
					Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType1, L"Wrong enum response", LINE_INFO());

					Assert::AreEqual(type1.int1, 1, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type1.int2, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type1.int3, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type1.int4, 4, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type2.int1, 1, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int2, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int3, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int4, 4, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type3.int1, 1, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int2, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int3, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int4, 4, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type4.int1, 1, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int2, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int3, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int4, 4, L"Struct does not contain same value", LINE_INFO());
				}

				if (i == 2) {
					Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType2, L"Wrong enum response", LINE_INFO());

					Assert::AreEqual(type2.int1, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type2.int2, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type2.int3, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type2.int4, 8, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type1.int1, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int2, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int3, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int4, 8, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type3.int1, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int2, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int3, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int4, 8, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type4.int1, 2, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int2, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int3, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int4, 8, L"Struct does not contain same value", LINE_INFO());
				}

				if (i == 3) {
					Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType3, L"Wrong enum response", LINE_INFO());

					Assert::AreEqual(type3.int1, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type3.int2, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type3.int3, 9, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type3.int4, 12, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type1.int1, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int2, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int3, 9, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int4, 12, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type2.int1, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int2, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int3, 9, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int4, 12, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type4.int1, 3, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int2, 6, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int3, 9, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type4.int4, 12, L"Struct does not contain same value", LINE_INFO());
				}

				if (i == 4) {
					Assert::IsTrue(structCons.calledbackEnum == DataTypes::DataType4, L"Wrong enum response", LINE_INFO());

					Assert::AreEqual(type4.int1, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type4.int2, 8, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type4.int3, 12, L"Struct does not contain same value", LINE_INFO());
					Assert::AreEqual(type4.int4, 16, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type1.int1, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int2, 8, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int3, 12, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type1.int4, 16, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type2.int1, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int2, 8, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int3, 12, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type2.int4, 16, L"Struct does not contain same value", LINE_INFO());

					Assert::AreNotEqual(type3.int1, 4, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int2, 8, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int3, 12, L"Struct does not contain same value", LINE_INFO());
					Assert::AreNotEqual(type3.int4, 16, L"Struct does not contain same value", LINE_INFO());
				}
			}

		}
	};
}