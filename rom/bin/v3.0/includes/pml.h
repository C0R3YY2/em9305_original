////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/pml.h
///
/// @project    T9305
///
/// @brief      PML driver.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic
/// @cond
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


#ifndef _PML_H
#define _PML_H


#include "arc.h"
#include "types.h"
#include "t9305_periph.h"
#include "config.h"


#if defined(HW_DI) && (HW_DI == 1 || HW_DI == 2 || HW_DI == 3 || HW_DI == 4)
#define ROM_V1_V2
#endif

/**
 * @addtogroup drivers
 * @{
 * @defgroup pml PML Driver
 * @brief Driver for the PML (Power Management Logic).
 *
 * This module contains all the code required to configure and control PML.
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/


/// Initial HF clk freq (in MHz)
#define HF_CLK_FREQ_MHZ                 (24)


/// Initial value for HF XTAL start time
#define PML_BOOT_OVERHEAD_XTAL_US       (1650)


/// Config time for transition to Sleep mode
#define PML_SLEEP_OVERHEAD_CONFIG_US    (100)


/// Margin for transition to Sleep mode
#define PML_SLEEP_OVERHEAD_MARGIN_US    (200)


// This includes also time LL needs for initialization before RF activity
/// Boot overhead (us)
#define PML_BOOT_OVERHEAD_US            (0)


/// Sleep overhead (us)
#define PML_SLEEP_OVERHEAD_US           (PML_SLEEP_OVERHEAD_CONFIG_US + PML_SLEEP_OVERHEAD_MARGIN_US)


/// Minimum sleep (us)
#define PML_SLEEP_MINIMAL_US            (15000)

// Register masks and mode definitions for PML config mode API
/// MASK for PML Config Mode bit
#define PML_CONFIG_MODE_REGMASK         (0x00800000u)
/// Macro to set the Config mode bit
#define PML_CONFIG_MODE                 (0x1u << 23)

// Register masks and mode definitions for PML mode API
/// MASK for PML Mode bits
#define PML_MODE_REGMASK                (0x00030000u)
/// Value of PML Mode bits for default Mode
#define PML_DEFAULT_MODE                (0x0u)
/// Value of PML Mode bits for Emcore Mode
#define PML_EM_CORE_MODE                (0x1u)
/// Value of PML Mode bits for Bootloader Mode
#define PML_BOOTLOADER_MODE             (0x2u)
/// Value of PML Mode bits for Application Mode
#define PML_APPLICATION_MODE            (0x3u)
/// Macro to set the delected mode
#define PML_MODE_TO_BIT(X)              (X << 16)
/// Macro to access mode as an enum
#define PML_BIT_TO_MODE(X)              (((X) & PML_MODE_REGMASK) >> 16)
/// MASK for PML CRC Error bit
#define PML_CRC_ERR_REGMASK             (0x00040000u)
/// Macro to set the CRC Error bit
#define PML_CRC_ERR_BIT                 (1 << 18)

#ifndef ROM_V1_V2
/// MASK for PML Precharge delay skip
#define PML_PRECHARGE_DELAY_SKIP_REGMASK    (0x00000001u)
/// Macro to read pre-charge delay skip
#define PML_PRECHARGE_DELAY_SKIP_BIT        (1 << 0)
/// Macro to set pre-charge delay skip bit
#define PML_PRECHARGE_DELAY_SKIP_TO_BIT(x)  (x << 0)
/// MASK for PML Precharge delay index
#define PML_PRECHARGE_DELAY_IDX_REGMASK     (0x00000006u)
/// Macro to read pre-charge delay index
#define PML_PRECHARGE_DELAY_IDX_BITS        (0x3 << 1)
/// Macro to set pre-charge delay index
#define PML_PRECHARGE_DELAY_IDX_TO_BITS(x)  (x << 1)
/// Macro to access pre-charge delay index
#define PML_BITS_TO_PRECHARGE_DELAY_IDX(X)  (((X) & PML_PRECHARGE_DELAY_IDX_REGMASK) >> 1)
/// Pre-charge delay index 1 ms delay
#define PML_PRECHARGE_DELAY_1000_US_IDX     0
/// Pre-charge delay index 1.3 ms delay
#define PML_PRECHARGE_DELAY_1300_US_IDX     1
/// Pre-charge delay index 0.7 ms delay
#define PML_PRECHARGE_DELAY_700_US_IDX      2
/// Pre-charge delay index 0.5 ms delay
#define PML_PRECHARGE_DELAY_500_US_IDX      3
/// MASK for PML NVM Brown out comparator status
#define PML_BROWNOUT_COMP_STAT_REGMASK      (0x00000008u)
/// Macro to read NVM Brown out comparator status
#define PML_BROWNOUT_COMP_STAT_BIT          (1 << 3)
/// Macro to set NVM Brown out comparator status
#define PML_BROWNOUT_COMPT_STAT_TO_BIT(x)   (x << 3)
#endif

/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/


/**
 * @brief Type for a voltage value.
 */
