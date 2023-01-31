#include <iostream>
#include <fstream>
#include "HashTable.cpp"
#include <string>

using namespace std;

/******************************************************************************
Purpose: This program is an compress-side application of Ziv-Lempel Algorithm
		 Made for Sabanci University CS 300 Course HW-3
Author: Pelinsu Sarac
Date: 30/12/2022
Written on: MS Visual Studio 2012 
******************************************************************************/

int main()
{
	/******************************************************************************
	The part below initializes the hash table as dictionary, with string keys and 
	integer values (codes) with size 4096*2 so that there won't be a need for rehash 
	in this application. After construction of table, 256 single characters are 
	inserted inside the table to prepare it for the compression.
	******************************************************************************/

	HashTable<string, int> dict(-1, 4096*2);
	int code;

	for (code = 0; code < 256; code++)
	{
		dict.insert(string(1, char(code)), code);
	}

	/******************************************************************************
	The part below opens compin file to read characters from and compout file to 
	write the codes. While reading the compin file, it searches for the longest 
	prefix in the dictionary is written to compout ans prefix + next character is
	added to the dictionary with next code.
	******************************************************************************/

	char ch;
	ifstream input("compin");  //reading characters from
	ofstream output("compout"); //writing codes to 

	string prefix = "";
	input.get(ch); //initializing the prefix
	prefix += ch;

	while (!input.eof()) //eof returns true if next character is eof
	{
		int isFound = dict.find(prefix); //query the prefix inside dictionary
		if (isFound != -1) //if it's found, we may not have the longest prefix 
							//so continue with next character
		{
			bool check = input.get(ch); //get command reads next character
										//unless you are at the end of the file
			if (check)
			{ prefix += ch; }
			else { break; }
		}
		else
		{
			//find the code of longest prefix and write it to compout file
			int longestPrefix = dict.find(prefix.substr(0, prefix.length()-1));
			output << longestPrefix << " ";
			if (code < 4096) //if we have less than 4096 codes
				dict.insert(prefix, code++); //insert new code with prefix + next character
			prefix = prefix.substr(prefix.length()-1,1); //continue with uncoded part
		}
	}

	//for last string of the file
	int lastPrefix = dict.find(prefix);
	output << lastPrefix << " ";

	//close the files
	input.close();
	output.close();

	return 0;
}