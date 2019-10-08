#include <unity.h>

#include <OSCArduino.h>
#include <OSCMessage.h>
#include <OSCStructMessage.h>
#include <PrintForTest.h>
#include <UdpForTest.h>
#include <StreamForTest.h>
#include <MessageConsumer.h>
#include <MessageProducer.h>
#include <Data.h>

#include <string>
#include <iostream>
#include <memory>

void OSCMessageContentTest()
{
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

    TEST_ASSERT_GREATER_THAN(0, print.dataCountInBuffer);

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    newMessage.process(print.dataCountInBuffer, print.printBuffer, buffer.get());

    for (int i = 0; i < 16; i++)
    {
        if (message.getDataType(i) == OSC::DataType::Float)
        {
            TEST_ASSERT_EQUAL_FLOAT(message.getFloat(i), newMessage.getFloat(i));
        }
        else if (message.getDataType(i) == OSC::DataType::Integer)
        {
            TEST_ASSERT_EQUAL_INT32(message.getInt(i), newMessage.getInt(i));
        }
        else
        {
            TEST_ABORT();
        }
    }

    TEST_ASSERT_TRUE(strcmp(message.address, newMessage.address) == 0);
}

void OSCSerializationBigEndian()
{
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

    std::string stringFromSerialization(print.printUnderscoredBuffer);

    TEST_ASSERT_EQUAL_STRING(
        "/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5",
        stringFromSerialization.c_str());
}

void OSCSerializationLittleEndian()
{
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

    std::string stringFromSerialization(print.printUnderscoredBuffer);

    TEST_ASSERT_EQUAL_STRING(
        "/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___",
        stringFromSerialization.c_str());
}

void OSCDeserializationBigEndian()
{
    auto message = OSC::Message();
    auto print = Print();

    std::string testString = "/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5";
    int length = testString.length();

    print.write(testString.c_str(), length);

    message.setAddress("/Some/Address");
    message.empty();

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    message.process(print.dataCountInBuffer, print.printNormalizedBuffer, buffer.get(), false);

    TEST_ASSERT_EQUAL(1u, message.getInt(0));
    TEST_ASSERT_EQUAL(2u, message.getInt(1));
    TEST_ASSERT_EQUAL(3u, message.getInt(2));
    TEST_ASSERT_EQUAL(4u, message.getInt(3));
    TEST_ASSERT_EQUAL(5u, message.getInt(4));
}

void OSCDeserializationPattern1BigEndian()
{
    auto message = OSC::Message();
    auto print = Print();

    std::string testString = "/S?me/Addre?s___,iiiii_____\x1___\x2___\x3___\x4___\x5";
    int length = testString.length();

    print.write(testString.c_str(), length);

    message.setAddress("/Some/Address");
    message.empty();

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    message.process(print.dataCountInBuffer, print.printNormalizedBuffer, buffer.get(), false);

    TEST_ASSERT_EQUAL(1u, message.getInt(0));
    TEST_ASSERT_EQUAL(2u, message.getInt(1));
    TEST_ASSERT_EQUAL(3u, message.getInt(2));
    TEST_ASSERT_EQUAL(4u, message.getInt(3));
    TEST_ASSERT_EQUAL(5u, message.getInt(4));
}

void OSCDeserializationPattern2BigEndian()
{
    auto message = OSC::Message();
    auto print = Print();

    std::string testString = "/S?me/*_,iiiii_____\x1___\x2___\x3___\x4___\x5";
    int length = testString.length();

    print.write(testString.c_str(), length);

    message.setAddress("/Some/Address");
    message.empty();

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    message.process(print.dataCountInBuffer, print.printNormalizedBuffer, buffer.get(), false);

    TEST_ASSERT_EQUAL(1u, message.getInt(0));
    TEST_ASSERT_EQUAL(2u, message.getInt(1));
    TEST_ASSERT_EQUAL(3u, message.getInt(2));
    TEST_ASSERT_EQUAL(4u, message.getInt(3));
    TEST_ASSERT_EQUAL(5u, message.getInt(4));
}

