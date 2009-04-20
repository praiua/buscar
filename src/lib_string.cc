/* --------------------------------------------------------------------------------
*
*  Lib String
*
* ------------------------------------------------------------------------------ */

#include "lib_string.h"




//-------------------------------------------------------------
// 
vector<string> StringToVector(string str)
{
	string buf;
	stringstream ss(str);
	vector<string> tokens;
	while (ss >> buf)      // Separator: scape
	   tokens.push_back(buf);
	return tokens;
}


//-----------------------------------------------------------------------
//
string VectorToString(vector<string> str)
{
	string result = "";
	
	for( vector<string>::iterator it = str.begin();	it != str.end(); it++ )
	{
		result += *it;
		if(it != str.end()-1)
			result += " ";
	}
		
	return result;
}


//-----------------------------------------------------------------------
//
void StringToUpper(const char* str, char* &returnStr)
{
	int len = strlen(str);
	returnStr = new char[len];	
	strcpy(returnStr, str);
	
	for(int i=0; i<len; i++)
		returnStr[i] = toupper(str[i]);		
}



//-----------------------------------------------------------------------
// 
void StringToLower(const char* str, char* &returnStr)
{
	int len = strlen(str);
	returnStr = new char[len];	
	strcpy(returnStr, str);
	
	for(int i=0; i<len; i++)
		returnStr[i] = tolower(str[i]);
}


//-----------------------------------------------------------------------
// 
int CharToInt(char c)
{
	if ((c >= '0') && (c <= '9'))
		return c - '0';
	
	return -1;
}


//-----------------------------------------------------------------------
// 
string IntToString(int value) 
{
	ostringstream out;
	out << value;
	return out.str();
}


//-----------------------------------------------------------------------
//
bool StringToInt(string str, int& num)
{
  istringstream iss(str);
  return (iss >> num);
}


//-----------------------------------------------------------------------
//
bool StringToFloat(string str, float& num)
{
  istringstream iss(str);
  return (iss >> num);
}



/**
*/

