#include "bn.h"
#include <ctime>
#include <string>

void test0()
{
	BayesianNetwork bn(5);

	//from the book B,E,A,J,M (Burglary,Earthquake,Alarm,Mary,John)
	//              0,1,2,3,4

	//this user-ordering is the same as native (see comment in generate.data.cpp)
	enum {B,E,A,J,M}; //same as native ordering

	bn.AddLink(B,A); //B becomes the first parent of A
	bn.AddLink(E,A); //E becomes the second parent of A
	bn.AddLink(A,J);
	bn.AddLink(A,M);

	bn.AddCPT(B, 0.25);
	bn.AddCPT(E, 0.1);

	//           ~B ~E  ~B E   B ~E   B E (using the order in which added)
	bn.AddCPT(A, 0.1,   0.9,   0.8,   0.95);

	//             ~A          A
	bn.AddCPT(J,   0.1,        0.5);

	//             ~A          A
	bn.AddCPT(M,   0.2,        0.9);

	bn.SeedRNG( static_cast<unsigned int>(std::time(0)));

	//this will generate a 10000 samples from the joint distribution
	//this is exactly the kind of data I provide for the assignment
	//the data will be used to create/train Bayesian Network
	bn.GenerateData(100000,"data");

	TrainingData td("data");
	std::string p1("---1-|-----"); // P(john|alarm)
	std::cout << "P(" <<p1<< ") = " << td.EstimateProbability(p1) << std::endl;
	std::string p2("---1-|0----"); //P(alarm|burglary,~earthquake)
	std::cout << "P(" <<p2<< ") = " << td.EstimateProbability(p2) << std::endl;
	std::string p3="---1-|1----"; 
	std::cout << "P(" <<p3<< ") = " << td.EstimateProbability(p3) << std::endl;
	            p2="---1-|-0---"; //P(alarm|burglary,~earthquake)
	std::cout << "P(" <<p2<< ") = " << td.EstimateProbability(p2) << std::endl;
	            p3="---1-|-1---"; 
	std::cout << "P(" <<p3<< ") = " << td.EstimateProbability(p3) << std::endl;
	            p2="---1-|--0--"; //P(alarm|burglary,~earthquake)
	std::cout << "P(" <<p2<< ") = " << td.EstimateProbability(p2) << std::endl;
	            p3="---1-|--1--"; 
	std::cout << "P(" <<p3<< ") = " << td.EstimateProbability(p3) << std::endl;
	std::string p4("---1-|00---"); 
	std::cout << "P(" <<p4<< ") = " << td.EstimateProbability(p4) << std::endl;
	std::string p5("---1-|01---"); 
	std::cout << "P(" <<p5<< ") = " << td.EstimateProbability(p5) << std::endl;
	std::string p6("---1-|10---"); 
	std::cout << "P(" <<p6<< ") = " << td.EstimateProbability(p6) << std::endl;
	std::string p7("---1-|11---"); 
	std::cout << "P(" <<p7<< ") = " << td.EstimateProbability(p7) << std::endl;
	std::string p4_1("---1-|-00--"); 
	std::cout << "P(" <<p4_1<< ") = " << td.EstimateProbability(p4_1) << std::endl;
	std::string p5_1("---1-|-01--"); 
	std::cout << "P(" <<p5_1<< ") = " << td.EstimateProbability(p5_1) << std::endl;
	std::string p6_1("---1-|-10--"); 
	std::cout << "P(" <<p6_1<< ") = " << td.EstimateProbability(p6_1) << std::endl;
	std::string p7_1("---1-|-11--"); 
	std::cout << "P(" <<p7_1<< ") = " << td.EstimateProbability(p7_1) << std::endl;
	std::string p4_2("---1-|0-0--"); 
	std::cout << "P(" <<p4_2<< ") = " << td.EstimateProbability(p4_2) << std::endl;
	std::string p5_2("---1-|0-1--"); 
	std::cout << "P(" <<p5_2<< ") = " << td.EstimateProbability(p5_2) << std::endl;
	std::string p6_2("---1-|1-0--"); 
	std::cout << "P(" <<p6_2<< ") = " << td.EstimateProbability(p6_2) << std::endl;
	std::string p7_2("---1-|1-1--"); 
	std::cout << "P(" <<p7_2<< ") = " << td.EstimateProbability(p7_2) << std::endl;
	std::string p8("---1-|000--"); 
	std::cout << "P(" <<p8<< ") = " << td.EstimateProbability(p8) << std::endl;
	std::string p9("---1-|001--"); 
	std::cout << "P(" <<p9<< ") = " << td.EstimateProbability(p9) << std::endl;
	std::string p10("---1-|010--"); 
	std::cout << "P(" <<p10<< ") = " << td.EstimateProbability(p10) << std::endl;
	std::string p11("---1-|011--"); 
	std::cout << "P(" <<p11<< ") = " << td.EstimateProbability(p11) << std::endl;
	std::string p12("---1-|100--"); 
	std::cout << "P(" <<p12<< ") = " << td.EstimateProbability(p12) << std::endl;
	std::string p13("---1-|101--"); 
	std::cout << "P(" <<p13<< ") = " << td.EstimateProbability(p13) << std::endl;
	std::string p14("---1-|110--"); 
	std::cout << "P(" <<p14<< ") = " << td.EstimateProbability(p14) << std::endl;
	std::string p15("---1-|111--"); 
	std::cout << "P(" <<p15<< ") = " << td.EstimateProbability(p15) << std::endl;

  bn.Print();
}

