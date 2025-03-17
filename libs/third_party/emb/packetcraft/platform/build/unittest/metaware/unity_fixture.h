/***
 * Excerpted from "Test-Driven Development for Embedded C",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/jgade for more book information.
***/
//- Copyright (c) 2010 James Grenning and Contributed to Unity Project
/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_FIXTURE_H_
#define UNITY_FIXTURE_H_


extern volatile int uartWrCompleteFlag;
extern volatile int uartRdCompleteFlag;
extern char uartData[10];

int UnityMain(int argc, char* argv[], void (*runAllTests)());


#define TEST_GROUP(group)\
    int TEST_GROUP_##group = 0

#define TEST_SETUP(group) void TEST_##group##_SETUP()

#define TEST_TEAR_DOWN(group) void TEST_##group##_TEAR_DOWN()


#define TEST(group, name) \
    void TEST_##group##_##name##_run();\
    void TEST_##group##_##name##_run()

#define IGNORE_TEST(group, name) \
    void TEST_##group##_##name##_();\
    void TEST_##group##_##name##_run()\
    {\
        UnityIgnoreTest("IGNORE_TEST(" #group ", " #name ")" );\
    }\
    void  TEST_##group##_##name##_()

#define DECLARE_TEST_CASE(group, name) \
    extern void TEST_##group##_##name##_run()

#define RUN_TEST_CASE(group, name) \
        DECLARE_TEST_CASE(group, name);\
    TEST_##group##_##name##_run();

//This goes at the bottom of each test file or in a separate c file
#define TEST_GROUP_RUNNER(group)\
    void TEST_##group##_GROUP_RUNNER_runAll();\
    void TEST_##group##_GROUP_RUNNER()\
    {\
        TEST_##group##_GROUP_RUNNER_runAll();\
    }\
    void TEST_##group##_GROUP_RUNNER_runAll()

//Call this from main
#define RUN_TEST_GROUP(group)\
    void TEST_##group##_GROUP_RUNNER();\
    TEST_##group##_GROUP_RUNNER();



#define TEST_ASSERT_EQUAL(expected, actual) if (expected != actual) while(1);

#endif /* UNITY_FIXTURE_H_ */
