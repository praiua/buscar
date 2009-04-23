/* --------------------------------------------------------------------------------
*
*  Lib String
*
* ------------------------------------------------------------------------------ */


#pragma once

#include <sstream>
#include <string>
#include <string.h>
#include <cctype>
#include <vector>

using namespace std;


	vector<string> StringToVector(string str);
	
	string VectorToString(vector<string> str);
	
	void StringToLower(const char* str, char* &returnCad);
	
	void StringToUpper(const char* str, char* &returnCad);
	
	int CharToInt(char c);
	
	string IntToString(int value);
	
	bool StringToInt(string str, int& num);
	
	bool StringToFloat(string str, float& num);
	
/**
 */
 	
