#ifndef ASTAR
#define ASTAR

#include <vector>
#include <list>

template <typename GraphType, typename Heuristic > 
class Astar
{
    // constants
  static const int NULL_ID = -1;

    // node struct: holds parent and cost info
  struct Node
  {
    Node( int id = 0, int parent = NULL_ID, float walk = 0.f, 
          float h = 0.f, float e = 0.f )
      : myID( id )
      , parentID( parent )
      , walkCost( walk )
      , hCost( h )
      , edgeCost( e )
    {}

    int    myID;
    int    parentID;
    float  walkCost;
    float  hCost;
    float  edgeCost;

    inline float fCost() const { return walkCost + hCost; }
    bool operator <( const Node &rhs ) const { return ( fCost() < rhs.fCost() ); }
    bool operator ==( const Node &rhs ) { return ( myID == rhs.myID ); }
  };

    // typedefs
  typedef std::list<Node> NodeList;
  //typedef NodeList::iterator NodeListIt;

  public:
      // ctor: takes graph
    Astar(const GraphType &_graph);

      // A* search function
    std::vector<typename GraphType::Edge> search(size_t start_id, size_t goal_id) const;
    std::vector<typename GraphType::Edge> search_d(size_t start_id, size_t goal_id) const;

  private:
      // Sub methods for A* search
    void OpenNode( const Node &node ) const;
    void CloseNode( const Node &node ) const;

      // data
    const   GraphType &graph;                     // graph being searched
    mutable NodeList   openList;                  // list of opened nodes
    mutable NodeList   closedList;                // list of visited nodes


};

#include "astar.cpp"

#endif