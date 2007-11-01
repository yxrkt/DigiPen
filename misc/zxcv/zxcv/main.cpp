#include <iostream>
#include <iomanip>

int main()
{
  float texels[4][3];

  for ( int i = 0; i < 4; ++i )
    std::cout << texels[i]  << " ";
  std::cout << std::endl;
}

//#include <iostream>
//#include <vector>
//
//struct Test
//{
//  Test( int size ) : pInts( new int[size] ), nSize( size )
//  {
//    for ( int i = 0; i < size; ++i )
//      pInts[i] = i + 1;
//  }
//
//  //Test( const Test &rhs )
//  //{
//  //  pInts = new int[10];
//  //  memcpy( pInts, rhs.pInts, 10 * sizeof( int ) );
//  //  nSize = rhs.nSize;
//  //}
//
//  ~Test()
//  {
//    delete [] pInts;
//  }
//
//  void Print()
//  {
//    for ( int i = 0; i < nSize; ++i )
//      std::cout << pInts[i] << " ";
//    std::cout << std::endl;
//  }
//
//  int *pInts;
//  int nSize;
//};
//
//struct Test2 : Test
//{
//  Test2( int size ) : Test( size )
//  {
//  }
//
//  Test2( const Test &rhs )
//  {
//    pInts = new int[10];
//    memcpy( pInts, rhs.pInts, 10 * sizeof( int ) );
//    nSize = rhs.nSize;
//  }
//};
//
//std::vector< Test > MyTests;
//
//void PushTests()
//{
//  Test2 t1(10), t2(10);
//  MyTests.push_back( t1 );
//  MyTests.push_back( t2 );
//}
//
//int main()
//{
//  PushTests();
//
//  size_t nTests = MyTests.size();
//  for ( size_t i = 0; i < nTests; ++i )
//    MyTests[i].Print();
//
//  return 0;
//}
