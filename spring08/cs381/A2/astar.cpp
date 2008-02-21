#include <iostream>
#include <string>
#include <algorithm>


// ctor
template <typename GraphType, typename Heuristic >
Astar<GraphType, Heuristic>::Astar(const GraphType &_graph) : graph(_graph) {}

// search
template <typename GraphType, typename Heuristic >
std::vector<typename GraphType::Edge> Astar<GraphType, Heuristic>::search(size_t start_id, size_t goal_id) const
{
  typedef std::vector<GraphType::Edge> EdgeVec;
  typedef EdgeVec::iterator EdgeVecIt;
  typedef NodeList::iterator NodeListIt;

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
    Node n( i->GetID2(), start_id, i->GetWeight(), 
            hcost( graph, i->GetID2(), goal_id ), i->GetWeight() );
    OpenNode( n );
  }

  Node start( start_id, NULL_ID, 0.f, hcost( graph, start_id, goal_id ), 0.f );
  CloseNode( start );
  openList.sort();

  curNode = openList.front();
  bool onDest = (curNode.myID == goal_id) ? true : false;

  // while not on goal node
  while ( !onDest && !openList.empty() )
  {
    edges = graph.GetOutEdges( curNode.myID );
    for ( EdgeVecIt i = edges.begin(); i != edges.end(); ++i )
    {
      size_t foundID = i->GetID2();
      NodeListIt found = std::find( closedList.begin(), closedList.end(), Node( foundID ) );

      if ( found == closedList.end() )
      {
        float walk = curNode.walkCost + i->GetWeight();
        found = std::find( openList.begin(), openList.end(), Node( foundID ) );
        if ( found != openList.end() )
        {
          if ( walk < found->walkCost )
          {
            found->parentID = curNode.myID;
            found->walkCost = walk;
            found->edgeCost = i->GetWeight();
          }
        }

        else
        {
          Node node( foundID, curNode.myID, walk, 
                     hcost( graph,  foundID, goal_id ), i->GetWeight() );
          OpenNode( node );
        }
      }
    }

    CloseNode( curNode );
    openList.sort();
    curNode = openList.front();

    // terminating case: we are on dest node
    onDest = (curNode.myID == goal_id) ? true : false;
  }

  // fill output list
  while ( curNode.parentID != NULL_ID )
  {
    soln.push_back( GraphType::Edge( curNode.parentID, curNode.myID, curNode.edgeCost ) );
    curNode = *std::find( closedList.begin(), closedList.end(), Node( curNode.parentID ) );
  }

  std::reverse( soln.begin(), soln.end() );

  return soln;
}

// Add a node to open list: assumes not on close list
template <typename GraphType, typename Heuristic >
void Astar<GraphType, Heuristic>::OpenNode( const Node &node ) const
{
  openList.push_back( node );
}

// Remove node from open list and add to closed list
template <typename GraphType, typename Heuristic >
void Astar<GraphType, Heuristic>::CloseNode( const Node &node ) const
{
  openList.remove( node );
  closedList.push_back( node );
}

// search dummy
//template <typename GraphType, typename Heuristic >
//std::vector<typename GraphType::Edge> Astar<GraphType, Heuristic>::search_d(size_t start_id, size_t goal_id) const
//{
//  //dummy solution
//  std::vector<GraphType::Edge> solution;
//  solution.push_back( GraphType::Edge(0,2,1) );
//  solution.push_back( GraphType::Edge(2,3,1) );
//
//  //test heuristic
//  Heuristic h;
//  std::cout << "Testing heuristic\n";
//
//  GraphType::Vertex goal = graph.GetVertex(goal_id);
//  GraphType::Vertex v    = graph.GetVertex(start_id);
//  std::cout << "Heuristic at start "
//            << h( graph,v,goal)
//            << std::endl;
//
//  v = graph.GetVertex(1);
//  std::cout << "Heuristic at 1 "
//            << h( graph,v,goal)
//            << std::endl;
//
//  //test neighbors
//  std::cout << "Testing neighbors\n";
//  std::vector<GraphType::Edge> outedges = graph.GetOutEdges(graph.GetVertex(goal_id));
//  size_t outedges_size = outedges.size();
//  for (size_t i = 0; i < outedges_size; ++i)
//  std::cout << "goal has a neighbor "
//            << outedges[i].GetID2()
//            << " at distance "
//            << outedges[i].GetWeight()
//            << std::endl;
//  return solution;
//}
