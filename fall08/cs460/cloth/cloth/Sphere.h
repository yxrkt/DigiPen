#pragma once

#include "Vertex.h"
#include "AutoList.h"

struct IndexTriangle;

typedef std::vector< IndexTriangle > ITriVec;

class Sphere : public AutoList< Sphere >
{
  public:
    Sphere( const D3DXVECTOR3 &center = D3DXVECTOR3( 0.f, 0.f, 0.f ), 
            float radius = 25.f, int nStacks = 10, int nSlices = 10 );
    ~Sphere( void );

    void GetTriangles( VertVec &triangles );
    const D3DXVECTOR3 &GetCenter( void ) const;
    float GetRadius( void ) const;

    void SetCenter( const D3DXVECTOR3 &center );
    void SetRadius( float radius );

  private:
    void FindVertices( void );
    void FindTriangles( void );
    void SetNormals( void );

    D3DXVECTOR3 m_center;
    float       m_radius;
    unsigned    m_nStacks, m_nSlices;
    VertVec     m_vertices;
    ITriVec     m_triangles;
};

struct IndexTriangle
{
	IndexTriangle( unsigned _v0, unsigned _v1, unsigned _v2 )
		: v0( _v0 ), v1( _v1 ), v2( _v2 ) {}
	unsigned v0;
	unsigned v1;
	unsigned v2;
};
