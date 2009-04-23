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
				vector<string> &nnAlgOpts,
				vector<string> &oracleOpts,
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
			if(i < argc-1 && argv[i+1][0] != '-')
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
			if(i < argc-1 && argv[i+1][0] != '-')
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
			if(i < argc-1 && argv[i+1][0] != '-')
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
			if(i < argc-1 && argv[i+1][0] != '-')
				operationList[operationList.size()-1].repetitions = atoi(argv[++i]);
			else {
				cout << "ERROR: Option '" << argv[i] << "' requires: <repetitions>\n";
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-s" ) 
		{
			if(i < argc-1 && argv[i+1][0] != '-')
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
			if(i < argc-2 && argv[i+1][0] != '-')
			{
				nnAlgOpts.push_back( argv[++i] );
				vector<string> aux = StringToVector( argv[++i] );
				nnAlgOpts.insert( nnAlgOpts.end(), aux.begin(), aux.end() );
			}
			else {
				cout << "ERROR: Option '" << argv[i];
				cout << "' requires: <NN algorithm> \"<options>\"" << endl;
				exit(-1);
			}
		} 
		else if( string( argv[i] ) == "-ora" ) 
		{
			if(i < argc-2 && argv[i+1][0] != '-')
			{
				oracleOpts.push_back( argv[++i] );
				vector<string> aux = StringToVector( argv[++i] );
				oracleOpts.insert( oracleOpts.end(), aux.begin(), aux.end() );
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

	if( oracleOpts.empty() ) {
		cout << "ERROR: you have to specify an oracle (\"-ora\" option) " << endl;
		exit(-1);
	}

	if( nnAlgOpts.empty() ) {
		cout << "ERROR: you have to specify a NN algorithm (\"-alg\" option)" << endl;
		exit(-1);
	}

}


//-------------------------------------------------------------
//
void PrintOperation(bool heading, string operation, int size, int seed, float dist, 
					vector<string> algOpts, vector<string> oraOpts)
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
	
	printf("   %-20s   %-20s\n",			
		VectorToString( algOpts ).c_str(), VectorToString ( oraOpts ).c_str() );
}



//-------------------------------------------------------------
// Inserting BULK (Option -b)
void OperationInsertingBulk(Operation operation, int seed, bool check, 
		NNAlg* nnAlg, NNAlg* nnAlgBF, Oracle* oracle,
		vector<string> nnAlgOpts, vector<string> oracleOpts)
{
	int accDis = 0;
	int bulk = operation.size;	
	Point v[bulk];
	
	for( int i = 0; i < bulk; i++ ) 
	{
		v[i] = oracle->NewPoint();
	}
	
	int dis = oracle->NumOfDistanceComputations();
	
	nnAlg->InsertBulk(v, bulk);  // It can only be done once 
	
	dis = oracle->NumOfDistanceComputations() - dis;
			
	accDis += dis;

	if( check ) {
		nnAlgBF->InsertBulk(v,bulk);
	}
		
	
	if( operation.print )
		PrintOperation(operation.heading, "-b", bulk, 
				seed, accDis, nnAlgOpts, oracleOpts);
}


//-------------------------------------------------------------
// Inserting One by One (Option -i)
void OperationInsertingOneByOne(Operation operation, int seed, bool check, 
		NNAlg* nnAlg, NNAlg* nnAlgBF, Oracle* oracle,
		vector<string> nnAlgOpts, vector<string> oracleOpts)
{
	int accDis = 0;
	int inc = operation.size;
	
	for( int i = 0; i < inc; i++ ) 
	{
		Point p = oracle->NewPoint();
		//    cout << "--- i: " << i << endl;
		int dis = oracle->NumOfDistanceComputations();
		
		nnAlg->Insert( p );
		
		dis = oracle->NumOfDistanceComputations() - dis;
		accDis += dis;
		
		if( check ) {
			nnAlgBF->Insert(p);
		}
	}
	
		
	if( operation.print )
		PrintOperation(operation.heading, "-i", inc, 
				seed, accDis, nnAlgOpts, oracleOpts);
}



//-------------------------------------------------------------
// Testing  (Option -t)
void OperationTesting(Operation operation, int seed, bool check, 
		NNAlg* nnAlg, NNAlg* nnAlgBF, Oracle* oracle,
		vector<string> nnAlgOpts, vector<string> oracleOpts)
{
	int accDis = 0;
	int testSize = operation.size;
	
	for( int i = 0; i < testSize; i++ ) 
	{
		Point p = oracle->NewPoint();
		int dis = oracle->NumOfDistanceComputations();
		
		//nnAlg->Insert( p );
		nnAlg->SearchNN( p );
		
		dis = oracle->NumOfDistanceComputations() - dis;
		accDis += dis;
		
		if(check) 
		{
			nnAlgBF->SearchNN( p );
			
			if( nnAlg->GetNNDistance() != nnAlgBF->GetNNDistance() ) 
			{
				cout << "ERROR: the result does not agrees with the brute force\n";
				cout << "  Iteration: " << i << endl;
				cout << "  BF point: " << nnAlgBF->GetNNPoint();
				cout << "  dis: " << nnAlgBF->GetNNDistance() << endl;
				cout << "  Alg (" << nnAlgOpts[0] << ") point: " << nnAlg->GetNNPoint();
				cout << "  Dis: " << nnAlg->GetNNDistance() << endl;
				exit(-1);
			}
		}
	}
	
	
	if( operation.print )
		PrintOperation(operation.heading, "-t", testSize, 
				seed, accDis/double(testSize), nnAlgOpts, oracleOpts);
}




//-------------------------------------------------------------
//
int main( int argc, char* argv[] ) 
{
	int seed = 1;
	bool check = false;
	bool debug = false;
	vector<string> nnAlgOpts, oracleOpts;
	vector<Operation> operationList;

	
	
	checkInputParameters( argc, argv, 
						  seed, check, debug, 
						  nnAlgOpts, oracleOpts,
						  operationList);


	if(debug)
	{
		cout << "# Seed : " << seed << endl;
		cout << "# check: " << (check ? "true" : "false") << endl;
		cout << "# Alg  : " << VectorToString( nnAlgOpts ) << endl;
		cout << "# Ora  : " << VectorToString( oracleOpts ) << endl;
	}

	srand(seed);


  	Oracle* oracle = CheckInOracle::Object( oracleOpts );
	if( oracle == 0 ) {
    	cout << "ERROR: Unknown oracle '" << oracleOpts[0] << "'" << endl;
    	exit(-1);
 	}

  	NNAlg* nnAlg = CheckInNNAlg::Object( nnAlgOpts, oracle );
  	if( nnAlg == 0 ) {
    	cout << "ERROR: Unknown NN algorithm '" << nnAlgOpts[0] << "'" << endl;
    	exit(-1);
  	}

  	NNAlg* nnAlgBF = 0;
  	if( check ) {
  		vector<string> nnAlgBFOpts;
  		nnAlgBFOpts.push_back("bf");
    	nnAlgBF = CheckInNNAlg::Object( nnAlgBFOpts, oracle );
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
							nnAlg, nnAlgBF, oracle, 
							nnAlgOpts, oracleOpts);
			} 
			
			else if( operationList[opn].name == "-i" ) 
			{
				// Inserting One by One
				OperationInsertingOneByOne(operationList[opn], seed, check,
							nnAlg, nnAlgBF, oracle,
							nnAlgOpts, oracleOpts);
			} 
			
			else if( operationList[opn].name == "-t" ) 
			{
				// Testing
				OperationTesting(operationList[opn], seed, check,
							nnAlg, nnAlgBF, oracle,
							nnAlgOpts, oracleOpts);
			} 
			else 
			{
				cout << "ERROR: this shouldent happed" << endl;
				cout << "unknown option: '" << operationList[opn].name << "'" << endl;
			}
		}
	}
	
	
	delete nnAlg;
	delete nnAlgBF;
	delete oracle;

	return 0;
}


