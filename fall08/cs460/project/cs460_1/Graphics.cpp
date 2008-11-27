#include "Graphics.h"
#include "ASSERT.h"

// =============================================================================
// ! Graphics constructor
// =============================================================================
Graphics::Graphics()
: MainCam( mainCam )
, Ready( ready )
, StaticModels( staticModels )
, AnimatedModels( animModels )
, ready( false )
{
}

// =============================================================================
// ! Graphics destructor
// =============================================================================
Graphics::~Graphics()
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
  ASSERT( pDevice, "Initializing Direct3D failed." );

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
void Graphics::Update()
{
  HRESULT hr;

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
    i->FrameMove( timeGetTime(), i->GetWorldTrans() );
    i->Render( AnimatedModel::RENDER_ALL );
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
void Graphics::LoadStaticModel( const std::string &file )
{
  StaticModel mesh( pDevice );
  mesh.Load( file );
  staticModels.push_back( mesh );

  float angle = 2.f * D3DX_PI / 3.f;
  float dist  = 3.f * 400.f;

  mainCam.lookAt  = D3DXVECTOR3( 0.f, 250.f, 0.f );
  mainCam.eye     = D3DXVECTOR3( dist * cos( angle ), 0.f, dist * sin( angle ) );
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
void Graphics::SetupMatrices()
{
  HRESULT hr;

  D3DXMATRIXA16 mtxWorld;
  D3DXMatrixIdentity( &mtxWorld );
  hr = pDevice->SetTransform( D3DTS_WORLD, &mtxWorld );
  ASSERT( hr == S_OK, "Setting world transform failed." );

  D3DXMATRIXA16 mtxView;
  D3DXMatrixLookAtLH( &mtxView, &mainCam.eye, &mainCam.lookAt, &mainCam.up );
  hr = pDevice->SetTransform( D3DTS_VIEW, &mtxView );
  ASSERT( hr == S_OK, "Setting view transform failed." );

  RECT rcWnd;
  GetClientRect( hWnd, &rcWnd );
  float aspect = fabs( (float)( rcWnd.left - rcWnd.right ) / (float)( rcWnd.bottom - rcWnd.top ) );

  D3DXMATRIXA16 mtxProj;
  D3DXMatrixPerspectiveFovLH( &mtxProj, D3DX_PI / 4.f, aspect, 1.f, 5000.f );
  hr = pDevice->SetTransform( D3DTS_PROJECTION, &mtxProj );
  ASSERT( hr == S_OK, "Setting perspective transform failed." );
}

// =============================================================================
// ! Releases Direct3D objects
// =============================================================================
void Graphics::Cleanup()
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
  std::stringstream info;

  DWORD tick = timeGetTime();
  info << "FPS: " << 1000.f/ (float)( tick - lastTick ) << std::endl << std::endl;
  lastTick = tick;

  AddModelInfo addMeshInfo( &info );
  std::for_each( animModels.begin(), animModels.end(), addMeshInfo );
  pFont->DrawText( NULL, info.str().c_str(), -1, &rcPos, DT_NOCLIP, D3DCOLOR_XRGB( 255, 0, 255 ) );
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
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS );
    lineVerts[1] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS );
    lineVerts[1] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS );
    AddLine( lineVerts );
    // the x
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS );
    AddLine( lineVerts );
    lineVerts[0] = ColoredVertex( i->x - CP_RADIUS, i->y + CP_HEIGHT, i->z + CP_RADIUS );
    lineVerts[1] = ColoredVertex( i->x + CP_RADIUS, i->y + CP_HEIGHT, i->z - CP_RADIUS );
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

bool Graphics::CCD( PFrameVec *pFramesOut, const PFrameVec *pFramesIn, 
                    const D3DXVECTOR3 &dest, const FloatVec *pWeights ) const
{
  pWeights;

  D3DXVECTOR3 origin( 0.f, 0.f, 0.f );
  size_t nJoints = pFramesIn->size(), nLast = nJoints - 1;
  std::vector< D3DXVECTOR3 > joints( nJoints );
  for ( size_t i = 0; i < nJoints; ++i )
    D3DXVec3TransformCoord( &joints[i], &origin, &( ( *pFramesIn )[i]->matCombined ) );

  float lastDist = FLT_MAX;

  while ( lastDist > CCD_SUCCESS )
  {
    for ( size_t i = nLast - 1; i >= 0; --i )
    {
      // find vectors
      D3DXVECTOR3 l1( joints[nLast] - joints[i] );
      D3DXVECTOR3 l2( dest - joints[i] );

      // find angle of rotation
      float angle = acos( D3DXVec3Dot( &l1, &l2 ) / ( D3DXVec3Length( &l1 ) * D3DXVec3Length( &l2 ) ) );

      // find axis of rotation
      // reverse this if necessary
      D3DXVECTOR3 axis;
      D3DXVec3Cross( &axis, &l1, &l2 );
      D3DXVec3Normalize( &axis, &axis );

      // rotate
      D3DXMATRIX matRot;
      D3DXMatrixRotationAxis( &matRot, &axis, angle );
      for ( size_t j = i + 1; j < nJoints; ++j )
        D3DXVec3TransformCoord( &joints[j], &joints[j], &matRot );
    }

    D3DXVECTOR3 difVec( dest - joints[nLast] );
    float dist = D3DXVec3Length( &difVec );

    if ( dist > CCD_SUCCESS && ( dist - lastDist ) < CCD_FAIL )
      return false;
    else
      lastDist = dist;
  }

  return true;
}
