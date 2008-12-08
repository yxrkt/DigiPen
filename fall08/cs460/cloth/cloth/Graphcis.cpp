#include "Graphics.h"
#include "CS460Proj.h"
#include "Cloth.h"


Graphics *Graphics::Instance( void )
{
  static Graphics graphics;
  return &graphics;
}

Graphics::Graphics( void )
{
}

Graphics::~Graphics( void )
{
  SAFE_RELEASE( pD3D );
  SAFE_RELEASE( pD3DDevice );
  SAFE_RELEASE( pD3DFont );
}

bool Graphics::Initialize( void )
{
  HRESULT hr;

  bgColor = D3DCOLOR_XRGB( 0, 0, 0 );

  pD3D = Direct3DCreate9( D3D_SDK_VERSION );
  assert( pD3D );

  D3DPRESENT_PARAMETERS params;
  ZeroMemory( &params, sizeof( params ) );

  params.Windowed               = true;
  params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  params.BackBufferFormat       = D3DFMT_UNKNOWN;
  params.EnableAutoDepthStencil = TRUE;
  params.AutoDepthStencilFormat = D3DFMT_D16;

  hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, CS460PROJ->GetWindow(),
                           D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &pD3DDevice );
  ASSERTHR( hr );

  hr = pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
  ASSERTHR( hr );

  hr = pD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 255, 255, 255 ) );
  ASSERTHR( hr );

  hr = D3DXCreateFont( pD3DDevice, 16, 0, FW_NORMAL, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New", &pD3DFont );
  ASSERTHR( hr );

  CreateSunLight();

  return true;
}

void Graphics::CreateSunLight( void )
{
  D3DLIGHT9 light;
  ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
  light.Type       = D3DLIGHT_DIRECTIONAL;
  light.Diffuse.r  = 1.f;
  light.Diffuse.g  = 1.f;
  light.Diffuse.b  = 1.f;
  light.Diffuse.a  = 1.f;
  light.Range      = 10000.0f;

  D3DXVECTOR3 vecDir;
  vecDir = D3DXVECTOR3( 0.5f, -1.f, 0.5f );
  D3DXVec3Normalize( (D3DXVECTOR3 *)&light.Direction, &vecDir );

  pD3DDevice->SetLight( 0, &light );
  pD3DDevice->LightEnable( 0, TRUE ); 
}

void Graphics::DrawPrimitives( void )
{
  pD3DDevice->SetFVF( D3DFVF_COLOREDVERTEX );
  //pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

  if ( !trianglePrimitives.empty() )
  {
    pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, (UINT)trianglePrimitives.size() / 3, 
                                 &trianglePrimitives[0], sizeof( ColoredVertex ) );
    trianglePrimitives.clear();
    pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
  }

  if ( !linePrimitives.empty() )
  {
    pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)linePrimitives.size() / 2, 
                                 &linePrimitives[0], sizeof( ColoredVertex ) );
    linePrimitives.clear();
  }

  pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void Graphics::SetupMatrices( void )
{
  HRESULT hr;

  D3DXMATRIX matWorld;
  D3DXMatrixIdentity( &matWorld );
  hr = pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
  ASSERTHR( hr );

  D3DXMATRIX matView;
  D3DXMatrixLookAtLH( &matView, &mainCam.Eye, &mainCam.LookAt, &mainCam.Up );
  hr = pD3DDevice->SetTransform( D3DTS_VIEW, &matView );
  ASSERTHR( hr );

  RECT rcWnd;
  GetClientRect( CS460PROJ->GetWindow(), &rcWnd );
  float aspect = fabs( (float)( rcWnd.left - rcWnd.right ) / (float)( rcWnd.bottom - rcWnd.top ) );

  D3DXMATRIX matProj;
  D3DXMatrixPerspectiveFovLH( &matProj, mainCam.FovY, aspect, mainCam.Near, mainCam.Far );
  hr = pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
  ASSERTHR( hr );
}

void Graphics::Render( void )
{
  HRESULT hr;

  hr = pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColor, 1.f, 0 );
  ASSERTHR( hr );

  hr = pD3DDevice->BeginScene();
  ASSERTHR( hr );

  SetupMatrices();
  AddCloths();
  AddSpheres();
  DrawPrimitives();

  hr = pD3DDevice->EndScene();
  ASSERTHR( hr );

  pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void Graphics::AddLine( const ColoredVertex *verts )
{
  linePrimitives.push_back( verts[0] );
  linePrimitives.push_back( verts[1] );
}

void Graphics::AddTriangle( const ColoredVertex *verts )
{
  for ( int i = 0; i < 3; ++i )
    trianglePrimitives.push_back( verts[i] );
}

void Graphics::AddQuad( const ColoredVertex *verts )
{
  for ( int i = 0; i < 3; ++i )
    trianglePrimitives.push_back( verts[i] );
  for ( int i = 2; i < 5; ++i )
    trianglePrimitives.push_back( verts[i < 4 ? i : 0] );
}

void Graphics::WriteText( const std::string &text, int x, int y, D3DCOLOR color )
{
  RECT rcPos = { x, y, 0, 0 };
  pD3DFont->DrawText( NULL, text.c_str(), -1, &rcPos, DT_NOCLIP, color );
}

void Graphics::AddCloths( void )
{
  AutoList< Cloth >::iter_type i = AutoList< Cloth >::List.begin();
  for ( ; i != AutoList< Cloth >::List.end(); ++i )
  {
    Cloth *pCloth = *i;
    ColoredVertex vertBuf[4];
    int resX = pCloth->GetResX(), resY = pCloth->GetResY();
    int lastCol = resX - 1, lastRow = resY - 1;
    for ( int r = 0; r < resY; ++r )
    {
      for ( int c = 0; c < resX; ++c )
      {
        if ( r != lastRow && c != lastCol )
        {
          vertBuf[0] = pCloth->GetVertex( r,     c     );
          vertBuf[1] = pCloth->GetVertex( r,     c + 1 );
          vertBuf[2] = pCloth->GetVertex( r + 1, c + 1 );
          vertBuf[3] = pCloth->GetVertex( r + 1, c     );
          AddQuad( vertBuf );
        }
      }
    }
  }
}

void Graphics::AddSpheres( void )
{
  VertVec vertices;
  AutoList< Sphere >::iter_type i = AutoList< Sphere >::List.begin();
  for ( ; i != AutoList< Sphere >::List.end(); ++i )
  {
    Sphere *pSphere = *i;
    pSphere->GetTriangles( vertices );
    for ( VertVecIt j = vertices.begin(); j != vertices.end(); ++j )
      trianglePrimitives.push_back( *j );
  }
}

Camera &Graphics::GetMainCam( void )
{
  return mainCam;
}
