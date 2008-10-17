#include "Graphics.h"
#include "ASSERT.h"

// =============================================================================
// ! Graphics constructor
// =============================================================================
Graphics::Graphics()
: MainCam( mainCam )
, Ready( ready )
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

  for ( StaticMeshList::iterator i = staticMeshes.begin(); i != staticMeshes.end(); ++i )
  {
    i->Render();
  }

  for ( AnimatedMeshList::iterator i = animMeshes.begin(); i != animMeshes.end(); ++i )
  {
    D3DXMATRIX mtxWorld;
    pDevice->GetTransform( D3DTS_WORLD, &mtxWorld );
    i->FrameMove( timeGetTime(), mtxWorld );

    i->Render();

    i->DrawBones();
  }

  hr = pDevice->EndScene();
  ASSERT( hr == S_OK, "EndScene failed." );

  pDevice->Present( NULL, NULL, NULL, NULL );
}

// =============================================================================
// ! Loads a non-animating mesh from a .x file
// =============================================================================
void Graphics::LoadStaticMesh( const std::string &file )
{
  StaticMesh mesh( pDevice );
  mesh.Load( file );
  staticMeshes.push_back( mesh );
}

// =============================================================================
// ! Loads a bone-animated mesh from a .x file
// =============================================================================
void Graphics::LoadAnimatedMesh( const std::string &file )
{
  AnimatedMesh mesh( pDevice );
  mesh.Load( file );
  animMeshes.push_back( mesh );

  float angle = 2.f * D3DX_PI / 3.f;
  float dist  = 3.f * mesh.BS.radius;

  mainCam.lookAt  = mesh.BS.center;
  mainCam.eye     = D3DXVECTOR3( dist * cos( angle ), 0.f, dist * sin( angle ) );
}

// =============================================================================
// ! Increment or decrement animation speeds
// =============================================================================
void Graphics::IncDecAnimSpeed( bool inc )
{
  std::for_each( animMeshes.begin(), animMeshes.end(), inc ? IncAnimSpeed : DecAnimSpeed );
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
}

// =============================================================================
// ! Draws a line
// =============================================================================
void Graphics::DrawLine( D3DXVECTOR3 p, D3DXVECTOR3 q )
{
  ColoredVertex line[2] =
  {
    ColoredVertex( p.x, p.y, p.z, D3DCOLOR_XRGB( 255, 0, 0 ) ),
    ColoredVertex( q.x, q.y, q.z, D3DCOLOR_XRGB( 255, 0, 0 ) )
  };

  LPDIRECT3DVERTEXBUFFER9 pVB;
  LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
  pDevice->CreateVertexBuffer( sizeof( line ), 0, D3DFVF_COLOREDVERTEX, D3DPOOL_DEFAULT, &pVB, NULL );
  pVB->Lock( 0, 2 * sizeof( line ), (void **)&pVertexBuffer, 0 );
  memcpy( pVertexBuffer, line, sizeof( line ) );
  pVB->Unlock();

  pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  pDevice->SetStreamSource( 0, pVB, 0, sizeof( ColoredVertex ) );
  pDevice->SetFVF( D3DFVF_COLOREDVERTEX );
  pDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
  pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void Graphics::DisplayInfo( void )
{
  static DWORD lastTick = 0;

  RECT rcPos = { 10, 10, 0, 0 };
  std::stringstream info;

  DWORD tick = timeGetTime();
  info << "FPS: " << 1000.f/ (float)( tick - lastTick ) << std::endl << std::endl;
  lastTick = tick;

  AddMeshInfo addMeshInfo( &info );
  std::for_each( animMeshes.begin(), animMeshes.end(), addMeshInfo );
  pFont->DrawText( NULL, info.str().c_str(), -1, &rcPos, DT_NOCLIP, D3DCOLOR_XRGB( 255, 0, 255 ) );
}
