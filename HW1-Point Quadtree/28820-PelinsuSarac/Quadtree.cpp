#include "Quadtree.h"
#include <iostream>

using namespace std;

/*****************************************************************************
This function is the constructor of Quadtree class.
Input: None
Returns: an empty Quadtree 
*****************************************************************************/

Quadtree::Quadtree (): root(NULL), isFirstSearch(true)
{}

/*****************************************************************************
This function calls private version of insertCity and passes city name with
coordinates to it.
Input: City name, x and y coordinates of the city
Returns: None / insertion of city to tree
*****************************************************************************/

//public insertCity
void Quadtree::insertCity(const string & cn, int xc, int yc)
{
	insertCity(cn, xc, yc, root);
}

/*****************************************************************************
This function is a helper function for insertCity which finds the correct
quadrant to proceed while inserting a city.
Input: x and y coordinates of city to be inserted, pointer to a city from
tree
Returns: Quadrant (as string)
*****************************************************************************/

//to find the correct quadrant

/* Begin: code taken from Point Quadtrees.pdf */

string Quadtree::compareCoordinates(int xc, int yc, QuadtreeNode * ptr)
{
	//by comparing coordinates, decides quadrant
	if (xc < ptr->xCoord) 
	{
		if (yc < ptr->yCoord)
		{
			return "SW";
		}
		else 
		{
			return "NW";
		}
	}
	else if (yc < ptr->yCoord)
	{
		return "SE";
	}
	else 
	{
		return "NE";
	}
}

/* End: code taken from Point Quadtrees.pdf */

/*****************************************************************************
This function inserts cities to Quadtree given their coordinates and names. It
also require a node pointer as it works in recursion to find the correct place
for the city given.
Input: Name of the city, x and y coordinates of the city, node pointer
Returns: None / insertion of the city into tree
*****************************************************************************/

/* Begin: code taken from 5-Trees.ppt and updated by Pelinsu Sarac */

void Quadtree::insertCity(const string & cn, int xc, int yc, QuadtreeNode * & r)
{
	//if tree was empty, it's the first node
	if (r == NULL)
	{
		r = new QuadtreeNode(cn, xc, yc, NULL, NULL, NULL, NULL);
	}
	else if (!(r->xCoord == xc && r->yCoord == yc))
	{
		//inserts according to quadrant returned by compareCoordinates
		string quadrant = compareCoordinates(xc, yc, r);
		if (quadrant == "SW")
		{
			insertCity(cn, xc, yc, r->SW);
		}
		else if (quadrant == "NW")
		{
			insertCity(cn, xc, yc, r->NW);
		}
		else if (quadrant == "SE")
		{
			insertCity(cn, xc, yc, r->SE);
		}
		else if (quadrant == "NE")
		{
			insertCity(cn, xc, yc, r->NE);
		}
	}

}

/* End: code taken from 5-Trees.ppt and updated by Pelinsu Sarac */

/*****************************************************************************
This function recursively prints the tree in the specified format.
Input: Node pointer
Returns: None / display of names of cities on tree
*****************************************************************************/

/* Begin: code taken from Point Quadtree Assignment.pdf */

void Quadtree::prettyPrintTree(QuadtreeNode * ptr)
{
	if (ptr != NULL)
	{
		cout << ptr->cityName << endl;
		prettyPrintTree(ptr->SE);
		prettyPrintTree(ptr->SW);
		prettyPrintTree(ptr->NE);
		prettyPrintTree(ptr->NW);
	}
}

/* End: code taken from Point Quadtree Assignment.pdf */

/*****************************************************************************
This function allows the user to call prettyPrint function as it's public.
Input: None
Returns: None / display of names of cities on tree
*****************************************************************************/

void Quadtree::prettyPrintTree()
{
	prettyPrintTree(root);
}

/*****************************************************************************
This function allows the user to do the queries with the tree and also handles
the display of the query result
Input: x and y coordinates of the center of circle, radius of circle 
Returns: None / displayment of query result
*****************************************************************************/

void Quadtree::searchCityInRadius(int xc, int yc, int radius)
{
	firstLine = "";
	secondLine = "";
	searchCityInRadius(xc, yc, radius, root);

	if (firstLine == "") 
	{
		firstLine = "<None>";
	}
	else
	{
		int length = firstLine.length();
		firstLine = firstLine.substr(0, length-2);
	}

	int lineLength = secondLine.length();
	secondLine = secondLine.substr(0, lineLength-2);

	if (isFirstSearch)
	{
		cout << "\n" << firstLine << "\n" << secondLine;
		isFirstSearch = false;
	}
	else 
	{
		cout << "\n\n" << firstLine << "\n" << secondLine;
	}

}

