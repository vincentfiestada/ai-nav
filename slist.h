/****************************************************************************
'slist.h' - implements functions that perform operations or manipulations
            on Sorted Doubly Linked Lists
            - Programmer: Vincent Paul Fiestada
*****************************************************************************/

#pragma once
#include "line.h"

typedef struct Node
{
	int f; // evaluated node cost
	int g; // level of this node in the search tree
	coordinate Data;
	struct Node * Prev;
	struct Node * Next;
} Node;

typedef struct
{
	Node * Head;
	Node * Tail;
} SortedList;


SortedList * CreateNewSortedList();
void AnnihilateSortedList(SortedList * targetList);
Node * InsertToSortedList(SortedList * targetList, unsigned int x, unsigned int y, int f, int g);
coordinate PopFromSortedList(SortedList * targetList);
void SortedListUnderflow();

// <summary>
// CreateNewSortedList - allocates space for a new empty Sorted List and returns a pointer to that List
//                     - the pointer DOES NOT point to the Head
//				       - The creator has the implicit responsibility of freeing up memory later
// </summary>
SortedList * CreateNewSortedList()
{
	SortedList * n = malloc(sizeof(SortedList));
	// Initialize as empty
	n->Head = NULL;
	n->Tail = NULL;
	return n;
}

// <summary>
// AnnihilateSortedList - goes through each Node in the List and pops them
//                 		- DOES NOT return the data in a Node; purely destructive
//                 		- also frees up the memory allocated for the List itself
// </summary>
void AnnihilateSortedList(SortedList * targetList)
{
	while (targetList->Head != NULL) // Do while list is not empty
	{
		(void)PopFromSortedList(targetList); // Pop each node and discard data returned
		// freeing up of memory used for each node is handled by the PopFromSortedList function
	}
	free(targetList);	// free up the empty list
	return;
}

// <summary>
// InsertToSortedList - inserts a new Node into a Sorted List
// </summary>
Node * InsertToSortedList(SortedList * targetList, unsigned int x, unsigned int y, int f, int g)
{
	Node * newNode = malloc(sizeof(Node)); // request space for new node
	newNode->Data.x = x;
	newNode->Data.y = y;
	newNode->f = f;
	newNode->g = g;
	newNode->Prev = NULL;
	newNode->Next = NULL;
	// find where to insert (look for a node with larger f)
	if (targetList->Head == NULL)
	{
		// List is empty
		targetList->Head = newNode;
		targetList->Tail = newNode;
	}
	else
	{
		// Perform insertion sort
		Node * compared = targetList->Head;
		while (compared != NULL && newNode->f > compared->f) // Find which node to insert newNode before
		{
			compared = compared->Next;
		}
		if (compared != NULL) // Insert newNode before the node compared
		{
			newNode->Next = compared;
			newNode->Prev = compared->Prev;
			if (newNode->Prev != NULL)
			{
				newNode->Prev->Next = newNode;
			}
			else
			{
				targetList->Head = newNode;
			}
			compared->Prev = newNode;
		}
		else
		{
			// insert into end of list
			newNode->Prev = targetList->Tail;
			targetList->Tail->Next = newNode;
			targetList->Tail = newNode;
		}
	}

	return newNode;
}

// <summary>
// PopFromSortedList - frees up the head node and returns its contents
// </summary>
coordinate PopFromSortedList(SortedList * targetList)
{
	if (targetList->Head == NULL)
	{
		SortedListUnderflow(); // List is empty; Call UNDERFLOW
	}
	else
	{
		Node * node = targetList->Head; // Get the Head node
		coordinate data = node->Data; // Salvage Data
		// f(n) is discarded
		targetList->Head = node->Next;	 // Get Next node, which becomes the new head
										// if Next == NULL, then the list is automatically empty
		if (targetList->Head != NULL)
		{
			targetList->Head->Prev = NULL;
		}
		free(node);	// Free up memory
		return data; // Return the 'salvaged Data'
	}
}

// <summary>
// SortedListUnderflow - do stuff if another function determines that the list is empty and a manipulation
//					     needs to be cancelled
// </summary>
void SortedListUnderflow()
{
	// Print error; DO NOT EXIT
	printf("\n\nERR: Sorted Doubly Linked List Underflow\nCannot Pop From List\n\n");
	return;
}
