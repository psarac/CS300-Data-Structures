#include <iostream>
#include <fstream> //for ifstream
#include <sstream> //for stringstream
#include <string>
#include "ModifiedPriorityQueue.h"

using namespace std;

/******************************************************************************
Purpose: This program is Skyline Program
		 Made for Sabanci University CS 300 Course HW-2
Author: Pelinsu Sarac
Date: 12/12/2022
Written on: MS Visual Studio 2012 
******************************************************************************/

/******************************************************************************
The struct below is used to track x coordinates with their label showing which 
building do they belong to and if they are the left or right side of that
building
******************************************************************************/

struct xCoord
{
	int x;
	int label;
	bool isLeft;
};

/*****************************************************************************
This function is an helper function for heapsort taking place in the program.
It helps with protecting heap order property while removing maximum element from
heap.
Input: index of removed element (hole), array to be sorted, current size of array
Returns: None / arranged array
*****************************************************************************/

/* Begin: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */
void percolateDown(int hole, vector<xCoord> & array, int currentSize)
{
	int child;
	xCoord temp = array[hole];

	for (; hole*2 <= currentSize; hole = child)
	{
		child = hole*2;
		if (child != currentSize && array[child+1].x > array[child].x)
			child++;
		if(array[child].x > temp.x)
			array[hole] = array[child];
		else
			break;
	}

	array[hole] = temp;
}
/* End: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */

int main() 
{
	/******************************************************************************
	The part below read the input.txt and extract buildings with their coordinations
	and heights, then proceeds to store x coordinates in an array with their label
	as which building do they belong to and information of which side are they.
	It also stores heights of buildings in a seperate array to make inserting 
	buildings into Priority Queue easily.
	******************************************************************************/
	string filename = "input.txt";
	int numOfBuildings, rightX, heightY, leftX;

	ifstream input;
	input.open(filename.c_str());

	//reading number of buildings
	string line;
	getline(input, line);
	stringstream streamLine1(line);
	streamLine1 >> numOfBuildings;

	int label = 0; //to label buildings in the order or appearing
	vector<int> heightWithLabels (numOfBuildings+1,-1); //height array
	int i = 1; //for xCoordArray
	vector<xCoord> xCoordArray (2*numOfBuildings+1); //x coordinate arrays

	//reading each line from input.txt
	while (getline(input, line)) 
	{
		stringstream streamLine(line);
		streamLine >> leftX >> heightY >> rightX;

		label++;
		//storing height of building with this label 
		heightWithLabels[label] = heightY;

		//placing left coordinate of building
		xCoord xc;
		xc.x = leftX;
		xc.label = label;
		xc.isLeft = true;
		xCoordArray[i] = xc;
		i++;

		//placing right coordinate of building
		xc.x = rightX;
		xc.isLeft = false;
		xCoordArray[i] = xc;
		i++;

	}

	/******************************************************************************
	The part below sorts the x coordinates coming from the file in ascending order
	with heapsort
	******************************************************************************/
	/* Begin: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */
	//Building heap for heapsort
	int currentSize = 2*numOfBuildings;

	for (int i = currentSize/2; i>0; i--)
		percolateDown(i, xCoordArray, currentSize);
	/* End: code taken from 10-Heaps.ppt and updated by Pelinsu Sarac */

	//heapsort
	xCoord currentMax;
	while (currentSize > 1)
	{
		currentMax = xCoordArray[1];
		xCoordArray[1] = xCoordArray[currentSize--];
		percolateDown(1, xCoordArray, currentSize);
		xCoordArray[currentSize+1] = currentMax;
	}

	/******************************************************************************
	The part below sweeps through sorted x coordinate array and while sweeping,
	inserts or removes buildings from Priority Queue to see the highest building
	during that span of x coordinates. If height of skyline changes, it prints out
	at which x coordinate it changes and what is the new height
	******************************************************************************/

	int oldHeight = 0; 
	ModifiedPriorityQueue queue(numOfBuildings);

	if (xCoordArray[1].x != 0)
	{
		cout << "0 0\n";
	}

	for (int i = 1; i <= 2*numOfBuildings; i++)
	{
		int label = xCoordArray[i].label;
		//if it's the left side of building, it means that building 
			//has entered the span so insert 
		if (xCoordArray[i].isLeft)
		{
			int value = heightWithLabels[label];
			queue.insert(value, label);
		}
		//if it's the right side of the building, it means that building 
			//is getting out of span so remove 
		else
		{
			queue.Remove(label);
		}

		//if there are multiple things happening at the same x coordinate,
			//give the final result 
		if (i != 2*numOfBuildings && xCoordArray[i].x == xCoordArray[i+1].x)
		{
			continue;
		}
		
		//if skyline height changed after insertion/deletion, then print out
			//the new skyline with the x coorinate it changed at 
		if (oldHeight != queue.GetMax())
		{
			cout << xCoordArray[i].x << " " << queue.GetMax();
			oldHeight = queue.GetMax();
			if (i != 2*numOfBuildings) {cout << endl;}
		}

	}

	return 0;
}