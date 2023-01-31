#include "ModifiedPriorityQueue.h"
#include <iostream>

using namespace std;

/*****************************************************************************
This function is the constructor of Modified Priority Queue class. It initializes
the heap and location arrays with given capacity and current size to 0
Input: capacity 
Returns: empty MPQ (Modified Priority Queue) 
*****************************************************************************/

ModifiedPriorityQueue::ModifiedPriorityQueue(int capacity)
{
	HeapElement sample;
	sample.value = -1;
	sample.label = -1;

	vector<HeapElement> temp(capacity+1, sample);
	heap = temp;

	vector<int> temp2(capacity+1, -1);
	location = temp2;

	currentSize = 0;
}

/*****************************************************************************
This function returns true if priority queue is empty
Input: None
Returns: True if queue is empty, false otherwise
*****************************************************************************/

bool ModifiedPriorityQueue::IsEmpty() const
{
	return currentSize == 0;
}

/*****************************************************************************
This function returns current maximum element of the priority queue without
removing it
Input: None
Returns: Maximum value stored in priority queue
*****************************************************************************/

int ModifiedPriorityQueue::GetMax() const
{
	if (!IsEmpty())
		return heap[1].value;
	else
		return 0;
}

/*****************************************************************************
This function inserts given value with given label to the priority queue without
disturbing the properties of max heap. It also updates location array according to
the changes happenning in the heap during insertion 
Input: value to be inserted and its label 
Returns: Npne/updated heap and location array 
*****************************************************************************/

/* Begin: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */
void ModifiedPriorityQueue::insert(int value, int label)
{
	int hole = ++currentSize; //starting from last available spot

	//while value is greater than parents
	for (; hole > 1 && value > heap[hole/2].value; hole /= 2)
	{
		heap[hole] = heap[hole/2]; //swap parent and hole
		location[heap[hole/2].label] = hole; //update location of the swapped element
	}

	//inserting value with its label into hole
	heap[hole].value = value;
	heap[hole].label = label;
	location[label] = hole;
}
/* End: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */

/*****************************************************************************
This function is a helper function for remove operation. It percolates an element
in heap (or hole) down until it fits the heap order property.
Input: hole index and label of element to be percolated down
Returns: updated queue with heap satisfying heap order property
*****************************************************************************/

/* Begin: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */
void ModifiedPriorityQueue::percolateDown(int hole, int label)
{
	int child;
	int temp = heap[hole].value;

	for(; hole*2 <= currentSize; hole = child)
	{
		child = hole*2;
		if (child != currentSize && heap[child+1].value > heap[child].value)
			child++;
		if (heap[child].value > temp)
		{
			heap[hole] = heap[child];
			location[heap[child].label] = hole; //update location of the swapped element
		}
		else
		{ break; }
	}
	heap[hole].value = temp;
	heap[hole].label = label;
	location[label] = hole;
}
/* End: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */

/*****************************************************************************
This function is a helper function for remove operation. It percolates an element
in heap (or hole) up until it fits the heap order property.
Input: hole index and label of element to be percolated up
Returns: updated queue with heap satisfying heap order property
*****************************************************************************/

/* Begin: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */
void ModifiedPriorityQueue::percolateUp(int hole, int label)
{
	int temp = heap[hole].value;
	//while value is greater than parents
	for (; hole > 1 && temp > heap[hole/2].value; hole /= 2)
	{
		heap[hole] = heap[hole/2]; //swap parent and hole
		location[heap[hole/2].label] = hole; //update location of the swapped element
	}

	//inserting value with its label into hole
	heap[hole].value = temp;
	heap[hole].label = label;
	location[label] = hole;
}
/* End: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */

/*****************************************************************************
This function removes an element with given label from the priority queue without
disturbing properties of heap
Input: label of element to be removed
Returns: updated queue with heap satisfying heap order property
*****************************************************************************/

/* Begin: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */
int ModifiedPriorityQueue::Remove(int label) 
{
	//if heap is not empty and an element exists with given label
	if (!IsEmpty() && !(location[label] == -1))
	{
		int retVal = heap[location[label]].value; //returning value with given label

		//removing label's index from location 
		int deletedIdx = location[label];
		location[label] = -1;

		if (deletedIdx == currentSize) //if last element is deleted
		{
			currentSize--;
		} else {

			//label of last element of heap
			int i = heap[currentSize].label;

			//replace deleted element with the last element of heap
			heap[deletedIdx] = heap[currentSize--];

			//if replaced element is larger than its new parent, percolate up
				//if not, percolate down
			if (deletedIdx != 1 && heap[deletedIdx].value > heap[deletedIdx/2].value)
			{ 
				percolateUp(deletedIdx, i); 
			}
			else { percolateDown(deletedIdx, i); }
		}

		return retVal;
	}

	return 0;
}
/* End: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */

//Destructor of Modified Priority Queue Class
ModifiedPriorityQueue::~ModifiedPriorityQueue()
{}

