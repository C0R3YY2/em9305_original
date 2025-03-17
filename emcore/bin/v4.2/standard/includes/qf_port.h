////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/QPC/ports/arcv2em/qf_port.h
///
/// @project    T9305
///
/// @brief      Port for QK.
///
/// @classification  Internal Use
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

#ifndef qf_port_h
#define qf_port_h

#include "apexextensions.h"
#include <irq.h>


/*! The maximum number of active objects in the application. */
/**
* @description
* This macro *must* be defined in the QF port and should be in range
* of 1..64, inclusive. The value of this macro determines the maximum
* priority level of an active object in the system. Not all priority
* levels must be used, but the maximum priority cannot exceed
* #QF_MAX_ACTIVE.
*
* @note Once you choose a certain value of #QF_MAX_ACTIVE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MAX_ACTIVE               16

/*! The maximum number of event pools in the application. */
/**
* @description
* This macro should be defined in the QF ports and should be in range
* of 1..255, inclusive. The value of this macro determines the maximum
* event pools in the system. Not all event pools must be actually used,
* but the maximum number of pools cannot exceed #QF_MAX_EPOOL.
*
* If the macro is not defined, the default value is 3. Defining the value
* below the maximum saves some memory, mostly for the subscriber-lists.
* @sa ::QSubscrList.
*
* @note Once you choose a certain value of #QF_MAX_EPOOL, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MAX_EPOOL               2

/*! The size (in bytes) of the event-size representation in the QF.
* Valid values: 1, 2, or 4; default 2
*/
/**
* @description
* This macro can be defined in the QF ports to configure the ::QEvtSize
* type. If the macro is not defined, the default of 2 byte will be chosen in
* qf.h. The valid #QF_EVENT_SIZ_SIZE values of 1, 2, or 4, correspond
* to ::QEvtSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QEvtSize data type determines the dynamic range of event-sizes in
* your application.
* @sa QF_poolInit(), QF_new_()
*
* @note Once you choose a certain value of #QF_EVENT_SIZ_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_EVENT_SIZ_SIZE           2

/*! The size (in bytes) of the ring-buffer counters used in the native QF
* event queue implementation. Valid values: 1, 2, or 4; default 1
*/
/**
* @description
* This macro can be defined in the QF ports to configure the ::QEQueueCtr
* type. If the macro is not defined, the default of 1 byte will be chosen in
* qequeue.h. The valid #QF_EQUEUE_CTR_SIZE values of 1, 2, or 4, correspond
* to ::QEQueueCtr of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QEQueueCtr data type determines the dynamic range of numerical values of
* ring-buffer counters inside event queues, or, in other words, the maximum
* number of events that the native QF event queue can manage.
* @sa ::QEQueue
*
* @note Once you choose a certain value of #QF_EQUEUE_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_EQUEUE_CTR_SIZE          1

/*! The size (in bytes) of the block-size representation in the native QF
* event pool. Valid values: 1, 2, or 4; default #QF_EVENT_SIZ_SIZE.
*/
/**
* @description
* This macro can be defined in the QF ports to configure the ::QMPoolSize
* type. If the macro is not defined, the default of #QF_EVENT_SIZ_SIZE
* will be chosen in qmpool.h, because the memory pool is primarily used for
* implementing event pools.
*
* The valid #QF_MPOOL_SIZ_SIZE values of 1, 2, or 4, correspond to
* ::QMPoolSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QMPoolSize data type determines the dynamic range of block-sizes that
* the native ::QMPool can handle.
* @sa #QF_EVENT_SIZ_SIZE, ::QMPool
*
* @note Once you choose a certain value of #QF_MPOOL_SIZ_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MPOOL_SIZ_SIZE           2

/*! The size (in bytes) of the block-counter representation in the
* native QF event pool. Valid values: 1, 2, or 4; default 2.
*/
/**
* @description
* This macro can be defined in the QF ports to configure the ::QMPoolCtr
* type. If the macro is not defined, the default of 2 bytes will be chosen
* in qmpool.h. The valid #QF_MPOOL_CTR_SIZE values of 1, 2, or 4, correspond
* to ::QMPoolSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QMPoolCtr data type determines the dynamic range of block-counters that
* the native ::QMPool can handle, or, in other words, the maximum number
* of blocks that the native QF event pool can manage.
* @sa ::QMPool
*
* @note Once you choose a certain value of #QF_MPOOL_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MPOOL_CTR_SIZE           2

/*! The size (in bytes) of the time event-counter representation
* in the ::QTimeEvt struct. Valid values: 1, 2, or 4; default 2.
*/
/**
* @description
* This macro can be defined in the QF ports to configure the internal tick
* counters of Time Events. If the macro is not defined, the default of 2
* bytes will be chosen in qf.h. The valid #QF_TIMEEVT_CTR_SIZE values of 1,
* 2, or 4, correspond to tick counters of uint8_t, uint16_t, and uint32_t,
* respectively. The tick counter representation determines the dynamic range
* of time delays that a Time Event can handle.
* @sa ::QTimeEvt
*
* @note Once you choose a certain value of #QF_TIMEEVT_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_TIMEEVT_CTR_SIZE         2


#define QF_MAX_TICK_RATE            0


/*! Define the interrupt disabling policy.
*
* This macro encapsulates platform-specific way of disabling interrupts
* from "C" for a given CPU and compiler.
*
* @note the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_DISABLE_GLOBAL()     IRQ_DisableInterrupts();
#define QF_INT_DISABLE(var_)        do { \
                                        var_ = IRQ_DisableAndSaveInterrupts(); \
                                    } while(0)

/*! Define the interrupt enabling policy. */
/**
* @description
* This macro encapsulates platform-specific way of enabling interrupts
* from "C" for a given CPU and compiler.
*
* @note the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_ENABLE_GLOBAL()      IRQ_EnableInterrupts();
#define QF_INT_ENABLE(var_)         IRQ_RestoreInterrupts(var_);


/*! Define the type of the critical section status. */
/**
* @description
* Defining this macro configures the "saving and restoring critical section
* status" policy. Coversely, if this macro is not defined, the simple
* "unconditional critical section exit" is used.
*/
#define QF_CRIT_STAT_TYPE           crit_stat_t

