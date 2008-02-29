#ifndef ASTAR
#define ASTAR

#include <vector>
#include <list>
#include <set>

#pragma warning( disable : 4290 ) // disable exception specification warning

template <typename GraphType, typename Heuristic > 
class Astar
{
    // constants
  static const int NULL_ID = -1;

    // node struct: holds parent and cost info
  struct Node
  {
    Node( int id = 0, int parent = NULL_ID, float walk = 0.f, float h = 0.f )
      : myID( id )
      , parentID( parent )
      , walkCost( walk )
      , hCost( h )
    {}

    inline float fCost() const { return walkCost + hCost; }
    inline bool operator <( const Node &rhs ) const { return ( fCost() < rhs.fCost() ); }
    inline bool operator ==( const Node &rhs ) { return ( myID == rhs.myID ); }

    int    myID;
    int    parentID;
    float  walkCost;
    float  hCost;
  };

  struct NodeIDLess
  {
    inline bool operator ()( const Node &lhs, const Node &rhs ) const
    {
      return ( lhs.myID < rhs.myID );
    }
  };

    // typedefs
  typedef std::multiset<Node> NodeMSet;
  typedef std::set<Node, NodeIDLess> NodeSet;

  public:
      // ctor: takes graph
    Astar( const GraphType &_graph );

      // A* search function
    std::vector<typename GraphType::Edge> search(size_t start_id, size_t goal_id) const;
    std::vector<typename GraphType::Edge> search_d(size_t start_id, size_t goal_id) const;

  private:
    void operator =( const Astar &rhs );

      // Sub methods for A* search
    void OpenNode( const Node &node ) const;
    void CloseNode() const;

      // data
    const   GraphType &graph;                     // graph being searched
    mutable NodeMSet   openList;                  // multiset of opened nodes
    mutable NodeSet    closedList;                // set of closed nodes
};

#include "astar.cpp"

#endif
