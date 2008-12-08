#include <sstream>

#include "CS460Proj.h"
#include "Graphics.h"
#include "Physics.h"


CS460Proj *CS460Proj::Instance( void )
{
  static CS460Proj cs460Proj;
  return &cs460Proj;
}

CS460Proj::CS460Proj( void )
{
}

CS460Proj::~CS460Proj( void )
{
  SAFE_DELETE( pCloth );
}

int CS460Proj::Run( void )
{
  MSG msg;
  ZeroMemory( &msg, sizeof( MSG ) );
  while ( msg.message != WM_QUIT )
  {
    Update();

    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }

  return (int)msg.wParam;
}

HWND CS460Proj::GetWindow( void )
{
  return m_hWndMain;
}

void CS460Proj::Initialize( HINSTANCE hInstance )
{
  m_hInstance = hInstance;
  CreateWindow( "CS460Proj" );

  PHYSICS->Initialize();

  GRAPHICS->Initialize();
  GRAPHICS->GetMainCam().Eye = D3DXVECTOR3( 0.f, 400.f, -500.f );

  pCloth  = new Cloth( 300.f, 300.f, 32, 32 );
  pSphere = new Sphere( D3DXVECTOR3( 0.f, -150.f, 0.f ), 100.f, 16, 32 );
}

void CS460Proj::Update( void )
{
  static DWORD lastTick = timeGetTime();
  DWORD tick = timeGetTime();

  static std::stringstream ssTime;
  ssTime << "FPS: " << ( 1000.f / (float)( tick - lastTick ) ) << std::endl;
  GRAPHICS->WriteText( ssTime.str().c_str(), 10, 10 );
  ssTime.str( "" );

  //*
  if ( !AutoList< Cloth >::List.empty() )
  {
    Cloth *pCloth = AutoList< Cloth >::List.front();
    int resX = pCloth->GetResX(), resY = pCloth->GetResY();

    float period = 2000.f, amp = 50.f;
    float xStep = period / (float)resX;
    for ( int r = 0; r < resY; ++r )
    {
      for ( int c = 0; c < resX; ++c )
      {
        ColoredVertex &vert = pCloth->GetVertex( r, c );
        int offset = c * (int)xStep;//(int)( ( (float)c * xStep + (float)r * xStep ) / 2.f );
        vert.Pos.y = amp * sin( 2.f * D3DX_PI * (float)( tick + offset % (int)period ) / period );
      }
    }
  }
  //*/

  PHYSICS->Update();
  GRAPHICS->Render();
}

LRESULT CS460Proj::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch ( msg )
  {
    case WM_DESTROY:
      PostQuitMessage( 0 );
      break;

    default:
      return DefWindowProc( m_hWndMain, msg, wParam, lParam );
  }

  return 0L;
}
