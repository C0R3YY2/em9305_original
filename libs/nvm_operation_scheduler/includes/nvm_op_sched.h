////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/nvm_operation_scheduler/includes/nvm_op_sched.h
///
/// @project    T9305
///
/// @brief      NVM operation scheduler library interface file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic
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


#ifndef _NVM_OP_SCHED_H
#define _NVM_OP_SCHED_H

#include "types.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup nvm_op_sched NVM operation scheduler
 * @brief Library to schedule the NVM operations.
 *
 * This module contains all the code required to schedule the NVM operations.
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/** @brief The maximum number of scheduled NVM operation that can be queued. */
#define NVM_OP_SCHED_QUEUE_SIZE (8u)

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/**
 * @brief Event type.
 */
typedef enum
{
    NVM_EVENT_ERASE = 0,
    NVM_EVENT_WRITE
} NVM_EventType_t;

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @brief Event type definition.
 */
typedef struct
{
    /// Event Type
    NVM_EventType_t type;

    /// Status
    int32_t status; // Negative errno status code. See errno.h.

    /// Event info
    union {
        struct {
            uint32_t addr;
            uint32_t length;
            uint32_t *pData;
        } write;
        struct {
            uint32_t addr;
            uint32_t length;
        } erase;
    };
} NVMOpSched_Event_t;

/**
 * @brief Callback function type definition.
 */
typedef void (*NVMOpSched_DriverCallback_t)(NVMOpSched_Event_t *event, void *pUserData);

/******************************************************************************\
 *  EXPORTED GLOBAL VARIABLES
\******************************************************************************/

/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/

/**
 * @brief Schedule a NVM-Main or NMV-Info erase page(s).
 * @note The maximum number of scheduled NVM operation is @ref NVM_OP_SCHED_QUEUE_SIZE.
 * @param addr Start address, must be NVM page size aligned.
 * @param length Number of bytes to erase, must be NVM page size aligned.
 * @param operationCb Callback that will be called when the operation is completed.
 * @param pUserData User data that is passed as a parameter when the callback is invoked.
 * @return Error code, see errno.h.
 */
int32_t NVMOpSched_ScheduleErase(uint32_t                    addr,
                                 uint32_t                    length,
                                 NVMOpSched_DriverCallback_t operationCb,
                                 void                       *pUserData);

/**
 * @brief Schedule a write to NVM-Main or NMV-Info.
 * @note The maximum number of scheduled NVM operation is @ref NVM_OP_SCHED_QUEUE_SIZE.
 * @param addr Start address, must be NVM word size aligned.
 * @param length Number of bytes to write, must be NVM word size aligned.
 * @param pWriteData Pointer to the data to write to NVM.
 * @param operationCb Callback that will be called when the operation is completed.
 * @param pUserData User data that is passed as a parameter when the callback is invoked.
 * @return Error code, see errno.h.
 */
int32_t NVMOpSched_ScheduleWrite(uint32_t                    addr,
                                 uint32_t                    length,
                                 uint32_t                   *pWriteData,
                                 NVMOpSched_DriverCallback_t operationCb,
                                 void                       *pUserData);

/**
 * @brief Try to perform the NVM operations that were scheduled.
 * @details This function is intended to be called when the system is executing
 *          its idle task. The NVM scheduler will query all modules that have
 *          registered a function with the protocol timer module using @ref
 *          ProtTimer_RegisterEventTimeCallback.
 * @note When NVM operations are waiting to be executed, the NVM operation
 *       scheduler will prohibit the CPU from going to sleep and halting.
 */
void NVMOpSched_TryToPerformNvmOp(void);

/** @} */ // End of group nvm_op_sched
/** @} */ // End of addtogroup drivers

#endif /* _NVM_OP_SCHED_H */
