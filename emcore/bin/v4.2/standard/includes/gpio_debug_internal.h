////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/gpio/includes/gpio_debug_internal.h
///
/// @project    T9305
///
/// @brief      GPIO debug mode helper.
///
/// @classification  Internal Use
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023, EM Microelectronic
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

#ifndef _GPIO_DEBUG_H
#define _GPIO_DEBUG_H

#include <types.h>
#include "gpio.h"
#include "t9305_system.h"
#include "t9305_periph.h"

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/// GPIO debug mode selector
typedef enum {
    /* GPIO 4 */
    GPIO4_DEBUG_DISABLE                       = ((4u << 8u) | 0u),
    GPIO4_DEBUG_PT_OUT_CMP_IRQ_0              = ((4u << 8u) | 1u),
    GPIO4_DEBUG_PT_OUT_CMP_IRQ_4              = ((4u << 8u) | 2u),
    GPIO4_DEBUG_ST_FULL_VAL_IRQ               = ((4u << 8u) | 3u),
    GPIO4_DEBUG_PT_FULL_VAL_IRQ               = ((4u << 8u) | 4u),
    GPIO4_DEBUG_UART_TX_IRQ                   = ((4u << 8u) | 5u),
    GPIO4_DEBUG_UART_RX_FRAME_ERROR           = ((4u << 8u) | 6u),
    GPIO4_DEBUG_SPIM_IRQ                      = ((4u << 8u) | 9u),
    GPIO4_DEBUG_UT2_FULL_VAL_IRQ              = ((4u << 8u) | 10u),
    GPIO4_DEBUG_UT3_FULL_VAL_IRQ              = ((4u << 8u) | 11u),
    GPIO4_DEBUG_ADC_SAR_IRQ_1                 = ((4u << 8u) | 13u),
    GPIO4_DEBUG_AES_IRQ                       = ((4u << 8u) | 14u),
    GPIO4_DEBUG_QDEC_REP_RDY_IRQ              = ((4u << 8u) | 15u),
    GPIO4_DEBUG_PML_LF_XTAL_IRQ               = ((4u << 8u) | 19u),
    GPIO4_DEBUG_CTE_RX_START                  = ((4u << 8u) | 20u),
    GPIO4_DEBUG_RX_DATA                       = ((4u << 8u) | 21u),
    GPIO4_DEBUG_PP_TX_DONE                    = ((4u << 8u) | 22u),
    GPIO4_DEBUG_RF_TX_DATA                    = ((4u << 8u) | 23u),
    GPIO4_DEBUG_RX_CRC_ERR                    = ((4u << 8u) | 24u),
    GPIO4_DEBUG_LR_BIT                        = ((4u << 8u) | 25u),
    GPIO4_DEBUG_TX_DATA_DW                    = ((4u << 8u) | 26u),
    GPIO4_DEBUG_SIF_CLK_I_1                   = ((4u << 8u) | 27u),
    GPIO4_DEBUG_SIF_CLK_I_2                   = ((4u << 8u) | 28u),
    GPIO4_DEBUG_SIF_CLK_I_3                   = ((4u << 8u) | 29u),
    GPIO4_DEBUG_XE                            = ((4u << 8u) | 30u),
    GPIO4_DEBUG_ERASE                         = ((4u << 8u) | 31u),
    GPIO4_DEBUG_LDO_USB_DETECTED              = ((4u << 8u) | 32u),
    GPIO4_DEBUG_ADC_SAR_IRQ_2                 = ((4u << 8u) | 33u),
    GPIO4_DEBUG_GPIO_COMPOSITE_IRQ            = ((4u << 8u) | 34u),
    GPIO4_DEBUG_UART_TX_COMPOSITE_IRQ         = ((4u << 8u) | 35u),
    GPIO4_DEBUG_UT2_COMPOSITE_IRQ             = ((4u << 8u) | 36u),
    GPIO4_DEBUG_UT3_COMPOSITE_IRQ             = ((4u << 8u) | 37u),
    GPIO4_DEBUG_DMA_COMPOSITE_IRQ             = ((4u << 8u) | 38u),
    GPIO4_DEBUG_ADC_PHI0                      = ((4u << 8u) | 39u),
    GPIO4_DEBUG_CLK_UART                      = ((4u << 8u) | 40u),
    GPIO4_DEBUG_CLK_SPI_MASTER                = ((4u << 8u) | 41u),
    GPIO4_DEBUG_CLK_QDEC                      = ((4u << 8u) | 42u),
    GPIO4_DEBUG_SYS_SLEEP_MODE_R_0            = ((4u << 8u) | 44u),
    GPIO4_DEBUG_SYS_HALT_R                    = ((4u << 8u) | 45u),
    GPIO4_DEBUG_SYS_SLEEP_R                   = ((4u << 8u) | 46u),
    GPIO4_DEBUG_CLK_SYS                       = ((4u << 8u) | 47u),

    /* GPIO 5 */
    GPIO5_DEBUG_DISABLE                       = ((5u << 8u) | 0u),
    GPIO5_DEBUG_PT_OUT_CMP_IRQ_1              = ((5u << 8u) | 1u),
    GPIO5_DEBUG_PT_OUT_CMP_IR_5               = ((5u << 8u) | 2u),
    GPIO5_DEBUG_PT_FULL_VAL_IRQ               = ((5u << 8u) | 3u),
    GPIO5_DEBUG_ST_FULL_VAL_IRQ               = ((5u << 8u) | 4u),
    GPIO5_DEBUG_UART_RX_IRQ                   = ((5u << 8u) | 5u),
    GPIO5_DEBUG_UART_TX_FIFO_UNDERFLOW_IRQ    = ((5u << 8u) | 6u),
    GPIO5_DEBUG_SPIS_TX_IRQ                   = ((5u << 8u) | 7u),
    GPIO5_DEBUG_SPIS_TX_FIFO_EMPTY_IRQ        = ((5u << 8u) | 8u),
    GPIO5_DEBUG_SPIM_IRQ                      = ((5u << 8u) | 9u),
    GPIO5_DEBUG_UT2_OUT_CMP_IRQ               = ((5u << 8u) | 10u),
    GPIO5_DEBUG_UT3_OUT_CMP_IRQ               = ((5u << 8u) | 11u),
    GPIO5_DEBUG_RC_CALIB_DONE_IRQ             = ((5u << 8u) | 13u),
    GPIO5_DEBUG_NVM_DONE_IRQ                  = ((5u << 8u) | 14u),
    GPIO5_DEBUG_QDEC_OVF_IRQ                  = ((5u << 8u) | 15u),
    GPIO5_DEBUG_USB_INTERRUPT                 = ((5u << 8u) | 16u),
    GPIO5_DEBUG_PML_RC_ACT_IRQ                = ((5u << 8u) | 19u),
    GPIO5_DEBUG_CTE_TX_START                  = ((5u << 8u) | 20u),
    GPIO5_DEBUG_RX_DATA_CLK                   = ((5u << 8u) | 21u),
    GPIO5_DEBUG_PP_TX_FIFO_LIMIT              = ((5u << 8u) | 22u),
    GPIO5_DEBUG_RF_TX_ON                      = ((5u << 8u) | 23u),
    GPIO5_DEBUG_PP_RX_DONE                    = ((5u << 8u) | 24u),
    GPIO5_DEBUG_LR_VALID                      = ((5u << 8u) | 25u),
    GPIO5_DEBUG_RX_DATA_DW                    = ((5u << 8u) | 26u),
    GPIO5_DEBUG_RX_DATA_CLK_O                 = ((5u << 8u) | 27u),
    GPIO5_DEBUG_STROBE_O                      = ((5u << 8u) | 28u),
    GPIO5_DEBUG_SIF_MISO_O                    = ((5u << 8u) | 29u),
    GPIO5_DEBUG_YE                            = ((5u << 8u) | 30u),
    GPIO5_DEBUG_PROG                          = ((5u << 8u) | 31u),
    GPIO5_DEBUG_USB_PHY_RX_DM_ASICORFPGA      = ((5u << 8u) | 32u),
    GPIO5_DEBUG_ADC_SAR_CMP                   = ((5u << 8u) | 33u),
    GPIO5_DEBUG_SPIS_TX_COMPOSITE_IRQ         = ((5u << 8u) | 34u),
    GPIO5_DEBUG_UART_RX_COMPOSITE_IRQ         = ((5u << 8u) | 35u),
    GPIO5_DEBUG_PT_COMPOSITE_IRQ              = ((5u << 8u) | 36u),
    GPIO5_DEBUG_RFC_COMPOSITE_IRQ             = ((5u << 8u) | 37u),
    GPIO5_DEBUG_USB_COMPOSITE_IRQ             = ((5u << 8u) | 38u),
    GPIO5_DEBUG_ADC_PHI1                      = ((5u << 8u) | 39u),
    GPIO5_DEBUG_CLK_SPI_SLAVE                 = ((5u << 8u) | 40u),
    GPIO5_DEBUG_CLK_PROT_TIM                  = ((5u << 8u) | 41u),
    GPIO5_DEBUG_CLK_I2S                       = ((5u << 8u) | 42u),
    GPIO5_DEBUG_SYS_SLEEP_MODE_R_1            = ((5u << 8u) | 44u),
    GPIO5_DEBUG_SYS_HALT_R                    = ((5u << 8u) | 45u),
    GPIO5_DEBUG_SYS_SLEEP_R                   = ((5u << 8u) | 46u),
    GPIO5_DEBUG_CLK_SYS                       = ((5u << 8u) | 47u),

    /* GPIO 6 */
    GPIO6_DEBUG_DISABLE                       = ((6u << 8u) | 0u),
    GPIO6_DEBUG_PT_OUT_CMP_IRQ_2              = ((6u << 8u) | 1u),
    GPIO6_DEBUG_PT_OUT_CMP_IRQ_6              = ((6u << 8u) | 2u),
    GPIO6_DEBUG_PT_SYNC_START_IRQ             = ((6u << 8u) | 3u),
    GPIO6_DEBUG_ST_OUT_CMP_IRQ_0              = ((6u << 8u) | 4u),
    GPIO6_DEBUG_UART_TX_FIFO_LIMIT_IRQ        = ((6u << 8u) | 5u),
    GPIO6_DEBUG_UART_RX_PARITY_ERROR          = ((6u << 8u) | 6u),
    GPIO6_DEBUG_SPIS_RX_IRQ                   = ((6u << 8u) | 7u),
    GPIO6_DEBUG_SPIS_RX_FIFO_FULL_IRQ         = ((6u << 8u) | 8u),
    GPIO6_DEBUG_I2C_IRQ                       = ((6u << 8u) | 9u),
    GPIO6_DEBUG_UT2_IN_CPT_IRQ                = ((6u << 8u) | 10u),
    GPIO6_DEBUG_UT3_IN_CPT_IRQ                = ((6u << 8u) | 11u),
    GPIO6_DEBUG_DMA_DONE_IRQ                  = ((6u << 8u) | 13u),
    GPIO6_DEBUG_TEST_RNG_OUT                  = ((6u << 8u) | 14u),
    GPIO6_DEBUG_QDEC_DBL_IRQ                  = ((6u << 8u) | 15u),
    GPIO6_DEBUG_PML_SVLD_IRQ                  = ((6u << 8u) | 18u),
    GPIO6_DEBUG_PML_XTAL_ACT_IRQ              = ((6u << 8u) | 19u),
    GPIO6_DEBUG_PER_CLK_EN                    = ((6u << 8u) | 20u),
    GPIO6_DEBUG_RF_ACCESS_DET                 = ((6u << 8u) | 21u),
    GPIO6_DEBUG_PP_TX_FIFO_UNDERFLOW          = ((6u << 8u) | 22u),
    GPIO6_DEBUG_RFC_TX_DONE                   = ((6u << 8u) | 23u),
    GPIO6_DEBUG_PP_RX_FIFO_LIMIT_IRQ          = ((6u << 8u) | 24u),
    GPIO6_DEBUG_PP_RX_DONE                    = ((6u << 8u) | 25u),
    GPIO6_DEBUG_TX_DATA_DW                    = ((6u << 8u) | 26u),
    GPIO6_DEBUG_RX_DATA_O                     = ((6u << 8u) | 27u),
    GPIO6_DEBUG_TX_DATA_I                     = ((6u << 8u) | 28u),
    GPIO6_DEBUG_SIF_MOSI_I                    = ((6u << 8u) | 29u),
    GPIO6_DEBUG_SE                            = ((6u << 8u) | 30u),
    GPIO6_DEBUG_NVSTR                         = ((6u << 8u) | 31u),
    GPIO6_DEBUG_SH_CTRL                       = ((6u << 8u) | 33u),
    GPIO6_DEBUG_SPIS_RX_COMPOSITE_IRQ         = ((6u << 8u) | 34u),
    GPIO6_DEBUG_UART_ER_COMPOSITE_IRQ         = ((6u << 8u) | 35u),
    GPIO6_DEBUG_ST_COMPOSITE_IRQ              = ((6u << 8u) | 36u),
    GPIO6_DEBUG_RF_TX_COMPOSITE_IRQ           = ((6u << 8u) | 37u),
    GPIO6_DEBUG_QDEC_COMPOSITE_IRQ            = ((6u << 8u) | 38u),
    GPIO6_DEBUG_ADC_PHI2                      = ((6u << 8u) | 39u),
    GPIO6_DEBUG_CLK_UT2                       = ((6u << 8u) | 40u),
    GPIO6_DEBUG_CLK_RC_CALIB                  = ((6u << 8u) | 41u),
    GPIO6_DEBUG_CLK_ADC                       = ((6u << 8u) | 42u),
    GPIO6_DEBUG_SYS_SLEEP_MODE_R_0            = ((6u << 8u) | 44u),
    GPIO6_DEBUG_SYS_HALT_R                    = ((6u << 8u) | 45u),
    GPIO6_DEBUG_SYS_SLEEP_R                   = ((6u << 8u) | 46u),
    GPIO6_DEBUG_CLK_SYS                       = ((6u << 8u) | 47u),

    /* GPIO 7 */
    GPIO7_DEBUG_DISABLE                       = ((7u << 8u) | 0u),
    GPIO7_DEBUG_PT_OUT_CMP_IRQ_3              = ((7u << 8u) | 1u),
    GPIO7_DEBUG_PT_OUT_CMP_IRQ_7              = ((7u << 8u) | 2u),
    GPIO7_DEBUG_PT_SYNC_STOP_IRQ              = ((7u << 8u) | 3u),
    GPIO7_DEBUG_ST_OUT_CMP_IRQ_1              = ((7u << 8u) | 4u),
    GPIO7_DEBUG_UART_RX_FIFO_LIMIT_IRQ        = ((7u << 8u) | 5u),
    GPIO7_DEBUG_UART_TX_IRQ                   = ((7u << 8u) | 6u),
    GPIO7_DEBUG_SPIS_TX_FIFO_LIMIT_IRQ        = ((7u << 8u) | 7u),
    GPIO7_DEBUG_SPIS_RX_FIFO_LIMIT_IRQ        = ((7u << 8u) | 8u),
    GPIO7_DEBUG_SPIM_IRQ                      = ((7u << 8u) | 9u),
    GPIO7_DEBUG_UT3_FULL_VAL_IRQ              = ((7u << 8u) | 10u),
    GPIO7_DEBUG_UT2_FULL_VAL_IRQ              = ((7u << 8u) | 11u),
    GPIO7_DEBUG_DMA_ERROR_IRQ                 = ((7u << 8u) | 13u),
    GPIO7_DEBUG_AES_IRQ                       = ((7u << 8u) | 14u),
    GPIO7_DEBUG_QDEC_REP_RDY_IRQ              = ((7u << 8u) | 15u),
    GPIO7_DEBUG_PML_RC_ACT_IRQ                = ((7u << 8u) | 18u),
    GPIO7_DEBUG_PML_LF_XTAL_IRQ               = ((7u << 8u) | 19u),
    GPIO7_DEBUG_RFC_RX_DONE                   = ((7u << 8u) | 21u),
    GPIO7_DEBUG_RFC_RX_TIMEOUT                = ((7u << 8u) | 23u),
    GPIO7_DEBUG_PP_RX_FIFO_OVERFLOW           = ((7u << 8u) | 24u),
    GPIO7_DEBUG_PP_TX_DONE                    = ((7u << 8u) | 25u),
    GPIO7_DEBUG_RX_DATA_DW                    = ((7u << 8u) | 26u),
    GPIO7_DEBUG_RX_ACCESS_DET_I               = ((7u << 8u) | 27u),
    GPIO7_DEBUG_TX_ON_I                       = ((7u << 8u) | 28u),
    GPIO7_DEBUG_SIF_EN_B_I                    = ((7u << 8u) | 29u),
    GPIO7_DEBUG_XADDR_0_1                     = ((7u << 8u) | 30u),
    GPIO7_DEBUG_XADDR_0_2                     = ((7u << 8u) | 31u),
    GPIO7_DEBUG_ADC_PHI0                      = ((7u << 8u) | 33u),
    GPIO7_DEBUG_SPIS_ER_COMPOSITE_IRQ         = ((7u << 8u) | 34u),
    GPIO7_DEBUG_PML_COMPOSITE_IRQ             = ((7u << 8u) | 35u),
    GPIO7_DEBUG_RF_TX_COMPOSITE_IRQ           = ((7u << 8u) | 36u),
    GPIO7_DEBUG_RF_RX_COMPOSITE_IRQ           = ((7u << 8u) | 37u),
    GPIO7_DEBUG_ADC_PHI3                      = ((7u << 8u) | 39u),
    GPIO7_DEBUG_CLK_UT3                       = ((7u << 8u) | 40u),
    GPIO7_DEBUG_CLK_RFC                       = ((7u << 8u) | 41u),
    GPIO7_DEBUG_CLK_AOAD                      = ((7u << 8u) | 42u),
    GPIO7_DEBUG_SYS_SLEEP_MODE_R_1            = ((7u << 8u) | 44u),
    GPIO7_DEBUG_SYS_HALT_R                    = ((7u << 8u) | 45u),
    GPIO7_DEBUG_SYS_SLEEP_R                   = ((7u << 8u) | 46u),
    GPIO7_DEBUG_CLK_SYS                       = ((7u << 8u) | 47u),

    /* GPIO 8 */
    GPIO8_DEBUG_DISABLE                       = ((8u << 8u) | 0u),
    GPIO8_DEBUG_PT_OUT_CMP_IRQ_4              = ((8u << 8u) | 1u),
    GPIO8_DEBUG_PT_OUT_CMP_IRQ_0              = ((8u << 8u) | 2u),
    GPIO8_DEBUG_ST_OUT_CMP_IRQ_0              = ((8u << 8u) | 3u),
    GPIO8_DEBUG_ST_OUT_CMP_IRQ_2              = ((8u << 8u) | 4u),
    GPIO8_DEBUG_UART_RX_FIFO_FULL_IRQ         = ((8u << 8u) | 5u),
    GPIO8_DEBUG_UART_RX_IRQ                   = ((8u << 8u) | 6u),
    GPIO8_DEBUG_SPIS_TX_FIFO_UNDERFLOW_IRQ    = ((8u << 8u) | 7u),
    GPIO8_DEBUG_SPIS_RX_FIFO_UNDERFLOW_IRQ    = ((8u << 8u) | 8u),
    GPIO8_DEBUG_I2C_IRQ                       = ((8u << 8u) | 9u),
    GPIO8_DEBUG_UT3_OUT_CMP_IRQ               = ((8u << 8u) | 10u),
    GPIO8_DEBUG_UT2_OUT_CMP_IRQ               = ((8u << 8u) | 11u),
    GPIO8_DEBUG_NVM_DONE_IRQ                  = ((8u << 8u) | 14u),
    GPIO8_DEBUG_QDEC_OVF_IRQ                  = ((8u << 8u) | 15u),
    GPIO8_DEBUG_USB_EP0_IRQ                   = ((8u << 8u) | 16u),
    GPIO8_DEBUG_PML_XTAL_ACT_IRQ              = ((8u << 8u) | 18u),
    GPIO8_DEBUG_CLK_PML_TIM                   = ((8u << 8u) | 19u),
    GPIO8_DEBUG_RFC_RX_TIMEOUT                = ((8u << 8u) | 21u),
    GPIO8_DEBUG_RX_CRC_ERR                    = ((8u << 8u) | 22u),
    GPIO8_DEBUG_RX_DATA                       = ((8u << 8u) | 23u),
    GPIO8_DEBUG_PP_TX_DONE                    = ((8u << 8u) | 24u),
    GPIO8_DEBUG_LR_BIT                        = ((8u << 8u) | 25u),
    GPIO8_DEBUG_TX_DATA_DW                    = ((8u << 8u) | 26u),
    GPIO8_DEBUG_DFT_MUX1_O_1                  = ((8u << 8u) | 27u),
    GPIO8_DEBUG_DFT_MUX1_O_2                  = ((8u << 8u) | 28u),
    GPIO8_DEBUG_SEQ_XO_EN_DIG                 = ((8u << 8u) | 29u),
    GPIO8_DEBUG_YADDR_0_1                     = ((8u << 8u) | 30u),
    GPIO8_DEBUG_YADDR_0_2                     = ((8u << 8u) | 31u),
    GPIO8_DEBUG_USB_PHY_RX_DP_ASICORFPGA      = ((8u << 8u) | 32u),
    GPIO8_DEBUG_ADC_PHI1                      = ((8u << 8u) | 33u),
    GPIO8_DEBUG_PT_COMPOSITE_IRQ              = ((8u << 8u) | 34u),
    GPIO8_DEBUG_PML_COMPOSITE_IRQ             = ((8u << 8u) | 35u),
    GPIO8_DEBUG_RF_RX_COMPOSITE_IRQ           = ((8u << 8u) | 36u),
    GPIO8_DEBUG_UART_TX_COMPOSITE_IRQ         = ((8u << 8u) | 37u),
    GPIO8_DEBUG_DMA_COMPOSITE_IRQ             = ((8u << 8u) | 38u),
    GPIO8_DEBUG_ADC_PHI4                      = ((8u << 8u) | 39u),
    GPIO8_DEBUG_CLK_GPIO                      = ((8u << 8u) | 40u),
    GPIO8_DEBUG_CLK_NVM                       = ((8u << 8u) | 41u),
    GPIO8_DEBUG_CLK_USB                       = ((8u << 8u) | 42u),
    GPIO8_DEBUG_SYS_SLEEP_MODE_R_2            = ((8u << 8u) | 44u),
    GPIO8_DEBUG_SYS_HALT_R                    = ((8u << 8u) | 45u),
    GPIO8_DEBUG_SYS_SLEEP_R                   = ((8u << 8u) | 46u),
    GPIO8_DEBUG_CLK_SYS                       = ((8u << 8u) | 47u),

    /* GPIO 9 */
    GPIO9_DEBUG_DISABLE                       = ((9u << 8u) | 0u),
    GPIO9_DEBUG_PT_OUT_CMP_IRQ_5              = ((9u << 8u) | 1u),
    GPIO9_DEBUG_PT_OUT_CMP_IRQ_1              = ((9u << 8u) | 2u),
    GPIO9_DEBUG_ST_OUT_CMP_IRQ_1              = ((9u << 8u) | 3u),
    GPIO9_DEBUG_ST_OUT_CMP_IRQ_3              = ((9u << 8u) | 4u),
    GPIO9_DEBUG_UART_TX_FIFO_EMPTY_IRQ        = ((9u << 8u) | 5u),
    GPIO9_DEBUG_UART_TX_FIFO_LIMIT_IRQ        = ((9u << 8u) | 6u),
    GPIO9_DEBUG_SPIS_TX_FIFO_OVERFLOW_IRQ     = ((9u << 8u) | 7u),
    GPIO9_DEBUG_SPIS_RX_FIFO_OVERFLOW_IRQ     = ((9u << 8u) | 8u),
    GPIO9_DEBUG_SPIM_IRQ                      = ((9u << 8u) | 9u),
    GPIO9_DEBUG_UT3_IN_CPT_IRQ                = ((9u << 8u) | 10u),
    GPIO9_DEBUG_UT2_IN_CPT_IRQ                = ((9u << 8u) | 11u),
    GPIO9_DEBUG_RC_CALIB_DONE_IRQ             = ((9u << 8u) | 13u),
    GPIO9_DEBUG_TEST_RNG_OUT                  = ((9u << 8u) | 14u),
    GPIO9_DEBUG_QDEC_DBL_IRQ                  = ((9u << 8u) | 15u),
    GPIO9_DEBUG_USB_EP1_IRQ                   = ((9u << 8u) | 16u),
    GPIO9_DEBUG_PML_LF_XTAL_IRQ               = ((9u << 8u) | 18u),
    GPIO9_DEBUG_CLK_PML_SYS                   = ((9u << 8u) | 19u),
    GPIO9_DEBUG_RF_TX_DATA                    = ((9u << 8u) | 21u),
    GPIO9_DEBUG_PP_RX_DONE                    = ((9u << 8u) | 22u),
    GPIO9_DEBUG_RX_DATA_CLK                   = ((9u << 8u) | 23u),
    GPIO9_DEBUG_PP_TX_FIFO_LIMIT              = ((9u << 8u) | 24u),
    GPIO9_DEBUG_LR_VALID                      = ((9u << 8u) | 25u),
    GPIO9_DEBUG_RX_DATA_DW                    = ((9u << 8u) | 26u),
    GPIO9_DEBUG_DFT_MUX2_O_1                  = ((9u << 8u) | 27u),
    GPIO9_DEBUG_DFT_MUX2_O_2                  = ((9u << 8u) | 28u),
    GPIO9_DEBUG_CLK_DIG_O                     = ((9u << 8u) | 29u),
    GPIO9_DEBUG_DOUT_0                        = ((9u << 8u) | 30u),
    GPIO9_DEBUG_DIN_0                         = ((9u << 8u) | 31u),
    GPIO9_DEBUG_PAD_USB_OVR_DP                = ((9u << 8u) | 32u),
    GPIO9_DEBUG_ADC_PHI2                      = ((9u << 8u) | 33u),
    GPIO9_DEBUG_ST_COMPOSITE_IRQ              = ((9u << 8u) | 34u),
    GPIO9_DEBUG_GPIO_COMPOSITE_IRQ            = ((9u << 8u) | 35u),
    GPIO9_DEBUG_SPIS_TX_COMPOSITE_IRQ         = ((9u << 8u) | 36u),
    GPIO9_DEBUG_UART_RX_COMPOSITE_IRQ         = ((9u << 8u) | 37u),
    GPIO9_DEBUG_USB_COMPOSITE_IRQ             = ((9u << 8u) | 38u),
    GPIO9_DEBUG_ADC_PHI5                      = ((9u << 8u) | 39u),
    GPIO9_DEBUG_CLK_CPU                       = ((9u << 8u) | 40u),
    GPIO9_DEBUG_CLK_RF_CTRL                   = ((9u << 8u) | 41u),
    GPIO9_DEBUG_SYS_SLEEP_MODE_R_0            = ((9u << 8u) | 44u),
    GPIO9_DEBUG_SYS_HALT_R                    = ((9u << 8u) | 45u),
    GPIO9_DEBUG_SYS_SLEEP_R                   = ((9u << 8u) | 46u),
    GPIO9_DEBUG_CLK_SYS                       = ((9u << 8u) | 47u),

    /* GPIO 10 */
    GPIO10_DEBUG_DISABLE                      = ((10u << 8u) | 0u),
    GPIO10_DEBUG_PT_OUT_CMP_IRQ_6             = ((10u << 8u) | 1u),
    GPIO10_DEBUG_PT_OUT_CMP_IRQ_2             = ((10u << 8u) | 2u),
    GPIO10_DEBUG_ST_OUT_CMP_IRQ_2             = ((10u << 8u) | 3u),
    GPIO10_DEBUG_PT_SYNC_START_IRQ            = ((10u << 8u) | 4u),
    GPIO10_DEBUG_UART_TX_FIFO_OVERFLOW_IRQ    = ((10u << 8u) | 5u),
    GPIO10_DEBUG_UART_RX_FIFO_OVERFLOW_IRQ    = ((10u << 8u) | 6u),
    GPIO10_DEBUG_SPIS_TX_FIFO_EMPTY_IRQ       = ((10u << 8u) | 7u),
    GPIO10_DEBUG_SPIS_TX_IRQ                  = ((10u << 8u) | 8u),
    GPIO10_DEBUG_I2C_IRQ                      = ((10u << 8u) | 9u),
    GPIO10_DEBUG_UT2_FULL_VAL_IRQ             = ((10u << 8u) | 10u),
    GPIO10_DEBUG_UT3_FULL_VAL_IRQ             = ((10u << 8u) | 11u),
    GPIO10_DEBUG_DMA_DONE_IRQ                 = ((10u << 8u) | 13u),
    GPIO10_DEBUG_QDEC_CLK                     = ((10u << 8u) | 15u),
    GPIO10_DEBUG_USB_EP2_IRQ                  = ((10u << 8u) | 16u),
    GPIO10_DEBUG_PML_SVLD_IRQ                 = ((10u << 8u) | 18u),
    GPIO10_DEBUG_RF_TX_ON                     = ((10u << 8u) | 21u),
    GPIO10_DEBUG_PP_RX_FIFO_LIMIT             = ((10u << 8u) | 22u),
    GPIO10_DEBUG_RF_ACCESS_DET                = ((10u << 8u) | 23u),
    GPIO10_DEBUG_PP_TX_FIFO_UNDERFLOW         = ((10u << 8u) | 24u),
    GPIO10_DEBUG_PP_RX_DONE                   = ((10u << 8u) | 25u),
    GPIO10_DEBUG_TX_DATA_DW                   = ((10u << 8u) | 26u),
    GPIO10_DEBUG_SEQ_ISOBDIG                  = ((10u << 8u) | 29u),
    GPIO10_DEBUG_DOUT_31                      = ((10u << 8u) | 30u),
    GPIO10_DEBUG_DIN_31                       = ((10u << 8u) | 31u),
    GPIO10_DEBUG_PAD_USB_OVR_DM               = ((10u << 8u) | 32u),
    GPIO10_DEBUG_ADC_PHI3                     = ((10u << 8u) | 33u),
    GPIO10_DEBUG_RF_TX_COMPOSITE_IRQ          = ((10u << 8u) | 34u),
    GPIO10_DEBUG_UT3_COMPOSITE_IRQ            = ((10u << 8u) | 35u),
    GPIO10_DEBUG_SPIS_RX_COMPOSITE_IRQ        = ((10u << 8u) | 36u),
    GPIO10_DEBUG_UART_ER_COMPOSITE_IRQ        = ((10u << 8u) | 37u),
    GPIO10_DEBUG_QDEC_COMPOSITE_IRQ           = ((10u << 8u) | 38u),
    GPIO10_DEBUG_ADC_PHI6                     = ((10u << 8u) | 39u),
    GPIO10_DEBUG_CLK_IRQ                      = ((10u << 8u) | 40u),
    GPIO10_DEBUG_CLK_I2C_MASTER               = ((10u << 8u) | 41u),
    GPIO10_DEBUG_SYS_SLEEP_MODE_R_1           = ((10u << 8u) | 44u),
    GPIO10_DEBUG_SYS_HALT_R                   = ((10u << 8u) | 45u),
    GPIO10_DEBUG_SYS_SLEEP_R                  = ((10u << 8u) | 46u),
    GPIO10_DEBUG_CLK_SYS                      = ((10u << 8u) | 47u),

    /* GPIO 11 */
    GPIO11_DEBUG_DISABLE                      = ((11u << 8u) | 0u),
    GPIO11_DEBUG_PT_OUT_CMP_IRQ_7             = ((11u << 8u) | 1u),
    GPIO11_DEBUG_PT_OUT_CMP_IRQ_3             = ((11u << 8u) | 2u),
    GPIO11_DEBUG_ST_OUT_CMP_IRQ_3             = ((11u << 8u) | 3u),
    GPIO11_DEBUG_PT_SYNC_STOP_IRQ             = ((11u << 8u) | 4u),
    GPIO11_DEBUG_UART_RX_FIFO_UNDERFLOW_IRQ   = ((11u << 8u) | 5u),
    GPIO11_DEBUG_UART_RX_FIFO_LIMIT_IRQ       = ((11u << 8u) | 6u),
    GPIO11_DEBUG_SPIS_RX_FIFO_FULL_IRQ        = ((11u << 8u) | 7u),
    GPIO11_DEBUG_SPIS_RX_IRQ                  = ((11u << 8u) | 8u),
    GPIO11_DEBUG_I2C_IRQ                      = ((11u << 8u) | 9u),
    GPIO11_DEBUG_UT2_OUT_CMP_IRQ              = ((11u << 8u) | 10u),
    GPIO11_DEBUG_UT3_OUT_CMP_IRQ              = ((11u << 8u) | 11u),
    GPIO11_DEBUG_DMA_ERROR_IRQ                = ((11u << 8u) | 13u),
    GPIO11_DEBUG_USB_EP3_IRQ                  = ((11u << 8u) | 16u),
    GPIO11_DEBUG_PML_LF_XTAL_IRQ              = ((11u << 8u) | 18u),
    GPIO11_DEBUG_RFC_TX_DONE                  = ((11u << 8u) | 21u),
    GPIO11_DEBUG_PP_RX_FIFO_OVERFLOW          = ((11u << 8u) | 22u),
    GPIO11_DEBUG_RFC_RX_DONE                  = ((11u << 8u) | 23u),
    GPIO11_DEBUG_PP_TX_DONE                   = ((11u << 8u) | 25u),
    GPIO11_DEBUG_PP_RX_DATA_DW                = ((11u << 8u) | 26u),
    GPIO11_DEBUG_DOUT_63                      = ((11u << 8u) | 30u),
    GPIO11_DEBUG_DIN_63                       = ((11u << 8u) | 31u),
    GPIO11_DEBUG_USB_PHY_RX_RCV_ASICORFPGA    = ((11u << 8u) | 32u),
    GPIO11_DEBUG_ADC_PHI4                     = ((11u << 8u) | 33u),
    GPIO11_DEBUG_RF_RX_COMPOSITE_IRQ          = ((11u << 8u) | 34u),
    GPIO11_DEBUG_UT2_COMPOSITE_IRQ            = ((11u << 8u) | 35u),
    GPIO11_DEBUG_SPIS_ER_COMPOSITE_IRQ        = ((11u << 8u) | 36u),
    GPIO11_DEBUG_RFC_COMPOSITE_IRQ            = ((11u << 8u) | 37u),
    GPIO11_DEBUG_ADC_PHI7                     = ((11u << 8u) | 39u),
    GPIO11_DEBUG_CLK_RNG                      = ((11u << 8u) | 40u),
    GPIO11_DEBUG_CLK_PP                       = ((11u << 8u) | 41u),
    GPIO11_DEBUG_CLK_SEL_FREQ                 = ((11u << 8u) | 42u),
    GPIO11_DEBUG_SYS_SLEEP_MODE_R_2           = ((11u << 8u) | 44u),
    GPIO11_DEBUG_SYS_HALT_R                   = ((11u << 8u) | 45u),
    GPIO11_DEBUG_SYS_SLEEP_R                  = ((11u << 8u) | 46u),
    GPIO11_DEBUG_CLK_SYS                      = ((11u << 8u) | 47u),
} GPIODebug_ModeSelector_t;

