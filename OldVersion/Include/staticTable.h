#ifndef staticTable_h
#define staticTable_h

//
//     Class name  : staticTable  inherited from : parent
//     Description : 
//

class StaticTable : public Table{

public:

// ****************************************************************
// **     Method Name : constructor 
// **     Input       : 
// **     Output      : 
// **     Complexity  : 
// **     Description : Sets the field table to 0
// ****************************************************************
  StaticTable():_table(0){}

  StaticTable(const int i,const int j, const double d=0):
    width(i), height(j){ 
    _table = new double[i*j];
    if (!_table){
      cerr << "cannot allocate " << i*j << " doubles \n";
      abort();
    }
    Fill(d);
  } 

// ****************************************************************
// **     Method Name : Destructor 
// **     Input       : 
// **     Output      : 
// **     Complexity  : 
// **     Description : 
// ****************************************************************
virtual  ~StaticTable(){delete[] (_table);}

// ****************************************************************
// **     Method Name : Fill 
// **     Input       : 
// **     Output      : 
// **     Complexity  : 
// **     Description : 
// ****************************************************************
  virtual  void Fill(const double v){
    for (int i=0; i<width*height; ++i)  _table[i] = v;
  }
  

// ****************************************************************
// **     Method Name : Get 
// **     Input       : Two indices
// **     Output      : The element at the indice
// **     Complexity  : 
// **     Description : Reg
// ****************************************************************
 virtual double Get(const int i, const int  j) const{
   return _table[(i*height)+j];
 }

 

// ****************************************************************
// **     Method Name : Set 
// **     Input       : Two indices, and a value
// **     Output      : 
// **     Complexity  : 
// **     Description : Set the value at position of the indices
// ****************************************************************
 virtual  void Set(const int  i, const int  j, const double value){
   _table[(i*height)+j]=value;
 }

// ****************************************************************
// **     Method Name : Get_Size 
// **     Input       : 
// **     Output      : 
// **     Complexity  : 
// **     Description : returns the size of the table (in Bytes)
// ****************************************************************
virtual int Get_Size(){
  return (width*height*sizeof(double));
};
  
protected:
private:
 double *_table;
 int width; 
 int height;
};


#endif

