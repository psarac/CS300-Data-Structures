#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/******************************************************************************
Purpose: This program is a decompress-side application of Ziv-Lempel Algorithm.
		 Made for Sabanci University CS 300 Course HW-3
Author: Pelinsu Sarac
Date: 30/12/2022
Written on: MS Visual Studio 2012 
******************************************************************************/

int main()
{
	/******************************************************************************
	The part below creates a dictionary of size 4096 utilizing string pointers. 
	It initializes the dictionary with 256 single characters to prepare it 
	for decompression.
	******************************************************************************/

	vector<string *> dict(4096,nullptr);
	int code;

	for (code = 0; code < 256; code++)
	{
		dict[code] = new string(1, char(code)); 
	}

	/******************************************************************************
	The part below opens comput file to read codes and decomput to write decompressed
	version of the compout file. While reading the file;

	- If code is on the dictionary, it writes corresponding string to decompout
	and adds a new code-string tuple to dictionary such that string is
	previously decoded string + currently decoded string's first character

	- If code is not on the dictionary, it writes decomput a string such that
	previously decoded string + this string's first character and also adds this
	to the dictionary

	i.e. it decompresses the file according to Ziv-Lempel algorithm
	******************************************************************************/

	//opening files
	ifstream input("compout");
	ofstream output("decompout");
	int compCode, prevCode;

	//for the first code, we directly write the string
	input >> compCode;
	output << *dict[compCode];
	prevCode = compCode;


	while (!input.eof())
	{
		if (input >> compCode) //while we continue to receive codes
		{
			//if code is on the dictionary 
			if (dict[compCode] != nullptr)
			{
				output << *dict[compCode];

				string prev = *dict[prevCode];
				string curr = *dict[compCode];

				dict[code] = new string;
				*dict[code] = prev + curr[0];
				code++;
			}
			else //if code is not on the dictionary
			{
				string prev = *dict[prevCode];
				string outputStr = prev + prev[0];

				dict[code] = new string;
				*dict[code] = outputStr;
				code++;

				output << outputStr;
			}

			prevCode = compCode;
		}
	}

	//to deallocate dynamic memory used for dictionary
	for (int j = 0; j < 4096; j++)
	{
		if (dict[j] != nullptr)
		{
			delete dict[j];
			dict[j] = nullptr;
		}
	}

	//closing the files
	input.close();
	output.close();

	return 0;
}
	