/**
 * @brief Set the debug mode for the gpio.
 * @param debugMode The debug mode of the gpio.
 * @note The gpio number is included in \p debugMode parameter.
 */
static inline void GPIODebug_ModeSet(GPIODebug_ModeSelector_t debugMode)
{
    // Extract the gpio number from the debugMode parameter.
    const uint32_t gpio = (uint32_t)debugMode >> 8u;

    if (gpio < 4u || gpio > 11u)
        return;

    // Extract the test mode from the debugMode parameter.
    const uint32_t testMode = (uint32_t)(debugMode & 0xFFu);

    // Take the offset from GPIO4.
    const uint32_t gpioOffset = gpio - 4;

    // Enable protected registers access.
    PML->RegTstEn.r32 = 0xCAFEBEEF;

    // Set the DPR (Digital PRocessing block) GPIO pad Test mode selector to
    // output the debug signal to the pad.
    // RegDPRTstSel0 and RegDPRTstSel1 are consecutive in memory, therefore we
    // can access RegDPRTstSel1 with gpioOffset > 4.
    // This is not MISRA C compatible but it is debug code.
    // RegDPRTstSel0 is for GPIO 4 to 7.
    // RegDPRTstSel1 is for GPIO 8 to 11.
    SYS->RegDPRTstSel0.r8[gpioOffset] &= ~SYS_GPIO4_TST_MODE_MASK;
    SYS->RegDPRTstSel0.r8[gpioOffset] |= SYS_GPIO4_TST_MODE(testMode);

    GPIO_EnableOutput((uint8_t)gpio);
}

#endif