void OSCDeserializationLittleEndian()
{
    auto message = OSC::Message();
    auto print = Print();

    std::string testString = "/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___";
    int length = testString.length();

    print.write(testString.c_str(), length);

    message.setAddress("/Some/Address");
    message.empty();

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    message.process(print.dataCountInBuffer, print.printNormalizedBuffer, buffer.get(), true);

    TEST_ASSERT_EQUAL(1u, message.getInt(0));
    TEST_ASSERT_EQUAL(2u, message.getInt(1));
    TEST_ASSERT_EQUAL(3u, message.getInt(2));
    TEST_ASSERT_EQUAL(4u, message.getInt(3));
    TEST_ASSERT_EQUAL(5u, message.getInt(4));
}

void OSCStructMessageContentTest()
{
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

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    newMessage.process(print.dataCountInBuffer, print.printBuffer, buffer.get());

    TEST_ASSERT_TRUE(message.messageStruct.int1 == newMessage.messageStruct.int1);
    TEST_ASSERT_TRUE(message.messageStruct.int2 == newMessage.messageStruct.int2);
    TEST_ASSERT_TRUE(message.messageStruct.int3 == newMessage.messageStruct.int3);
    TEST_ASSERT_TRUE(message.messageStruct.int4 == newMessage.messageStruct.int4);
    TEST_ASSERT_TRUE(message.messageStruct.int5 == newMessage.messageStruct.int5);

    TEST_ASSERT_TRUE(strcmp(message.address, newMessage.address) == 0);
}

void OSCStructSerializationBigEndian()
{
    auto message = OSC::StructMessage<Data, uint32_t>();
    auto print = Print();

    message.setAddress("/Some/Address");

    message.messageStruct.int1 = 1;
    message.messageStruct.int2 = 2;
    message.messageStruct.int3 = 3;
    message.messageStruct.int4 = 4;
    message.messageStruct.int5 = 5;

    message.send(&print, false);

    std::string stringFromSerialization(print.printUnderscoredBuffer);

    TEST_ASSERT_EQUAL_STRING(
        "/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5",
        stringFromSerialization.c_str());
}

void OSCStructSerializationLittleEndian()
{
    auto message = OSC::StructMessage<Data, uint32_t>();
    auto print = Print();

    message.setAddress("/Some/Address");

    message.messageStruct.int1 = 1;
    message.messageStruct.int2 = 2;
    message.messageStruct.int3 = 3;
    message.messageStruct.int4 = 4;
    message.messageStruct.int5 = 5;

    message.send(&print, true);

    std::string stringFromSerialization(print.printUnderscoredBuffer);

    TEST_ASSERT_EQUAL_STRING(
        "/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___",
        stringFromSerialization.c_str());
}

void OSCStructDeserializationBigEndian()
{
    auto message = OSC::StructMessage<Data, uint32_t>();
    auto print = Print();

    std::string testString = "/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5";
    int length = testString.length();

    print.write(testString.c_str(), length);

    message.setAddress("/Some/Address");

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    message.process(print.dataCountInBuffer, print.printNormalizedBuffer, buffer.get(), false);

    TEST_ASSERT_EQUAL(1u, message.messageStruct.int1);
    TEST_ASSERT_EQUAL(2u, message.messageStruct.int2);
    TEST_ASSERT_EQUAL(3u, message.messageStruct.int3);
    TEST_ASSERT_EQUAL(4u, message.messageStruct.int4);
    TEST_ASSERT_EQUAL(5u, message.messageStruct.int5);
}

void OSCStructDeserializationLittleEndian()
{
    auto message = OSC::StructMessage<Data, uint32_t>();
    auto print = Print();

    std::string testString = "/Some/Address___,iiiii__\x1___\x2___\x3___\x4___\x5___";
    int length = testString.length();

    print.write(testString.c_str(), length);

    message.setAddress("/Some/Address");

    std::unique_ptr<char[]> buffer(new char[print.dataCountInBuffer]);

    message.process(print.dataCountInBuffer, print.printNormalizedBuffer, buffer.get(), true);

    TEST_ASSERT_EQUAL(1u, message.messageStruct.int1);
    TEST_ASSERT_EQUAL(2u, message.messageStruct.int2);
    TEST_ASSERT_EQUAL(3u, message.messageStruct.int3);
    TEST_ASSERT_EQUAL(4u, message.messageStruct.int4);
    TEST_ASSERT_EQUAL(5u, message.messageStruct.int5);
}

