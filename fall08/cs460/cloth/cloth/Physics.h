#pragma once

#include "Cloth.h"

#define PHYSICS Physics::Instance()

struct PhysCloth;
struct VertPair;

class Physics
{
  public:
    static Physics *Instance( void );
    friend class Cloth;

    void Initialize( void );
    void Update( void );

  private:
    void UpdateCloths( void );

    D3DXVECTOR3 m_gravity;

    static void GetVertPair( VertPair *vpOut, PhysCloth *pPhysCloth, int r, int c );

  private:
    Physics( void );
    Physics( const Physics &rhs );
    void operator =( const Physics &rhs );
    ~Physics( void );
};

struct VertInfo
{
  VertInfo( void )
    : accel( 0.f, 0.f, 0.f )
    , vel( 0.f, 0.f, 0.f )
    , force( 0.f, 0.f, 0.f )
    , locked( false )
  {}

  D3DXVECTOR3 accel, vel, force;
  bool        locked;
};

struct VertPair
{
  VertPair( ColoredVertex *_pVert = NULL, VertInfo *_pVertInfo = NULL )
    : pVert( _pVert ), pVertInfo( _pVertInfo ) {}

  ColoredVertex *pVert;
  VertInfo      *pVertInfo;
};

typedef std::vector< VertPair > VertPairVec;

struct PhysCloth : AutoList< PhysCloth >
{
  PhysCloth( Cloth *const _pCloth, float _k = 40.0f, float _mass = 10.f, float _minForce = .15f )
    : pCloth( _pCloth )
    , resX( pCloth->GetResX() )
    , resY( pCloth->GetResY() )
    , k( _k )
    , restDist( pCloth->GetWidth() / (float)pCloth->GetResX() )
    , mass( _mass ) 
    , minForce( _minForce )
    , verts( pCloth->GetNumNodes() )
  {
  }

  VertInfo &GetVertexInfo( int r, int c )
  {
    return verts[r * resX + c];
  }

  Cloth *const  pCloth;
  const int     resX, resY;
  const float   k, restDist, mass, minForce;

  std::vector< VertInfo > verts;
};
