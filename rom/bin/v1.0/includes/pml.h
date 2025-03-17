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
/// @copyright Copyright (c) 2020, EM Microelectronic
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

#include <arc.h>
#include <types.h>
#include <t9305_periph.h>
#include <gpio.h>
#include <config.h>
#include "rom_pml.h"


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

/**
 * @defgroup pml_def PML definitions
 * @{
 */


#define HF_CLK_FREQ_MHZ                 (24)

/// initial value for HF XTAL start time
#define PML_BOOT_OVERHEAD_XTAL_US       (1650)

/// config time for transition to Sleep mode
#define PML_SLEEP_OVERHEAD_CONFIG_US    (100)
/// margin for transition to Sleep mode
#define PML_SLEEP_OVERHEAD_MARGIN_US    (200)

// This includes also time LL needs for initialization before RF activity
#define PML_BOOT_OVERHEAD_US            (750)

#define PML_SLEEP_OVERHEAD_US           ( PML_SLEEP_OVERHEAD_CONFIG_US \
                                        + PML_SLEEP_OVERHEAD_MARGIN_US )

#define PML_SLEEP_MINIMAL_US            (15000)

#define PML_AUTO_SLEEP_ENABLE_HCI       (0x01u)
#define PML_AUTO_SLEEP_ENABLE_LL        (0x02u)


/**
 * @} end of group pml_def
 */


/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @defgroup pml_typedef PML type definitions
 * @{
 */


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
    /// Type of LF clk source
    uint8_t lfClkSourceType;
    /// Accuracy of LF clk source (Sleep Clock Accuracy)
    uint8_t lfClkSourceAccuracy;
    /// Skip RC calibration and use constants (freq)
    bool rcCalibSkip;
    /// Use LDO_VCC during RC calibration to limit ripple from DCDC
    bool useLdoVccDuringCalib;
    /// RC calibration HF limit
    uint32_t rcCalibHfLimit;
    /// RC calibration LF limit
    uint32_t rcCalibLfLimit;
    /// RC calibration period in LF clock numbers.
    /// 0 means this value is ignored.
    uint32_t rcCalibPeriod;
    /// correction to applied to calibration results
    /// Currently not used but kept here for possible patch
    uint16_t rcCalibCorrection;
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
    /// reserved
    uint16_t reserved;

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
 * @brief Sleep CLock Accuracy values
 */
typedef enum
{
    /// 251 ppm to 500 ppm
    PML_SCA_500_PPM = 0,
    /// 151 ppm to 250 ppm
    PML_SCA_250_PPM = 1,
    /// 101 ppm to 150 ppm
    PML_SCA_150_PPM = 2,
    /// 76 ppm to 100 ppm
    PML_SCA_100_PPM = 3,
    /// 51 ppm to 75 ppm
    PML_SCA_75_PPM  = 4,
    /// 31 ppm to 50 ppm
    PML_SCA_50_PPM  = 5,
    /// 21 ppm to 30 ppm
    PML_SCA_30_PPM  = 6,
    /// 0 ppm to 20 ppm
    PML_SCA_20_PPM  = 7,
} PML_SleepClockAccuracy_t;


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


/**
 * @} end of group pml_typedef
 */


/******************************************************************************\
 *  Inline functions
\******************************************************************************/

/**
 * @defgroup pml_inline PML inline functions
 * @{
 */


/**
 * @brief Get USB voltage detected flag.
 * @return Function returns true if USB voltage was detected.
 */
static ALWAYS_INLINE bool PML_IsUsbDetected( void )
{
    return GET_USB_DETECTED_R(SYS->RegPmlSts.regs.PmlMode);
}


/**
 * @brief Get Current Aplication mode.
 * @return Function returns current application mode.
 */
static ALWAYS_INLINE uint8_t PML_GetPmlAppMode( void )
{
    return GET_PML_APP_MODE_R(SYS->RegPmlSts.regs.PmlMode);
}


/**
 * @brief Get Current System mode.
 * @return Function returns current system mode.
 */
static ALWAYS_INLINE PML_SysMode_t PML_GetPmlSysMode( void )
{
    return GET_PML_SYS_MODE_R(SYS->RegPmlSts.regs.PmlMode);
}


/**
 * @brief Enter Sleep CPU mode - least power saving
 */
static ALWAYS_INLINE void PML_EnterSleepCPU( void )
{
    SleepWithMode(PML_SLEEP_CPU);
}


/**
 * @brief Enter Sleep mode
 * @note
 *   Function enters Sleep mode.
 *   Interrupts are not enabled since they cannot wake up CPU from Sleep mode.
 */
static ALWAYS_INLINE void PML_EnterSleep( void )
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
static ALWAYS_INLINE void PML_EnterDeepSleep( void )
{
    SleepWithMode(PML_DEEP_SLEEP);
}


/**
 * @brief Is XTAL source of HF clk
 * @return TRUE if HF source is XTAL
 */
static ALWAYS_INLINE bool PML_HfClkIsXtal( void )
{
    return (GET_PML_SYS_MODE( SYS->RegPmlSts.r32 ) == PML_SYS_MODE_ACTIVE_XTAL);
}


/**
 * @brief Is RC source of HF clk
 * @return TRUE if HF source is RC, FALSE otherwise
 */
static ALWAYS_INLINE bool PML_HfClkIsRc( void )
{
    return (GET_PML_SYS_MODE( SYS->RegPmlSts.r32 ) == PML_SYS_MODE_ACTIVE_RC);
}


/**
 * @brief Is DRAMx retention on
 * @param dram DRAM block number: 0..6
 * @return TRUE, if retention of DRAMx is on, FALSE otherwise
 */
static ALWAYS_INLINE bool PML_IsDramRetOn( uint8_t dram )
{
    uint8_t dramRet = (uint8_t)GET_PML_DRAM_RET_ON(PML->RegPmlDomain.r32);
    return ((dramRet & (1 << dram)) != 0x00);
}


/**
 * @brief Get wake-up status flags
 * @return Wakeup flags for 12 GPIOs, 4 Sleep Timer channels, and 3 for QDec.
 */
static ALWAYS_INLINE uint32_t PML_GetWakeUpFlags()
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
static ALWAYS_INLINE void PML_SetRamRetentionEnable( uint8_t ramRetention )
{
    PML->RegPmlDomain.regs.PmlDRAMRetOn = (uint8_t)PML_DRAM_RET_ON_R(ramRetention);
}


/**
 * @brief Get RAM retention state
 * @returns State of RAM retention configuration
 */
static ALWAYS_INLINE uint8_t PML_GetRamRetentionEnable( void )
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
 * @} end of group pml_inline
 */


