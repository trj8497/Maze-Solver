/*!
 * File Name: QueueADT.h
 *
 * Author's name: Tejaswini Jagtap
 * Date: 11/3/2018
 *
 * Header file for the QueueADT
 */

#ifndef HEAPADT_H
#define HEAPADT_H

/*!
 * Structure that contains array, its capacity, current size, fri=ont and rear
 */
typedef struct Queue_S * Queue;

/*!
 * Function that creates a Queue
 *
 * \param capacity: number of elements that the queueu can store
 */
Queue createQueue(size_t capacity);

/*!
 * Function that inserts item in the Queue
 *
 * \param aQueue: Queue
 * \param item: item to be added 
 */
void enQueue(Queue aQueue, void * item);

/*!
 * This function checks whether or not the queue is empty 
 *
 * \param aQueue: queue
 */
int isEmpty(Queue aQueue);

/*!
 * This function removes the last element from the queue
 *
 * \param aQueue: queue
 */
void* deQueue(Queue aQueue);

/*!
 * Function that returns the front of the Queue;
 *
 * \param aQueue: queue
 */
void* front(Queue aQueue);

/*!
 * Function that returns the rear of the queue
 *
 * \param aQueue: queue
 */
void* rear(Queue aQueue);

/*! 
 * Function thatv frees the queue
 *
 * \param aQueue: queue
 */
void freeQueue(Queue aQueue);

#endif 
