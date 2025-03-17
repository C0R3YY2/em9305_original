////////////////////////////////////////////////////////////////////////////////
///
/// @file       unitTest.h
///
/// @project    EM9305
///
/// @brief      Support functions for Unit Test
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2018 EM Microelectronic @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////
#ifndef _UNITTEST_H
#define _UNITTEST_H

// For printf support.
// #include <printf.h>
// #include <puts_uart.h>

/**
 * @brief printf function when debug messages are enabled, nothing otherwise.
 */
// #define log_msg(...)                printf(__VA_ARGS__)

#define EOL     "\r\n"


#include <types.h>
#include "unity_fixture.h"

#ifndef STRINGIFY
#define _STRINGIFY(__x__)   #__x__
#define STRINGIFY(__x__)    _STRINGIFY(__x__)
#endif

// List of pre/post functions:
#define TEST_FUNC_NOP           (0u)

#define TEST_PRE_SET_HOST_GP    (1u)

#define TEST_SYNC_READ_CHANNELS         (1u)
#define TEST_SYNC_READ_CHANNELS_EMPTY   (2u)
#define TEST_WRITE_CHANNEL              (3u)
#define TEST_4X_READ_CHANNEL1           (4u)

#define TEST_POST_CHECK_HOST_GP         (1u)
#define TEST_POST_CHECK_PROC_GP         (2u)
#define TEST_POST_CHECK_WAKE_FIFO       (3u)
#define TEST_POST_CHECK_NONWAKE_FIFO    (4u)
#define TEST_POST_READ_CHANNELS         (5u)

typedef enum _unit_test_status_
{
    UNIT_TEST_STATUS_NONE,
    UNIT_TEST_STATUS_PASS,
    UNIT_TEST_STATUS_CANNOT_TEST,
    UNIT_TEST_STATUS_FAIL,
    UNIT_TEST_STATUS_BAD_SENSOR_NUM,
    UNIT_TEST_STATUS_BAD_CUST_ID,
    UNIT_TEST_STATUS_MISSING_SENSOR,
    UNIT_TEST_STATUS_BAD_TEST_NUM,
    UNIT_TEST_STATUS_WARNING,
    UNIT_TEST_STATUS_END_OF_TESTS,
    UNIT_TEST_STATUS_TIMEOUT,
    UNIT_TEST_STATUS_SIGNAL_HOST,
} UNIT_TEST_STATUS;

typedef enum _unit_test_mode_
{
    UNIT_TEST_MODE_NONE,
    UNIT_TEST_MODE_CONTINUE,
    UNIT_TEST_MODE_STEP,
    UNIT_TEST_MODE_LOOP
} UNIT_TEST_MODE;

typedef void(*test_func)(void);
typedef void (*test_init_t)(uint8_t param1, uint8_t param2);

typedef struct _test_list_t_
{
    test_func func;
    uint8_t     last;
    uint8_t     pre_func;
    uint8_t     test_func;
    uint8_t     post_func;
    char      testName[60];
    char* sw_requirements;
    uint32_t sw_requirements_len;
} test_list_t;

typedef struct TestGroup
{
    test_list_t*  tests;
    uint8_t       last;
    char          groupName[27];
} TestGroup;

// #define DECLARE_TEST_GROUPS       TestGroup __attribute__((section(".test_group"))) testGroup[]
// GSc: Test!
// #define DECLARE_TEST_GROUPS       TestGroup __attribute__((section(".persistent"))) testGroup[]
#define DECLARE_TEST_GROUPS        TestGroup testGroup[]

#define TEST_LIST_SKIP()    {.func = NULL,                     .last = 0,    .testName = ""}
#define TEST_LIST_END()     {.func = NULL,                     .last = 0xFF, .testName = ""}
#define TEST_RUN_FUNC(g, t, pre, mid, post) {.func = test_run_ ## g ## _ ## t, .last = 0, .pre_func = pre, .test_func = mid, .post_func = post, .testName = STRINGIFY(t), .sw_requirements = NULL, .sw_requirements_len=0 }
#define TEST_RUN_SIMPLE_FUNC(g, t) {.func = test_run_ ## g ## _ ## t, .last = 0, .pre_func = TEST_FUNC_NOP, .test_func = TEST_FUNC_NOP, .post_func = TEST_FUNC_NOP, .testName = STRINGIFY(t), .sw_requirements = NULL, .sw_requirements_len=0 }
#define TEST_RUN_SIMPLE_FUNC_VREQ(g, t, req) {.func = test_run_ ## g ## _ ## t, .last = 0, .pre_func = TEST_FUNC_NOP, .test_func = TEST_FUNC_NOP, .post_func = TEST_FUNC_NOP, .testName = STRINGIFY(t), .sw_requirements = (char*)req, .sw_requirements_len = sizeof(req) }
#define DEFINE_TEST_RUN(g, t) void test_run_ ## g ## _ ## t () { RUN_TEST_CASE(g, t ); }