typedef int8_t Voltage_t;


/**
 * @brief  Application modes
 */
typedef enum
{
    PML_APP_MODE_STEP_DOWN      = 0,    /**< Step-down configuration */
    PML_APP_MODE_DCDC_OFF       = 1,    /**< DCDC-off configuration */
    PML_APP_MODE_STEP_UP        = 2,    /**< Step-up configuration */
    PML_APP_MODE_EXTERNAL_UP    = 3,    /**< External-up configuration */
} PML_AppMode_t;


/**
 * @brief Data structure holding the voltage monitor configuration
 */
typedef struct PACKED_STRUCT
{
    /// Actual voltage level corresponding to application mode - VBAT1/VCC [level]
    Voltage_t voltage;
    //PML_LdoPaLvl_t ldoPa;
    //PML_LdoVccLvl_t ldoVcc;
    //PML_LdoDigLvl_t ldoDig;
} PML_Voltage_t;


/**
 * @brief  HF clock frequency
 */
typedef enum
{
    PML_HF_CLK_FREQ_24MHZ = 0,      /**< 24MHz */
    PML_HF_CLK_FREQ_48MHZ = 1,      /**< 48MHz */
} PML_HfClkFrequency_t;


/**
 * @brief PML configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// LF XTAL freqency expressed by ratio HF to LF clks - numerator
    uint32_t lfClkFreqRatioHf;
    /// LF XTAL freqency expressed by ratio HF to LF clks - denumerator
    uint32_t lfClkFreqRatioLf;
    /// Accuracy of LF clk source (Sleep Clock Accuracy) in [ppm]
    uint16_t lfClkSourceAccuracy;
    /// Type of LF clk source
    uint8_t lfClkSourceType;
    /// Skip RC calibration and use constants (freq)
    bool rcCalibSkip;
    /// RC calibration HF limit
    uint32_t rcCalibHfLimit;
    /// RC calibration LF limit
    uint32_t rcCalibLfLimit;
    /// RC calibration period in LF clock numbers.
    /// 0 means this value is ignored.
    uint32_t rcCalibPeriod;
    /// Static correction applied to calibration results [ppm].
    /// Positive value prolongs Sleep interval.
    int16_t rcCalibCorrection;
    /// HF clk frequency in MHz
    uint16_t hfClkFreq;
    /// Boot overhead time [us]
    uint32_t overheadBootTime;
    /// Sleep overhead time [us]
    uint32_t overheadSleepTime;
    /// Minimal time in sleep [us]
    uint32_t minimalSleepTime;
    /// Do not go to sleep mode
    uint8_t sleepModeForbiden;
    /// Bypass switch enable in Sleep mode
    bool bypassSwitchEnable;
    /// Use LDO_VCC during RC calibration to limit ripple from DCDC
    bool useLdoVccDuringCalib;
    /// reserved
    uint8_t reserved;

} PML_Configuration_t;


/**
 * @brief Wakeup action task
 */
typedef enum
{
    /// enable HF XTAL
    PML_WAKEUP_ACTION_RUN_HF_XTAL     = 0,
    /// update protocol timer
    PML_WAKEUP_ACTION_RUN_PROT_TIM    = 1,
} PML_WakeupActionType_t;


/**
 * @brief  System modes (active power mode)
 */
typedef enum
{
    /// Start-up
    PML_SYS_MODE_STARTUP        = 0,
    /// Low power
    PML_SYS_MODE_LOW_POWER      = 1,
    /// Active RC
    PML_SYS_MODE_ACTIVE_RC      = 2,
    /// Active XTAL
    PML_SYS_MODE_ACTIVE_XTAL    = 3,
} PML_SysMode_t;


/**
 * @brief  HF clock source
 */
typedef enum
{
    /// RC
    PML_HF_CLK_RC   = PML_SYS_MODE_ACTIVE_RC,
    /// XTAL
    PML_HF_CLK_XTAL = PML_SYS_MODE_ACTIVE_XTAL,
} PML_HfClkSource_t;


/**
 * @brief  LF Clk source modes
 */
typedef enum
{
    /// LF XTAL disabled
    PML_LF_CLK_XTAL_DIS   = 0,
    /// LF XTAL enabled
    PML_LF_CLK_XTAL_EN    = 1,
    /// External square wave clock is used
    PML_LF_CLK_XTAL_SQ    = 2,
    /// External sine wave clock is used
    PML_LF_CLK_XTAL_SINE  = 3,
} PML_LfClkSource_t;


/**
 * @brief  Sleep modes
 */
