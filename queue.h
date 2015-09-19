/****************************************************************************
'queue.c' - implements functions that perform operations or manipulations
            on Queues and QueueNodes
          - Programmer: Vincent Fiestada
*****************************************************************************/
#pragma once
#include "line.h"

typedef struct QueueNode
{
	coordinate Data;
	struct QueueNode * Next;
} QueueNode;

typedef struct
{
	QueueNode * Head;
	QueueNode * Tail;
} Queue;

// Function declarations; see 'stack.c' for definitions
Queue * CreateNewQueue();
void AnnihilateQueue(Queue * targetQueue);
QueueNode * Enqueue(Queue * targetQueue, unsigned int x, unsigned int y);
coordinate Dequeue(Queue * targetQueue);
void QueueUnderflow();
void PrintQueue(Queue * targetQueue);

// <summary>
// CreateNewQueue - allocates space for a new empty Queue and returns a pointer to that Queue
//                - the pointer DOES NOT point to the head of the Queue
//                - Members: Head - First element of the Queue
//				  - The creator of the Stack has the implicit responsibility of freeing up the Queue Later
//                  using the AnnihilateQueue(..) function
// </summary>
Queue * CreateNewQueue()
{
	Queue * q = malloc(sizeof(Queue));
	q->Head = NULL; // Initialize Queue as empty
	q->Tail = NULL;
	return q;
}

// <summary>
// AnnihilateQueue - goes through each Node in the Queue and pops them
//                 - DOES NOT return the data in a Node; purely destructive
//                 - also frees up the memory allocated for the queue itself
// </summary>
void AnnihilateQueue(Queue * targetQueue)
{
	while (targetQueue->Head != NULL) // Do while queue is not empty
	{
		(void)Dequeue(targetQueue); // Pop each node and discard data returned
		// freeing up of memory used for each node is handled by the PopFromQueue function
	}
	free(targetQueue);	// free up the empty queue
	return;
}

// <summary>
// Enqueue - allocates space for a new node and pushes it into the queue.
//             - returns a pointer to the new node
// </summary>
QueueNode * Enqueue(Queue * targetQueue, unsigned int x, unsigned int y)
{
	QueueNode * n = malloc(sizeof(QueueNode));
	n->Next = NULL;
	n->Data.x = x;
	n->Data.y = y;
	if (targetQueue->Head == NULL) // If empty, the new node becomes the head and tail
	{
		targetQueue->Head = n;
		targetQueue->Tail = n;
	}
	else // If not empty, the tail's 'next' points to the new node
	{
		targetQueue->Tail->Next = n;
		targetQueue->Tail = n; // The new node becomes the tail
	}
	return n;
}

// <summary>
// Dequeue - frees up a the head of the queue and returns its contents
// </summary>
coordinate Dequeue(Queue * targetQueue)
{
	if (targetQueue->Head == NULL)
	{
		QueueUnderflow(); // Queue is empty; Call UNDERFLOW
	}
	else
	{
		QueueNode * node = targetQueue->Head; // Get the Head node
		coordinate data;
		data.x = node->Data.x; // Salvage Data
		data.y = node->Data.y;
		targetQueue->Head = node->Next;	 // Get Next node, which becomes the new Head
										// if Next == NULL, then the Queue is automatically empty
		free(node);	// Free up memory
		return data; // Return the 'salvaged Data'
	}
}

// <summary>
// QueueUnderflow - do stuff if another function determines that the Queue is empty and a manipulation
//					 needs to be cancelled
// </summary>
void QueueUnderflow()
{
	// Print error; DO NOT EXIT
	printf("\n\nERR: Queue Underflow\nCannot Pop From Empty Queue\n\n");
	return;
}
// <summary>
// PrintQueue - prints the contents of the queue w/out popping them
// </summary>
void PrintQueue(Queue * targetQueue)
{
	QueueNode * node = targetQueue->Head;
	while(node != NULL)
	{
		printf("(%d, %d) ", node->Data.x, node->Data.y);
		node = node->Next;
	}
}
