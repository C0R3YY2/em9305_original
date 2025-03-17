/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Platform dependant FreeRTOS configuration.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.  All rights reserved.
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

#ifndef PAL_FREERTOS_H
#define PAL_FREERTOS_H

#include "nrf.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define configCPU_CLOCK_HZ                  (SystemCoreClock)
#define configTICK_RATE_HZ                  ((TickType_t)1000)
#define configPRIO_BITS                      __NVIC_PRIO_BITS
#define configKERNEL_INTERRUPT_PRIORITY      (0xFF << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (1 << (8 - configPRIO_BITS))

#endif /* PAL_FREERTOS_H */