typedef enum
{
    /// Sleep CPU - least power saving
    PML_SLEEP_CPU   = 1,
    /// Sleep mode
    PML_SLEEP       = 5,
    /// Deep Sleep mode
    PML_DEEP_SLEEP  = 6,
} PML_SleepMode_t;


/**
 * @brief structure to enable VDD domains
 */
/// Disable retention
#define PML_RETENTION_DIS       (0)
/// Enable DRAM0 retention
#define PML_DRAM0_RETENTION     PML_DRAM_RET_ON(1<<0)
/// Enable DRAM1 retention
#define PML_DRAM1_RETENTION     PML_DRAM_RET_ON(1<<1)
/// Enable DRAM2 retention
#define PML_DRAM2_RETENTION     PML_DRAM_RET_ON(1<<2)
/// Enable DRAM3 retention
#define PML_DRAM3_RETENTION     PML_DRAM_RET_ON(1<<3)
/// Enable DRAM4 retention
#define PML_DRAM4_RETENTION     PML_DRAM_RET_ON(1<<4)
/// Enable DRAM5 retention
#define PML_DRAM5_RETENTION     PML_DRAM_RET_ON(1<<5)
/// Enable DRAM6 retention
#define PML_DRAM6_RETENTION     PML_DRAM_RET_ON(1<<6)
/// Enable DRAMx retention
#define PML_DRAM_RETENTION(x)   PML_DRAM_RET_ON(1<<x)
/// Number of DRAM
#define PML_DRAM_COUNT          (7)


/**
 * @brief structure to enable VDD domains
 */
/// Enable VDD NVM domain
#define PML_VDD_NVM    PML_NVM_REQ_ON_R(1)
/// Enable VDD QDEC domain
#define PML_VDD_QDEC   PML_QDEC_REQ_ON_R(1)


/**
 * @brief Mask for automatic Sleep mode enable/disable
 */
typedef enum
{
    /// Sleep mode disable via HCI command
    PML_AUTO_SLEEP_DISABLE_HCI  = 0x01,
    /// Sleep mode disable due RF operations
    PML_AUTO_SLEEP_DISABLE_RF   = 0x02,
    /// Sleep mode disable due RF operations
    PML_AUTO_SLEEP_DISABLE_ALL  = 0x03,
} PML_AutomaticSleepDisableMask_t;


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

/// Application Mode
extern volatile PML_AppMode_t gPmlAppMode;
/// Voltage monitor configuration
extern volatile PML_Voltage_t gPmlVoltage;


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Wait till domain is really turn on/off.
 * @param domain PML_VDD_NVM - select VDD9 domain
 *               PML_VDD_QDEC - select VDD0 domain
 *               note that input domain can be ORed
 *                   (e.g. PML_VDD_NVM | PML_VDD_QDEC)
 * @param enable TRUE switch selected domain on
 *               FALSE switch selected domain off
 * @note
 *   This function just waits till the selected
 *   domain is switch on.
 */
void PML_EnableVddDomainWait(uint8_t domain, bool enable);


/**
 * @brief Turn on/off selected voltage domain.
 * @param domain PML_VDD_NVM - select VDD9 domain
 *               PML_VDD_QDEC - select VDD0 domain
 *               note that input domain can be ORed
 *                   (e.g. PML_VDD_NVM | PML_VDD_QDEC)
 * @param enable TRUE switch selected domain on
 *               FALSE switch selected domain off
 * @note
 *   This function is blocking, it waits till the selected
 *   domain is switched on or off.
 */
void PML_EnableVddDomain(uint8_t domain, bool enable);


/**
 * @brief Turn on/off selected voltage domain.
 * @param domain PML_VDD_NVM - select VDD9 domain
 *               PML_VDD_QDEC - select VDD0 domain
 *               note that input domain can be ORed
 *                   (e.g. PML_VDD_NVM | PML_VDD_QDEC)
 * @param enable TRUE switch selected domain on
 *               FALSE switch selected domain off
 * @note
 *   This function is non-blocking.
 */
void PML_EnableVddDomainNonBlocking(uint8_t domain, bool enable);


/**
 * @brief Set HF clock frequency configuration.
 * @param freq HF clock frequency to be set.
 */
void PML_SetHfClkFrequency(PML_HfClkFrequency_t freq);


/**
 * @brief PML Wait till clock is restored
 * @note
 *   If HF XTAL was used before sleep then it will wait
 *   until HF XTAL is running again
 */
void PML_WaitTillClockRestored(void);


/**
 * @brief Wait till the source of HF clock oscillator is switched
 * @param expHfClkSrc expected source:
 *            PML_HF_CLK_RC   = RC
 *            PML_HF_CLK_XTAL = XTAL
 * @note This function is blocking, it waits till the clock
 *       source is switched. Function uses IRQs and goes to SleepCPU thus IRQs
 *       have to be enabled.
 */
