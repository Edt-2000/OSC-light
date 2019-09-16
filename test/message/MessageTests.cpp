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

void OSCMessageMatchTest()
{
    auto message = OSC::Message();

    message.setAddress("/M");

    TEST_ASSERT_TRUE(message.isValidRoute("/M"));

    message.setAddress("/New/Address");

    TEST_ASSERT_FALSE(message.isValidRoute("/M"));
    TEST_ASSERT_TRUE(message.isValidRoute("/New/Address"));

    message.setAddress("/M");

    TEST_ASSERT_TRUE(message.isValidRoute("/M"));
    TEST_ASSERT_FALSE(message.isValidRoute("/New/Address"));
}

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

    newMessage.reserveBuffer(print.bufferSize);
    newMessage.processBuffer = print.buffer;
    newMessage.process();

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

    int bufferLength = newMessage.bufferLength;

    for (int i = 0; i < 1000; i++)
    {

        newMessage.reserveBuffer(print.bufferSize);
        newMessage.processBuffer = print.buffer;
        newMessage.process();
    }

    TEST_ASSERT_TRUE(bufferLength == newMessage.bufferLength);
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

    std::string stringFromSerialization(print.fullBuffer);

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

    std::string stringFromSerialization(print.fullBuffer);

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
    message.reserveBuffer(length);
    memcpy(message.processBuffer, print.reversedBuffer, length);

    message.process(false);

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
    message.reserveBuffer(length);
    memcpy(message.processBuffer, print.reversedBuffer, length);

    message.process(true);

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

    newMessage.reserveBuffer(print.bufferSize);
    newMessage.processBuffer = print.buffer;
    newMessage.process();

    TEST_ASSERT_TRUE(message.messageStruct.int1 == newMessage.messageStruct.int1);
    TEST_ASSERT_TRUE(message.messageStruct.int2 == newMessage.messageStruct.int2);
    TEST_ASSERT_TRUE(message.messageStruct.int3 == newMessage.messageStruct.int3);
    TEST_ASSERT_TRUE(message.messageStruct.int4 == newMessage.messageStruct.int4);
    TEST_ASSERT_TRUE(message.messageStruct.int5 == newMessage.messageStruct.int5);

    TEST_ASSERT_TRUE(strcmp(message.address, newMessage.address) == 0);

    int bufferLength = newMessage.bufferLength;

    for (int i = 0; i < 1000; i++)
    {

        newMessage.reserveBuffer(print.bufferSize);
        newMessage.processBuffer = print.buffer;
        newMessage.process();
    }

    TEST_ASSERT_TRUE(bufferLength == newMessage.bufferLength);
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

    std::string stringFromSerialization(print.fullBuffer);

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

    std::string stringFromSerialization(print.fullBuffer);

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
    message.reserveBuffer(length);
    memcpy(message.processBuffer, print.reversedBuffer, length);

    message.process(false);

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
    message.reserveBuffer(length);
    memcpy(message.processBuffer, print.reversedBuffer, length);

    message.process(true);

    TEST_ASSERT_EQUAL(1u, message.messageStruct.int1);
    TEST_ASSERT_EQUAL(2u, message.messageStruct.int2);
    TEST_ASSERT_EQUAL(3u, message.messageStruct.int3);
    TEST_ASSERT_EQUAL(4u, message.messageStruct.int4);
    TEST_ASSERT_EQUAL(5u, message.messageStruct.int5);
}

void OSCStructLoop()
{
    auto OSC = OSC::Arduino<OSC::StructMessage<Data, uint32_t>>(1, 1);
    auto Udp = UDP();

    auto cons = MessageConsumer();
    auto prod = MessageProducer();

    OSC.bindUDP(&Udp, 1, 1);

    OSC.addConsumer(&cons);
    OSC.addProducer(&prod);

    // complete a full write and a full read write and then store buffer sizes
    OSC.loop(true);
    OSC.loop(true);

    int bufferSizePre = OSC.bufferMessage.bufferLength;

    for (int i = 0; i < 1000; i++)
    {
        prod.message.messageStruct.int1 = i + 0;
        prod.message.messageStruct.int2 = i + 1;
        prod.message.messageStruct.int3 = i + 2;
        prod.message.messageStruct.int4 = i + 3;
        prod.message.messageStruct.int5 = i + 4;

        OSC.loop(true);

        TEST_ASSERT_EQUAL_INT32(i + 0, cons.data.int1);
        TEST_ASSERT_EQUAL_INT32(i + 1, cons.data.int2);
        TEST_ASSERT_EQUAL_INT32(i + 2, cons.data.int3);
        TEST_ASSERT_EQUAL_INT32(i + 3, cons.data.int4);
        TEST_ASSERT_EQUAL_INT32(i + 4, cons.data.int5);
    }

    int bufferSizePost = OSC.bufferMessage.bufferLength;

    TEST_ASSERT_EQUAL_INT32(bufferSizePre, bufferSizePost);
}

void OSCStructLoopStream()
{
    auto OSC = OSC::Arduino<OSC::StructMessage<Data, uint32_t>>(1, 1);
    auto stream = Stream();

    auto cons = MessageConsumer();
    auto prod = MessageProducer();

    OSC.bindStream(&stream);

    OSC.addConsumer(&cons);
    OSC.addProducer(&prod);

    // complete a full write and a full read write and then store buffer sizes
    OSC.loop(true);
    OSC.loop(true);

    int bufferSizePre = OSC.bufferMessage.bufferLength;

    for (int i = 0; i < 1000; i++)
    {
        prod.message.messageStruct.int1 = i + 0;
        prod.message.messageStruct.int2 = i + 1;
        prod.message.messageStruct.int3 = i + 2;
        prod.message.messageStruct.int4 = i + 3;
        prod.message.messageStruct.int5 = i + 4;

        OSC.loop(true);

        TEST_ASSERT_EQUAL_INT32(i + 0, cons.data.int1);
        TEST_ASSERT_EQUAL_INT32(i + 1, cons.data.int2);
        TEST_ASSERT_EQUAL_INT32(i + 2, cons.data.int3);
        TEST_ASSERT_EQUAL_INT32(i + 3, cons.data.int4);
        TEST_ASSERT_EQUAL_INT32(i + 4, cons.data.int5);
    }

    int bufferSizePost = OSC.bufferMessage.bufferLength;

    TEST_ASSERT_EQUAL_INT32(bufferSizePre, bufferSizePost);
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(OSCMessageMatchTest);

    RUN_TEST(OSCMessageContentTest);
    
    RUN_TEST(OSCSerializationBigEndian);
    RUN_TEST(OSCSerializationLittleEndian);
    RUN_TEST(OSCDeserializationBigEndian);
    RUN_TEST(OSCDeserializationLittleEndian);

    RUN_TEST(OSCStructMessageContentTest);

    RUN_TEST(OSCStructSerializationBigEndian);
    RUN_TEST(OSCStructSerializationLittleEndian);
    RUN_TEST(OSCStructDeserializationBigEndian);
    RUN_TEST(OSCStructDeserializationLittleEndian);

    RUN_TEST(OSCStructLoop);
    RUN_TEST(OSCStructLoopStream);

    UNITY_END();
}

int main(int argc, char **argv)
{
    process();
    return 0;
}
