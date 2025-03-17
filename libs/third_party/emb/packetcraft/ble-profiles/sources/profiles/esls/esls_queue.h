#ifndef ESLS_QUEUE_H
#define ESLS_QUEUE_H

#include <stdint.h>
#include "esls_main.h"

// Function prototypes
void eslsInitQueue(eslsOpQueue_t *queue);
int eslsEnqueue(eslsOpQueue_t *queue, eslsOp_t op);
int eslsDequeue(eslsOpQueue_t *queue, eslsOp_t *op);

void eslsInitTimedQueue(eslsOpQueue_t *queue);
int eslsEnqueueTimed(eslsOpQueue_t *queue, eslsOp_t op);
int eslsDequeueTimed(eslsOpQueue_t *queue, eslsOp_t *op);

/*************************************************************************************************/
/*!
 *  \brief  Iterate through the queue and free all nodes.
 *
 *  \param  queue   Pointer to the queue.
 */
/*************************************************************************************************/
void eslsResetQueue(eslsOpQueue_t *queue);

void eslsPrintQueue(eslsOpQueue_t *queue);

#endif // ESLS_QUEUE_H
