/*!
 * File name: QueueADT.c
 *
 * Author's name: Tejaswini Jagtap
 * Date: 11/3/2018
 *
 * Implementation of the functions in the QueueADT.h
 */

#include<stdio.h>
#include<stdlib.h> 

/*!
 * Structure for the queue that keeps track of it capacity, current size, front
 * and rear element 
 */
struct Queue_S
{
  void **array;
  size_t capacity;
  size_t size;
  int front, rear;
};

#include "QueueADT.h" 

/*!
 * This function creates a Queue that we will be using in the breadth first 
 * search
 *
 * \param capacity: capacity of the array of the Queue to store 
 */
Queue createQueue(size_t capacity)
{
  Queue newQueue;
  newQueue = (Queue)malloc(sizeof(struct Queue_S));
  if (newQueue != 0)
  {
    newQueue->capacity = capacity;
    newQueue->front = newQueue->size = 0;
    newQueue->rear = 0;
    newQueue->array = malloc(capacity * sizeof(void*));
  }
  return newQueue;
}

/*!
 * This function check whether or not the Queue is empty
 *
 * \param aQueue: Queue
 */
int isEmpty(Queue aQueue)
{
  return(aQueue->size == 0);
}

/*!
 * Helper function for the enQueue. This function shiftes every element to the
 * next position so that last element in the array will be the front and and 
 * first will be rear
 *
 * \param aQueue: Queue
 * \param size: current size of the queue
 */ 
static void siftDown(Queue aQueue, size_t size)
{
  size_t i = size;
  void **array_dup = aQueue->array;
  while(i > 0)
  {
    void *element = array_dup[i-1];
    array_dup[i] = element;
    i--;
  }
  array_dup[0] = NULL;
}

/*!
 * This function inserts the element to thr array in the front 
 *
 * \param aQueue: Queue
 * \param item: item to be added 
 */
void enQueue(Queue aQueue, void * item)
{
  if (isEmpty(aQueue))
  {
    aQueue->front = aQueue->size;
    aQueue->array[aQueue->rear] = item;
    aQueue->size = aQueue->size +1;
  }
  else if ((aQueue->size + 1) >= aQueue->capacity)
  {
    aQueue->capacity = (2 * aQueue->capacity);
    aQueue->array = realloc(aQueue->array, (aQueue->capacity * sizeof(void *)));
    aQueue->front = aQueue->size;
    siftDown(aQueue, aQueue->size);
    aQueue->array[aQueue->rear] = item;
    aQueue->size = aQueue->size +1;
  }
  else
  {
    aQueue->front = aQueue->size;
    siftDown(aQueue, aQueue->size);
    aQueue->array[aQueue->rear] = item;
    aQueue->size = aQueue->size +1;
  }
}

/*!
 * This function removes the element form the array from the back so that we 
 * get the first element
 *
 * \param aQueue: Queue
 */
void* deQueue(Queue aQueue)
{
  if (!isEmpty(aQueue))
  {
  void* element = (void *)aQueue->array[aQueue->front];
  aQueue->array[aQueue->front] = NULL;
  aQueue->size = aQueue->size - 1;
  aQueue->front = aQueue->front - 1;
  return element;
  }
  return NULL;
}

/*!
 * Function that returns the first element that entered
 *
 * \param aQueue: Queue
 */
void* front(Queue aQueue)
{
  if (!isEmpty(aQueue))
  {
  return aQueue->array[aQueue->front];
  } 
  else 
  {
  return NULL;
  }
}

/*!
 * function that returns the element that entered the last 
 *
 * \param aQueue: queue
 */
void* rear(Queue aQueue)
{
  if (isEmpty(aQueue))
  {
    return NULL;
  }
  return aQueue->array[aQueue->rear];
}

/*!
 * Function that frees the queue
 *
 *\param aQueue: queue
 */
void freeQueue(Queue aQueue)
{
  if (!isEmpty(aQueue))
  {
    if(aQueue->array != 0)
    {
      free(aQueue->array);
    }
    free(aQueue);
  }
}
