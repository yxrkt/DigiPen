#include "Physics.h"
#include "Macros.h"
#include "Sphere.h"


Physics *Physics::Instance( void )
{
  static Physics physics;
  return &physics;
}

Physics::Physics( void )
{
}

Physics::~Physics( void )
{
}

void Physics::Initialize( void )
{
  m_gravity = D3DXVECTOR3( 0.f, -0.98f, 0.f );
}

void Physics::Update( void )
{
  UpdateCloths();
}

void Physics::UpdateCloths( void )
{
  static DWORD lastTick = timeGetTime();
  DWORD tick = timeGetTime();
  float frameStep = (float)( tick - lastTick ) / 1000.f;
  lastTick = tick;

  VertPair vertPairs[8]; // eight neighbors (l, tl, t, tr, r, br, b, bl)

  FOR_EACH_AUTO( PhysCloth, pPhysCloth )
  {
    Cloth *pCloth = (Cloth *)pPhysCloth->pCloth;
    int resY = pCloth->GetResY(), resX = pCloth->GetResX();
    // update positions
    for ( int r = 0; r < resY; ++r )
    {
      for ( int c = 0; c < resX; ++c )
      {
        ColoredVertex &vert = pCloth->GetVertex( r, c );
        VertInfo      &vertInfo = pPhysCloth->GetVertexInfo( r, c );
        if ( vertInfo.locked ) continue;

        D3DXVECTOR3 newPos( vert.Pos + vertInfo.vel );
        FOR_EACH_AUTO( Sphere, pSphere )
        {
          D3DXVECTOR3 center( pSphere->GetCenter() );
          D3DXVECTOR3 pc( center - vert.Pos ), u, pq( newPos - vert.Pos ), v;
          if ( D3DXVec3Dot( &pc, &pq ) > 0.f )
          {
            float radius = 1.05f * pSphere->GetRadius();
            D3DXVec3Normalize( &u, &pc );
            D3DXVec3Normalize( &v, &pq );
            D3DXVECTOR3 m;
            D3DXVec3Cross( &m, &u, &v );
            D3DXVec3Cross( &m, &v, &m );
            D3DXVec3Normalize( &m, &m );
            float d = D3DXVec3Dot( &pc, &m );
            if ( d <= radius )
            {
              float mag = D3DXVec3Length( &pc ) * D3DXVec3Dot( &u, &v )
                            - sqrt( radius * radius - d * d );
              float lenSq = D3DXVec3LengthSq( &pq );
              if ( lenSq >= ( mag * mag ) )
              {
                newPos = vert.Pos + mag * v;
                vertInfo.locked = true;
              }
            }
          }
        }
        vert.Pos = newPos;
        vertInfo.vel += ( vertInfo.accel * frameStep );
        vertInfo.force += ( pPhysCloth->mass * m_gravity );
      }
    }

    // apply forces: F = -k * ( dist - restDist )
    int lastRow = pPhysCloth->resY - 1, lastCol = pPhysCloth->resX - 1;

    for ( int r = 0; r < resY; ++r )
    {
      for ( int c = 0; c < resX; ++c )
      {
        ColoredVertex &vert     = pCloth->GetVertex( r, c );
        VertInfo      &vertInfo = pPhysCloth->GetVertexInfo( r, c );

        //if ( vertInfo.locked ) continue;

        GetVertPair( &vertPairs[0], pPhysCloth, r    , c - 1 );
        GetVertPair( &vertPairs[1], pPhysCloth, r - 1, c - 1 );
        GetVertPair( &vertPairs[2], pPhysCloth, r - 1, c     );
        GetVertPair( &vertPairs[3], pPhysCloth, r - 1, c + 1 );
        GetVertPair( &vertPairs[4], pPhysCloth, r    , c + 1 );
        GetVertPair( &vertPairs[5], pPhysCloth, r + 1, c + 1 );
        GetVertPair( &vertPairs[6], pPhysCloth, r + 1, c     );
        GetVertPair( &vertPairs[7], pPhysCloth, r + 1, c - 1 );

        for ( int n = 0; n < 8; ++n )
        {
          D3DXVECTOR3 springForce( 0.f, 0.f, 0.f );
          if ( vertPairs[n].pVert )
          {
            D3DXVECTOR3 dir( vertPairs[n].pVert->Pos - vert.Pos );
            float dist = D3DXVec3Length( &dir );
            float fMag = -pPhysCloth->k * ( D3DXVec3Length( &dir ) - pPhysCloth->restDist );
            D3DXVec3Normalize( &dir, &dir );
            dir *= -fMag;
            if ( fabs( fMag ) > pPhysCloth->minForce )
              springForce += dir;

            D3DXVECTOR3 damp( pPhysCloth->k * ( vertInfo.vel /*+  frameStep * ( vertInfo.force / pPhysCloth->mass )*/ ) );
            if ( D3DXVec3Length( &damp ) > pPhysCloth->minForce )
              springForce -= damp;
          }
          //D3DXVECTOR3 damp( pPhysCloth->k * ( vertInfo.vel + frameStep * 
          //                                   ( ( vertInfo.force + springForce ) / pPhysCloth->mass ) ) );
          //vertInfo.force += ( springForce - damp );
          vertInfo.force += springForce;
        }
        vertInfo.accel = vertInfo.force / pPhysCloth->mass;
        vertInfo.force = D3DXVECTOR3( 0.f, 0.f, 0.f );
      }
    }
    // ...
  }
}

void Physics::GetVertPair( VertPair *pOut, PhysCloth *pPhysCloth, int r, int c )
{
  if ( r < 0 || c < 0 || r >= pPhysCloth->resY || c >= pPhysCloth->resX )
  {
    pOut->pVert     = NULL;
    pOut->pVertInfo = NULL;
    return;
  }

  pOut->pVert     = &pPhysCloth->pCloth->GetVertex( r, c );
  pOut->pVertInfo = &pPhysCloth->GetVertexInfo( r, c );
}
