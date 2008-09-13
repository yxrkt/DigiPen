#include "precompiled.h"
#include "networking.h"

const DWORD ONE_SECOND   = 1000;
const DWORD REFRESH_RATE = 250;

class KeyboardInput
{
  public:
    KeyboardInput() : cursorPos( 0 ), enterHit( false ) {}
    void Update()
    {
      GetKeyboardState( keyboard );
    }
    inline bool IsDown( BYTE key ) const
    {
      return ( ( GetAsyncKeyState( key ) & 0x8000 ) != 0 );
    }

    // text input
    const std::string &GetActiveText() const
    {
      return activeText;
    }
    void SetActiveText( const std::string &text )
    {
      activeText = text;
      End();
    }
    void FlushActiveText()
    {
      activeText.clear();
      Home();
      enterHit  = false;
    }
    void SetTypedKey( char newkey )
    {
      switch ( newkey )
      {
        case '\b': // backspace
          Backspace();
          break;

        case '\r':
          enterHit = true;
          break;

        default:
          activeText.insert( cursorPos++, 1, newkey );
      }
    }

    const unsigned &GetCursorPos() const
    {
      return cursorPos;
    }

    void SetCursorPos( unsigned pos )
    {
      unsigned len = (unsigned) activeText.size();

      if ( pos < 0 )        cursorPos = 0;
      else if ( pos > len ) cursorPos = len;
      else                  cursorPos = pos;
    }

    void Backspace()
    {
      if ( DecCursorPos() ) Delete();
    }

    void Delete()
    {
      activeText.erase( cursorPos, 1 );
    }

    void Home()
    {
      SetCursorPos(0);
    }

    void End()
    {
      SetCursorPos( (unsigned)std::string::npos );
    }

    bool IncCursorPos()
    {
      if ( cursorPos < activeText.size() )
      {
        cursorPos++;
        return true;
      }

      return false;
    }

    bool DecCursorPos()
    {
      if ( cursorPos != 0 )
      {
        cursorPos--;
        return true;
      }

      return false;
    }

    bool EnterHit() const
    {
      return enterHit;
    }

  private:
    BYTE        keyboard[256];
    std::string activeText;
    unsigned    cursorPos;
    bool        enterHit;

} Input;

enum UPDATE_STATE { US_MENU, US_JOIN, US_LOBBY };

// Forward Decls
void Update( HWND &hDebug, UPDATE_STATE &state );
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void UpdateMenu( UPDATE_STATE &state, std::stringstream &cwnd );
void UpdateJoin( UPDATE_STATE &state, std::stringstream &cwnd );
void UpdateLobby( UPDATE_STATE &state, std::stringstream &cwnd );
void DisplayInfo( HWND &hDebug, std::stringstream &cwnd );
void LeaveGame( UPDATE_STATE &state );
void UpdateState( UPDATE_STATE &state );

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
                              350, 300, 570, 575, NULL, NULL, hInstance, NULL );

  ShowWindow( hDebug, nShowCmd );

  UPDATE_STATE state = US_MENU;

#pragma message( "*** REMOVE VLAN ADDR LINES WHEN NOT TESTING OVER VIRTUAL MACHINES ***" )
  Networking.AddVLANAddr( "192.168.1.107" );
  Networking.AddVLANAddr( "192.168.1.108" );
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
    case WM_CHAR:
      Input.SetTypedKey( (char)wParam );
      break;

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
  UpdateState( state );
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
  if ( Input.IsDown( VK_F2 ) )
  {
    Networking.BrowseGames();
    state = US_JOIN;
  }
  else if ( Input.IsDown( VK_F1 ) )
  {
    Networking.HostGame();
    state = US_LOBBY;
  }
  else
  {
    cwnd << "[F1]Host Game" << std::endl;
    cwnd << "[F2]Join Game" << std::endl;
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
    cwnd << "F1 to leave game" << std::endl;
    if ( Input.IsDown( VK_F1 ) )
      Networking.LeaveGame();
  }
  else
  {
    cwnd << "F1 to boot first client" << std::endl;
    if ( Input.IsDown( VK_F1 ) && !Networking.GetPlayers().empty() )
    {
      Networking.BootPlayer( Networking.GetPlayers().front().szName );
    }
  }

  static StringQueue output;

  StringQueue &netMessages( Networking.GetText() );
  while ( !netMessages.empty() )
  {
    output.push( netMessages.front() );
    netMessages.pop();
  }

  while ( output.size() > 5 )
  {
    output.pop();
  }

  // show messages received over network
  cwnd << std::endl;
  size_t nLines = output.size();
  for ( size_t i = 0; i < nLines; ++i )
  {
    cwnd << output.c[i] << std::endl;
  }
  for ( size_t i = nLines; i < 6; ++i )
  {
    cwnd << std::endl;
  }

  // get input from user
  cwnd << Input.GetActiveText();
  if ( Input.EnterHit() )
  {
    std::string message( Networking.GetMe().h_name );
    message += ": ";
    message += Input.GetActiveText();
    Networking.PushText( message );
    output.push( message );
    Input.FlushActiveText();
  }
}

void DisplayInfo( HWND &hDebug, std::stringstream &cwnd )
{
  // Add extra space if text added by state
  if ( !cwnd.str().empty() )
  {
    for ( unsigned i = 0; i < 2; ++i )
      cwnd << std::endl;
  }

  // Add net debug strings to output stream
  StringQueue dbgStrs = Networking.GetDbgStrs();
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

void LeaveGame( UPDATE_STATE &state )
{
  Networking.LeaveGame();
  state = US_MENU;
}

void UpdateState( UPDATE_STATE &state )
{
  switch( Networking.GetMode() )
  {
    case GS_MENU:
      if ( state != US_MENU )
      {
        state = US_MENU;
      }
      break;
    case GS_JOIN:
      if ( state != US_JOIN )
      {
        state = US_JOIN;
      }
      break;
    case GS_SESSION:
      if ( state != US_LOBBY )
      {
        state = US_LOBBY;
        Input.FlushActiveText();
      }
      break;
  }
}
