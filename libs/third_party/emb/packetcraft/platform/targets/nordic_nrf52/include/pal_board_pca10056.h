/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      PAL internal definitions for PCA10056 (nRF52840-DK) board support.
 *
 *  Copyright (c) 2019-2024 Packetcraft, Inc.  All rights reserved.
 *  Packetcraft, Inc. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef PAL_BOARD_PCA10056_H
#define PAL_BOARD_PCA10056_H

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** Button ***/

#define NRF_NUM_BTN           4
#define NRF_PIN_BTN_1         NRF_GPIO_PIN_MAP(0, 11)
#define NRF_PIN_BTN_2         NRF_GPIO_PIN_MAP(0, 12)
#define NRF_PIN_BTN_3         NRF_GPIO_PIN_MAP(0, 24)
#define NRF_PIN_BTN_4         NRF_GPIO_PIN_MAP(0, 25)

/*** I2S ***/

#define NRF_PIN_I2S_SCK       NRF_GPIO_PIN_MAP(1, 11)
#define NRF_PIN_I2S_LRCK      NRF_GPIO_PIN_MAP(1, 12)
#define NRF_PIN_I2S_MCK       NRF_GPIO_PIN_MAP(1, 10)
#define NRF_PIN_I2S_SDOUT     NRF_GPIO_PIN_MAP(1, 14)
#define NRF_PIN_I2S_SDIN      NRF_GPIO_PIN_MAP(1, 13)

/*** LED ***/

#define NRF_PIN_LED_1         NRF_GPIO_PIN_MAP(0, 13)
#define NRF_PIN_LED_2         NRF_GPIO_PIN_MAP(0, 14)
#define NRF_PIN_LED_3         NRF_GPIO_PIN_MAP(0, 15)
#define NRF_PIN_LED_4         NRF_GPIO_PIN_MAP(0, 16)

/*** Logic ***/

/* P3 Header */
#define NRF_PIN_LOGIC_0       NRF_GPIO_PIN_MAP(1,  1)
#define NRF_PIN_LOGIC_1       NRF_GPIO_PIN_MAP(1,  2)
#define NRF_PIN_LOGIC_2       NRF_GPIO_PIN_MAP(1,  3)
#define NRF_PIN_LOGIC_3       NRF_GPIO_PIN_MAP(1,  4)
#define NRF_PIN_LOGIC_4       NRF_GPIO_PIN_MAP(1,  5)
#define NRF_PIN_LOGIC_5       NRF_GPIO_PIN_MAP(1,  6)
#define NRF_PIN_LOGIC_6       NRF_GPIO_PIN_MAP(1,  7)
#define NRF_PIN_LOGIC_7       NRF_GPIO_PIN_MAP(1,  8)

/*** QSPI ***/

#define NRF_PIN_QSPI_SCK      NRF_GPIO_PIN_MAP(0, 19)
#define NRF_PIN_QSPI_CSN      NRF_GPIO_PIN_MAP(0, 17)
#define NRF_PIN_QSPI_IO0      NRF_GPIO_PIN_MAP(0, 20)
#define NRF_PIN_QSPI_IO1      NRF_GPIO_PIN_MAP(0, 21)
#define NRF_PIN_QSPI_IO2      NRF_GPIO_PIN_MAP(0, 22)
#define NRF_PIN_QSPI_IO3      NRF_GPIO_PIN_MAP(0, 23)

/*** SPI ***/

#define NRF_PIN_SPI_SCK       NRF_GPIO_PIN_MAP(1, 15) /* Use Arduino SPI pin definition. */
#define NRF_PIN_SPI_MOSI      NRF_GPIO_PIN_MAP(1, 13)
#define NRF_PIN_SPI_MISO      NRF_GPIO_PIN_MAP(1, 14)
#define NRF_PIN_SPI_SS        NRF_GPIO_PIN_MAP(1, 12)

/*** TWI ***/

#define NRF_PIN_TWI_SCL       NRF_GPIO_PIN_MAP(0, 27) /* Use Arduino TWI pin definition. */
#define NRF_PIN_TWI_SDA       NRF_GPIO_PIN_MAP(0, 26)

/*** UART ***/

#define NRF_PIN_UART_RXD      NRF_GPIO_PIN_MAP(0, 8)
#define NRF_PIN_UART_TXD      NRF_GPIO_PIN_MAP(0, 6)
#define NRF_PIN_UART_CTS      NRF_GPIO_PIN_MAP(0, 7)
#define NRF_PIN_UART_RTS      NRF_GPIO_PIN_MAP(0, 5)

#endif /* PAL_BOARD_PCA10095_H */