void test1()
{
  std::cout << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << " Test 1: Estimating Probabilities  " << std::endl;
  std::cout << "===================================" << std::endl;

	BayesianNetwork bn( 5 );
	//this is not a native ordering (notice that A has parents B and E which
	//are ordered after A. GenerateData(...) will fix that)
	//to see internal ordering uncomment DEBUG in command line (see Makefile)
	enum { A, J, M, B, E }; //non-native, but still works, see nativeOrdering

	bn.AddLink( B, A ); //B becomes the first parent of A
	bn.AddLink( E, A ); //E becomes the second parent of A
	bn.AddLink( A, J );
	bn.AddLink( A, M );

	bn.AddCPT( B, 0.25 );
	bn.AddCPT( E, 0.10 );
	bn.AddCPT( A, 0.10, 0.90, 0.80, 0.95 );
	bn.AddCPT( J, 0.10, 0.50 );
	bn.AddCPT( M, 0.20, 0.90 );

	bn.SeedRNG( static_cast<unsigned int>( std::time( 0 ) ) );

	bn.GenerateData( 10000, "data2" );

	TrainingData td( "data2" );

	std::string p1( "-1---|1----" );
	std::cout << "P(" <<p1<< ") = " << td.EstimateProbability(p1) << std::endl;

	std::string p2( "---1-|-----" );
	std::cout << "P(" <<p2<< ") = " << td.EstimateProbability(p2) << std::endl;

	std::string p3( "1----|---10" );
	std::cout << "P(" <<p3<< ") = " << td.EstimateProbability(p3) << std::endl;

  std::string p4( "-1---|--100" );
	std::cout << "P(" <<p4<< ") = " << td.EstimateProbability(p4) << std::endl;

 // std::cout << std::endl;
 // std::string p5("1----|---1-");
	//std::cout << "P(" << p5 << ") = " << td.EstimateProbability( p5 ) << std::endl;
 // std::string p6("1--1-|---1-");
	//std::cout << "P(" << p6 << ") = " << td.EstimateProbability( p6 ) << std::endl;
 // std::string p7("1--1-|-----");
	//std::cout << "P(" << p7 << ") = " << td.EstimateProbability( p7 ) << std::endl;
 // std::cout << std::endl;

  //bn.Print();
}

void test2()
{
  std::cout << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << " Test 2: Exact Inference           " << std::endl;
  std::cout << "===================================" << std::endl;

	BayesianNetwork bn( 5 );
	enum { A, J, M, B, E };

	bn.AddLink( B, A );
	bn.AddLink( E, A );
	bn.AddLink( A, J );
	bn.AddLink( A, M );

	bn.AddCPT( B, 0.25 );
	bn.AddCPT( E, 0.10 );
	bn.AddCPT( A, 0.10, 0.90, 0.80, 0.95 );
	bn.AddCPT( J, 0.10, 0.50 );
	bn.AddCPT( M, 0.20, 0.90 );

	//this is the first part of the assignment - exact inference in BN
	std::string p1( "-1---|1----" ); 
  std::cout << "P(" << p1 << ") = " << bn.ExactInference( p1 ) << std::endl;

  std::string p2( "---1-|-----" );
  std::cout << "P(" << p2 << ") = " << bn.ExactInference( p2 ) << std::endl;

  std::string p3( "1----|---10" );
  std::cout << "P(" << p3 << ") = " << bn.ExactInference( p3 ) << std::endl;

  std::string p4( "-1---|--100" );
  std::cout << "P(" << p4 << ") = " << bn.ExactInference( p4 ) << std::endl;
}

void test3()
{
  std::cout << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << " Test 3: Creating Bayesian network " << std::endl;
  std::cout << "===================================" << std::endl;
	//this is the learning part of the assignment - learn BN from data
	TrainingData td("data2");
	BayesianNetwork my_bn = td.Learn();

	std::string p1("-1---|1----"); 
	std::cout << "P(" <<p1<< ") = " << my_bn.ExactInference(p1) << std::endl;
	std::string p2("---1-|-----");
	std::cout << "P(" <<p2<< ") = " << my_bn.ExactInference(p2) << std::endl;
	std::string p3("1----|---10");
	std::cout << "P(" <<p3<< ") = " << my_bn.ExactInference(p3) << std::endl;
	std::string p4("-1---|--1--");
	std::cout << "P(" <<p4<< ") = " << my_bn.ExactInference(p4) << std::endl;
}


int main()
{
	//test0();
	//test1();
  test2();
  //test3();
}