void OSCConsumeStructMessageLoop()
{
    auto OSC = OSC::Arduino<1, 0>();
    auto Udp = UDP();

    auto cons = MessageConsumer();

    OSC.bindUDP(&Udp, 1, 1);

    std::string testString = "/TEST___,iiiii_____\x1___\x2___\x3___\x4___\x5";
    int length = testString.length();

    Udp.write(testString.c_str(), length);
    Udp.normalizeBuffer();

    TEST_ASSERT_GREATER_THAN(0, Udp.parsePacket());

    OSC.addConsumer(&cons);

    OSC.loop(false);

    TEST_ASSERT_TRUE(cons.callbackCalled);
    TEST_ASSERT_EQUAL_INT32(1, cons._message.messageStruct.int1);
    TEST_ASSERT_EQUAL_INT32(2, cons._message.messageStruct.int2);
    TEST_ASSERT_EQUAL_INT32(3, cons._message.messageStruct.int3);
    TEST_ASSERT_EQUAL_INT32(4, cons._message.messageStruct.int4);
    TEST_ASSERT_EQUAL_INT32(5, cons._message.messageStruct.int5);
}

void OSCConsumeTooSmallStructMessageLoop()
{
    auto OSC = OSC::Arduino<1, 0>();
    auto Udp = UDP();

    auto cons = MessageConsumer();

    OSC.bindUDP(&Udp, 1, 1);

    std::string testString = "/TEST___,i_____\x1";
    int length = testString.length();

    Udp.write(testString.c_str(), length);
    Udp.normalizeBuffer();

    TEST_ASSERT_GREATER_THAN(0, Udp.parsePacket());

    OSC.addConsumer(&cons);

    OSC.loop(false);

    TEST_ASSERT_TRUE(cons.callbackCalled);
    TEST_ASSERT_EQUAL_INT32(1, cons._message.messageStruct.int1);
}

void OSCConsumeTooBigStructMessageLoop()
{
    auto OSC = OSC::Arduino<1, 0>();
    auto Udp = UDP();

    auto cons = MessageConsumer();

    OSC.bindUDP(&Udp, 1, 1);

    std::string testString = "/TEST___,iiiiiiiiiiii______\x1___\x2___\x3___\x4___\x5___\x6____\x7___\x8___\x9___\x10___\x11___\x12";
    int length = testString.length();

    Udp.write(testString.c_str(), length);
    Udp.normalizeBuffer();

    TEST_ASSERT_GREATER_THAN(0, Udp.parsePacket());

    OSC.addConsumer(&cons);

    OSC.loop(false);

    TEST_ASSERT_TRUE(cons.callbackCalled);
    TEST_ASSERT_EQUAL_INT32(1, cons._message.messageStruct.int1);
    TEST_ASSERT_EQUAL_INT32(2, cons._message.messageStruct.int2);
    TEST_ASSERT_EQUAL_INT32(3, cons._message.messageStruct.int3);
    TEST_ASSERT_EQUAL_INT32(4, cons._message.messageStruct.int4);
    TEST_ASSERT_EQUAL_INT32(5, cons._message.messageStruct.int5);
}

void OSCProduceStructMessageLoop()
{
    auto OSC = OSC::Arduino<0, 1>();
    auto Udp = UDP();

    auto prod = MessageProducer();

    OSC.bindUDP(&Udp, 1, 1);

    OSC.addProducer(&prod);

    prod._message.messageStruct.int1 = 1;
    prod._message.messageStruct.int2 = 2;
    prod._message.messageStruct.int3 = 3;
    prod._message.messageStruct.int4 = 4;
    prod._message.messageStruct.int5 = 5;

    OSC.loop(true);

    std::string stringFromSerialization(Udp.printUnderscoredBuffer);

    TEST_ASSERT_EQUAL_STRING(
        "/TEST___,iiiii_____\x1___\x2___\x3___\x4___\x5",
        stringFromSerialization.c_str());
}

