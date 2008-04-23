#include "bn.h"
#include <cassert>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

////////////////////////////////////////////////////////////
BayesianNetwork::BayesianNetwork(unsigned int size) : 
	BN_size(size),
	topologyDone(false),
	nativeOrdering(size),
	parents(size),
	CPTs(size)
{
	for (unsigned int j=0;j<BN_size;++j) { nativeOrdering[j]=j; }
}

////////////////////////////////////////////////////////////
//creates parent relation
void BayesianNetwork::AddLink(unsigned int parent, unsigned int child) {
	//quit if attempting to modify topology after some CPT have been specified
	//CPT size depends on the number of parents!
	assert (!topologyDone); 
	parents[ child ].push_back ( parent ); //absolutely need this
}

////////////////////////////////////////////////////////////
//IMPORTANT
//cpt should be ordered from all FALSE to all TRUE, using standard
//ordering of binary numbers. 
//Example:
//a conditional probability table for a node with 3 parents is
//0 0 0  P0
//0 0 1  P1
//0 1 0  P2
//0 1 1  P3
//1 0 0  P4
//1 0 1  P5
//1 1 0  P6
//1 1 1  P7
//so cpt = std::vector{P0,P1,P2,P3,P4,P5,P6,P7}
//notice that indicies are exactly the decimal values corresponding
//to binary number made of parents' values.
//IMPORTANT: cpt.size()=2^<number of parents>
//cpt = vector of probabilities
void BayesianNetwork::AddCPT(unsigned int nodeID, std::vector<double> cpt) {

	topologyDone=true; //do not allow topology modifications
	//after the first CPT is assigned
	unsigned int one = 1;
	if (cpt.size() != (one<<parents[nodeID].size()) ) {
		std::cerr << "number of parents " << parents[nodeID].size()
			<< "; while CPT size is " << cpt.size() << std::endl;
		assert(false);
	}
	CPTs[nodeID] = cpt;
}
////////////////////////////////////////////////////////////
//a couple overloads for convenience
void BayesianNetwork::AddCPT(unsigned int nodeID, double p) {
	topologyDone=true; //do not allow topology modifications
	//after the first CPT is assigned
	assert ( 0 == parents[nodeID].size());
	std::vector<double> cpt(1);
	cpt[0] = p;
	CPTs[nodeID] = cpt;
}
////////////////////////////////////////////////////////////
void BayesianNetwork::AddCPT(unsigned int nodeID, double p0, double p1) {
	topologyDone=true; //do not allow topology modifications
	//after the first CPT is assigned
	assert ( 1 == parents[nodeID].size());
	std::vector<double> cpt(2);
	cpt[0] = p0;
	cpt[1] = p1;
	CPTs[nodeID] = cpt;
}
////////////////////////////////////////////////////////////
void BayesianNetwork::AddCPT(unsigned int nodeID, double p0, double p1, double p2, double p3) {
	topologyDone=true; //do not allow topology modifications 
	//after the first CPT is assigned 
	assert ( 2 == parents[nodeID].size());
	std::vector<double> cpt(4);
	cpt[0] = p0;
	cpt[1] = p1;
	cpt[2] = p2;
	cpt[3] = p3;
	CPTs[nodeID] = cpt;
}

