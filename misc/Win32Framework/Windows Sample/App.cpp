#include "App.h"


App::App( HINSTANCE hInstance )
{
  m_hInstance = hInstance;
  CreateWindow( "App" );
}

App::~App( void )
{
}

int App::Run( void )
{
  Initialize();

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

void App::Initialize( void )
{
}

void App::Update( void )
{
}

LRESULT App::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
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
