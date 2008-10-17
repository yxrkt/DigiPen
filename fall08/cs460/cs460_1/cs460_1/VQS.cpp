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

void VQS::Interpolate( VQS &vqsOut, const VQS &vqs0, const VQS &vqs1, float t )
{
  vqsOut.v = vqs0.v + t * ( vqs1.v - vqs0.v );

  D3DXQuaternionSlerp( &vqsOut.q, &vqs0.q, &vqs1.q, t );
  //vqsOut.q = vqs0.q + t * ( vqs1.q - vqs0.q );

  vqsOut.s = vqs0.s + t * ( vqs1.s - vqs0.s );
}
