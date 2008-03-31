#include "precompiled.h"

//-----------------------------------------------------------------------------
#ifndef NETTEST_H
#define NETTEST_H
//-----------------------------------------------------------------------------

#include "networking.h"
#include "cursor.h"
#include "font.h"
#include "FSM.h"
#include "camera2d.h"


// ============================================================================
// Automatically connects players for chatting
// ============================================================================
class NetTest
{
  static const DWORD MAX_SEARCH_TIME  = 10000;
  static const UINT  CHAT_FONT_SIZE   = 24;
  static const UINT  CHAT_IN_POS      = 600;
  static const UINT  INFO_FONT_SIZE   = 24;

  public:
    NetTest();
    ~NetTest();

    void RegisterMe( FSM& Graphics, FSM &Physics, int state );
    void UnregisterMe( FSM &Graphics, FSM &Physics );
    void GraphicsInit( int state, FSM *TheFSM );
    void GraphicsUpdate( int state, FSM *TheFSM );
    void GraphicsExit( int state, FSM *TheFSM );
    void PhysicsInit( int state, FSM *TheFSM );
    void PhysicsUpdate( int state, FSM *TheFSM );
    void PhysicsExit( int state, FSM *TheFSM );

  private:
    FSM *GraphicsFSM;
    FSM *PhysicsFSM;
    Font *CenturyGothic;
    FSMCallBack Callbacks[6];
    int MainState;
    Camera2D DebugGUICam;

    DWORD dwStart;
    std::string strChatIn;

    struct SChatOut
    {
      SChatOut( std::string rhs ) : strText( rhs ), dwStart( timeGetTime() ) {}

      std::string strText;
      DWORD dwStart;
    };

    std::list< SChatOut > lsChatOut;
    typedef std::list< SChatOut >::iterator ChatIt;
};

#endif