/*****************************************************************************
This function does the query recursively and records traversed cities and
which one of them are in the circle
Input: x and y coordinates of the center of circle, radius of circle
node pointer to be evaluated 
Returns: None / tracks and directs the query
*****************************************************************************/

void Quadtree::searchCityInRadius(int xc, int yc, int radius, QuadtreeNode * ptr)
{

	if (ptr != NULL)
	{
		secondLine += ptr->cityName + ", ";

		int xCoord = ptr->xCoord;
		int yCoord = ptr->yCoord;

		//inside the search circle or not 
		if ( (xCoord-xc)*(xCoord-xc) + (yCoord-yc)*(yCoord-yc) <= radius*radius )
		{
			//13
			firstLine += ptr->cityName + ", ";
			searchCityInRadius(xc, yc, radius, ptr->SE);
			searchCityInRadius(xc, yc, radius, ptr->SW);
			searchCityInRadius(xc, yc, radius, ptr->NE);
			searchCityInRadius(xc, yc, radius, ptr->NW);
		}
		else if ( (xCoord <= xc+radius) && (xCoord > xc-radius) )
		{
			//inside the square?
			if ( (yCoord <= yc+radius) && (yCoord > yc-radius) )
			{  
				if (xCoord < xc) 
				{ 
					if (yCoord < yc)
					{ 
						//11 
						searchCityInRadius(xc, yc, radius, ptr->SE);
						searchCityInRadius(xc, yc, radius, ptr->NE);
						searchCityInRadius(xc, yc, radius, ptr->NW);
					}
					else 
					{ 
						//9
						searchCityInRadius(xc, yc, radius, ptr->SE);
						searchCityInRadius(xc, yc, radius, ptr->SW);
						searchCityInRadius(xc, yc, radius, ptr->NE);
					}
				}
				else
				{ 
					if (yCoord < yc)
					{
						//12
						searchCityInRadius(xc, yc, radius, ptr->SW);
						searchCityInRadius(xc, yc, radius, ptr->NE);
						searchCityInRadius(xc, yc, radius, ptr->NW);
					}
					else 
					{
						//10
						searchCityInRadius(xc, yc, radius, ptr->SE);
						searchCityInRadius(xc, yc, radius, ptr->SW);
						searchCityInRadius(xc, yc, radius, ptr->NW);
					}
				}
			}
			else 
			{
				if (yCoord > yc+radius)
				{
					//2
					searchCityInRadius(xc, yc, radius, ptr->SE);
					searchCityInRadius(xc, yc, radius, ptr->SW);

				}
				else 
				{
					//7
					searchCityInRadius(xc, yc, radius, ptr->NE);
					searchCityInRadius(xc, yc, radius, ptr->NW);
				}
			}
		}
		else 
		{
			if (xCoord <= xc-radius)
			{
				if (yCoord > yc+radius)
				{
					//1
					searchCityInRadius(xc, yc, radius, ptr->SE);
				}
				else if (yCoord <= yc-radius)
				{
					//6
					searchCityInRadius(xc, yc, radius, ptr->NE);
				}
				else 
				{
					//4
					searchCityInRadius(xc, yc, radius, ptr->SE);
					searchCityInRadius(xc, yc, radius, ptr->NE);
				}
			}
			else 
			{
				if (yCoord > yc+radius)
				{
					//3
					searchCityInRadius(xc, yc, radius, ptr->SW);
				}
				else if (yCoord <= yc-radius)
				{
					//8
					searchCityInRadius(xc, yc, radius, ptr->NW);
				}
				else 
				{
					//5
					searchCityInRadius(xc, yc, radius, ptr->SW);
					searchCityInRadius(xc, yc, radius, ptr->NW);
				}
			}
		}
	}
}

/*****************************************************************************
This function is the destructor of the Quadtree class and it empties the tree
so that there is no memory leaks.
Input: None
Returns: None / deletion and destruction of tree
*****************************************************************************/

Quadtree::~Quadtree() 
{
	makeEmpty();
}

/*****************************************************************************
This function allows user to delete the tree.
Input: None
Returns: None / deletion of tree
*****************************************************************************/

void Quadtree::makeEmpty()
{
	makeEmpty(root);
}

/*****************************************************************************
This function empties the tree recursively.
Input: None
Returns: None / deletion of tree
*****************************************************************************/

/* Begin: code taken from 5-Trees.ppt and updated by Pelinsu Sarac */

void Quadtree::makeEmpty(QuadtreeNode * & r)
{
	if (r != NULL)
	{
		makeEmpty(r->NE);
		makeEmpty(r->NW);
		makeEmpty(r->SE);
		makeEmpty(r->SW);
		delete r;
	}
	r = NULL;
}

/* End: code taken from 5-Trees.ppt and updated by Pelinsu Sarac */

