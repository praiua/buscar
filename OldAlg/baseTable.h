#ifndef baseTable_h
#define baseTable_h

//
//     Class name  : baseTable
//     Description : Base class for the table
//

class Table{

public:

// ****************************************************************
// **     Method Name : constructor 
// **     Input       : 
// **     Output      : 
// **     Complexity  : 
// **     Description : Does nothing
// ****************************************************************
  Table(){};

// ****************************************************************
// **     Method Name : Destructor 
// **     Input       : 
// **     Output      : 
// **     Complexity  : 
// **     Description : Free  allocated space 
// ****************************************************************
  virtual ~Table(){};

// ****************************************************************
// **     Method Name : Fill 
// **     Input       : A double d
// **     Output      : 
// **     Complexity  : 
// **     Description : Fill the table with d, default 0 
// ****************************************************************
virtual  void Fill(const double=0) =0;


// ****************************************************************
// **     Method Name : Get 
// **     Input       : Two indices
// **     Output      : The element at the indice
// **     Complexity  : 
// **     Description : Get the element at the indices 
// ****************************************************************
virtual   double Get(const int, const int)  const =0;
 

// ****************************************************************
// **     Method Name : Set 
// **     Input       : Two indices, and a value
// **     Output      : 
// **     Complexity  : 
// **     Description : Set the value at position of the indices
// ****************************************************************
virtual  void Set(const int , const int, const double) =0;
 
// ****************************************************************
// **     Method Name : Get_Size 
// **     Input       : 
// **     Output      : 
// **     Complexity  : 
// **     Description : returns the size of the table (in Bytes)
// ****************************************************************
virtual int Get_Size()=0;

protected:
private:

};


#endif
