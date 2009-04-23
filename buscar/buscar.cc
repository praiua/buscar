/*-------------------------------------------------------------------
 * 
 * Workbench for Nearest Neighbour algorithms.
 * 
 * -----------------------------------------------------------------*/
 
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include "lib_string.h"
#include "oracle.h"
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "checkInOracle.h"

using namespace std;


struct Operation {
  Operation( string _name, int _size ) {
    name = _name;
    size = _size;
    print = true;
    heading = false;
    repetitions = 1;
  }

  string name;
  int    size;
  bool   print;
  bool   heading;
  int    repetitions;
};
  


//-------------------------------------------------------------
//
void PrintUsage(string mainFileName)
{
	cout << "* Workbench for Nearest Neighbour algorithms." << endl;
	cout << "* Usage:  " << mainFileName << " [options]" << endl;
	cout << "  -b <size> [options]     Bulk initialization" << endl;
	cout << "  -i <size> [options]     Incremental initialization" << endl;
	cout << "  -t <size> [options]     Test sample size" << endl;
	cout << "       -np                No print" << endl;
	cout << "       -nh                No heading" << endl;
	cout << "       -ph                Print heading" << endl;	
	cout << "       -r <int>           Repetitions" << endl;
	cout << "  [-s <int>]              Seed" << endl;
	cout << "  [-check]                Compare with the brute force" << endl;
	cout << "  [-debug]                Debug mode" << endl;	
	cout << "  -alg <name> \"<options>\" NN algorithm" << endl; 
	cout << "  -ora <name> \"<options>\" Data generator" << endl;

	cout << "\n--- NN Algorithm options ---" << endl;
	CheckInNNAlg::ListInfo();

	cout << "--- Oracle options ---" << endl;
	CheckInOracle::ListInfo();
}



