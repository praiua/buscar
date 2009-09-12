//---------------------------------------------------------------
//
// Base Class for Oracles
//

#ifndef oracle_h
#define oracle_h

#include <iostream>
using namespace std;


typedef int Point;

class Oracle 
{
	public:

		Oracle(): numDistances(0) {};

		//virtual		~Oracle() = 0;
		virtual ~Oracle() {};

		virtual	Point 	NewPoint() = 0;

		virtual	void 	PrintPoint( Point p) = 0;
		
		
		double 	GetDistance( Point p1, Point p2 ) {	
					numDistances++; return CalculateDistance(p1, p2); };

		int 	NumOfDistanceComputations() { return numDistances; };
	
	
	protected:
	
		virtual double CalculateDistance( Point p1, Point p2 ) = 0;


	private:
        int numDistances;
        
};




#endif
