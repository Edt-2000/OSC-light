#include <unity.h>

#include <OSCArduino.h>
#include <OSCMessage.h>
#include <OSCStructMessage.h>
#ifdef ARDUINO
#include <Arduino.h>
#include <StreamForArduinoTest.h>
#else
#include <PrintForTest.h>
#include <UdpForTest.h>
#include <StreamForTest.h>
#endif
#include <MessageConsumer.h>
#include <MessageProducer.h>
#include <Data.h>

#include <string>
#include <iostream>
#include <memory>
#include <chrono>

void OSCStructLoop()
{
    auto OSC = OSC::Arduino<1, 1>();
#ifdef ARDUINO
    auto stream = TestStream();
#else
    auto stream = Stream();
#endif

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

void OSCStructLoopSpeedTest()
{
#ifdef ARDUINO
    int iters[] = {1000, 10000, 100000};
#else
    int iters[] = {1000, 10000, 100000, 1000000, 10000000};
#endif

    auto OSC = OSC::Arduino<1, 1>();
#ifdef ARDUINO
    auto stream = TestStream();
#else
    auto stream = Stream();
#endif

    auto cons = MessageConsumer();
    auto prod = MessageProducer();

    OSC.bindStream(&stream);

    OSC.addConsumer(&cons);
    OSC.addProducer(&prod);

    // complete a full write and a full read write and then store buffer sizes
    OSC.loop(true);
    OSC.loop(true);

    for (auto &iter : iters)
    {
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < iter; i++)
        {
            prod._message.messageStruct.int1 = i + 0;
            prod._message.messageStruct.int2 = i + 1;
            prod._message.messageStruct.int3 = i + 2;
            prod._message.messageStruct.int4 = i + 3;
            prod._message.messageStruct.int5 = i + 4;

            OSC.loop(true);
        }

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

        printf("%d iterations took %d ms \r\n", iter, (duration.count()));
    }
}

void process()
{
    UNITY_BEGIN();

    RUN_TEST(OSCStructLoop);
    RUN_TEST(OSCStructLoopSpeedTest);

    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup()
{
    delay(4000);

    process();
}

void loop()
{
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv)
{
    process();
    return 0;
}

#endif