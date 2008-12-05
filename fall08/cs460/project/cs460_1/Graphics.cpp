#include "Graphics.h"
#include "ASSERT.h"
#include "GlobalTime.h"

extern AnimatedModel::RENDER_FLAG renderState;

Graphics *Graphics::Instance( void )
{
  static Graphics graphics;
  return &graphics;
}

// =============================================================================
// ! Graphics constructor
// =============================================================================
Graphics::Graphics( void )
: MainCam( mainCam )
, Ready( ready )
, StaticModels( staticModels )
, AnimatedModels( animModels )
, ready( false )
, paused( false )
, animCallback( NULL )
{
}

// =============================================================================
// ! Graphics destructor
// =============================================================================
Graphics::~Graphics( void )
{
}

// =============================================================================
// ! Initializes graphics
// =============================================================================
void Graphics::Initialize( HWND hWndApp )
{
  HRESULT hr;

  hWnd = hWndApp;
  bgColor = D3DCOLOR_XRGB( 0, 0, 0 );

  pD3D = Direct3DCreate9( D3D_SDK_VERSION );
  ASSERT( pD3D, "Initializing Direct3D failed." );

  D3DPRESENT_PARAMETERS params;
  ZeroMemory( &params, sizeof( params ) );

  params.Windowed               = true;
  params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  params.BackBufferFormat       = D3DFMT_UNKNOWN;
  params.EnableAutoDepthStencil = TRUE;
  params.AutoDepthStencilFormat = D3DFMT_D16;

  hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                           D3DCREATE_HARDWARE_VERTEXPROCESSING,
                           &params, &pDevice );
  ASSERT( hr == S_OK, "Creating Direct3D device failed." ); 

  hr = pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
  ASSERT( hr == S_OK, "Enabling z-buffer failed." );

  hr = pDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
  ASSERT( hr == S_OK, "Turning on ambient lighting failed." );

  mainCam.eye     = D3DXVECTOR3( 0.f, 3.f, -5.f );
  mainCam.lookAt  = D3DXVECTOR3( 0.f, 0.f, 0.f );
  mainCam.up      = D3DXVECTOR3( 0.f, 1.f, 0.f );

  hr = D3DXCreateFont( pDevice, 16, 0, FW_NORMAL, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New", &pFont );
  ASSERT( hr == S_OK, "Creating font failed." );

  CreateSunLight();

  D3DXCreateMatrixStack( 0, &pMatrixStack );
  pMatrixStack->Push();
  pMatrixStack->LoadIdentity();

  ready = true;
}

// =============================================================================
// ! Updates graphics
// =============================================================================
void Graphics::Update( void )
{
  HRESULT hr;

  DWORD curTick = timeGetTime();
  static DWORD lastTick = curTick;
  if ( !paused )
    curTime += ( curTick - lastTick );
  lastTick = curTick;

  hr = pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColor, 1.f, 0 );
  ASSERT( hr == S_OK, "Clearing the device failed." );

  hr = pDevice->BeginScene();
  ASSERT( hr == S_OK, "BeginScene failed." );

  DisplayInfo();

  SetupMatrices();

  for ( StaticModelList::iterator i = staticModels.begin(); i != staticModels.end(); ++i )
  {
    i->Render();
  }

  for ( AnimatedModelList::iterator i = animModels.begin(); i != animModels.end(); ++i )
  {
    i->FrameMove( curTime, i->GetWorldTrans(), animCallback );
    i->Render( renderState );
  }

  DrawControlPoints();

  pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  pDevice->SetFVF( D3DFVF_COLOREDVERTEX );

  if ( !quadPrimitives.empty() )
  {
    //pDevice->SetTexture( 0, pFloorTex );
    pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    pDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, (UINT)quadPrimitives.size() / 3, 
                              &quadPrimitives[0], sizeof( ColoredVertex ) );
    quadPrimitives.clear();
    pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
  }

  if ( !linePrimitives.empty() )
  {
    pDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)linePrimitives.size() / 2, 
                              &linePrimitives[0], sizeof( ColoredVertex ) );
    linePrimitives.clear();
  }

  if ( !polylinePrimitives.empty() )
  {
    pDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, (UINT)polylinePrimitives.size() - 1, 
                              &polylinePrimitives[0], sizeof( ColoredVertex ) );
    polylinePrimitives.clear();
  }

  pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

  hr = pDevice->EndScene();
  ASSERT( hr == S_OK, "EndScene failed." );

  pDevice->Present( NULL, NULL, NULL, NULL );
}

