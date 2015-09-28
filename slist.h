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
	coordinate Data;
	struct Node * Prev;
	struct Node * Next;
} Node;

typedef struct
{
	Node * Head;
} SortedList;


SortedList * CreateNewSortedList();
void AnnihilateSortedList(SortedList * targetList);
Node * InsertToSortedList(SortedList * targetList, unsigned int x, unsigned int y, int f);
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
Node * InsertToSortedList(SortedList * targetList, unsigned int x, unsigned int y, int f)
{
	Node * newNode = malloc(sizeof(Node)); // request space for new node
	newNode->Data.x = x;
	newNode->Data.y = y;
	newNode->f = f;
	newNode->Prev = NULL;
	newNode->Next = NULL;
	// find where to insert (look for a node with larger f)
	Node * compared = targetList->Head;
	while (compared != NULL)
	{
		if (compared->f >= newNode->f)
		{
			newNode->Next = compared->Next;
			compared->Next = newNode;
			break;
		}
		compared = compared->Next;
	}
	newNode->Prev = compared;
	if (targetList->Head == NULL)
	{
		targetList->Head = newNode;
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