void PML_SetHfClkSourceWait(PML_HfClkSource_t expHfClkSrc);


/**
 * @brief Select HF clock oscillator: XTAL or RC
 * @param hfClkSrc select oscillator:
 *            PML_HF_CLK_RC   = RC
 *            PML_HF_CLK_XTAL = XTAL
 * @note This function is non blocking
 *          - it does not wait till the clock source is switched
 */
void PML_SetHfClkSourceNonBlocking(PML_HfClkSource_t hfClkSrc);


/**
 * @brief Select HF clock oscillator: XTAL or RC
 * @param hfClkSrc select oscillator:
 *            PML_HF_CLK_RC   = RC
 *            PML_HF_CLK_XTAL = XTAL
 * @note This function is blocking
 *          - it waits till the clock source is switched
 */
void PML_SetHfClkSource(PML_HfClkSource_t hfClkSrc);


/**
 * @brief Select LF clock oscillator: XTAL or RC, SINE, SQUARE
 * @param lfClkSrc select clk source:
 *            PML_LF_CLK_XTAL_DIS  = Internal RC
 *            PML_LF_CLK_XTAL_EN   = External XTAL
 *            PML_LF_CLK_XTAL_SQ   = External source of square wave signal
 *            PML_LF_CLK_XTAL_SINE = External source of sine wave signal
 * @note This function is non blocking
 */
void PML_SetLfClkSource(PML_LfClkSource_t lfClkSrc);


/**
 * @brief Use LF XTAL/External clock for sleep timer
 * @param enable TRUE use xtal/external clock, FALSE uses RC clock
 */
void PML_UseLfXtalClk(bool enable);


/**
 * @brief Convert LF clk to HF clock
 * @note Calibration values stored in PML are used. Their value depends on the
 *        LF clock mode. If LF XTAL is used, the calibration constant from OTP
 *        are used. Otherwise calibration constants are obtained from RC Calib.
 * @param lfClk Value of LF clks
 * @return Value of HF clks converted from LF clks.
 */
uint64_t PML_ConvertLf2HfClk(uint32_t lfClk);


/**
 * @brief Convert HF clk to LF clock
 * @note Calibration values stored in PML are used. Their value depends on the
 *        LF clock mode. If LF XTAL is used, the calibration constant from OTP
 *        are used. Otherwise calibration constants are obtained from RC Calib.
 * @param hfClk Value of HF clks
 * @return Value of LF clks converted from HF clks.
 */
uint32_t PML_ConvertHf2LfClk(uint64_t hfClk);


/**
 * @brief Configure RC Calibration
 * @note Just register calibration limits of both counters, enable irq and
 *       update boot overhead time.
 */
void PML_ConfigureSleepRcCalibBeforeSleep(void);


/**
 * @brief Disable RC Calibration
 * @note Just disable irq and updated boot overhead time;
 */
void PML_DisableSleepRcCalibBeforeSleep(void);


/**
 * @brief Go to sleep
 * @param sleepMode 5 = PML_SLEEP
 *                  6 = PML_DEEP_SLEEP   (relaxed RC Osc)
 * @return FALSE if sleep mode entry is denied, otherwise system goes to sleep
 *               and never return from this function.
 */
bool PML_GoToSleep(PML_SleepMode_t sleepMode);


/**
 * @brief Go to sleep without check (Sleep mode is forced)
 * @param sleepMode 5 = PML_SLEEP
 *                  6 = PML_DEEP_SLEEP   (relaxed RC Osc)
 * @return FALSE if sleep mode entry is denied, otherwise system goes to sleep
 *               and never return from this function.
 */
bool PML_GoToSleepWithoutChecks(PML_SleepMode_t sleepMode);


/**
 * @brief System shut-down
 * @note Function trigger system shutting-down. Shut-down means that
 *       sleep timer is disabled, all IRQs are disabled and system goes to
 *       deep sleep mode.
 */
void PML_SystemShutDown(void);


/**
 * @brief Configuration of wake-up by GPIO pad
 * @note Function configures wake-up event triggered by GPIO pad
 * @param gpio gpio pad used to wakeing-up
 * @param wakePolarity gpio pad wake-up polarity
 * @param wakeEnable gpio pad wake-up enable
 * @param irqEnable enable IRQ (it substitutes wakeup function in active mode)
 */
void PML_ConfigWakeUpByGpio(uint8_t gpio, bool wakePolarity, bool wakeEnable, bool irqEnable);


/**
 * @brief Update (recalculate) wakeup time already set in sleep timer
 * @note Function goes through the internal PML wake up time set by
 *       PML_ConfigWakeByStInHfClk function saved in HF clk. If wakeup time is
 *       equaled zero, skip it.
 */
