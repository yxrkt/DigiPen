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

