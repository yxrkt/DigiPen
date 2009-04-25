#include "Macros.h"
#include "Joint.h"
#include "VQS.h"

typedef D3DXMATRIX* ( WINAPI *ROTATION_MATRIX_FN )( D3DXMATRIX *, FLOAT );

Joint::Joint( const LPFRAME pFrame, float scale, const Constraints &constraints )
: m_pFrame( pFrame )
, m_constraints( constraints )
, m_pParent( NULL )
, m_pChild( NULL )
, m_end( 0.f, 0.f, 0.f )
, m_transform( pFrame->TransformationMatrix )
, m_combined( pFrame->matCombined )
, m_scale( scale )
, m_interpolating( false )
{
}

Joint::~Joint( void )
{
}

void Joint::Reset( void )
{
  m_transform     = m_pFrame->TransformationMatrix;
  m_combined      = m_pFrame->matCombined;
  m_interpolating = false;
}

void Joint::AlignTo( const D3DXVECTOR3 &world )
{
  if ( m_pChild == NULL ) return;

  // move end to local space
  D3DXVec3TransformCoord( &m_end, &m_pChild->m_end, &m_pChild->m_transform );

  // move destination to local space
  float       det;
  D3DXMATRIX  matInverse;
  D3DXVECTOR3 goal;
  D3DXMatrixInverse( &matInverse, &det, &m_combined );
  D3DXVec3TransformCoord( &goal, &world, &matInverse );

  // build and apply rotation
  D3DXVECTOR3 axis;
  D3DXVec3Cross( &axis, &m_end, &goal );
  D3DXVec3Normalize( &axis, &axis );

  D3DXVECTOR3 endN, goalN;
  D3DXVec3Normalize( &endN, &m_end );
  D3DXVec3Normalize( &goalN, &goal );
  float angle = acos( D3DXVec3Dot( &endN, &goalN ) );

  D3DXMATRIX matRot;
  D3DXMatrixRotationAxis( &matRot, &axis, angle );

  ApplyRotation( matRot );
  UpdateCombined();

  //// SAD ATTEMPT AT CONSTRAINTS
  //ROTATION_MATRIX_FN rotFunctions[3] = { D3DXMatrixRotationX, D3DXMatrixRotationY, D3DXMatrixRotationZ };
  //D3DXMATRIX matRotFinal;
  //D3DXMatrixIdentity( &matRotFinal );
  //for ( int i = 0; i < 3; ++i )
  //{
  //  if ( i != 2 ) continue;

  //  // move destination to local space
  //  float       det;
  //  D3DXMATRIX  matInverse;

  //  D3DXVECTOR3 goal;
  //  D3DXMatrixInverse( &matInverse, &det, &m_combined );
  //  D3DXVec3TransformCoord( &goal, &world, &matInverse );

  //  D3DXVECTOR3 toEnd2D( m_end ), toDest2D( goal );
  //  toEnd2D[i]  = 0.f;
  //  toDest2D[i] = 0.f;
  //  D3DXVec3Normalize( &toEnd2D, &toEnd2D );
  //  D3DXVec3Normalize( &toDest2D, &toDest2D );

  //  // Get the current angle
  //  float angle = acos( toEnd2D[(i + 1) % 3] );
  //  if ( toEnd2D[(i + 2) % 3] > 0.f )
  //    angle = -angle; // TODO: Verify this is the right way

  //  // Get the angle delta
  //  float angleDelta = acos( D3DXVec3Dot( &toEnd2D, &toDest2D ) );

  //  D3DXVECTOR3 crossOut;
  //  D3DXVec3Cross( &crossOut, &toEnd2D, &toDest2D );
  //  if ( crossOut[i] < 0.f ) // TODO: Verify this is the right way
  //    angleDelta = -angleDelta;

  //  float newAngle = angle + angleDelta;

  //  int cIndex = i * 2; // constraint index (xMin, yMin, zMin)
  //  newAngle   = clamp( newAngle, m_constraints[cIndex], m_constraints[cIndex + 1] );
  //  angleDelta = newAngle - angle;

  //  if ( angleDelta )
  //  {
  //    D3DXMATRIX matRot;
  //    rotFunctions[i]( &matRot, angleDelta );
  //    D3DXMatrixMultiply( &matRotFinal, &matRotFinal, &matRot );
  //  }

  //  ApplyRotation( matRotFinal );
  //  UpdateCombined();
  //}
}

void Joint::UpdateFrame( float t )
{
  if ( !m_interpolating )
  {
    m_transformBegin = m_pFrame->TransformationMatrix;
    m_interpolating  = true;
  }

  VQS vqs0( m_transformBegin ), vqs1( m_transform ), vqsOut;
  VQS::Interpolate( vqsOut, vqs0, vqs1, t );
  vqsOut.GetMatrix( m_pFrame->TransformationMatrix );
}

void Joint::GetWorldPos( D3DXVECTOR3 &out ) const
{
  D3DXVECTOR3 localOrigin( 0.f, 0.f, 0.f );
  D3DXVec3TransformCoord( &out, &localOrigin, &m_combined );
}

float Joint::GetWorldDistFrom( const D3DXVECTOR3 &world ) const
{
  D3DXVECTOR3 temp;
  GetWorldPos( temp );
  temp = world - temp;
  return D3DXVec3Length( &temp );
}

LPFRAME Joint::GetFrame( void ) const
{
  return m_pFrame;
}

float Joint::GetScale( void ) const
{
  return m_scale;
}

void Joint::SetParent( Joint *pParent )
{
  m_pParent = pParent;
  UpdateCombined();
}

void Joint::SetChild( Joint *pChild )
{
  m_pChild = pChild;
  UpdateCombined();
}

void Joint::UpdateCombined( void )
{
  if ( !m_pParent ) return;

  D3DXMATRIX parentCombined;
  parentCombined = m_pParent->m_combined;

  D3DXMatrixMultiply( &m_combined, &m_transform, &parentCombined );
  if ( m_pChild )
    m_pChild->UpdateCombined();
}

void Joint::ApplyRotation( const D3DXMATRIX &matRot )
{
  D3DXVECTOR3    vecTrans, vecScale;
  D3DXQUATERNION quatRot;

  D3DXMatrixDecompose( &vecScale, &quatRot, &vecTrans, &m_transform );

  D3DXQUATERNION quatRotIn;
  D3DXQuaternionRotationMatrix( &quatRotIn, &matRot );

  D3DXQuaternionMultiply( &quatRot, &quatRotIn, &quatRot );

  D3DXMatrixTransformation( &m_transform, NULL, NULL, &vecScale, NULL, &quatRot, &vecTrans );
}
