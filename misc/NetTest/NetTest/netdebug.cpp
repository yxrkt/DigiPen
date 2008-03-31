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

    // Host or client
  std::string strMyRank = bHost ? "HOST" : "CLIENT";
  debugStrings.push( strMyRank.c_str() );

  if ( bHost )
  {
      // Number of players
    ssDebug << "Players: " << lsPlayers.size() + 1;
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");
  }

    // Player info
  unsigned nPlayer = bHost ? 2 : 1;
  for ( PlayerIter i = lsPlayers.begin(); i != lsPlayers.end(); ++i )
  {
    debugStrings.push("");

    if ( bHost )
      ssDebug << "Player " << nPlayer;
    else
      ssDebug << "Host";

    debugStrings.push( ssDebug.str() );
    ssDebug.str("");

      // Window info
    size_t nPkts = i->qPktWnd.size();
    ssDebug << "Wnd: " << i->qPktWnd.size() << ( nPkts ? " ( " : "" );
    for ( size_t j = 0; j < nPkts; ++j )
      ssDebug << (unsigned)i->qPktWnd.c[j].nIndex << " ";
    ssDebug << ( nPkts ? ")" : "" );
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");      

      // Last recvd
    ssDebug << "LR: " << (unsigned)i->nLRIndex;
    debugStrings.push( ssDebug.str() );
    ssDebug.str("");

      // Last sent
    ssDebug << "LS: " << (unsigned)nLSIndex;
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
