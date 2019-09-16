#include <unity.h>
#include <OSCData.h>

void test_setting_bytes_getting_integer1() {
    OSC::Data data;
    unsigned char bytes[4] = { 0x01, 0x00, 0x00, 0x00 };

    data.set(bytes, OSC::DataType::Integer);

    TEST_ASSERT_EQUAL_INT32(1, data.getInt());
}

void test_setting_bytes_getting_integer2() {
    OSC::Data data;
    unsigned char bytes[4] = { 0x01, 0x00, 0x01, 0xff };

    data.set(bytes, OSC::DataType::Integer);

    TEST_ASSERT_EQUAL_INT32(4278255617, data.getInt());
}

void test_setting_bytes_getting_float1() {
    OSC::Data data;
    unsigned char bytes[4] = { 0xff, 0xff, 0xff, 0xb9 };

    data.set(bytes, OSC::DataType::Float);

    TEST_ASSERT_EQUAL_FLOAT(-0.000488281f, data.getFloat());
}

void test_setting_bytes_getting_float2() {
    OSC::Data data;
    unsigned char bytes[4] = { 0x12, 0x34, 0x56, 0x78 };

    data.set(bytes, OSC::DataType::Float);

    TEST_ASSERT_EQUAL_FLOAT(1.73782e+34f, data.getFloat());
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_setting_bytes_getting_integer1);
    RUN_TEST(test_setting_bytes_getting_integer2);
    RUN_TEST(test_setting_bytes_getting_float1);
    RUN_TEST(test_setting_bytes_getting_float2);
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    delay(4000);

    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif