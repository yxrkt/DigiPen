#include <iostream>
#include <vector>

struct Test
{
  Test() : pInts( new int[10] )
  {
    for ( int i = 0; i < 10; ++i )
      pInts[i] = i;
  }

  Test( const Test &rhs )
  {
    pInts = new int[10];
    memcpy( pInts, rhs.pInts, 10 * sizeof( int ) );
  }

  ~Test()
  {
    delete [] pInts;
  }

  void Print()
  {
    for ( unsigned i = 0; i < 10; ++i )
      std::cout << pInts[i] << " ";
    std::cout << std::endl;
  }

  int *pInts;
};

struct Test2 : public Test
{
};

std::vector< Test > MyTests;

void PushTests()
{
  Test t1, t2;
  Test2 t4;
  MyTests.push_back( t1 );
  MyTests.push_back( t2 );
  MyTests.push_back( t4 );
}

int main()
{
  PushTests();

  size_t nTests = MyTests.size();
  for ( size_t i = 0; i < nTests; ++i )
    MyTests[i].Print();

  return 0;
}