/*! Define the critical section entry policy. */
/**
* @description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro sets the @a status_ argument to the critical
* section status just before the entry. When the policy of "unconditional
* critical section exit" is used, the macro does not use the @a status_
* argument.
*
* @note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_ENTRY(stat_)        ((stat_) = critEntry())

/*! Define the critical section exit policy. */
/**
* @description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro restores the critical section status from the
* @a status_ argument. When the policy of "unconditional critical section
* exit" is used, the macro does not use the @a status argument and
* exits the critical section unconditionally (often by means of enabling
* interrupts).
*
* @note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_EXIT(stat_)         critExit(stat_)

typedef uint32_t crit_stat_t;
static QF_CRIT_STAT_TYPE critEntry(void);
static void critExit(QF_CRIT_STAT_TYPE stat);

#ifndef USE_SW_LOG2P1
#define QF_LOG2(x)      ( (uint_fast8_t) log2p1( (int) x ) )
#endif // USE_SW_LOG2P1

#include "qep.h"        /* qep */
#include "qf.h"         /* QF platform-independent public interface */
#include "qk.h"         /* QK platform-independent kernel */

static void qkIsrEntry(void);
static void qkIsrExit(void);

#define QK_ISR_ENTRY()  qkIsrEntry()
#define QK_ISR_EXIT()   qkIsrExit()

#include <stdint.h>
#include <stdbool.h>

#include "qassert.h"

static ALWAYS_INLINE void qkIsrEntry(void)
{
    uint32_t savedInterrupts;
    QF_INT_DISABLE(savedInterrupts);
    QK_attr_.intNest++;
    QF_INT_ENABLE(savedInterrupts);
}

static ALWAYS_INLINE void qkIsrExit(void)
{
    uint32_t savedInterrupts;
    QF_INT_DISABLE(savedInterrupts);
    if (QK_attr_.intNest > 0u)
    {
        QK_attr_.intNest--;
    }
    IRQ_Trigger(IRQ_GROUP_SWI, SWI(0));
    QF_INT_ENABLE(savedInterrupts);
}

static ALWAYS_INLINE QF_CRIT_STAT_TYPE critEntry(void)
{
    uint32_t stat;
    QF_INT_DISABLE(stat);
    return (QF_CRIT_STAT_TYPE)stat;
}

static ALWAYS_INLINE void critExit(QF_CRIT_STAT_TYPE stat)
{
    QF_INT_ENABLE((uint32_t)stat);
}

#endif /* qf_port_h */