typedef struct TestDescriptor
{
    TestGroup *group;
    /* uint8_t      majorNum; */
    /* uint8_t      minorNum; */
    /* UNIT_TEST_MODE   testModes; */
    UNIT_TEST_STATUS testStatus;
    uint8_t      data[6];
    char       outStr[150];
    uint8_t      outStrPos;
} TestDescriptor;



#define MKNAME(n) n ## _tests
#define INIT_TEST_GROUP(n) {.last = 0,      .tests = MKNAME(n), .groupName = STRINGIFY(n) }
#define SKIP_TEST_GROUP()  {.last = 0,      .tests = NULL,      .groupName = ""}
#define END_TEST_GROUP()   {.last = 0xFF,   .tests = NULL,      .groupName = ""}

extern TestDescriptor gCurTest;
extern volatile bool timed_out;
extern volatile bool status_reported;

bool isFloatEqual(float f1, float f2);

void UNITTEST_sendStatus(void);
void UNITTEST_clearTest(void);
void UNITTEST_reportStatus(uint8_t d0, uint16_t d12, uint16_t d34, uint8_t d5);
void UNITTEST_registerTestUnit(test_init_t init_func);

void delay_ms(uint16_t ms);
void start_ms_timer(void);
void start_timeout_ms(uint16_t timeout);
uint16_t stop_timeout_ms(void);

#define UNIT_TEST_COMMAND_REGISTER      HREG_RESERVED_0

void unitTest_waitForHost(void);
void unitTest_signalHost(void);

void unitTest_requiresRegisterInterface(void);

#endif
#define PACK __attribute__((packed))
#define ALIGNED(__x__) __attribute__((aligned(__x__)))

typedef enum _status_values_
{
    STAT_DLE,                        // 0x00
    STAT_INITIALIZED,                 // 0x01
    STAT_RESERVED1,                   // 0x02 unused
    STAT_CRASH_DUMP,                  // 0x03
    STAT_INJECTED_SENSOR_CONFIG_REQ,  // 0x04
    STAT_PASSTHROUGH_RESULTS,         // 0x05
    STAT_SENSOR_SELFTEST_RESULTS,     // 0x06
    STAT_SENSOR_FOC_RESULTS,          // 0x07
    STAT_RESERVED2,                   // 0x08 unused
    STAT_RESERVED3,                   // 0x09 unused
    STAT_FLASH_ERASE_COMPLETE,        // 0x0A
    STAT_FLASH_WRITE_COMPLETE,        // 0x0B
    STAT_RESERVED4,                   // 0x0C unused
    STAT_HOST_EV_TIMESTAMP,           // 0x0D
    STAT_DUT_TEST,                    // 0x0E
    STAT_COMMAND_ERROR,               // 0x0F
    STAT_OTP_CONTENTS,                // 0x10
    STAT_DUT_GROUP_INFO,              // 0x11
    STAT_DUT_NUM_GROUPS,              // 0x12
    STAT_DUT_TEST_INFO,               // 0x13
    STAT_GET_PARAMETER_BASE = 0x0100, // 0x0100 -- upper nibble = 0; least significant 12 bits are parameter page and parameter
    STAT_GET_PARAMETER_TOP = 0x0FFF,  // 0x0FFF
} STATUS_CODE;


typedef struct _command_header_
{
    uint16_t command;
    uint16_t length;
} COMMAND_HEADER;

typedef struct PACK ALIGNED(4) _status_header_
{
    uint16_t status_code;
    uint16_t length;
} STATUS_HEADER;

typedef struct PACK ALIGNED(4) _dut_status_
{
    STATUS_HEADER header;
    uint8_t status;
    uint8_t unused;
    /* uint8_t test_lo; */
    /* uint8_t data[6]; // previously used AUX, AUX1, AUx2, AUX3, AUX4 */
    /* char  str[151]; */
} DUT_STATUS;

typedef struct PACK ALIGNED(4) _dut_num_groups_
{
    STATUS_HEADER header;
    uint8_t num_groups;
    uint8_t reserved[3];
} DUT_NUM_GROUPS;

typedef struct PACK ALIGNED(4) _dut_group_info_
{
    STATUS_HEADER header;
    uint8_t group;
    uint8_t num_tests;
    char  name[48];
} DUT_GROUP_INFO;

typedef struct PACK ALIGNED(4) _dut_test_info_
{
    STATUS_HEADER header;

    uint8_t group;
    uint8_t test;
    uint8_t pre_func;
    uint8_t test_func;

    uint8_t post_func;
    char name[64];
} DUT_TEST_INFO;

/*Added by Guillaume - HOOK in unitTest.h not implemented as done in t7189 project*/
void UNITTEST_init(void);
uint8_t UNITTEST_NumGroupsHIF(void);
DUT_GROUP_INFO* UNITTEST_groupInfoHIF(uint8_t group);
DUT_TEST_INFO* UNITTEST_testInfoHIF(uint8_t group, uint8_t test);
void UNITTEST_continue(void);
void UNITTEST_genericRunner(void);
DUT_STATUS *UNITTEST_runner(uint8_t group, uint8_t test);
/*end*/
