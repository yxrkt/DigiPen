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
  D3DXMATRIX trans( 1.f,  0.f,  0.f,  0.f,
                    0.f,  1.f,  0.f,  0.f,
                    0.f,  0.f,  1.f,  0.f,
                    v[0], v[1], v[2], 1.f );

  float x = q.x, y = q.y, z = q.z, w = q.w;

  D3DXMATRIX rot(
    1.f - 2.f * ( y * y + z * z ), 2.f * ( x * y + w * z ),       2.f * ( x * z - w * y ),       0.f,
    2.f * ( x * y - w * z ),       1.f - 2.f * ( x * x + z * z ), 2.f * ( y * z + w * x ),       0.f,
    2.f * ( x * z + w * y ),       2.f * ( y * z - w * x ),       1.f - 2.f * ( x * x + y * y ), 0.f,
    0.f,                           0.f,                           0.f,                           1.f );

  D3DXMATRIX scale( s,    0.f,  0.f,  0.f,
                    0.f,  s,    0.f,  0.f,
                    0.f,  0.f,  s,    0.f,
                    0.f,  0.f,  0.f,  1.f );

  D3DXMatrixMultiply( &matrix, &scale, &rot );
  D3DXMatrixMultiply( &matrix, &matrix, &trans );
}

void VQS::Interpolate( VQS &vqsOut, const VQS &vqs0, const VQS &vqs1, float t )
{
  vqsOut.v = vqs0.v + t * ( vqs1.v - vqs0.v );
  Slerp( vqsOut.q, vqs0.q, vqs1.q, t );
  vqsOut.s = vqs0.s + t * ( vqs1.s - vqs0.s );
}

void VQS::Slerp( D3DXQUATERNION &qOut, const D3DXQUATERNION &q0, const D3DXQUATERNION &q1, float t )
{
  float dot = min( q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w, 1.f );
  dot = max( q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w, -1.f );
  D3DXQUATERNION _q1( q1 );
  if ( dot < 0.f )
  {
    dot = -dot;
    _q1 = D3DXQUATERNION( _q1.x * -1.f, _q1.y * -1.f, _q1.z * -1.f, _q1.w * -1.f );
  }
  else if ( 1.f - dot < FLOAT_EPSILON )
  {
    qOut = q0;
    return;
  } // will prevent divide by zero

  float angle = acos( dot );
  float sina  = sin( angle );
  float s0 = sin( angle - t * angle ) / sina;
  float s1 = sin( t * angle ) / sina;
  qOut = D3DXQUATERNION( s0 * q0.x + s1 * _q1.x, s0 * q0.y + s1 * _q1.y, s0 * q0.z + s1 * _q1.z, s0 * q0.w + s1 * _q1.w );

  // normalize
  float mag = sqrt( qOut.x * qOut.x + qOut.y * qOut.y + qOut.z * qOut.z + qOut.w * qOut.w );
  if ( mag )
  {
    qOut.x = qOut.x / mag;
    qOut.y = qOut.y / mag;
    qOut.z = qOut.z / mag;
    qOut.w = qOut.w / mag;
  }
}
