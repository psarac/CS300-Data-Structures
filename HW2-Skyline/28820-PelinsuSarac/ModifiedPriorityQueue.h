#ifndef _MODIFIEDPRIORITYQUEUE_H
#define _MODIFIEDPRIORITYQUEUE_H

#include <vector>

using namespace std;

/******************************************************************************
This struct below is to keep values inside priority queue together with their
labels so that it's easier to remove them
******************************************************************************/

struct HeapElement
{
	int value;
	int label;
};

class ModifiedPriorityQueue
{
public:
	ModifiedPriorityQueue(int capacity); //Parametric Constructor
	~ModifiedPriorityQueue(); //Destructor

	bool IsEmpty() const; //checks if queue is empty
	void insert(int value, int label); //inserts a value with its label into priority queue
	int Remove(int label); //removes and returns the element with given label from priority queue
	int GetMax() const; //returns the maximum value from priority queue but does not remove it 

private:
	int currentSize; //how many elements are in queue?
	vector<HeapElement> heap;
	vector<int> location;
	//helper functions for remove operation
	void percolateUp(int hole, int label);
	void percolateDown(int hole, int label);
};

#endif