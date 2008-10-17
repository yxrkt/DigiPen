#include "CS460Project.h"


// =============================================================================
// ! Constructor.
// =============================================================================
CS460Project::CS460Project( HINSTANCE hInstance )
{
  hInstance_ = hInstance;

  CreateMainWindow( "CS460 Project 1", 800, 600 );
}

// =============================================================================
// ! Destructor.
// =============================================================================
CS460Project::~CS460Project() {}

// =============================================================================
// ! Initialize.
// =============================================================================
void CS460Project::Initialize()
{
  graphics.Initialize( hWndMain_ );
  graphics.LoadAnimatedMesh( std::string( ASSETS_DIR ) + "run_inPlace.x" );
  //graphics.MainCam.eye.y = 600.f;
  //graphics.MainCam.lookAt.y = 300.f;
}

// =============================================================================
// ! Updates modules.
// =============================================================================
void CS460Project::Update()
{
  float t = .5f * D3DX_PI * (float)( timeGetTime() % 4000 ) / 1000.f;
  graphics.MainCam.eye.x = 1300.f * cos( t );
  graphics.MainCam.eye.z = 1300.f * sin( t );

  graphics.Update();
}

// =============================================================================
// ! Run application.
// =============================================================================
int CS460Project::Run()
{
  MSG msg;
  ZeroMemory( &msg, sizeof( MSG ) );

  Initialize();

  while ( msg.message != WM_QUIT )
  {
    Update();

    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }

  Cleanup();

  return (int)msg.wParam;
}

// =============================================================================
// ! Cleanup.
// =============================================================================
void CS460Project::Cleanup()
{
  UnregisterClass( szClassName, hInstance_ );
  graphics.Cleanup();
}

// =============================================================================
// ! Window procedure.
// =============================================================================
LRESULT CS460Project::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch ( msg )
  {
    case WM_KEYDOWN:
    {
      switch( wParam )
      {
        case VK_ADD:
          graphics.IncDecAnimSpeed( true );
          break;

        case VK_SUBTRACT:
          graphics.IncDecAnimSpeed( false );
          break;

        case VK_ESCAPE:
          PostQuitMessage( 0 );
          break;
      }
      break;
    }

    case WM_DESTROY:
      PostQuitMessage( 0 );
      break;

    case WM_SIZE:
      if ( graphics.Ready )
        graphics.Update();
      break;

    default:
      return DefWindowProc( hWndMain_, msg, wParam, lParam );
  }

  return 0L;
}