void OSCStructLoop()
{
    auto OSC = OSC::Arduino<1, 1>();
    auto Udp = UDP();

    auto cons = MessageConsumer();
    auto prod = MessageProducer();

    OSC.bindUDP(&Udp, 1, 1);

    OSC.addConsumer(&cons);
    OSC.addProducer(&prod);

    // complete a full write and a full read write and then store buffer sizes
    OSC.loop(true);
    OSC.loop(true);

    for (int i = 0; i < 1000; i++)
    {
        prod._message.messageStruct.int1 = i + 0;
        prod._message.messageStruct.int2 = i + 1;
        prod._message.messageStruct.int3 = i + 2;
        prod._message.messageStruct.int4 = i + 3;
        prod._message.messageStruct.int5 = i + 4;

        OSC.loop(true);

        TEST_ASSERT_EQUAL_INT32(i + 0, cons._message.messageStruct.int1);
        TEST_ASSERT_EQUAL_INT32(i + 1, cons._message.messageStruct.int2);
        TEST_ASSERT_EQUAL_INT32(i + 2, cons._message.messageStruct.int3);
        TEST_ASSERT_EQUAL_INT32(i + 3, cons._message.messageStruct.int4);
        TEST_ASSERT_EQUAL_INT32(i + 4, cons._message.messageStruct.int5);
    }
}

void OSCStructLoopStream()
{
    auto OSC = OSC::Arduino<1, 1>();
    auto stream = Stream();

    auto cons = MessageConsumer();
    auto prod = MessageProducer();

    OSC.bindStream(&stream);

    OSC.addConsumer(&cons);
    OSC.addProducer(&prod);

    // complete a full write and a full read write and then store buffer sizes
    OSC.loop(true);
    OSC.loop(true);

    for (int i = 0; i < 1000; i++)
    {
        prod._message.messageStruct.int1 = i + 0;
        prod._message.messageStruct.int2 = i + 1;
        prod._message.messageStruct.int3 = i + 2;
        prod._message.messageStruct.int4 = i + 3;
        prod._message.messageStruct.int5 = i + 4;

        OSC.loop(true);

        TEST_ASSERT_EQUAL_INT32(i + 0, cons._message.messageStruct.int1);
        TEST_ASSERT_EQUAL_INT32(i + 1, cons._message.messageStruct.int2);
        TEST_ASSERT_EQUAL_INT32(i + 2, cons._message.messageStruct.int3);
        TEST_ASSERT_EQUAL_INT32(i + 3, cons._message.messageStruct.int4);
        TEST_ASSERT_EQUAL_INT32(i + 4, cons._message.messageStruct.int5);
    }
}

void process()
{
    UNITY_BEGIN();

    RUN_TEST(OSCMessageContentTest);

    RUN_TEST(OSCSerializationBigEndian);
    RUN_TEST(OSCSerializationLittleEndian);
    RUN_TEST(OSCDeserializationBigEndian);
    RUN_TEST(OSCDeserializationPattern1BigEndian);
    RUN_TEST(OSCDeserializationPattern2BigEndian);
    RUN_TEST(OSCDeserializationLittleEndian);

    RUN_TEST(OSCStructMessageContentTest);

    RUN_TEST(OSCStructSerializationBigEndian);
    RUN_TEST(OSCStructSerializationLittleEndian);
    RUN_TEST(OSCStructDeserializationBigEndian);
    RUN_TEST(OSCStructDeserializationLittleEndian);

    RUN_TEST(OSCConsumeStructMessageLoop);
    RUN_TEST(OSCConsumeTooBigStructMessageLoop);
    RUN_TEST(OSCConsumeTooSmallStructMessageLoop);
    RUN_TEST(OSCProduceStructMessageLoop);
    RUN_TEST(OSCStructLoop);
    RUN_TEST(OSCStructLoopStream);

    UNITY_END();
}

int main(int argc, char **argv)
{
    process();
    return 0;
}
