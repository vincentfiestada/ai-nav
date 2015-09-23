/****************************************************************************
'stack.h' - implements functions that perform operations or manipulations
            on Stacks and StackNodes
          - Programmer: Vincent Paul Fiestada
*****************************************************************************/

#pragma once
#include "line.h"

typedef struct StackNode
{
	coordinate Data;
	struct StackNode * Next;
} StackNode;

typedef struct
{
	StackNode * Top;
} Stack;


Stack * CreateNewStack();
void AnnihilateStack(Stack * targetStack);
StackNode * PushToStack(Stack * targetStack, unsigned int x, unsigned int y);
coordinate PopFromStack(Stack * targetStack);
void StackUnderflow();
void PrintStack(Stack * targetStack);
unsigned int GetStackDepth(Stack * targetStack);
// <summary>
// CreateNewStack - allocates space for a new empty Stack and returns a pointer to that Stack
//                - the pointer DOES NOT point to the top of the Stack
//                - Members: Top - Top of Stack
//				  - The creator of the Stack has the implicit responsibility of freeing up the stack later
// </summary>
Stack * CreateNewStack()
{
	Stack * thenewstack = malloc(sizeof(Stack));
	thenewstack->Top = NULL; // Initialize Stack as empty
	return thenewstack;
}

// <summary>
// AnnihilateStack - goes through each Node in the Stack and pops them
//                 - DOES NOT return the data in a Node; purely destructive
//                 - also frees up the memory allocated for the stack itself
// </summary>
void AnnihilateStack(Stack * targetStack)
{
	while (targetStack->Top != NULL) // Do while stack is not empty
	{
		(void)PopFromStack(targetStack); // Pop each node and discard data returned
		// freeing up of memory used for each node is handled by the PopFromStack function
	}
	free(targetStack);	// free up the empty stack
	return;
}

// <summary>
// PushToStack - inserts a new StackNode at the top of the Stack and moves the Top pointer
//                accordingly.
// </summary>
StackNode * PushToStack(Stack * targetStack, unsigned int x, unsigned int y)
{
	StackNode * newNode = malloc(sizeof(StackNode)); // request space for new node
	newNode->Data.x = x;
	newNode->Data.y = y;
	newNode->Next = targetStack->Top; // The new node points to the previous top and
	targetStack->Top = newNode;      // becomes the new top
}

// <summary>
// PopFromStack - free ups a the topmost node and returns its contents
// </summary>
coordinate PopFromStack(Stack * targetStack)
{
	if (targetStack->Top == NULL)
	{
		StackUnderflow(); // Stack is empty; Call UNDERFLOW
	}
	else
	{
		StackNode * node = targetStack->Top; // Get the Top node
		coordinate data = node->Data; // Salvage Data
		targetStack->Top = node->Next;	 // Get Next node, which becomes the new top
										// if Next == NULL, then the Stack is automatically empty
		free(node);	// Free up memory
		return data; // Return the 'salvaged Data'
	}
}

// <summary>
// StackUnderflow - do stuff if another function determines that the Stack is empty and a manipulation
//					 needs to be cancelled
// </summary>
void StackUnderflow()
{
	// Print error; DO NOT EXIT
	printf("\n\nERR: Stack Underflow\nCannot Pop From Empty Stack\n\n");
	return;
}

// <summary>
// PrintStack - goes through every node in the Stack, starting from the Top, and prints the contents of each
// </summary>
void PrintStack(Stack * targetStack)
{
	StackNode * node = targetStack->Top;
	while (node != NULL)
	{
		printf("(%d, %d) ", node->Data.x, node->Data.y); // Each character is separated by a <space>
		node = node->Next;
	}
	return;
}

// <summary>
// GetStackDepth - returns the depth or number of items in the stack
// </summary>
unsigned int GetStackDepth(Stack * targetStack)
{
	unsigned int depth = 0;
	StackNode * node = targetStack->Top;
	while (node != NULL)
	{
		depth++;
		node = node->Next;
	}
	return depth;
}