void PML_UpdateWakeUpTime(void);


/**
 * @brief Configuration of wake-up by sleep timer (set by LF clk)
 * @note Function configures wake-up event triggered by sleep timer in
 *       requested time.
 * @param sleepTimeLfClk sleep time in LF clock
 * @param sleepChannel channel of ST to be used
 */
void PML_ConfigWakeByStInLfClk(uint32_t sleepTimeLfClk, uint8_t sleepChannel);


/**
 * @brief Configuration of wake-up by sleep timer (set by HF clk)
 * @note Function configures wake-up event triggered by sleep timer in
 *       requested time.
 * @param sleepTimeHfClk sleep time in HF clock (~40.63ns)
 * @param sleepChannel channel of ST to be used
 */
void PML_ConfigWakeByStInHfClk(uint32_t sleepTimeHfClk, uint8_t sleepChannel);


/**
 * @brief Configuration of wake-up by sleep timer (set by PT clk and shorten)
 * @note Function configures wake-up event triggered by sleep timer in
 *       requested time. Sleep time will be shorted to ensure CPU is woken and
 *       initialized in requested time.
 * @param wakeTimestamp time to wake up in system clocks
 * @param sleepChannel channel of ST to be used
 * @param wakeDelta
 * @param wakeReference
 * @return TRUE, requested time is corrected and set, FALSE otherwise
 */
bool PML_ConfigWakeByStInSysClkWithBootTimeComp(SystemTime_t wakeTimestamp,
                                                uint8_t sleepChannel,
                                                SystemTime_t wakeDelta,
                                                SystemTime_t wakeReference);


/**
 * @brief Register action which will be performed during booting-up after sleep.
 * @note Function registers selected action which will be performed during
 *       booting up after sleep mode to requested wake-up source.
 *       Function also registers all task which are required to selected one.
 * @param actionType select action to be performed (PML_WakeupActionType_t)
 *              PML_WAKEUP_ACTION_RUN_HF_XTAL   - enable HF XTAL
 *              PML_WAKEUP_ACTION_RUN_PROT_TIM  - update protocol timer
 *                                                enable HF XTAL
 *              PML_WAKEUP_ACTION_RUN_RF        - enable RF
 *                                                update protocol timer
 *                                                enable HF XTAL
 * @param mask select source of wake-up by 16bits mask,
 *               GPIO (0-11), SLEEP_CHANNEL (12-15)
 * @param enable TRUE - action is applied to requested wake-up source
 *               FALSE - action is removed from requested wake-up source
 */
void PML_RegisterWakeUpAction(PML_WakeupActionType_t actionType,
                              uint32_t mask,
                              bool enable);


/**
 * @brief Return whether automatic sleep mode should
 * be used.  Sleep mode will be automatically entered if there is sufficient
 * time to enter sleep mode and the link layer is not currently active.
 * @returns if automated sleep mode should be used.
 */
bool PML_IsVendorSleepAutomatic(void);


/**
 * @brief Return actual frequency of HF clock in MHz
 * @returns HF clock freqency
 */
uint16_t PML_GetHfClkFreq(void);


/**
 * @brief Get Sleep Clock Accuracy used in chip
 * @returns Sleep clock accuracy [ppm]
 */
uint16_t PML_GetSleepClockAccuracy(void);


/**
 * @brief Disable or enable automatic Sleep mode
 * @param mask Mask defining reason to disable/enable Sleep mode
 * @param disable TRUE disables Sleep mode
 *               FALSE enables Sleep mode
 * @returns State of the disable reason before it was changed.
 * @note This functions sets/clears flags indicating that Sleep mode
 * cannot be used. There is several flags defined by
 * @ref PML_AutomaticSleepDisableMask_t
 * and if at least one of those flags is set Sleep mode cannot be used.
 */
bool PML_AutomaticSleepModeDisable(PML_AutomaticSleepDisableMask_t mask,
                                   bool disable);


/**
 * @brief Update resume time after resuming from Sleep mode
 * @param timeInHfClk Measured time in HF clocks for resume from Sleep mode
 * @param timeInLfClk Measured time in LF clocks for resume from Sleep mode
 * @note Both values timeInHfClk and timeInLfClk are added
 */
void PML_UpdateResumeTime(uint32_t timeInHfClk, uint32_t timeInLfClk);


/**
 * @brief Get reset flags which caused last chip reset
 * @return reset flags
 */
uint16_t PML_GetResetFlags(void);


/**
 * @brief Indicates if the DRAM retention was disabled during last chip reset.
 * @return TRUE if the DRAM retention was disabled, FALSE otherwise.
 */
bool PML_DidBootWithoutDRAMRetention(void);


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/