// =============================================================================
// ! Loads a non-animating mesh from a .x file
// =============================================================================
void Graphics::LoadStaticModel( const std::string &file, const D3DXVECTOR3 &pos, float scale )
{
  StaticModel mesh( pDevice );
  mesh.Load( std::string( ASSETS_DIR ) + file );
  mesh.Pos    = pos;
  mesh.Scale  = scale;
  staticModels.push_back( mesh );
}

// =============================================================================
// ! Loads a bone-animated mesh from a .x file
// =============================================================================
void Graphics::LoadAnimatedModel( const std::string &file )
{
  AnimatedModel mesh( pDevice );
  mesh.Load( file );
  animModels.push_back( mesh );
}

// =============================================================================
// ! Increment or decrement animation speeds
// =============================================================================
void Graphics::IncDecAnimSpeed( bool inc )
{
  std::for_each( animModels.begin(), animModels.end(), inc ? IncAnimSpeed : DecAnimSpeed );
}

// =============================================================================
// ! Prepares the world, view, and projection matrices
// =============================================================================
void Graphics::SetupMatrices( void )
{
  HRESULT hr;

  D3DXMATRIXA16 matWorld;
  D3DXMatrixIdentity( &matWorld );
  hr = pDevice->SetTransform( D3DTS_WORLD, &matWorld );
  ASSERT( hr == S_OK, "Setting world transform failed." );

  D3DXMATRIXA16 matView;
  D3DXMatrixLookAtLH( &matView, &mainCam.eye, &mainCam.lookAt, &mainCam.up );
  hr = pDevice->SetTransform( D3DTS_VIEW, &matView );
  ASSERT( hr == S_OK, "Setting view transform failed." );

  RECT rcWnd;
  GetClientRect( hWnd, &rcWnd );
  float aspect = fabs( (float)( rcWnd.left - rcWnd.right ) / (float)( rcWnd.bottom - rcWnd.top ) );

  D3DXMATRIXA16 matProj;
  D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4.f, aspect, 1.f, 5000.f );
  hr = pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
  ASSERT( hr == S_OK, "Setting perspective transform failed." );
}

// =============================================================================
// ! Releases Direct3D objects
// =============================================================================
void Graphics::Cleanup( void )
{
  SAFE_RELEASE( pD3D );
  SAFE_RELEASE( pDevice );
  SAFE_RELEASE( pFont );
  SAFE_RELEASE( pMatrixStack );
}

void Graphics::AddQuad( const ColoredVertex *verts )
{
  for ( size_t i = 0; i < 3; ++i )
    quadPrimitives.push_back( verts[i] );
  for ( size_t i = 2; i < 5; ++i )
    quadPrimitives.push_back( verts[i == 4 ? 0 : i] );
}

void Graphics::DisplayInfo( void )
{
  static DWORD lastTick = 0;

  RECT rcPos = { 10, 10, 0, 0 };
  static std::stringstream info;

  DWORD tick = timeGetTime();
  info << "FPS: " << 1000.f / (float)( tick - lastTick ) << std::endl;
  lastTick = tick;

  info << infoStrings.str();
  info << std::endl;

  AddModelInfo addMeshInfo( &info );
  std::for_each( animModels.begin(), animModels.end(), addMeshInfo );
  pFont->DrawText( NULL, info.str().c_str(), -1, &rcPos, DT_NOCLIP, D3DCOLOR_XRGB( 255, 0, 255 ) );

  infoStrings.str( "" );
  info.str( "" );
}

void Graphics::CreateSunLight( void )
{
  D3DLIGHT9 light;
  ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
  light.Type       = D3DLIGHT_DIRECTIONAL;
  light.Diffuse.r  = 0.3f;
  light.Diffuse.g  = 0.3f;
  light.Diffuse.b  = 0.3f;
  light.Diffuse.a  = 1.0f;
  light.Range      = 100.0f;

  // Create a direction for our light - it must be normalized  
  D3DXVECTOR3 vecDir;
  vecDir = D3DXVECTOR3( 0.5f, -0.5f, -0.3f );
  D3DXVec3Normalize( (D3DXVECTOR3 *)&light.Direction, &vecDir );

  // Tell the device about the light and turn it on
  pDevice->SetLight( 0, &light );
  pDevice->LightEnable( 0, TRUE ); 
}

