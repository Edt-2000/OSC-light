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

void OSCBundle()
{
    auto consumer1 = MessageConsumer("/C1");
    auto consumer2 = MessageConsumer("/C2");

    auto OSC = OSC::Arduino<2, 0>();
    OSC.addConsumer(&consumer1);
    OSC.addConsumer(&consumer2);

    auto Udp = UDP();

    std::string testString = "#bundle_\x1\x2\x3\x4\x5\x6\x7\x8___\x20/C1_,iiiii_____\x1___\x2___\x3___\x4___\x5___\x20/C2_,iiiii_____\x6___\x7___\x8___\x9___\xa";
    int length = testString.length();

    Udp.write(testString.c_str(), length);
    Udp.normalizeBuffer();

    OSC.bindUDP(&Udp, 0, 0);

    OSC.loop();

    TEST_ASSERT_TRUE(consumer1.callbackCalled);
    TEST_ASSERT_TRUE(consumer2.callbackCalled);

    TEST_ASSERT_EQUAL_INT32(1, consumer1._message.messageStruct.int1);
    TEST_ASSERT_EQUAL_INT32(2, consumer1._message.messageStruct.int2);
    TEST_ASSERT_EQUAL_INT32(3, consumer1._message.messageStruct.int3);
    TEST_ASSERT_EQUAL_INT32(4, consumer1._message.messageStruct.int4);
    TEST_ASSERT_EQUAL_INT32(5, consumer1._message.messageStruct.int5);

    TEST_ASSERT_EQUAL_INT32(6, consumer2._message.messageStruct.int1);
    TEST_ASSERT_EQUAL_INT32(7, consumer2._message.messageStruct.int2);
    TEST_ASSERT_EQUAL_INT32(8, consumer2._message.messageStruct.int3);
    TEST_ASSERT_EQUAL_INT32(9, consumer2._message.messageStruct.int4);
    TEST_ASSERT_EQUAL_INT32(10, consumer2._message.messageStruct.int5);
}

void OSCSingleBundle()
{
    auto consumer1 = MessageConsumer("/C1");
    auto consumer2 = MessageConsumer("/C2");

    auto OSC = OSC::Arduino<2, 0>();
    OSC.addConsumer(&consumer1);
    OSC.addConsumer(&consumer2);

    auto Udp = UDP();

    std::string testString = "#bundle_\x1\x2\x3\x4\x5\x6\x7\x8___\x20/C1_,iiiii_____\x1___\x2___\x3___\x4___\x5";
    int length = testString.length();

    Udp.write(testString.c_str(), length);
    Udp.normalizeBuffer();

    OSC.bindUDP(&Udp, 0, 0);

    OSC.loop();

    TEST_ASSERT_TRUE(consumer1.callbackCalled);
    TEST_ASSERT_FALSE(consumer2.callbackCalled);

    TEST_ASSERT_EQUAL_INT32(1, consumer1._message.messageStruct.int1);
    TEST_ASSERT_EQUAL_INT32(2, consumer1._message.messageStruct.int2);
    TEST_ASSERT_EQUAL_INT32(3, consumer1._message.messageStruct.int3);
    TEST_ASSERT_EQUAL_INT32(4, consumer1._message.messageStruct.int4);
    TEST_ASSERT_EQUAL_INT32(5, consumer1._message.messageStruct.int5);
}

void OSCEmptyBundle()
{
    auto consumer1 = MessageConsumer("/C1");
    auto consumer2 = MessageConsumer("/C2");

    auto OSC = OSC::Arduino<2, 0>();
    OSC.addConsumer(&consumer1);
    OSC.addConsumer(&consumer2);

    auto Udp = UDP();

    std::string testString = "#bundle_\x1\x2\x3\x4\x5\x6\x7\x8___\x0";
    int length = testString.length();

    Udp.write(testString.c_str(), length);
    Udp.normalizeBuffer();

    OSC.bindUDP(&Udp, 0, 0);

    OSC.loop();

    TEST_ASSERT_FALSE(consumer1.callbackCalled);
    TEST_ASSERT_FALSE(consumer2.callbackCalled);
}

void OSCErrorBundle()
{
    auto consumer1 = MessageConsumer("/C1");
    auto consumer2 = MessageConsumer("/C2");

    auto OSC = OSC::Arduino<2, 0>();
    OSC.addConsumer(&consumer1);
    OSC.addConsumer(&consumer2);

    auto Udp = UDP();

    std::string testString = "#bundle_\x1\x2\x3\x4\x5\x6\x7\x8___\x20/C1_,iiiii_____\x1___\x2___\x3___\x4___\x5___\x20";
    int length = testString.length();

    Udp.write(testString.c_str(), length);
    Udp.normalizeBuffer();

    OSC.bindUDP(&Udp, 0, 0);

    OSC.loop();

    TEST_ASSERT_TRUE(consumer1.callbackCalled);
    TEST_ASSERT_FALSE(consumer2.callbackCalled);

    TEST_ASSERT_EQUAL_INT32(1, consumer1._message.messageStruct.int1);
    TEST_ASSERT_EQUAL_INT32(2, consumer1._message.messageStruct.int2);
    TEST_ASSERT_EQUAL_INT32(3, consumer1._message.messageStruct.int3);
    TEST_ASSERT_EQUAL_INT32(4, consumer1._message.messageStruct.int4);
    TEST_ASSERT_EQUAL_INT32(5, consumer1._message.messageStruct.int5);
}

void process()
{
    UNITY_BEGIN();

    RUN_TEST(OSCBundle);
    RUN_TEST(OSCSingleBundle);
    RUN_TEST(OSCEmptyBundle);
    RUN_TEST(OSCErrorBundle);

    UNITY_END();
}

int main(int argc, char **argv)
{
    process();
    return 0;
}
