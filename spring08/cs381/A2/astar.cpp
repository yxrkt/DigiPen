#include <iostream>
#include <string>
#include <algorithm>
#include <time.h>


// ctor
template <typename GraphType, typename Heuristic >
Astar<GraphType, Heuristic>::Astar(const GraphType &_graph) : graph(_graph) {}

// search
template <typename GraphType, typename Heuristic >
std::vector<typename GraphType::Edge> Astar<GraphType, Heuristic>::search(size_t start_id, size_t goal_id) const
{
  typedef std::vector<typename GraphType::Edge> EdgeVec;
  typedef EdgeVec::iterator EdgeVecIt;
  typedef NodeMSet::iterator NodeMSetIt;
  typedef NodeSet::iterator NodeSetIt;

  EdgeVec   soln;                             // final value to be returned
  EdgeVec   edges;                            // reused for each node
  Heuristic hcost;                            // functor for heuristic cost
  Node      curNode;                          // current node

  openList.clear();
  closedList.clear();

  // init case: on starting node
  edges = graph.GetOutEdges( start_id );
  for ( EdgeVecIt i = edges.begin(); i != edges.end(); ++i )
  {
    Node n( (int)i->GetID2(), (int)start_id, i->GetWeight(), 
            hcost( graph, i->GetID2(), goal_id ) );
    OpenNode( n );
  }

  Node start( (int)start_id, NULL_ID, 0.f, hcost( graph, start_id, goal_id ) );
  OpenNode( start );
  CloseNode();

  curNode = *openList.begin();
  bool onDest = (curNode.myID == goal_id) ? true : false;

  // while not on goal node
  while ( !onDest && !openList.empty() )
  {
    edges = graph.GetOutEdges( curNode.myID );
    for ( EdgeVecIt i = edges.begin(); i != edges.end(); ++i )
    {
      size_t foundID = i->GetID2();
      NodeSetIt found = closedList.find( Node( (int)foundID ) );

      if ( found == closedList.end() )
      {
        float walk = curNode.walkCost + i->GetWeight();
        NodeMSetIt found;
        for ( found = openList.begin(); found != openList.end(); ++found )
          if ( found->myID == (int)foundID ) break;
        if ( found != openList.end() )
        {
          if ( walk < found->walkCost )
          {
            found->parentID = curNode.myID;
            found->walkCost = walk;
          }
        }
        else
        {
          Node node( (int)foundID, curNode.myID, walk, hcost( graph, foundID, goal_id ) );
          OpenNode( node );
        }
      }
    }

    CloseNode();
    curNode = *openList.begin();

    // terminating case: we are on dest node
    onDest = (curNode.myID == goal_id) ? true : false;
  }

  // fill output list
  while ( curNode.parentID != NULL_ID )
  {
    std::vector<GraphType::Edge> edges = graph.GetOutEdges( curNode.myID );
    size_t nNeighbors = edges.size();
    for ( size_t i = 0; i < nNeighbors; ++i )
    {
      if ( edges[i].GetID2() == (size_t)curNode.parentID )
      {
        soln.push_back( edges[i] );
        curNode = *closedList.find( curNode.parentID );
        break;
      }
    }
  }

  //std::reverse( soln.begin(), soln.end() );

  return soln;
}

// Add a node to open list: assumes not on close list
template <typename GraphType, typename Heuristic >
inline void Astar<GraphType, Heuristic>::OpenNode( const Node &node ) const
{
  openList.insert( node );
}

// Remove node from open list and add to closed list
template <typename GraphType, typename Heuristic >
inline void Astar<GraphType, Heuristic>::CloseNode() const
{
    // find correct node
  closedList.insert( *openList.begin() );
  openList.erase( openList.begin() );
}