//-------------------------------------------------------------
//
void checkInputParameters( int argc, char* argv[], 
				int &seed, bool &check, bool &debug,
				string &nnAlg,
				string &nnAlgOpts,
				string &oracle,
				string &oracleOpts,
				vector<Operation> &operationList)
{
	bool selectedOneMode = false;
	
	if(argc == 1) {
		PrintUsage( argv[0] );
		exit(0);
	}	
	
	for( int i = 1; i < argc; i++ ) 
	{
		if( string( argv[i] ) == "-b" ) 
		{
			if(i < argc-1 )
			{
				operationList.push_back( Operation(string("-b"), atoi(argv[++i]) ) );
				selectedOneMode = true;
			}
			else {
				cout << "ERROR: Option '" << argv[i] << "' requires: <size>" << endl;
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-i" ) 
		{
			if(i < argc-1 )
			{
				operationList.push_back( Operation(string("-i"), atoi(argv[++i]) ) );
				selectedOneMode = true;
			}
			else {
				cout << "ERROR: Option '" << argv[i] << "' requires: <size>" << endl;
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-t" ) 
		{
			if(i < argc-1 )
			{
				operationList.push_back( Operation(string("-t"), atoi(argv[++i]) ) );
				selectedOneMode = true;
			}
			else {
				cout << "ERROR: Option '" << argv[i] << "' requires: <size>" << endl;
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-np" ) 
		{
			operationList[operationList.size()-1].print = false;
		} 
		else if( string( argv[i] ) == "-nh" ) 
		{
			operationList[operationList.size()-1].heading = false;
		} 
		else if( string( argv[i] ) == "-ph" ) 
		{
			operationList[operationList.size()-1].heading = true;
		} 
		else if( string( argv[i] ) == "-r" ) 
		{
			if(i < argc-1 )
				operationList[operationList.size()-1].repetitions = atoi(argv[++i]);
			else {
				cout << "ERROR: Option '" << argv[i] << "' requires: <repetitions>\n";
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-s" ) 
		{
			if(i < argc-1 )
				seed = atoi(argv[++i]);
			else {
				cout << "ERROR: Option '" << argv[i] << "' requires: <seed>" << endl;
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-check" ) 
		{
			check = true;
		} 
		else if( string( argv[i] ) == "-debug" ) 
		{
			debug = true;
		} 
		else if( string( argv[i] ) == "-alg" ) 
		{
			if(i < argc-2 )
			{
				nnAlg = argv[++i];
				nnAlgOpts = argv[++i];
			}
			else {
				cout << "ERROR: Option '" << argv[i];
				cout << "' requires: <NN algorithm> \"<options>\"" << endl;
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-ora" ) 
		{
			if(i < argc-2 )
			{
				oracle = argv[++i];
				oracleOpts = argv[++i];
			}
			else {
				cout << "ERROR: Option '" << argv[i];
				cout << "' requires: <Data generator> \"<options>\"" << endl;
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-h" )
		{
			PrintUsage( argv[0] );
			exit(0);
		} 
		else 
		{
			cout << "ERROR: unknown option '" << argv[i] << "'" << endl;
			exit(-1);
		}
	}

	
	if( selectedOneMode == false) {
		cout << "ERROR: You have to select at least one mode: -b|-i|-t" << endl;
		exit(-1);
	}			
	
	/*
	if( testSize == 0 ) {
		cout << "ERROR: you have to specify a test size (\"-t\" option) " << endl;
		exit(-1);
	}
	*/

	if( oracle.empty() ) {
		cout << "ERROR: you have to specify an oracle (\"-ora\" option) " << endl;
		exit(-1);
	}

	if( nnAlg.empty() ) {
		cout << "ERROR: you have to specify a NN algorithm (\"-alg\" option)" << endl;
		exit(-1);
	}

}


//-------------------------------------------------------------
//
void PrintOperation(bool heading, string operation, int size, int seed, float dist, 
					string nnAlg, string nnAlgOpts, string oracle, string oracleOpts)
{
	if( heading ) {
		printf("%6s %6s %4s %7s   %-20s   %-20s\n",
				"# Oper", "size", "Seed", "Dist", 
				"NN Algorithm", "Oracle");
	}

	printf("%6s %6d %4d ", operation.c_str(), size, seed);
	
	if(operation == "-t")
		printf("%7.2f", dist);
	else
		printf("%7d", (int)dist);
	
	printf("   %-5s %-14s", nnAlg.c_str(), nnAlgOpts.c_str() );
	printf("   %-5s %-14s\n", oracle.c_str(), oracleOpts.c_str() );
}



//-------------------------------------------------------------
// Inserting BULK (Option -b)
void OperationInsertingBulk(Operation operation, int seed, bool check, 
		NNAlg* a, NNAlg* bf, Oracle* o,
		string nnAlg, string nnAlgOpts,
		string oracle, string oracleOpts)
{
	int accDis = 0;
	int bulk = operation.size;	
	Point v[bulk];
	
	for( int i = 0; i < bulk; i++ ) 
	{
		v[i] = o->NewPoint();
	}
	
	cout << "Bulk: " << bulk << endl;
	int dis = o->NumOfDistanceComputations();
	
	a->InsertBulk(v, bulk);  // It can only be done once 
	
	dis = o->NumOfDistanceComputations() - dis;
	cout << "dis: " << dis << endl;
			
	accDis += dis;

	if( check ) {
		bf->InsertBulk(v,bulk);
	}
		
	
	if( operation.print )
		PrintOperation(operation.heading, "-b", bulk, 
				seed, accDis, nnAlg, nnAlgOpts, oracle, oracleOpts);
}


//-------------------------------------------------------------
// Inserting One by One (Option -i)
void OperationInsertingOneByOne(Operation operation, int seed, bool check, 
		NNAlg* a, NNAlg* bf, Oracle* o,
		string nnAlg, string nnAlgOpts, string oracle, string oracleOpts)
{
	int accDis = 0;
	int inc = operation.size;
	
	for( int i = 0; i < inc; i++ ) 
	{
		Point p = o->NewPoint();
		//    cout << "--- i: " << i << endl;
		int dis = o->NumOfDistanceComputations();
		
		a->Insert( p );
		
		dis = o->NumOfDistanceComputations() - dis;
		accDis += dis;
		
		if( check ) {
			bf->Insert(p);
		}
	}
	
		
	if( operation.print )
		PrintOperation(operation.heading, "-i", inc, 
				seed, accDis, nnAlg, nnAlgOpts, oracle, oracleOpts);
}



//-------------------------------------------------------------
// Testing  (Option -t)
void OperationTesting(Operation operation, int seed, bool check, 
		NNAlg* a, NNAlg* bf, Oracle* o,
		string nnAlg, string nnAlgOpts, 
		string oracle, string oracleOpts)
{
	int accDis = 0;
	int testSize = operation.size;
	
	for( int i = 0; i < testSize; i++ ) 
	{
		Point p = o->NewPoint();
		int dis = o->NumOfDistanceComputations();
		
		a->SearchNN( p );
		
		dis = o->NumOfDistanceComputations() - dis;
		accDis += dis;
		
		if(check) 
		{
			bf->SearchNN( p );
			
			if( a->GetNNDistance() != bf->GetNNDistance() ) 
			{
				cout << "ERROR: the result does not agrees with the brute force\n";
				cout << "  Iteration: " << i << endl;
				cout << "  BF point: " << bf->GetNNPoint();
				cout << "  dis: " << bf->GetNNDistance() << endl;
				cout << "  Alg (" << nnAlg << ") point: " << a->GetNNPoint();
				cout << "  Dis: " << a->GetNNDistance() << endl;
				exit(-1);
			}
		}
	}
	
	
	if( operation.print )
		PrintOperation(operation.heading, "-t", testSize, 
				seed, accDis/double(testSize), nnAlg, nnAlgOpts, oracle, oracleOpts);
}




//-------------------------------------------------------------
//
int main( int argc, char* argv[] ) 
{
	int seed = 1;
	bool check = false;
	bool debug = false;
	string nnAlg, oracle;
	string nnAlgOpts, oracleOpts;
	vector<Operation> operationList;

	
	
	checkInputParameters( 
		argc, argv, 
		seed, check, debug, 
		nnAlg, nnAlgOpts,
		oracle, oracleOpts,
		operationList );


	if(debug)
	{
		cout << "# Seed    : " << seed << endl;
		cout << "# check   : " << (check ? "true" : "false") << endl;
		cout << "# Alg     : " << nnAlg << endl;
		cout << "# AlgOpts : " << nnAlgOpts << endl;
		cout << "# Ora     : " << oracle << endl;
		cout << "# OraOpts : " << oracleOpts << endl;
	}

	srand(seed);


  	Oracle* o = CheckInOracle::Object( oracle, oracleOpts );
	if( o == 0 ) {
    	cout << "ERROR: Unknown oracle '" << oracle << "'" << endl;
    	exit(-1);
 	}

  	NNAlg* a = CheckInNNAlg::Object( nnAlg, nnAlgOpts, o );
  	if( a == 0 ) {
    	cout << "ERROR: Unknown NN algorithm '" << nnAlg << "'" << endl;
    	exit(-1);
  	}

  	NNAlg* bf = 0;
  	if( check ) {
    		bf = CheckInNNAlg::Object( "bf", "", o );
  	}



	// MAIN LOOP
	for( unsigned int opn = 0; opn < operationList.size(); opn++ ) 
	{
		for( int r = 0; r < operationList[opn].repetitions; r++ ) 
		{
			if( operationList[opn].name == "-b" ) 
			{
				// Inserting BULK
				OperationInsertingBulk(operationList[opn], seed, check,
							a, bf, o,
							nnAlg, nnAlgOpts, oracle, oracleOpts);
			} 
			
			else if( operationList[opn].name == "-i" ) 
			{
				// Inserting One by One
				OperationInsertingOneByOne(operationList[opn], seed, check,
							a, bf, o,
							nnAlg, nnAlgOpts, oracle, oracleOpts);
			} 
			
			else if( operationList[opn].name == "-t" ) 
			{
				// Testing
				OperationTesting(operationList[opn], seed, check,
							a, bf, o, 
							nnAlg, nnAlgOpts, oracle, oracleOpts);
			} 
			else 
			{
				cout << "ERROR: this shouldent happed" << endl;
				cout << "unknown option: '" << operationList[opn].name << "'" << endl;
			}
		}
	}
	
	
	delete a;
	delete bf;
	delete o;

	return 0;
}


