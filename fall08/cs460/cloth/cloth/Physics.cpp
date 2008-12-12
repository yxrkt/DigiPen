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
  applyGravity = true;
}

Physics::~Physics( void )
{
}

void Physics::Initialize( void )
{
  m_gravity = D3DXVECTOR3( 0.f, -0.5f, 0.f );
}

void Physics::Update( void )
{
  UpdateCloths();
}

void Physics::UpdateCloths( void )
{
  static DWORD lastTick = timeGetTime();
  DWORD tick = timeGetTime();
  float frameStep = min( (float)( tick - lastTick ) / 1000.f, 1.f / 60.f );
  lastTick = tick;

  VertPair vertPairs[8]; // eight neighbors (l, tl, t, tr, r, br, b, bl)

  //*
  FOR_EACH_AUTO( PhysCloth, pPhysCloth )
  {
    Cloth *pCloth = (Cloth *)pPhysCloth->pCloth;
    int resY = pCloth->GetResY(), resX = pCloth->GetResX();

    float pctOfB = .25f;
    float b = pctOfB * sqrt( 4.f * pPhysCloth->mass * pPhysCloth->k );

    VertVec vecPrevVerts( resX * resY );
    std::copy( pCloth->GetVertices().begin(), pCloth->GetVertices().end(), vecPrevVerts.begin() );

    float restDistLong = sqrt( 2 * pPhysCloth->restDist * pPhysCloth->restDist );

    for ( int r = 0; r < resY; ++r )
    {
      for ( int c = 0; c < resX; ++c )
      {
        ColoredVertex &vert     = pCloth->GetVertex( r, c );
        VertInfo      &vertInfo = pPhysCloth->GetVertexInfo( r, c );

        if ( vertInfo.locked ) continue;

        GetVertPair( &vertPairs[0], pPhysCloth, &vecPrevVerts, r    , c - 1 );
        GetVertPair( &vertPairs[1], pPhysCloth, &vecPrevVerts, r - 1, c - 1 );
        GetVertPair( &vertPairs[2], pPhysCloth, &vecPrevVerts, r - 1, c     );
        GetVertPair( &vertPairs[3], pPhysCloth, &vecPrevVerts, r - 1, c + 1 );
        GetVertPair( &vertPairs[4], pPhysCloth, &vecPrevVerts, r    , c + 1 );
        GetVertPair( &vertPairs[5], pPhysCloth, &vecPrevVerts, r + 1, c + 1 );
        GetVertPair( &vertPairs[6], pPhysCloth, &vecPrevVerts, r + 1, c     );
        GetVertPair( &vertPairs[7], pPhysCloth, &vecPrevVerts, r + 1, c - 1 );

        D3DXVECTOR3 springForce( 0.f, 0.f, 0.f );
        for ( int n = 0; n < 8; ++n )
        {
          if ( vertPairs[n].pVert )
          {
            float restDist = ( ( n % 2 ) ? restDistLong : pPhysCloth->restDist );
            D3DXVECTOR3 dir( vertPairs[n].pVert->Pos - vert.Pos );
            float fMag = -pPhysCloth->k * ( D3DXVec3Length( &dir ) - restDist );
            D3DXVec3Normalize( &dir, &dir );
            dir *= -fMag;
            if ( fabs( fMag ) >= pPhysCloth->minForce )
              springForce += dir;
          }
        }
        // dampen here
        springForce -= b * ( vertInfo.vel );

        vertInfo.accel   = ( springForce / pPhysCloth->mass );
        if ( applyGravity )
          vertInfo.accel += m_gravity;
        vertInfo.vel    += ( frameStep * vertInfo.accel );
        vertInfo.force   = D3DXVECTOR3( 0.f, 0.f, 0.f );
        D3DXVECTOR3 newPos = vert.Pos + vertInfo.vel;

        FOR_EACH_AUTO( Sphere, pSphere )
        {
          D3DXVECTOR3 center( pSphere->GetCenter() );
          D3DXVECTOR3 pc( center - vert.Pos ), u, pq( newPos - vert.Pos ), v;
          if ( D3DXVec3Dot( &pc, &pq ) > 0.f )
          {
            float radius = pSphere->GetRadius() + 5.f;
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
                vertInfo.vel = D3DXVECTOR3( 0.f, 0.f, 0.f );
              }
            }
          }
        }
        vert.Pos = newPos;
      }
    }
    //*/
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

void Physics::GetVertPair( VertPair *pOut, PhysCloth *pPhysCloth, VertVec *pVerts, int r, int c )
{
  if ( r < 0 || c < 0 || r >= pPhysCloth->resY || c >= pPhysCloth->resX )
  {
    pOut->pVert     = NULL;
    pOut->pVertInfo = NULL;
    return;
  }

  pOut->pVert     = &(*pVerts)[pPhysCloth->resX * r + c];
  pOut->pVertInfo = &pPhysCloth->GetVertexInfo( r, c );
}
