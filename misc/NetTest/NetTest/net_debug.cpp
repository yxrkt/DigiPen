#include "precompiled.h"

#include "networking.h"

// -----------------------------------------------------------------------------
// Update debugging networking strings
// -----------------------------------------------------------------------------
void Networking_impl::UpdateDebug()
{
  std::stringstream ssDebug;

  // Reset String
  debugStrings.c.clear();

  // Title
  debugStrings.push( "Networking Debugger" );

  // Errors
  for ( StringVecIt i = errStrings.begin(); i != errStrings.end(); ++i )
    debugStrings.push( "ERROR: " +  *i );

  // Host or client
  std::string strMyRank = bHost ? "(Host)" : "(Client)";
  debugStrings.push( strMyRank.c_str() );

  if ( bHost )
  {
    // Number of players
    ssDebug << "Players:  " << lsPlayers.size() + 1;
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");
  }
  else
  {
    // Join info
    ssDebug << "Joining: " << ( bJoining ? "true" : "false" );
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");

    ssDebug << "Join Attempt: " << nJoinAttempt;
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");
  }

  // Game mode
  ssDebug << "Update State: ";
  switch ( gsMode )
  {
    case GS_MENU:
      ssDebug << "Title";
      break;
    case GS_JOIN:
      ssDebug << "Join Menu";
      break;
    case GS_SESSION:
      ssDebug << "Session";
      break;
  }
  debugStrings.push( ssDebug.str() );
  ssDebug.str("");

  // Player info
  unsigned nPlayer = bHost ? 2 : 1;
  for ( PlayerIter i = lsPlayers.begin(); i != lsPlayers.end(); ++i )
  {
    debugStrings.push("");

    if ( bHost )
      ssDebug << "Player " << nPlayer << " Info";
    else
      ssDebug << "Host Info";

    debugStrings.push( ssDebug.str() );
    ssDebug.str("");

    // Window info
    size_t nPkts = i->qPktWnd.size();
    ssDebug << "WndSize: " << i->qPktWnd.size() << ( nPkts ? " ( " : "" );
    for ( size_t j = 0; j < nPkts; ++j )
      ssDebug << (unsigned)i->qPktWnd.c[j].nIndex << " ";
    ssDebug << ( nPkts ? ")" : "" );
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");      

    // Last recvd
    ssDebug << "Last Received: " << (unsigned)i->nLRIndex;
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");

    // Last sent
    ssDebug << "Last Sent: " << (unsigned)nLSIndex;
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");

    nPlayer++;
  }
}

// -----------------------------------------------------------------------------
// return debugging info string
// -----------------------------------------------------------------------------
Networking_impl::StringQueue Networking_impl::GetDbgStrs() const
{
  return debugStrings;
}

// -----------------------------------------------------------------------------
// get state of network
// -----------------------------------------------------------------------------
GAME_STATE Networking_impl::GetMode() const
{
  return gsMode;
}

// -----------------------------------------------------------------------------
// push a text message
// -----------------------------------------------------------------------------
void Networking_impl::PushText( const std::string &text )
{
  NetMessage msgOut;
  msgOut.mType = MSG_TEXT;
  strcpy( (char *)msgOut.data, text.c_str() );
  PushMessage( msgOut );
}

// -----------------------------------------------------------------------------
// return string buffer for text messages
// -----------------------------------------------------------------------------
Networking_impl::StringQueue &Networking_impl::GetText()
{
  return netStrings;
}

// -----------------------------------------------------------------------------
// return networking info on user
// -----------------------------------------------------------------------------
const hostent &Networking_impl::GetMe() const
{
  return entMe;
}