/**
 * @brief Get USB voltage detected flag.
 * @return Function returns true if USB voltage was detected.
 */
static ALWAYS_INLINE bool PML_IsUsbDetected(void)
{
    return GET_USB_DETECTED_R(SYS->RegPmlSts.regs.PmlMode);
}


/**
 * @brief Get Current Application mode.
 * @return Function returns current application mode.
 */
static ALWAYS_INLINE uint8_t PML_GetPmlAppMode(void)
{
    return GET_PML_APP_MODE_R(SYS->RegPmlSts.regs.PmlMode);
}


/**
 * @brief Get Current System mode.
 * @return Function returns current system mode.
 */
static ALWAYS_INLINE PML_SysMode_t PML_GetPmlSysMode(void)
{
    return GET_PML_SYS_MODE_R(SYS->RegPmlSts.regs.PmlMode);
}


/**
 * @brief Enter Sleep CPU mode - least power saving
 */
static ALWAYS_INLINE void PML_EnterSleepCPU(void)
{
    SleepWithMode(PML_SLEEP_CPU);
}


/**
 * @brief Enter Sleep mode
 * @note
 *   Function enters Sleep mode.
 *   Interrupts are not enabled since they cannot wake up CPU from Sleep mode.
 */
static ALWAYS_INLINE void PML_EnterSleep(void)
{
    SleepWithMode(PML_SLEEP);
}


/**
 * @brief Enter Deep Sleep mode
 * @note
 *   Function enters Deep Sleep mode.
 *   Interrupts are not enabled since they cannot wake up CPU from Deep Sleep
 *   mode.
 */
static ALWAYS_INLINE void PML_EnterDeepSleep(void)
{
    SleepWithMode(PML_DEEP_SLEEP);
}


/**
 * @brief Is XTAL source of HF clk
 * @return TRUE if HF source is XTAL
 */
static ALWAYS_INLINE bool PML_HfClkIsXtal(void)
{
    return (GET_PML_SYS_MODE( SYS->RegPmlSts.r32 ) == PML_SYS_MODE_ACTIVE_XTAL);
}


/**
 * @brief Is RC source of HF clk
 * @return TRUE if HF source is RC, FALSE otherwise
 */
static ALWAYS_INLINE bool PML_HfClkIsRc(void)
{
    return (GET_PML_SYS_MODE( SYS->RegPmlSts.r32 ) == PML_SYS_MODE_ACTIVE_RC);
}


/**
 * @brief Is DRAMx retention on
 * @param dram DRAM block number: 0..6
 * @return TRUE, if retention of DRAMx is on, FALSE otherwise
 */
static ALWAYS_INLINE bool PML_IsDramRetOn(uint8_t dram)
{
    uint8_t dramRet = (uint8_t)GET_PML_DRAM_RET_ON(PML->RegPmlDomain.r32);
    return ((dramRet & (1 << dram)) != 0x00);
}


/**
 * @brief Get wake-up status flags
 * @return Wakeup flags for 12 GPIOs, 4 Sleep Timer channels, and 3 for QDec.
 */
static ALWAYS_INLINE uint32_t PML_GetWakeUpFlags(void)
{
    return SYS->RegPmlWakeSts.r32;
}


static ALWAYS_INLINE bool PML_IsVddDomainEnabled(uint8_t domain)
{
    return SYS->RegPmlSts.regs.PmlDomainSts & domain;
}


/**
 * @brief Switch on/off RAM retention
 * @param ramRetention RAM retention configuration for DRAM0 - DRAM6.
 */
static ALWAYS_INLINE void PML_SetRamRetentionEnable(uint8_t ramRetention)
{
    PML->RegPmlDomain.regs.PmlDRAMRetOn = (uint8_t)PML_DRAM_RET_ON_R(ramRetention);
}


/**
 * @brief Get RAM retention state
 * @returns State of RAM retention configuration
 */
static ALWAYS_INLINE uint8_t PML_GetRamRetentionEnable(void)
{
    return PML->RegPmlDomain.regs.PmlDRAMRetOn;
}


/**
 * @brief Enable wake-up flags feature
 * @param enable TRUE wake event is registered, FALSE is not registered
 */
static ALWAYS_INLINE void PML_EnableWakeUpFlags(bool enable)
{
    REG_WRITE_BITS_SAFE(PML->RegPmlCtrl.regs.PmlWakeCtrl, PML_WAKE_FLG_EN_RMASK,
                        PML_WAKE_FLG_EN_R((uint32_t)enable));
}


/**
 * @brief Clear wake-up flags
 */
static ALWAYS_INLINE void PML_ClearWakeUpFlags(void)
{
    REG_SET_BITS_SAFE(PML->RegPmlCtrl.regs.PmlModeCtrl, PML_WAKE_CLEAR_RMASK);
}


