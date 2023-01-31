#include <iostream>
#include <fstream> //for ifstream
#include <sstream> //for stringstream
#include <string>
#include "Quadtree.h"

using namespace std;

/******************************************************************************
Purpose: This program is a Quadtree Implementation
		 Made for Sabanci University CS 300 Course HW-1 
Author: Pelinsu Sarac
Date: 14/11/2022
Written on: MS Visual Studio 2012 
******************************************************************************/

int main() 
{
	/******************************************************************************
	The part below read the cities.txt and extract cities with their coordinations,
	then proceeds to fill the Quadtree with them.
	******************************************************************************/

	string filename = "cities.txt";
	int xMax, yMax, xCoordinate, yCoordinate;
	string cityName;

	ifstream input;
	input.open(filename.c_str());

	string line;
	getline(input, line);
	stringstream streamLine1(line);
	streamLine1 >> xMax >> yMax;

	Quadtree map; //Quadtree initialized

	while (getline(input, line))
	{
		stringstream streamLine(line);
		streamLine >> cityName >> xCoordinate >> yCoordinate; //reads city name and coordinates 
		map.insertCity(cityName, xCoordinate, yCoordinate); //insert the city into tree

	}

	map.prettyPrintTree(); //prints the tree in the specified format after insertions

	input.close();

	/******************************************************************************
	The part below read the queries.txt line by line and for each query, it prints
	out the query result as cities in the radius in the first line and all cities
	traversed through the query in the second line.
	******************************************************************************/

	string filename2 = "queries.txt";
	string line2, strXc, strYc;
	int radius, centerXc, centerYc, length;


	ifstream input2;
	input2.open(filename2.c_str());

	while (getline(input2, line2))
	{
		//reads the queries 
		stringstream streamLine(line2);
		streamLine >> strXc >> strYc >> radius;

		//getting rid of commas at the end of coordinates
		length = strXc.length();
		strXc = strXc.substr(0, length-1);
		centerXc = stoi(strXc);

		length = strYc.length();
		strYc = strYc.substr(0, length-1);
		centerYc = stoi(strYc);

		//does the query
		map.searchCityInRadius(centerXc, centerYc, radius);

	}

	input2.close();


	return 0;
}