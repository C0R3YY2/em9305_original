////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/printf/includes/puts.h
///
/// @project    T9305
///
/// @brief      PUTS functions for printf library.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2016 EM Microelectronic
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


#ifndef PUTS_H
#define PUTS_H


#include "driver.h"
#include <types.h>


/**
 * @addtogroup drivers
 * @{
 * @defgroup puts PUTS
 * @brief PUTS functions for printf library.
 *
 * @{
*/


/******************************************************************************\
 *  DEFINITIONS
\******************************************************************************/

/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * @defgroup puts_fct PUTS function prototypes
 * @{
 */


/**
 * @brief tiny putc function
 *
 * Printf a char over UART. This is a blocking function.
 * This function should not be called directly. It is used to init the
 * printf library.
 *
 * init_printf(NULL, tiny_putc);
 *
 * @param p Unused parameter
 * @param c The character to print
 */
void tiny_putc(void* p, char c);


/**
 * @} end of group puts_fct
 */


/**
 * @} end of group puts
 * @} end of addtogroup drivers
 */


#endif /* PUTS_H */