/**
 * @brief Enable pad latches
 * @param enable TRUE pad latches are active, FALSE latches are transparent.
 */
static ALWAYS_INLINE void PML_EnablePadLatch(bool enable)
{
    REG_WRITE_BITS_SAFE(PML->RegPmlCtrl.regs.PmlWakeCtrl, PML_LATCH_PAD_EN_RMASK,
                        PML_LATCH_PAD_EN_R((uint32_t)enable));
}


/**
 * @brief Get current HF clock frequency.
 * @returns Current HF clock frequency configuration.
 */
static ALWAYS_INLINE PML_HfClkFrequency_t PML_GetHfClkFrequency(void)
{
    if (1 == GET_CLK_SEL_FREQ(SYS->RegClkCfg.r32))
    {
        return PML_HF_CLK_FREQ_48MHZ;
    }
    else
    {
        return PML_HF_CLK_FREQ_24MHZ;
    }
}


/**
 * @brief Get if the chip has booted from POR.
 * @return TRUE if the chip booted from POR, FALSE otherwise.
 */
static ALWAYS_INLINE uint8_t PML_DidBootFromPOR(void)
{
    return (0 != (PML_GetResetFlags() & PML_POR_FLG_MASK));
}


/**
 * @brief Get if the chip has booted from sleep.
 * @return TRUE if the chip booted from sleep, FALSE otherwise.
 */
static ALWAYS_INLINE uint8_t PML_DidBootFromSleep(void)
{
    return (0 != (PML_GetResetFlags() & (PML_SLEEP_FLG_MASK |
                                         PML_DEEP_SLEEP_FLG_MASK))
        && !PML_DidBootWithoutDRAMRetention());
}


/**
 * @brief Get if the chip has booted from reset.
 * @return TRUE if the chip booted from reset, FALSE otherwise.
 */
static ALWAYS_INLINE uint8_t PML_DidBootFromReset(void)
{
    return (0 != (PML_GetResetFlags() & (PML_PWR_CHK_FLG_MASK |
                                         PML_SW_RST_FLG_MASK  |
                                         PML_WD_FLG_MASK      |
                                         PML_NVM_BO_FLG_MASK)));
}


/**
 * @brief Get the state of the Configuration Mode flag.
 * @returns Return the state of the Configuration Mode flag.
 */
static ALWAYS_INLINE bool PML_GetConfigurationModeFlag(void)
{
    return (PML->RegPmlSysMode.r32 & PML_CONFIG_MODE_REGMASK) == PML_CONFIG_MODE;
}


/**
 * @brief Set the Configuration Mode flag.
 */
static ALWAYS_INLINE void PML_SetConfigurationModeFlag(void)
{
    PML->RegPmlSysMode.r32 |= PML_CONFIG_MODE_REGMASK;
}


/**
 * @brief Clear the Configuration Mode flag.
 */
static ALWAYS_INLINE void PML_ClearConfigurationModeFlag(void)
{
    PML->RegPmlSysMode.r32 &= ~PML_CONFIG_MODE_REGMASK;
}


/**
 * @brief Get the state of the EM Core Mode flag.
 * @returns Return the state of the EM Core Mode flag.
 */
static ALWAYS_INLINE bool PML_GetEmCoreModeFlag(void)
{
    return (PML->RegPmlSysMode.r32 & PML_MODE_REGMASK) == PML_MODE_TO_BIT(PML_EM_CORE_MODE);
}


/**
 * @brief Set the EM Core Mode flag. (Must not be in configuration mode for
 * this to have any effect)
 */
static ALWAYS_INLINE void PML_SetEmCoreModeFlag(void)
{
    // Clear the mode
    PML->RegPmlSysMode.r32 &= ~PML_MODE_REGMASK;
    // Set the mode
    PML->RegPmlSysMode.r32 |= PML_MODE_TO_BIT(PML_EM_CORE_MODE);
}


/**
 * @brief Get the state of the Bootloader Mode flag.
 * @returns Return the state of the Bootloader Mode flag.
 */
static ALWAYS_INLINE bool PML_GetBootloaderModeFlag(void)
{
    return (PML->RegPmlSysMode.r32 & PML_MODE_REGMASK) == PML_MODE_TO_BIT(PML_BOOTLOADER_MODE);
}


/**
 * @brief Set the Bootloader Mode flag. (Must not be in configuration mode for
 * this to have any effect)
 */
static ALWAYS_INLINE void PML_SetBootloaderModeFlag(void)
{
    // Clear the mode
    PML->RegPmlSysMode.r32 &= ~PML_MODE_REGMASK;
    // Set the mode
    PML->RegPmlSysMode.r32 |= PML_MODE_TO_BIT(PML_BOOTLOADER_MODE);
}


