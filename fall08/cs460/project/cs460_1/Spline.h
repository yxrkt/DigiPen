#pragma once

#include <d3dx9.h>
#include <list>
#include <math.h>
#include "Vertex.h"


static inline int Factorial( int lhs )
{
  int final = lhs;
  for ( int i = 2; i < lhs; ++i )
    final *= i;
  return final;
}

static inline int Choose( int lhs, int rhs )
{
  if ( rhs == 0 || lhs == rhs )
    return 1;
  return Factorial( lhs ) / ( Factorial( rhs ) * Factorial( lhs - rhs ) );
}

static void GetSplinePoint( D3DXVECTOR3 &final, const VertVec &points, float t )
{
  final = D3DXVECTOR3( 0.f, 0.f, 0.f );
  int nDegree = (int)points.size() - 1;
  VertVecItC pointIter = points.begin();
  for ( int i = 0; i <= nDegree; ++i )
  {
    float coef = (float)Choose( nDegree, i ) * pow( ( 1.f - t ), nDegree - i ) * pow( t, i );
    final += ( coef * ( *pointIter->ToLPD3DXVECTOR3() ) );
    ++pointIter;
  }
}

static void GetSplineFirstDeriv( D3DXVECTOR3 &final, const VertVec &points, float t )
{
  final = D3DXVECTOR3( 0.f, 0.f, 0.f );
  int nDegree = (int)points.size() - 1, nDegree2 = nDegree - 1;
  VertVecItC pointIter = points.begin();
  for ( int i = 0; i <= nDegree; ++i )
  {
    float coef = (float)Choose( nDegree, i ) * 
                 ( ( i == nDegree ? 0 : (float)( i - nDegree ) * pow( 1.f - t, nDegree2 - i ) * pow( t, i ) ) +
                   ( i == 0       ? 0 : (float)i * pow( 1 - t, nDegree - i ) * pow( t, i - 1 ) ) );
    D3DXVECTOR3 shiz( coef * ( *pointIter->ToLPD3DXVECTOR3() ) );
    final += ( coef * ( *pointIter->ToLPD3DXVECTOR3() ) );
    ++pointIter;
  }
}

static inline float Dist( const ColoredVertex &lhs, const ColoredVertex &rhs )
{
  float xDif = rhs.x - lhs.x, yDif = rhs.y - lhs.y, zDif = rhs.z - lhs.z;
  return sqrt( xDif * xDif + yDif * yDif + zDif * zDif );
}
