#include <iostream>
#include <fstream>
#include <algorithm>
#include "astar.h"
#include "graph.h"

struct VertexTest
{
  public:
	  VertexTest (size_t _id=0, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
      : id(_id)
      , x(_x)
      , y(_y)
      , z(_z)
    {	
		  //std::cout << "VertexTest("<<id<<") called\n";
	  }

	  size_t ID() const { return id; }
	  float X() const { return x; }
	  float Y() const { return y; }
	  float Z() const { return z; }
	  bool operator< (const VertexTest& rhs) const
    {
		  return id<rhs.id;
	  }

	private:
	  size_t id;
	  float x,y,z;
};

template<typename EdgeType>
class EdgePrint
{
	public:
		EdgePrint(std::ostream & _os) : os(_os) {}
		void operator() ( const EdgeType & e) const
    {
			os << "take " << e.GetID1() << " to " << e.GetID2() << "("<< e.GetWeight() <<")\n";
		}

	private:
		std::ostream &os;
};

struct EdgeTest
{
  public:
	  EdgeTest(size_t id1, size_t id2, float weight)
      : id1(id1)
      , id2(id2)
      , weight(weight)
    {}

	  size_t GetID1() const { return id1; }
	  size_t GetID2() const { return id2; }
	  float  GetWeight() const { return weight; }

	private:
	  size_t id1;
	  size_t id2;
	  float weight;
};

template <typename GraphType>
struct TestHeuristic
{
	float operator () (const GraphType &graph, 
			               const typename GraphType::Vertex &v1,  
			               const typename GraphType::Vertex &v2) const
  {
		float h = 0;
		switch ( v1.ID() )
    {
			case 0: h = 1.0f; break;
			case 1: h = 1.0f; break;
			case 2: h = 1.0f; break;
			case 3: h = 0.0f; break;
		};
		return h;
	}
};

int main() try
{
	typedef Graph<VertexTest,EdgeTest> Graph;
	Graph graph;
	//        0
	//     1/  \3
	//     1    2
	//    10\  /1
	//        3
	graph.InsertVertex( Graph::Vertex(0) );
	graph.InsertVertex( Graph::Vertex(1) );
	graph.InsertVertex( Graph::Vertex(2) );
	graph.InsertVertex( Graph::Vertex(3) );
	graph.InsertEdge( Graph::Edge(0,1,1) );
	graph.InsertEdge( Graph::Edge(1,0,1) );
	graph.InsertEdge( Graph::Edge(0,2,3) );
	graph.InsertEdge( Graph::Edge(2,0,3) );
	graph.InsertEdge( Graph::Edge(1,3,10) );
	graph.InsertEdge( Graph::Edge(3,1,10) );
	graph.InsertEdge( Graph::Edge(2,3,1) );
	graph.InsertEdge( Graph::Edge(3,2,1) );

	//throws
	//graph.InsertEdge( Graph::Edge(5,2,1) ); 

	Astar<Graph,TestHeuristic<Graph> > astar(graph);
	std::vector<EdgeTest> solution = astar.search(0,3);
	std::cout << "Print solution:" << std::endl;
	std::for_each( solution.begin(), solution.end(), EdgePrint<EdgeTest>(std::cout) );

  return 0;
}

catch (const char *msg)
{
	std::cout << msg << std::endl;
}