LPDIRECT3DDEVICE9 Graphics::GetDevice( void ) const
{
  return pDevice;
}

D3DXVECTOR3 Graphics::IsectGroundPlane( int x, int y )
{
  D3DVIEWPORT9 viewPort;
  pDevice->GetViewport( &viewPort );
  D3DXMATRIX matProj, matView, matWorld;
  pDevice->GetTransform( D3DTS_PROJECTION, &matProj );
  pDevice->GetTransform( D3DTS_VIEW, &matView );
  pDevice->GetTransform( D3DTS_WORLD, &matWorld );
  D3DXVECTOR3 nearPt, screen( (float)x, (float)y, 0.f );
  D3DXVec3Unproject( &nearPt, &screen, &viewPort, &matProj, &matView, &matWorld );

  D3DXVECTOR3 isect;
  D3DXPLANE groundPlane( 0.f, 1.f, 0.f, 0.f );
  D3DXPlaneIntersectLine( &isect, &groundPlane, &mainCam.eye, &nearPt );

  return isect;
}

void Graphics::DrawControlPoints( void )
{
  ColoredVertex lineVerts[2];
  for ( VertVec::iterator i = controlPoints.begin(); i != controlPoints.end(); ++i )
  {
    // the box
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS, i->color );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS, i->color );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS, i->color );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS, i->color );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS, i->color );
    lineVerts[1] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS, i->color );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS, i->color );
    lineVerts[1] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS, i->color );
    AddLine( lineVerts );
    // the x
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS, i->color );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS, i->color );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS, i->color );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS, i->color );
    AddLine( lineVerts );
  }
}

void Graphics::AddLine( const ColoredVertex *verts )
{
  linePrimitives.push_back( verts[0] );
  linePrimitives.push_back( verts[1] );
}

void Graphics::AddLine( const ColoredVertex &begin, const ColoredVertex &end )
{
  linePrimitives.push_back( begin );
  linePrimitives.push_back( end );
}

void Graphics::AddPolyline( const ColoredVertex *verts, size_t nVerts, bool closed )
{
  for ( size_t i = 0; i < nVerts; ++i )
  {
    polylinePrimitives.push_back( verts[i] );
  }
  if ( closed )
  {
    polylinePrimitives.push_back( verts[0] );
  }
}

