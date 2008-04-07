// Adapter class
class Graph
{
  public:
    // To be called by client
    inline void AddVertex( const Vertex &v )
    {
      // Add vertex by calling vector's push_back
      verts.push_back( v );
      
      // Adds necessary edges by calling vector's push_back
      size_t nNeighbors = v.neighbors.size();
      for ( size_t i = 0; i < nNeighbors; ++i )
        edges.push_back( Edge( v, v.neighbors[i] ) );
    }
  
  private:
    // Adaptee classes
    std::vector< Edge >   edges;
    std::vector< Vertex > verts;
};
