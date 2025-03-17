/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      NRFX glue definitions.
 *
 *  Copyright (c) 2019-2022 Packetcraft, Inc.  All rights reserved.
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

#ifndef NRFX_GLUE_H
#define NRFX_GLUE_H

/* Use MDK interrupt handler bindings. */
#include <soc/nrfx_irqs.h>

//------------------------------------------------------------------------------

#define NRFX_ASSERT(expression)

#define NRFX_STATIC_ASSERT(expression)

//------------------------------------------------------------------------------

#define NRFX_IRQ_PRIORITY_SET(irq_number, priority)   NVIC_SetPriority(irq_number, priority)

#define NRFX_IRQ_ENABLE(irq_number)                   NVIC_EnableIRQ(irq_number)

#define NRFX_IRQ_IS_ENABLED(irq_number)               NVIC_GetEnableIRQ(irq_number)

#define NRFX_IRQ_DISABLE(irq_number)                  NVIC_DisableIRQ(irq_number)

#define NRFX_IRQ_PENDING_SET(irq_number)              NVIC_SetPendingIRQ(irq_number)

#define NRFX_IRQ_PENDING_CLEAR(irq_number)            NVIC_ClearPendingIRQ(irq_number)

#define NRFX_IRQ_IS_PENDING(irq_number)               (NVIC_GetPendingIRQ(irq_number) == 1)

//------------------------------------------------------------------------------

#define NRFX_DELAY_US(us_time)

//------------------------------------------------------------------------------

#define NRFX_CRITICAL_SECTION_ENTER()                 __disable_irq()

#define NRFX_CRITICAL_SECTION_EXIT()                  __enable_irq()

//------------------------------------------------------------------------------

#define nrfx_atomic_t     volatile uint32_t

//------------------------------------------------------------------------------

#endif /* NRFX_GLUE_H */
