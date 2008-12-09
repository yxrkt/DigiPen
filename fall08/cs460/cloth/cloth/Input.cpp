#include "Input.h"
#include "CS460Proj.h"


Input *Input::Instance( void )
{
  static Input input;
  return &input;
}

Input::Input( void )
{
}

Input::~Input( void )
{
}

void Input::Initialize( void )
{
  ZeroMemory( m_prevKeyState,   NUM_KEYBOARD_KEYS );
  ZeroMemory( m_curKeyState,    NUM_KEYBOARD_KEYS );
  ZeroMemory( m_prevMouseState, NUM_MOUSE_BUTTONS );
  ZeroMemory( m_curMouseState,  NUM_MOUSE_BUTTONS );
  m_mousePos = D3DXVECTOR3( 0.f, 0.f, 0.f );
}

void Input::Update( void )
{
  memcpy( m_prevKeyState, m_curKeyState, NUM_KEYBOARD_KEYS );
  GetKeyboardState( m_curKeyState );

  memcpy( m_prevMouseState, m_curMouseState, NUM_MOUSE_BUTTONS );
}

bool Input::IsKeyHit( int key ) const
{
  return ( !( m_prevKeyState[key] & 0x80 ) && ( m_curKeyState[key] & 0x80 ) );
}

bool Input::IsKeyReleased( int key ) const
{
  return ( ( m_prevKeyState[key] & 0x80 ) && !( m_curKeyState[key] & 0x80 ) );
}

bool Input::IsKeyHeld( int key ) const
{
  return ( ( m_curKeyState[key] & 0x80 ) && true );
}

bool Input::IsMouseHit( int button ) const
{
  return ( !m_prevMouseState[button] && m_curMouseState[button] );
}

bool Input::IsMouseDblClk( int button ) const
{
  return ( m_curMouseState[button] == STATE_DBLCLK && m_prevMouseState[button] == STATE_UP );
}

bool Input::IsMouseReleased( int button ) const
{
  return ( m_prevMouseState[button] && !m_curMouseState[button] );
}

bool Input::IsMouseHeld( int button ) const
{
  return ( m_curMouseState[button] == STATE_DOWN );
}

const D3DXVECTOR3 &Input::GetMousePos( void ) const
{
  return m_mousePos;
}

LRESULT Input::HandleMessage( UINT msg, WPARAM wParam, LPARAM lParam )
{
  m_mousePos.x = (float)LOWORD( lParam );
  m_mousePos.y = (float)HIWORD( lParam );

  switch ( msg )
  {
    case WM_LBUTTONDOWN:
      m_curMouseState[LEFT_BUTTON] = STATE_DOWN;
      SetCapture( CS460PROJ->GetWindow() );
      break;

    case WM_LBUTTONUP:
      m_curMouseState[LEFT_BUTTON] = STATE_UP;
      ReleaseCapture();
      break;

    case WM_LBUTTONDBLCLK:
      m_curMouseState[LEFT_BUTTON] = STATE_DBLCLK;
      break;

    case WM_RBUTTONDOWN:
      m_curMouseState[RIGHT_BUTTON] = STATE_DOWN;
      SetCapture( CS460PROJ->GetWindow() );
      break;

    case WM_RBUTTONUP:
      m_curMouseState[RIGHT_BUTTON] = STATE_UP;
      ReleaseCapture();
      break;

    case WM_RBUTTONDBLCLK:
      m_curMouseState[RIGHT_BUTTON] = STATE_DBLCLK;
      break;

    case WM_MBUTTONDOWN:
      m_curMouseState[MIDDLE_BUTTON] = STATE_DOWN;
      SetCapture( CS460PROJ->GetWindow() );
      break;

    case WM_MBUTTONUP:
      m_curMouseState[MIDDLE_BUTTON] = STATE_UP;
      ReleaseCapture();
      break;

    case WM_MBUTTONDBLCLK:
      m_curMouseState[MIDDLE_BUTTON] = STATE_DBLCLK;
      break;
  }

  return 0L;
}
