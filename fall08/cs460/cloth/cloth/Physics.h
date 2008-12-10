#pragma once

#include "Cloth.h"

#define PHYSICS Physics::Instance()

enum CLOTH_ANCHOR
{
  CA_TL  = 0x01,
  CA_TR  = 0x02,
  CA_BR  = 0x04,
  CA_BL  = 0x08,
  CA_ALL = 0x0F,
};

struct PhysCloth;
struct VertPair;

class Physics
{
  public:
    static Physics *Instance( void );
    friend class Cloth;

    void Initialize( void );
    void Update( void );

    bool applyGravity;

  private:
    void UpdateCloths( void );

    D3DXVECTOR3 m_gravity;

    static void GetVertPair( VertPair *pOut, PhysCloth *pPhysCloth, int r, int c );
    static void GetVertPair( VertPair *pOut, PhysCloth *pPhysCloth, VertVec *pVerts, int r, int c );

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

  void Reset( void )
  {
    accel   = D3DXVECTOR3( 0.f, 0.f, 0.f );
    vel     = D3DXVECTOR3( 0.f, 0.f, 0.f );
    force   = D3DXVECTOR3( 0.f, 0.f, 0.f );
    locked  = false;
  }

  D3DXVECTOR3 accel, vel, force, relVel;
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
  PhysCloth( Cloth *const _pCloth, float _k = 80.0f, float _mass = 10.f, float _minForce = .005f )
    : pCloth( _pCloth )
    , resX( pCloth->GetResX() )
    , resY( pCloth->GetResY() )
    , k( _k )
    , restDist( pCloth->GetWidth() / ( (float)pCloth->GetResX() - 1.f ) )
    , mass( _mass ) 
    , minForce( _minForce )
    , verts( pCloth->GetNumNodes() )
  {
  }

  void ToggleAnchors( CLOTH_ANCHOR anchors, bool state )
  {
    if ( !this ) return;
    if ( anchors & CA_TL )
      GetVertexInfo( resY - 1,        0 ).locked = state;
    if ( anchors & CA_TR )
      GetVertexInfo( resY - 1, resX - 1 ).locked = state;
    if ( anchors & CA_BR )
      GetVertexInfo(        0, resX - 1 ).locked = state;
    if ( anchors & CA_BL )
      GetVertexInfo(        0,        0 ).locked = state;
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
