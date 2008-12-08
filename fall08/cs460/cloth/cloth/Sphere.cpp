#include "Sphere.h"


Sphere::Sphere( const D3DXVECTOR3 &center, float radius, int nStacks, int nSlices )
: m_center( D3DXVECTOR3( 0.f, 0.f, 0.f ) )
, m_radius( radius )
, m_nStacks( nStacks )
, m_nSlices( nSlices )
{
  FindVertices();
  FindTriangles();
  SetNormals();
  SetCenter( center );
}

Sphere::~Sphere( void )
{
}

void Sphere::GetTriangles( VertVec &triangles )
{
  triangles.clear();
  size_t nTris = m_triangles.size();
  triangles.reserve( nTris );

  for ( size_t i = 0; i < nTris; ++i )
  {
    triangles.push_back( m_vertices[m_triangles[i].v0] );
    triangles.push_back( m_vertices[m_triangles[i].v1] );
    triangles.push_back( m_vertices[m_triangles[i].v2] );
  }
}

const D3DXVECTOR3 &Sphere::GetCenter( void ) const
{
  return m_center;
}

float Sphere::GetRadius( void ) const
{
  return m_radius;
}

void Sphere::SetCenter( const D3DXVECTOR3 &center )
{
  size_t nVerts = m_vertices.size();
  for ( size_t i = 0; i < nVerts; ++i )
    m_vertices[i].Pos = center + ( m_vertices[i].Pos - m_center );
  m_center = center;
}

void Sphere::SetRadius( float radius )
{
  size_t nVerts = m_vertices.size();
  for ( size_t i = 0; i < nVerts; ++i )
  {
    D3DXVECTOR3 dir( m_vertices[i].Pos - m_center );
    D3DXVec3Normalize( &dir, &dir );
    m_vertices[i].Pos = radius * dir + m_center;
  }
  m_radius = radius;
}

void Sphere::FindVertices( void )
{
	// Top D3DXVECTOR3
	m_vertices.push_back( D3DXVECTOR3( 0.f, m_radius, 0.f ) );

	// Angle determining the current latitude
	float theta = D3DX_PI / (float)m_nStacks;

	// Angle to determine the current longitude
	float alpha = 2.f * D3DX_PI / (float)m_nSlices;

	// Trig functions calculated once
	float cos_alpha = cos( alpha );
	float sin_alpha = sin( alpha );

	// Loop through layers
	for ( unsigned v = 1; v < m_nStacks; ++v )
	{
		float y = m_radius * cos( v * theta ), z = 0;
		float x = sqrt( m_radius * m_radius - y * y );

		// Loop through slices
		for ( unsigned h = 0; h < m_nSlices; ++h )
		{
			// Store D3DXVECTOR3
			m_vertices.push_back( D3DXVECTOR3( x, y, z ) );
			float temp = x;
			x = x * cos_alpha + z * sin_alpha;
			z = temp * -sin_alpha + z * cos_alpha;
		}
	}

	// Bottom D3DXVECTOR3
	m_vertices.push_back( D3DXVECTOR3( 0.f, -(float)m_radius, 0 ) );
}

void Sphere::FindTriangles( void )
{
	// Index of final D3DXVECTOR3
	unsigned final_index = (unsigned)( m_vertices.size() - 1 );

	// Loop through layers
	for ( unsigned h = 0; h < m_nStacks; ++h )
	{
		// Used to mark first index of layer
		unsigned v = h ? ( h - 1 ) * m_nSlices + 1 : 1;
		unsigned end = h ? h * m_nSlices : m_nSlices;
		// Loop through slices
		for ( ; v <= end; ++v )
		{
			// Case: Connecting top D3DXVECTOR3
			if ( h == 0 )
			{
				if ( v % m_nSlices )
					m_triangles.push_back( IndexTriangle( h, v, v + 1 ) );
				else
					m_triangles.push_back( IndexTriangle( h, v, h + 1 ) );
			}
			// Case: Connecting middle layers
			else if ( h < m_nStacks - 1 )
			{
				if ( v % m_nSlices == 1 )
				{
					m_triangles.push_back( IndexTriangle( v, v + m_nSlices - 1, v + m_nSlices ) );
					m_triangles.push_back( IndexTriangle( v, v + m_nSlices, v + m_nSlices + 1 ) );
				}
				else if ( v % m_nSlices )
				{
					m_triangles.push_back( IndexTriangle( v, v - 1, v + m_nSlices ) );
					m_triangles.push_back( IndexTriangle( v, v + m_nSlices, v + m_nSlices + 1 ) );
				}
				else
				{
					m_triangles.push_back( IndexTriangle( v, v - 1, v + m_nSlices ) );
					m_triangles.push_back( IndexTriangle( v, v + m_nSlices, v + 1 ) );
				}
			}
			// Case: Connecting botton D3DXVECTOR3
			else
			{
				if ( v % m_nSlices )
					m_triangles.push_back( IndexTriangle( v, final_index, v + 1 ) );
				else
					m_triangles.push_back( IndexTriangle( v, final_index, v - m_nSlices + 1 ) );
			}
		}
	}
}

void Sphere::SetNormals( void )
{
  size_t nVerts = m_vertices.size();
  for ( size_t i = 0; i < nVerts; ++i )
  {
    D3DXVECTOR3 dir( m_vertices[i].Pos - m_center );
    D3DXVec3Normalize( &dir, &dir );
    m_vertices[i].Norm = dir + m_center;
  }
}
