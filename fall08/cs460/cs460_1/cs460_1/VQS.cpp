#include "VQS.h"


VQS::VQS( D3DXVECTOR3 _v, D3DXQUATERNION _q, float _s )
: v( _v )
, q( _q )
, s( _s )
, V( v )
, Q( q )
, S( s )
{
}

VQS::VQS( const VQS &rhs )
: v( rhs.v )
, q( rhs.q )
, s( rhs.s )
, V( v )
, Q( q )
, S( s )
{
}

VQS::~VQS()
{
}

void VQS::GetMatrix( D3DXMATRIX &matrix )
{
  D3DXMATRIX mtxTS;
  for ( int r = 0; r < 4; ++r )
  {
    for ( int c = 0; c < 4; ++c )
    {
      if ( r == c && r != 3 )
        mtxTS.m[r][c] = s;
      else if ( r == 3 &&  c != 3 )
        mtxTS.m[r][c] = v[c];
      else
        mtxTS.m[r][c] = 0.f;
    }
  }
  mtxTS.m[3][3] = 1.f;

  float x = q.x, y = q.y, z = q.z, w = q.w;
  /*
  D3DXMATRIX mtxRot(
    1.f - 2.f * ( y * y - z * z ), 2.f * ( x * y - w * z ),       2.f * ( x * z + w * y ),       0.f,
    2.f * ( x * y + w * z ),       1.f - 2.f * ( x * x - z * z ), 2.f * ( y * z - w * x ),       0.f,
    2.f * ( x * z - w * y ),       2.f * ( y * z + w * x ),       1.f - 2.f * ( x * x - y * y ), 0.f,
    0.f,                           0.f,                           0.f,                           1.f );
  /*/
  D3DXMATRIX mtxRot( // transpose of above
    1.f - 2.f * ( y * y - z * z ), 2.f * ( x * y + w * z ),       2.f * ( x * z - w * y ),       0.f,
    2.f * ( x * y - w * z ),       1.f - 2.f * ( x * x - z * z ), 2.f * ( y * z + w * x ),       0.f,
    2.f * ( x * z + w * y ),       2.f * ( y * z - w * x ),       1.f - 2.f * ( x * x - y * y ), 0.f,
    0.f,                           0.f,                           0.f,                           1.f );
  //*/


  /*
  D3DXMatrixMultiply( &matrix, &mtxTS, &mtxRot );
  /*/
  D3DXMatrixMultiply( &matrix, &mtxRot, &mtxTS );
  //*/
}

void VQS::Interpolate( VQS &vqsOut, const VQS &vqs0, const VQS &vqs1, float t )
{
  vqsOut.v = vqs0.v + t * ( vqs1.v - vqs0.v );

  D3DXQuaternionSlerp( &vqsOut.q, &vqs0.q, &vqs1.q, t );
  //vqsOut.q = vqs0.q + t * ( vqs1.q - vqs0.q );

  vqsOut.s = vqs0.s + t * ( vqs1.s - vqs0.s );
}
