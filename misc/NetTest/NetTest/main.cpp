#include "precompiled.h"
#include "networking.h"

const DWORD ONE_SECOND   = 1000;
const DWORD REFRESH_RATE = 250;

class KeyboardInput
{
  public:
    void Update()
    {
      GetKeyboardState( keyboard );
    }
    inline bool IsDown( BYTE key ) const
    {
      return ( ( GetAsyncKeyState( key ) & 0x8000 ) != 0 );
    }

  private:
    BYTE keyboard[256];
} Input;

enum UPDATE_STATE { US_MENU, US_JOIN, US_LOBBY };

// Forward Decls
void Update( HWND &hDebug, UPDATE_STATE &state );
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void UpdateMenu( UPDATE_STATE &state, std::stringstream &cwnd );
void UpdateJoin( UPDATE_STATE &state, std::stringstream &cwnd );
void UpdateLobby( UPDATE_STATE &state, std::stringstream &cwnd );
void DisplayInfo( HWND &hDebug, std::stringstream &cwnd );

// Entry point
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd )
{
  WNDCLASSEX wcex;
  memset( &wcex, 0, sizeof( WNDCLASSEX ) );
  wcex.cbSize         = sizeof( WNDCLASSEX );
  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.hbrBackground  = (HBRUSH)GetStockObject( WHITE_BRUSH );
  wcex.hInstance      = hInstance;
  wcex.lpfnWndProc    = WndProc;
  wcex.lpszClassName  = "SimpleWindow";
  wcex.hCursor        = LoadCursor( hInstance, IDC_ARROW );
  if ( !RegisterClassEx( &wcex ) )
    return MessageBoxA( NULL, "Registering class failed", "ERROR", MB_ICONERROR );

  HWND hDebug = CreateWindow( "SimpleWindow", "Net Test", WS_OVERLAPPEDWINDOW,
                              350, 300, 570, 375, NULL, NULL, hInstance, NULL );

  ShowWindow( hDebug, nShowCmd );

  UPDATE_STATE state = US_MENU;
  Networking.AddVLANAddr( "192.168.1.107" );
  Networking.AddVLANAddr( "192.168.1.110" );

  MSG msg;
  memset( &msg, 0, sizeof( MSG ) );
  while ( msg.message != WM_QUIT )
  {
    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }

    Update( hDebug, state );
    Sleep( 10 );
  }

  return 0;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch ( msg )
  {
    case WM_MOUSEMOVE:
      SetCursor( LoadCursor( NULL, IDC_ARROW ) );
      break;

    case WM_CLOSE:
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc( hWnd, msg, wParam, lParam );
  }

  return 0L;
}

void Update( HWND &hDebug, UPDATE_STATE &state )
{
  std::stringstream cwnd;
  Networking.Update();
  Input.Update();

  switch ( state )
  {
    case US_MENU:
      UpdateMenu( state, cwnd );
      break;
    case US_JOIN:
      UpdateJoin( state, cwnd );
      break;
    case US_LOBBY:
      UpdateLobby( state, cwnd );
      break;
  }

  DisplayInfo( hDebug, cwnd );
}

void UpdateMenu( UPDATE_STATE &state, std::stringstream &cwnd )
{
  if ( Input.IsDown( 'J' ) )
  {
    Networking.BrowseGames();
    state = US_JOIN;
  }
  else if ( Input.IsDown( 'H' ) )
  {
    Networking.HostGame();
    state = US_LOBBY;
  }
  else
  {
    cwnd << "[H]ost Game" << std::endl;
    cwnd << "[J]oin Game" << std::endl;
  }
}

void UpdateJoin( UPDATE_STATE &state, std::stringstream &cwnd )
{
  static DWORD lastTick = timeGetTime();
  static unsigned nDots = 0;

  if ( Networking.GetPlayers().empty() )
  {
    if ( timeGetTime() - lastTick > ONE_SECOND )
    {
      nDots++;
      lastTick = timeGetTime();
    }
    cwnd << "Searching";
    for ( unsigned i = 0; i < nDots; ++i )
      cwnd << ".";
    cwnd << std::endl;
  }
  else
  {
    Networking.JoinGame( Networking.GetPlayers().front() );
    state = US_LOBBY;
  }
}

void UpdateLobby( UPDATE_STATE &state, std::stringstream &cwnd )
{
  if ( !Networking.Hosting() )
  {
    cwnd << "L to leave game" << std::endl;
    if ( Input.IsDown( 'L' ) )
      Networking.LeaveGame();
  }
  else
  {
    cwnd << "B to boot first client" << std::endl;
    if ( Input.IsDown( 'B' ) && !Networking.GetPlayers().empty() )
    {
      Networking.BootPlayer( Networking.GetPlayers().front().szName );
    }
  }
}

void DisplayInfo( HWND &hDebug, std::stringstream &cwnd )
{
  // Add extra space if text added by state
  if ( !cwnd.str().empty() )
    cwnd << std::endl;

  // Add net debug strings to output stream
  Networking_impl::StringQueue dbgStrs = Networking.GetDbgStrs();
  size_t nStrs = dbgStrs.size();
  for ( size_t i = 0; i < nStrs; ++i )
    cwnd << dbgStrs.c[i] << std::endl;
  cwnd << " ";

  // Draw text to window
  RECT rcClient;
  GetClientRect( hDebug, &rcClient );
  DrawText( GetDC( hDebug ), cwnd.str().c_str(), (int)cwnd.str().length(), &rcClient, DT_LEFT );

  // Clear screen periodically
  static DWORD lastClear = timeGetTime();
  if ( timeGetTime() - lastClear > REFRESH_RATE )
  {
    InvalidateRect( hDebug, NULL, TRUE );
    lastClear = timeGetTime();
  }
}
