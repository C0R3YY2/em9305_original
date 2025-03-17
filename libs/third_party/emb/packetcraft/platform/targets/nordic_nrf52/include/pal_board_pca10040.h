/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      PAL internal definitions for PCA10040 board support.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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

#ifndef PAL_BOARD_PCA10040_H
#define PAL_BOARD_PCA10040_H

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** Button ***/

#define NRF_NUM_BTN           4
#define NRF_PIN_BTN_1         NRF_GPIO_PIN_MAP(0, 13)
#define NRF_PIN_BTN_2         NRF_GPIO_PIN_MAP(0, 14)
#define NRF_PIN_BTN_3         NRF_GPIO_PIN_MAP(0, 15)
#define NRF_PIN_BTN_4         NRF_GPIO_PIN_MAP(0, 16)

/*** LED ***/

#define NRF_PIN_LED_1         NRF_GPIO_PIN_MAP(0, 17)
#define NRF_PIN_LED_2         NRF_GPIO_PIN_MAP(0, 18)
#define NRF_PIN_LED_3         NRF_GPIO_PIN_MAP(0, 19)
#define NRF_PIN_LED_4         NRF_GPIO_PIN_MAP(0, 20)

/*** Logic ***/

/* P3 Header */
#define NRF_PIN_LOGIC_0       NRF_GPIO_PIN_MAP(0, 11)
#define NRF_PIN_LOGIC_1       NRF_GPIO_PIN_MAP(0, 12)
#define NRF_PIN_LOGIC_2       NRF_GPIO_PIN_MAP(0, 13)
#define NRF_PIN_LOGIC_3       NRF_GPIO_PIN_MAP(0, 14)
#define NRF_PIN_LOGIC_4       NRF_GPIO_PIN_MAP(0, 15)
#define NRF_PIN_LOGIC_5       NRF_GPIO_PIN_MAP(0, 16)
#define NRF_PIN_LOGIC_6       NRF_GPIO_PIN_MAP(0, 17)
#define NRF_PIN_LOGIC_7       NRF_GPIO_PIN_MAP(0, 18)

/*** SPI ***/

#define NRF_PIN_SPI_SCK       NRF_GPIO_PIN_MAP(0, 29)
#define NRF_PIN_SPI_MOSI      NRF_GPIO_PIN_MAP(0, 25)
#define NRF_PIN_SPI_MISO      NRF_GPIO_PIN_MAP(0, 28)
#define NRF_PIN_SPI_SS        NRF_GPIO_PIN_MAP(0, 12)

/*** TWI ***/

#define NRF_PIN_TWI_SCL       NRF_GPIO_PIN_MAP(0, 27)
#define NRF_PIN_TWI_SDA       NRF_GPIO_PIN_MAP(0, 26)

/*** UART ***/

#define NRF_PIN_UART_RXD      NRF_GPIO_PIN_MAP(0, 8)
#define NRF_PIN_UART_TXD      NRF_GPIO_PIN_MAP(0, 6)
#define NRF_PIN_UART_CTS      NRF_GPIO_PIN_MAP(0, 7)
#define NRF_PIN_UART_RTS      NRF_GPIO_PIN_MAP(0, 5)

#endif /* PAL_BOARD_PCA10040_H */