////////////////////////////////////////////////////////////
void BayesianNetwork::Print () const {
	std::cout << "Number of nodes: " << BN_size << std::endl;
	for (unsigned i=0; i<BN_size;++i) {
		std::cout << "Node " << i << "\nparents:  ";
		for (unsigned int j=0;j<parents[i].size();++j) {
			std::cout << parents[i][j] << " ";
		}
		std::cout << std::endl;
		for (unsigned int j=0;j<CPTs[i].size();++j) {
			std::cout << CPTs[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

////////////////////////////////////////////////////////////
class ParentOrder {
	public:
		ParentOrder(const std::vector<std::set<unsigned int> > & p) : predecessors(p) {}
		bool operator () (unsigned int x, unsigned int y) {
			return 
				std::find(predecessors[y].begin(),predecessors[y].end(),x)
				!= predecessors[y].end();
		}
	private:
		const std::vector<std::set<unsigned int> > & predecessors;
};

////////////////////////////////////////////////////////////
//returns true if there are no loops, also initializes nativeOrdering 
//returns false if there are loops
bool BayesianNetwork::NativeOrder() {
	//parendts, grandparents, etc.
	std::vector<std::set<unsigned int> > predecessors(BN_size);
	//initialize with parents
	for (unsigned int i=0;i<BN_size;++i) {
		predecessors[i].insert( parents[i].begin(), parents[i].end() );
	}

	//iterate by adding predecessors of parents, till there are no changes - then stop
	bool go = true;
	while (go) {
		//to keep track of changes
		size_t predecessors_size = 0;
		for (size_t i=0;i<BN_size;++i) {
			predecessors_size += predecessors[i].size();
		}
		for (unsigned int i=0;i<BN_size;++i) {
			for (unsigned int j=0;j<parents[i].size();++j) {
				predecessors[i].insert(
						predecessors[parents[i][j]].begin(),
						predecessors[parents[i][j]].end()
						);
			}
		}
		//to keep track of changes
		size_t new_predecessors_size = 0;
		for (size_t i=0;i<BN_size;++i) {
			new_predecessors_size += predecessors[i].size();
		}
		go = (new_predecessors_size!=predecessors_size);
	}

	//check for loops (node is it's own predecessor)
	bool no_loops=true;
	for (unsigned int i=0;i<BN_size;++i) {
		no_loops = ( predecessors[i].find(i)== predecessors[i].end());
		if (!no_loops) break;
	}

	if (no_loops) {
		//"native" bayesian order
		std::stable_sort(
				nativeOrdering.begin(),
				nativeOrdering.end(),
				ParentOrder(predecessors)
				);
	}
	return no_loops;
}

////////////////////////////////////////////////////////////
struct InsertIntoVector
{
	InsertIntoVector( TrainingData & _td ) : td(_td) {}
	void operator () (const std::vector<bool> & record) const {
		td.AddRecord( record );
	};
	private:
	TrainingData & td;
};

void BayesianNetwork::ParseQuery( const std::string &query, Statement &lhs, Statement &rhs )
{
  lhs = ParseSide( std::string( &query[0], BN_size ) );
  rhs = ParseSide( std::string( &query[BN_size + 1], BN_size ) );
}

BayesianNetwork::Statement BayesianNetwork::ParseSide( const std::string &in )
{
  Statement out;

	for ( unsigned i = 0; i < BN_size; ++i )
  {
		switch ( in[i] )
    {
			case '0':
				out.push_back( 0 );
				break;
			case '1':
				out.push_back( 1 );
				break;
			case '-':
				out.push_back( 2 );
				break;
		}
	}

  return out;
}

////////////////////////////////////////////////////////////
void BayesianNetwork::GenerateData( unsigned int datasize, TrainingData &td )
{
	GenerateData_aux( datasize, InsertIntoVector( td ) );
}

////////////////////////////////////////////////////////////
struct InsertIntoFile {
	InsertIntoFile( std::ofstream & _fout) : fout(_fout) {}
	void operator () (const std::vector<bool> & record) const {
		//print new assignment
		size_t size = record.size();
		for (size_t j=0;j<size;++j) {
#ifdef DEBUG
			fout << ((record[j]==1)?"1":"0") << " ";
#else
			fout << record[j] << " ";
#endif
		}
		fout << std::endl;
	};
	private:
	std::ofstream & fout;
};

////////////////////////////////////////////////////////////
void BayesianNetwork::GenerateData( unsigned int datasize, std::string filename )  {
	std::ofstream fout(filename.c_str());

	//print the number of random variable
	fout << BN_size << std::endl;

	GenerateData_aux( datasize, InsertIntoFile(fout) );
	fout.close();
}

////////////////////////////////////////////////////////////
template <typename T>
void BayesianNetwork::GenerateData_aux( unsigned int datasize, T inserter )
{
	if (!NativeOrder()) {
		std::cerr << "there are directed cycles in the network - quitting\n";
		assert(false);
	}

	//print current nativeOrdering
#ifdef DEBUG
	for (unsigned int j=0;j<BN_size;++j) { 
		std::cout << nativeOrdering[j] << " -> "; 
	}
	std::cout << std::endl;
#endif


	for (unsigned int i=0;i<datasize;++i) {
		//this is an assignment - a single line in out dataset
#ifdef DEBUG
		std::vector<char> assignment(BN_size,2); //char, so that we can use 2 
		//(i.e. unassigned) for debugging
#else
		std::vector<bool> assignment(BN_size,false); //bit_vector - save memory
#endif

		//move through nodes in nativeOrdering 
		//assigning values according to CPTs and parent values
		for (unsigned int j=0;j<BN_size;++j) {
			unsigned int nodeID = nativeOrdering[j];

#ifdef DEBUG3
			fout << "assigning " << nodeID << " based on values of ";
#endif
			//get parents and theirs current values
			std::vector<bool> parent_values;
			for (unsigned int k=0;
					k < parents[nodeID].size();
					++k) {

#ifdef DEBUG3
				fout << parents[nodeID][k] << "(" 
					<< ((assignment[parents[nodeID][k]]==1)?"1":"0") << ")  ";
#endif

#ifdef DEBUG
				if (assignment[parents[nodeID][k]]==2) {
					std::cerr << "parent " << parents[nodeID][k] << " wasn't assigned a value - very-very wrong\n";
					assert(false);
				}
				parent_values.push_back( assignment[parents[nodeID][k]]==1 );
#else
				parent_values.push_back( assignment[parents[nodeID][k]] );
#endif
			}
#ifdef DEBUG3
			fout << std::endl;
#endif


			//look up probability in the corresponding CPT, and use it
			//to generate a random value for the current node
			std::reverse(parent_values.begin(),parent_values.end());
			double probability = CPTs[nodeID][ bin2dec(parent_values)];
			assignment[nodeID] = RandomBool( probability );
		}
		inserter( assignment );
	}
}

double BayesianNetwork::ExactInference( const std::string &query )
{
  Statement lhs, rhs;
  ParseQuery( query, lhs, rhs );

  assert( lhs.size() == BN_size );

  for ( unsigned i = 0; i < BN_size; ++i )
  {
    if ( lhs[i] == 0 )
      return not( P( i, rhs ) );
    else if ( lhs[i] == 1 )
      return P( i, rhs );
  }

  return 1.0;
}

double BayesianNetwork::P( unsigned e, const Statement &given )
{
  if ( given[e] != 2 )
    return (double)given[e];

  if ( parents[e].empty() )
    return CPTs[e][0];

  double sum = 0.0;

  size_t nParents = parents[e].size();
  std::vector<double> p( parents[e].size() );
  for ( size_t i = 0; i < nParents; ++i )
    p[i] = P( parents[e][i], given );

  size_t nStates = CPTs[e].size();
  for ( size_t i = 0; i < nStates; ++i )
  {
    double totalP = 1.0;
    for ( size_t j = 0; j < nParents; ++j )
      totalP *= ( ( i & ( nParents - j ) ) ? p[j] : not( p[j] ) );
    sum += CPTs[e][i] * totalP;
  }

  for ( unsigned i = 0; i < BN_size; ++i )
  {
    if ( given[i] == 2 ) continue;
    if ( std::find( parents[i].begin(), parents[i].end(), e ) == parents[i].end() ) continue;
    Statement next( given );
    next[i] = 2;
    if ( given[i] == 0 )
      sum = not( P( i, e ) ) * sum / not( P( i, next ) );
    else
      sum = P( i, e ) * sum / P( i, next );
  }

  return sum;
}

double BayesianNetwork::P( unsigned e, unsigned given )
{
  Statement state( BN_size, 2 );
  state[given] = 1;
  return P( e, state );
}

////////////////////////////////////////////////////////////
//auxiliary functions

////////////////////////////////////////////////////////////
//return boolean with P(true)=probability
bool BayesianNetwork::RandomBool( double probability ) const {
	return
		static_cast<double>(std::rand()%1000)
		< probability*static_cast<double>(1000);
}

////////////////////////////////////////////////////////////
//binary to decimal conversion
//vector {0,1,1,1,0,1} -> 1+4+8+16 = 29
unsigned int BayesianNetwork::bin2dec(const std::vector<bool> & b) const {
	unsigned int result=0;
	for (unsigned int i=0; i < b.size(); ++i) {
		if (b[i]) {
			result |= 1<<i;
		}
	}
	/*
		 unsigned int power_of_2=1;
		 for(std::vector<bool>::const_reverse_iterator ri = b.rbegin();
		 ri!=b.rend();
		 ++ri) {
		 result += *ri * power_of_2;
		 power_of_2*=2;
		 }*/
	return result;
}

////////////////////////////////////////////////////////////
TrainingData::TrainingData(unsigned int size) : BN_size(size) {
}

////////////////////////////////////////////////////////////
void TrainingData::AddRecord(const std::vector<bool> & record) {
	dataset.push_back( record );
}

////////////////////////////////////////////////////////////
TrainingData::TrainingData( std::string filename) {
	std::fstream in(filename.c_str());
	if (!in.good()) {
		std::cout << "cannot open " << filename << "\n";
	}

	if ( !in.eof() ) { 
		in >> BN_size; 
		//std::cout << "BN_size = " << Size() << std::endl;
	}

	//remove new line
	std::string line;
	std::getline(in,line);

	while ( !in.eof() ) {
		std::vector<bool> record(BN_size);
		std::string line;
		std::getline(in,line);
		std::istringstream iss( line ); 

		if (line.size() > 1) {
			for (unsigned int i=0; i<BN_size; ++i) { 
				bool v; 
				iss >> v; 
				record[i] = v; 
			}
			dataset.push_back( record );
		}
	}
	//std::cout << "dataset.size() = " << dataset.size() << std::endl;
}

////////////////////////////////////////////////////////////
double TrainingData::EstimateProbability( const std::string & query)
{
	std::map<std::string,double>::const_iterator it = cache.find( query );
	if ( it != cache.end() )
  {
		//std::cout << "Hit!!! " << query << "\n";
		return it->second; //found cached
	}

	//std::cout << "EstimateProbability(\"" << query << "\")" << std::endl;
	std::vector<unsigned char> LHS; //0 - false, 1 - true, 2 don't care
	std::vector<unsigned char> RHS; //0 - false, 1 - true, 2 don't care
	std::vector<unsigned char> LHSandRHS; //0 - false, 1 - true, 2 don't care


	//parse the query - read all variables to the left of "|"
	//query looks like "1---0|--00-,"
	//which means P( X_0=1,P_4=0 | X_2=0,X_3=0 )

	std::string::size_type pipe_pos = query.find( '|' );

	for ( unsigned i = 0; i < pipe_pos; ++i )
  {
		switch ( query[i] )
    {
			case '0':
				LHS.push_back(0);
				break;
			case '1':
				LHS.push_back(1);
				break;
			case '-':
				LHS.push_back(2);
				break;
		}
	}

#ifdef DEBUG2
	for (unsigned int i =0; i<LHS.size(); ++i) {
		std::cout << static_cast<unsigned short>(LHS[i]) << " ";
	}
	std::cout << std::endl;
#endif
	assert (LHS.size()==BN_size);

	for (size_t i =pipe_pos; i<query.size(); ++i)
  {
		switch ( query[i] ) {
			case '0':
				RHS.push_back(0);
				break;
			case '1':
				RHS.push_back(1);
				break;
			case '-':
				RHS.push_back(2);
				break;
		}
	}

#ifdef DEBUG2
	for (unsigned int i =0; i<RHS.size(); ++i) {
		std::cout << static_cast<unsigned short>(RHS[i]) << " ";
	}
	std::cout << std::endl;
#endif
	assert (RHS.size()==BN_size);

	for (unsigned int i=0; i<BN_size; ++i) {
		unsigned int s = LHS[i]*10 + RHS[i];
		switch (s) {
			case 22: LHSandRHS.push_back(2); break;
			case 21: LHSandRHS.push_back(1); break;
			case 20: LHSandRHS.push_back(0); break;

			case 12: LHSandRHS.push_back(1); break;
			case 11: LHSandRHS.push_back(1); break; //variable set to 1 in both LHS and RHS - useless, but OK
			case 10: std::cout << "X_" << i << " is set to different values in LHS and RHS. Check query\n"; assert(false); break;

			case  2: LHSandRHS.push_back(0); break;
			case  1: std::cout << "X_" << i << " is set to different values in LHS and RHS. Check query\n"; assert(false); break;
			case  0: LHSandRHS.push_back(0); break; //variable set to 0 in both LHS and RHS - useless, but OK
		};
	}
#ifdef DEBUG2
	for (unsigned int i =0; i<LHSandRHS.size(); ++i) {
		std::cout << static_cast<unsigned short>(LHSandRHS[i]) << " ";
	}
	std::cout << std::endl;
#endif
	assert (RHS.size()==BN_size);

	unsigned int freq_RHS = 0;       //P(A)
	unsigned int freq_LHSandRHS = 0; //P(AB)
	//remember P(B|A) = P(AB)/P(A),
	//so estimate of conditional probability is freq_LHSandRHS/freq_RHS

	//go through all records in the dataset (random sampling
	//may be more appropriate for large/huge datasets

	for (unsigned int i=0; i<dataset.size(); ++i) {
		if (Match(dataset[i],LHSandRHS)) ++freq_LHSandRHS;
		if (Match(dataset[i],RHS))       ++freq_RHS;
	}
#ifdef DEBUG
	std::cout << "freq_LHSandRHS " << freq_LHSandRHS << std::endl;
	std::cout << "freq_RHS " << freq_RHS << std::endl;
#endif

	//freq_RHS==0 may cause problems
	double prob = static_cast<double>(freq_LHSandRHS)/freq_RHS;
	//cache.insert( std::make_pair<std::string,double>(query,prob));
	cache[query]=prob;
	return prob;
}

////////////////////////////////////////////////////////////
unsigned int TrainingData::Size() const { return BN_size; }

////////////////////////////////////////////////////////////
bool TrainingData::Match(
		const std::vector<bool> & record,
		const std::vector<unsigned char> & pattern
		) const {
	for (unsigned int i=0; i<BN_size; ++i) { 
		if ( record[i]==true && pattern[i]==0 ) return false; 
		if ( record[i]==false && pattern[i]==1 ) return false; 
	}
	return true;
}

BayesianNetwork TrainingData::Learn()
{
  BayesianNetwork bn( BN_size );

  // Connect the Nodes
  for ( unsigned i = 1; i < BN_size; ++i )
  {
    std::string given( BN_size, '-' );

    // value to beat
    for ( unsigned j = 0; j < i; ++j )
      given[j] = '1';
    double prob = EstimateProbability( MakeQuery( i, given ) );

    if ( Equals( EstimateProbability( MakeQuery( i, std::string( BN_size, '-' ) ) ), prob ) )
      continue; // add no parents

    unsigned level = i;
    unsigned limit = 1;
    for ( unsigned j = 0; j < i; ++j )
      limit <<= 1;

    // for each possible case
    std::string tempGiven( BN_size, '-' );
    for ( unsigned j = 0; j < limit; ++j )
    {
      MakeBitsQuery( j, tempGiven );
      unsigned nOnes = (unsigned)std::count( tempGiven.begin(), tempGiven.end(), '1' );
      if ( nOnes < level )
      {
        if ( Equals( EstimateProbability( MakeQuery( i, tempGiven ) ), prob ) )
        {
          given = tempGiven;
          level = nOnes;
        }
      }
      if ( level <= 1 ) break;
    }

    for ( unsigned j = 0; j < i; ++j )
    {
      if ( given[j] == '1' )
        bn.AddLink( j, i );
    }
  }

  // REMOVE THIS ===============================================================
  //enum { A, J, M, B, E };

  //bn.AddLink( B, A );
  //bn.AddLink( E, A );
  //bn.AddLink( A, J );
  //bn.AddLink( A, M );
  // REMOVE THIS ===============================================================

  unsigned nDone = 0;
  std::vector<unsigned> finished( BN_size, BN_size );
  while ( nDone != BN_size )
  {
    // for each non-root
    for ( unsigned i = 0; i < BN_size; ++i )
    {
      if ( finished[i] != BN_size )
        continue;

      if ( bn.parents[i].empty() )
      {
        bn.AddCPT( i, EstimateProbability( MakeQuery( i, std::string( BN_size, '-' ) ) ) );
      }
      else
      {
        unsigned nCases = 1, nParents = (unsigned)bn.parents[i].size();
        for ( unsigned j = 0; j < nParents; ++j )
          nCases <<= 1;

        std::string given( BN_size, '-' );

        // for each case
        bool tryAgain = false;
        std::vector<double> weights( nCases );
        for ( unsigned j = 0; j < nCases; ++j )
        {
          unsigned parBit = nCases >> 1;
          for ( unsigned p = 0; p < nParents; ++p )
          {
            // make sure parent is done first
            if ( bn.CPTs[bn.parents[i][p]].empty() )
            {
              tryAgain = true;
              break;
            }

            if ( ( parBit & j ) != 0 )
              given[bn.parents[i][p]] = '1';
            else
              given[bn.parents[i][p]] = '0';
            parBit >>= 1;
          }
          if ( tryAgain ) break;
          weights[j] = EstimateProbability( MakeQuery( i, given ) );
        }
        if ( tryAgain ) continue;
        bn.AddCPT( i, weights );
      }

      finished[i] = i;
      nDone++;
    }
  }

  return bn;
}

std::string TrainingData::MakeQuery( unsigned state, const std::string &given )
{
  std::string query( BN_size * 2 + 1, '-' );
  query[BN_size] = '|';
  query[state] = '1';
  query.replace( BN_size + 1, BN_size, &given[0] );
  return query;
}

std::string TrainingData::MakeQuery( unsigned state, unsigned given )
{
  std::string query( BN_size * 2 + 1, '-' );
  query[BN_size] = '|';
  query[state] = '1';
  query[BN_size + given + 1] = '1';
  return query;
}

void TrainingData::MakeBitsQuery( unsigned bits, std::string &query )
{
  unsigned bit = 1;
  for ( unsigned i = 0; i < BN_size; ++i )
  {
    if ( ( bits & bit ) != 0 )
      query[i] = '1';
    else
      query[i] = '-';
    bit <<= 1;
  }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
