#pragma once

#include <windows.h>
#include <d3dx9.h>

#define INPUT Input::Instance()

#define LEFT_BUTTON   0
#define RIGHT_BUTTON  1
#define MIDDLE_BUTTON 2

#define STATE_UP      0
#define STATE_DOWN    1
#define STATE_DBLCLK  2

#define NUM_KEYBOARD_KEYS 256
#define NUM_MOUSE_BUTTONS 3

class Input
{
  public:
    static Input *Instance( void );

    void Initialize( void );
    void Update( void );

    bool IsKeyHit( int key ) const;
    bool IsKeyReleased( int key ) const;
    bool IsKeyHeld( int key ) const;

    bool IsMouseHit( int button ) const;
    bool IsMouseDblClk( int button ) const;
    bool IsMouseReleased( int button ) const;
    bool IsMouseHeld( int button ) const;
    const D3DXVECTOR3 &GetMousePos( void ) const;

    LRESULT HandleMessage( UINT msg, WPARAM wParam, LPARAM lparam );

  private:
    byte m_prevKeyState[NUM_KEYBOARD_KEYS];
    byte m_curKeyState[NUM_KEYBOARD_KEYS];

    byte m_prevMouseState[NUM_MOUSE_BUTTONS];
    byte m_curMouseState[NUM_MOUSE_BUTTONS];

    D3DXVECTOR3 m_mousePos;

  private:
    Input( void );
    Input( const Input &rhs );
    void operator =( const Input &rhs );
    ~Input( void );
};
