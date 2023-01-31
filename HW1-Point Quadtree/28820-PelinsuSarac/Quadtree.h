#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <string>

using namespace std;

/******************************************************************************
This structure is the nodes in the Quadtree that holds city names and coordinates.
******************************************************************************/

/* Begin: code taken from 5-Trees.ppt */

struct QuadtreeNode 
{
	string cityName;
	int xCoord, yCoord;
	QuadtreeNode * NW; 
	QuadtreeNode * NE;
	QuadtreeNode * SW;
	QuadtreeNode * SE;

	QuadtreeNode(const string & cn, int xc, int yc, QuadtreeNode * nw, QuadtreeNode * ne, QuadtreeNode * sw, QuadtreeNode * se):
		cityName(cn), xCoord(xc), yCoord(yc), NW(nw), NE(ne), SW(sw), SE(se) {}
};

/* End: code taken from 5-Trees.ppt */

class Quadtree 
{
public:
	Quadtree(); //Constructor
	~Quadtree(); //Destructor
	
	void insertCity(const string & cn, int xc, int yc); //insertion
	void prettyPrintTree(); //to prettyprint the quadtree
	void makeEmpty(); //to empty the tree, so that no memory leaks occur
	void searchCityInRadius(int xc, int yc, int radius); //search cities and track them 

private:
	QuadtreeNode * root; //root of the tree
	string firstLine; //first line of the query, cities in the radius
	string secondLine; //second line of the query, cities visited during search 
	bool isFirstSearch;

	void makeEmpty(QuadtreeNode * & r); //to clean tree given the root of it
	void insertCity(const string & cn, int xc, int yc, QuadtreeNode * & ptr); //insertion with given node pointer
	void prettyPrintTree(QuadtreeNode * ptr); //to prettyprint the tree given its root
	string compareCoordinates(int xc, int yc, QuadtreeNode * ptr); //to be able to compare coordinates and find quadrant to insert
	void searchCityInRadius(int xc, int yc, int radius, QuadtreeNode * ptr); //search cities in a radius from a given coordinate
};

#endif