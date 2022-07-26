#include <unity.h>

#include <OSCMatch.h>

void OSCEqualMatchesTest()
{
    auto tester = OSC::Match();

    TEST_ASSERT_TRUE(tester.isMatch("/Unit1", "/Unit1"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset", "/Unit1/Preset"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit1", "/Unit2"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit1/Preset", "/Unit2"));
}

void OSCSimplePathMatchesTest()
{
    auto tester = OSC::Match();

    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset", "/Unit1"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/Deeper", "/Unit1"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset/Deeper"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset/Deeper/MoreDeeper"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Preset/MoreDeeper"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit1/Preset/Deeper/MoreDeeper", "/Unit1/Deeper/MoreDeeper"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/a", "/Unit1/Preset/a"));
}

void OSCSimpleWildcardMatchesTest()
{
    auto tester = OSC::Match();

    auto pattern = "/F?\0iiii";
    auto address = "/F1";

    TEST_ASSERT_TRUE(tester.isMatch("/F1", "/F?"));
    TEST_ASSERT_TRUE(tester.isMatch("/R1", "/R?"));
    TEST_ASSERT_TRUE(tester.isMatch(address, pattern));
    TEST_ASSERT_FALSE(tester.isMatch(pattern, address));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit12345", "/Unit?2345"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit12345", "/Unit?2?45"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit12345", "/Unit?2?4?"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit12345", "/Unit?2?3?"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit12345", "/Unit?2?1?"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit12345", "/Unit?2???"));
    TEST_ASSERT_TRUE(tester.isMatch("/MegaUnitMegaUnitMegaUnitMegaUnit", "/MegaUnit?egaUnitMegaUnitMeg?Unit"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset", "/Unit?/Preset"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset", "/?n?t?/Preset"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset", "/Unit?/Preset"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit2/Preset/a", "/Unit?/Preset/a"));
}

void OSCComplexWildcardMatchesTest()
{
    auto tester = OSC::Match();

    TEST_ASSERT_TRUE(tester.isMatch("/Unit1", "/*"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit1", "/"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset", "/Unit1/*"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/a", "/Unit1/*/a"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/a", "/Unit1/Preset/*"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/a", "/*/Preset/a"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit1/Preset", "/Unit1/*/a"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/a/b/c", "/Unit1/*/a/*/c"));
    TEST_ASSERT_FALSE(tester.isMatch("/Unit1/Preset/a/b/d", "/Unit1/*/a/*/c"));
    TEST_ASSERT_TRUE(tester.isMatch("/Unit1/Preset/a/c/a", "/*/*/a/*/*"));
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(OSCEqualMatchesTest);
    RUN_TEST(OSCSimplePathMatchesTest);
    RUN_TEST(OSCSimpleWildcardMatchesTest);
    RUN_TEST(OSCComplexWildcardMatchesTest);
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