/**
 * @brief Get the state of the Application Mode flag.
 * @returns Return the state of the Application Mode flag.
 */
static ALWAYS_INLINE bool PML_GetApplicationModeFlag(void)
{
    return (PML->RegPmlSysMode.r32 & PML_MODE_REGMASK) == PML_MODE_TO_BIT(PML_APPLICATION_MODE);
}


/**
 * @brief Set the Application Mode flag. (Must not be in configuration mode for
 * this to have any effect)
 */
static ALWAYS_INLINE void PML_SetApplicationModeFlag(void)
{
    // Clear the mode
    PML->RegPmlSysMode.r32 &= ~PML_MODE_REGMASK;
    // Set the mode
    PML->RegPmlSysMode.r32 |= PML_MODE_TO_BIT(PML_APPLICATION_MODE);
}


/**
 * @brief Clear the Boot mode flags (will end up in default mode) DOES NOT
 * CHANGE THE CONFIGURATION MODE FLAG
 */
static ALWAYS_INLINE void PML_ClearModeFlag(void)
{
    PML->RegPmlSysMode.r32 &= ~PML_MODE_REGMASK;
}

/**
 * @brief Get the state of the CRC Error flag.
 * @returns Return the state of the CRC Error flag.
 */
static ALWAYS_INLINE bool PML_GetCRCErrorFlag(void)
{
    return (PML->RegPmlSysMode.r32 & PML_CRC_ERR_REGMASK) == PML_CRC_ERR_BIT;
}


/**
 * @brief Set the CRC Error flag. (Must not be in configuration mode for
 * this to have any effect)
 */
static ALWAYS_INLINE void PML_SetCRCErrorFlag(void)
{
    PML->RegPmlSysMode.r32 |= PML_CRC_ERR_BIT;
}

/**
 * @brief Clear the CRC Error flag
 */
static ALWAYS_INLINE void PML_ClearCRCErrorFlag(void)
{
    PML->RegPmlSysMode.r32 &= ~PML_CRC_ERR_REGMASK;
}

// These functions are only available for ROM V3 included in DI05 and later
#ifndef ROM_V1_V2

/**
 * @brief Check if pre-charge delay skip bit is set (used by ROM V3)
 * @return true if pre-charge delay skip bit is set
 */
static ALWAYS_INLINE bool PML_IsPrechargeDelaySkip(void)
{
    return ((PML->RegPmlPadClk.r32 & PML_PRECHARGE_DELAY_SKIP_REGMASK) == PML_PRECHARGE_DELAY_SKIP_REGMASK);
}

/**
 * @brief Set the Precharge delay skip bit for the ROM V3 and later.
 */
static ALWAYS_INLINE void PML_SetPrechargeDelaySkip(bool skip)
{
    if (skip)
    {
        PML->RegPmlPadClk.r32 |= PML_PRECHARGE_DELAY_SKIP_REGMASK;
    }
    else
    {
        PML->RegPmlPadClk.r32 &= ~PML_PRECHARGE_DELAY_SKIP_REGMASK;
    }
}

/**
 * @brief Get pre-charge delay index from PML bits
 */
static ALWAYS_INLINE uint8_t PML_GetPrechargeDelayIndex(void)
{
    return (uint8_t)PML_BITS_TO_PRECHARGE_DELAY_IDX(PML->RegPmlPadClk.r32);
}

/**
 * @brief Set pre-charge delay index to PML bits
 */
static ALWAYS_INLINE void PML_SetPrechargedelayIndex(uint8_t index)
{
    PML->RegPmlPadClk.r32 = ((PML->RegPmlPadClk.r32 & ~PML_PRECHARGE_DELAY_IDX_REGMASK) | (uint32_t)PML_PRECHARGE_DELAY_IDX_TO_BITS(index));
}

/**
 * @brief Return NVM Brown out comparator status set by ROM V3
 * @return true if NVM Brown out comparator status bit is set
 */
static ALWAYS_INLINE bool PML_GetNVMBrownOutCompStat(void)
{
    return (PML->RegPmlPadClk.r32 & PML_BROWNOUT_COMP_STAT_REGMASK) == PML_BROWNOUT_COMP_STAT_REGMASK;
}

/**
 * @brief Set NVM Brown out comparator status bit (used by ROM V3)
 */
static ALWAYS_INLINE void PML_SetNVMBrownOutCompStat(bool status)
{
    if (status)
    {
        PML->RegPmlPadClk.r32 |= PML_BROWNOUT_COMP_STAT_BIT;
    }
    else
    {
        PML->RegPmlPadClk.r32 &= ~PML_BROWNOUT_COMP_STAT_REGMASK;
    }
}

#endif // if not defined ROM_V1_V2

/** @} */ // End of group pml
/** @} */ // End of addtogroup drivers


#endif /* _PML_H */