bool Graphics::CCD( MatrixVec *pMatricesOut, const PFrameVec *pFramesIn, 
                    const D3DXVECTOR3 &dest, const FloatVec *pConstraints ) const
{
  D3DXVECTOR3 origin( 0.f, 0.f, 0.f );
  int nJoints = (int)pFramesIn->size(), nLast = nJoints - 1;

  std::vector< D3DXVECTOR3 > joints( nJoints );
  for ( int i = 0; i < nJoints; ++i )
    D3DXVec3TransformCoord( &joints[i], &origin, &( ( *pFramesIn )[i]->matCombined ) );

  std::vector< D3DXVECTOR3 > jointsL( nJoints );
  for ( int i = 0; i < nJoints; ++i )
    D3DXVec3TransformCoord( &jointsL[i], &origin, &( ( *pFramesIn )[i]->TransformationMatrix ) );

  pMatricesOut->reserve( nLast );
  for ( int i = 0; i < nLast; ++i )
  {
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );
    pMatricesOut->push_back( matIdentity );
  }

  float lastDist = FLT_MAX;

  int nTries = 0;
  while ( lastDist > CCD_SUCCESS )
  {
    for ( int i = nLast - 1; i >= 0; --i )
    {
      // find vectors
      D3DXVECTOR3 l1( joints[nLast] - joints[i] );
      D3DXVECTOR3 l2( dest - joints[i] );

      // find angle of rotation
      float angle = acos( D3DXVec3Dot( &l1, &l2 ) / ( D3DXVec3Length( &l1 ) * D3DXVec3Length( &l2 ) ) );
      if ( pConstraints )
      {
        angle = min( ( *pConstraints )[i * 2 + 1], angle );
        angle = max( ( *pConstraints )[i * 2    ], angle );
      }

      // find axis of rotation
      D3DXVECTOR3 axis;
      D3DXVec3Cross( &axis, &l1, &l2 );
      D3DXVec3Normalize( &axis, &axis );

      // rotate
      D3DXMATRIX matRot;
      D3DXMatrixRotationAxis( &matRot, &axis, angle );
      for ( int j = i + 1; j < nJoints; ++j )
      {
        joints[j] -= joints[i];
        D3DXVec3TransformCoord( &joints[j], &joints[j], &matRot );
        joints[j] += joints[i];
      }

      // get rotation matrix for frame
      int nCurFrame = i + 1 - 1;

      D3DXMATRIX &matCombined = ( *pFramesIn )[nCurFrame]->matCombined;

      float det;
      D3DXMATRIX matWorldInverse;
      D3DXMatrixInverse( &matWorldInverse, &det, &matCombined );

      D3DXVECTOR3 v0, v1;
      D3DXVec3TransformCoord( &v0, &origin, &matWorldInverse );
      D3DXVec3TransformCoord( &v1, &axis, &matWorldInverse );
      D3DXVECTOR3 axisLocal( v1 - v0 );
      D3DXVec3Normalize( &axisLocal, &axisLocal );

      D3DXMATRIX matRotLocal;
      D3DXMatrixRotationAxis( &matRotLocal, &axisLocal, angle );

      D3DXVec3TransformCoord( &v0, &origin, &matCombined );
      D3DXVec3TransformCoord( &v1, &axisLocal, &matCombined );
      D3DXVECTOR3 axisWorld( v1 - v0 );
      D3DXVec3Normalize( &axisWorld, &axisWorld );

      D3DXMatrixMultiply( &( *pMatricesOut )[i], &( *pMatricesOut )[i], &matRotLocal );

      D3DXVECTOR3 difVec( dest - joints[nLast] );
      float dist = D3DXVec3Length( &difVec );
      if ( dist < CCD_SUCCESS ) return true;
    }

    D3DXMATRIX matParent( pFramesIn->front()->matCombined );
    for ( int i = 1; i < nJoints; ++i )
    {
      D3DXMatrixMultiply( &( *pFramesIn )[i]->matCombined, &( *pFramesIn )[i]->TransformationMatrix, &( *pMatricesOut )[i - 1] );
      D3DXMatrixMultiply( &( *pFramesIn )[i]->matCombined, &( *pFramesIn )[i]->matCombined, &matParent );
      matParent = ( *pFramesIn )[i]->matCombined;
    }

    /* draw arm in yellow (after full ccd)
    D3DCOLOR yellow = D3DCOLOR_XRGB( 255, 255, 0 );
    for ( int i = 1; i < nJoints; ++i )
    {
      int j = i - 1;
      ColoredVertex p0( joints[j].x, joints[j].y, joints[j].z, yellow );
      ColoredVertex p1( joints[i].x, joints[i].y, joints[i].z, yellow );
      ( (Graphics *)this )->AddLine( p0, p1 );
    }
    //*/

    D3DXVECTOR3 difVec( dest - joints[nLast] );
    float dist = D3DXVec3Length( &difVec );

    if ( ++nTries > 100 )
      return false;

    lastDist = dist;
  }

  /* draw arm in red (after ccd)
  D3DCOLOR red = D3DCOLOR_XRGB( 255, 0, 0 );
  for ( int i = 1; i < nJoints; ++i )
  {
    int j = i - 1;
    ColoredVertex p0( joints[j].x, joints[j].y, joints[j].z, red );
    ColoredVertex p1( joints[i].x, joints[i].y, joints[i].z, red );
    ( (Graphics *)this )->AddLine( p0, p1 );
  }
  //*/

  return true;
}

void Graphics::PauseAnims( void )
{
  paused = true;
}

void Graphics::PlayAnims( void )
{
  paused = false;
}

bool Graphics::IsPaused( void ) const
{
  return paused;
}

LPDIRECT3DDEVICE9 Graphics::GetDevice( void )
{
  return pDevice;
}

void Graphics::WriteText( const std::string &text )
{
  infoStrings << text << std::endl;
}