/******************************************************************************\
 *  PROTOTYPES FOR REGISTER ACCESS
\******************************************************************************/

/**
 * @defgroup pml_fct PML function prototypes
 * @{
 */


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
void PML_EnableVddDomainWait( uint8_t domain, bool enable );


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
 *   domain is switch on.
 */
void PML_EnableVddDomain( uint8_t domain, bool enable );


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
void PML_EnableVddDomainNonBlocking( uint8_t domain, bool enable );


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

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
 *            PML_LF_CLK_XTAL_EN   = Exernal XTAL
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
void PML_SystemShutDown( void );


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
 * @param correctRcCalib TRUE rc calibration is corrected
 *                       FALSE rc calibration is not corrected
 */
void PML_UpdateWakeUpTime(bool correctRcCalib);


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
void PML_ConfigWakeByStInHfClk(uint64_t sleepTimeHfClk, uint8_t sleepChannel);


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
 *       Function alse registers all task which are required to selected one.
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
 * @brief Return whether automotic sleep mode should
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
 * @returns Sleep clock accuracy
 */
PML_SleepClockAccuracy_t PML_GetSleepClockAccuracy(void);


/**
 * @brief Disable or enable automatic Sleep mode
 * @param mask Mask defining reason to disable/enale Sleep mode
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



/******************************************************************************\
 *  API FUNCTIONS FOR INITIALIZATION, STORE AND RESTORE CONFIG
\******************************************************************************/
/**
 * @brief PML initialize
 */
void PML_Init(void);


/**
 * @brief PML store config before sleep
 */
void PML_StoreConfig(void);


/**
 * @brief PML restore config
 */
void PML_ReStoreConfig(void);


/**
 * @} end of group pml_fct
 */


/**
 * @} end of group pml
 * @} end of addtogroup drivers
 */


#endif /* _PML_H */
