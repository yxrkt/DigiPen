#include "precompiled.h"

#include "networking.h"

// -----------------------------------------------------------------------------
// Error checker
// -----------------------------------------------------------------------------
void Networking_impl::ErrCheck( int err, const char *msg ) const
{

  if ( err == SOCKET_ERROR )
  {
    err = WSAGetLastError();
    if ( err == WSAEWOULDBLOCK )
      return;

    std::stringstream ss;
    ss << err << ": " << msg;
    MessageBox( NULL, ss.str().c_str(), "Winsock Error", MB_ICONERROR );
    exit( 0 );
  }
}

// -----------------------------------------------------------------------------
// Set the user's SOCKADDR_IN
// -----------------------------------------------------------------------------
void Networking_impl::SetMySA()
{
  int err;
  char szMyName[260];

    // Get user's name
  err = gethostname( szMyName, sizeof( szMyName ) );
  ErrCheck( err, "Retrieving host name failed." );

    // Get user's info
  entMe = *gethostbyname( szMyName );

  saMyAddr.sin_family = AF_INET;
  saMyAddr.sin_port   = htons(PORT);
  saMyAddr.sin_addr   = *(in_addr *) entMe.h_addr;
}

// -----------------------------------------------------------------------------
// Broadcasts a hosting message periodically
// -----------------------------------------------------------------------------
void Networking_impl::AdvertiseSession() const
{
  if ( timeGetTime() - dwAdTimer >= BROADCAST_COOLDOWN )
  {
    int err;

    // create packet
    NetMessage msgHost;
    msgHost.mType = MSG_HOSTING;
    msgHost.reliable = false;
    msgHost.addrFrom = saMyAddr.sin_addr.s_addr;
    std::string name( entMe.h_name );
    strcpy( (char *)msgHost.data, name.c_str() );
    size_t l1 = strlen( entMe.h_name );
    size_t l2 = name.length();
    strcpy( (char *)msgHost.data + name.length() + 1, creationTime.c_str() );

    // load packet into message
    NetPacket pktHost;
    pktHost.nMsgs = 1;
    pktHost.Messages[0] = msgHost;

    // send broadcast
    err = sendto( sUDP, (char *)&pktHost, pktHost.Size(), 0, (sockaddr *)&saBroadcast, sizeof( SOCKADDR_IN ) );
    ErrCheck( err, "Broadcasting message failed." );

    // broadcast on 'virtual lan'
    for ( SockAddrVecItC i = vLANAddrs.begin(); i != vLANAddrs.end(); ++i )
    {
      err = sendto( sUDP, (char *)&pktHost, pktHost.Size(), 0, (sockaddr *)&(*i), sizeof( SOCKADDR_IN ) );
      ErrCheck( err, "Advertising on VLAN failed." );
    }

    dwAdTimer = timeGetTime();
  }
}

// -----------------------------------------------------------------------------
// Updates list of sessions to join
// -----------------------------------------------------------------------------
void Networking_impl::UpdateHostList()
{

}

// -----------------------------------------------------------------------------
// Break received packet into messages to be stored in qPktsIn
// -----------------------------------------------------------------------------
void Networking_impl::ExtractMessages( const NetPacket &pkt )
{
    // push packets into queue
  for ( BYTE i = 0; i < pkt.nMsgs; ++i )
  {
      // ignore messages relayed back from host
    if ( pkt.Messages[i].addrFrom != saMyAddr.sin_addr.s_addr )
      qMsgsIn.push( pkt.Messages[i] );
  }
}

// -----------------------------------------------------------------------------
// Determines if an index succeeds another (for wrap around problem)
// -----------------------------------------------------------------------------
bool Networking_impl::IsNext( const BYTE first, const BYTE second ) const
{
  BYTE temp = first + 1;
  return ( second == temp ) ? true : false;
}

// -----------------------------------------------------------------------------
// Add a message to the outgoing packet
// -----------------------------------------------------------------------------
void Networking_impl::PushMessage( const NetMessage &msg, bool relay, bool reliable )
{
  NetMessage msgOut = msg;
  msgOut.reliable = reliable;
  if ( !relay )
    msgOut.addrFrom = saMyAddr.sin_addr.s_addr;
  qMsgsOut.push( msgOut );
}

// -----------------------------------------------------------------------------
// Retrieve packets from queue
// -----------------------------------------------------------------------------
void Networking_impl::ReceivePackets()
{
  int err = 0;

  NetPacket pktIn;
  SOCKADDR_IN saFrom;
  int nFromLen = sizeof( SOCKADDR_IN );
  int nIndex = 0;

  // receive messages until a WSAEWOULDBLOCK or another error occurs
  while ( 1 )
  {
    err = recvfrom( sUDP, (char *)&pktIn, sizeof( NetPacket ), 0, (sockaddr *)&saFrom, &nFromLen );

    // break on socket error (most likely WSAEWOULDBLOCK)
    if ( err == SOCKET_ERROR ) break;

    // verify message header
    if ( pktIn.nHeader != MESSAGE_HEADER ) continue;

    PlayerIter iPlayer = GetPlayer( saFrom );

    // if message comes from someone not in current session
    if ( iPlayer == lsPlayers.end() )
    {
      // if a join message
      nIndex = Contains( pktIn, MSG_JOIN );
      if ( nIndex != INDEX_NOT_FOUND )
      {
        if ( bHost )
        {
          qMsgsIn.push( pktIn.Messages[nIndex] );
          TranslateMessages();
          continue;
        }
      }
      // otherwise, ignore it if not an ad message
      else if ( Contains( pktIn, MSG_HOSTING ) == INDEX_NOT_FOUND )
        continue;
    }

    // if join message from host
    if ( ( Contains( pktIn, MSG_JOIN ) != INDEX_NOT_FOUND ) && bJoining )
    {
      lsPlayers.front().nLRIndex = pktIn.nIndex - 1;
      gsMode                     = GS_SESSION;
      bJoining                   = false;
    }

    // ignore messages if one was missed
    if ( !pktIn.msgFront.reliable || IsNext( iPlayer->nLRIndex, pktIn.nIndex ) )
    {
      ExtractMessages( pktIn );
      if ( pktIn.msgFront.reliable )
        iPlayer->nLRIndex = pktIn.nIndex;
    }

    AdjustPacketWindow( pktIn, iPlayer );
  }

  ErrCheck( err, "Receiving message failed." );
}

// -----------------------------------------------------------------------------
// Send the outgoing packets if necessary
// -----------------------------------------------------------------------------
void Networking_impl::SendMessages()
{
  int err;

  while ( !qMsgsOut.empty() )
  {
    BuildPackets();

    // send reliable outgoing packet if there is one
    if ( reliablePktOut.nMsgs )
    {
      // stamp message with index
      reliablePktOut.nIndex = ++nLSIndex;

      for ( PlayerIter i = lsPlayers.begin(); i != lsPlayers.end(); ++i )
      {
        // resend messages in window first if needed
        ResendPackets( i );

        // stamp message with index of last received
        reliablePktOut.nLRIndex = i->nLRIndex;

        i->qPktWnd.push( reliablePktOut );

        // check if player is lagging badly
        if ( i->qPktWnd.size() > WINDOW_SIZE )
        {
          if ( bHost )
          {
            ManageLagger( i );
            continue;
          }
          else
          {
            ReportError( "Connection to host lost" );
            Reset();
            return;
          }
        }

        // send the packet
        err = sendto( sUDP, (char *)&reliablePktOut, reliablePktOut.Size(), 0,
                      (sockaddr *)&i->saAddr, sizeof( SOCKADDR_IN ) );

        ErrCheck( err, "Sending reliable message failed." );

        // reset resend timer
        i->dwResendTimer = timeGetTime();
      }

      reliablePktOut.nMsgs = 0;
    }
    
    // unreliable messages
    if ( unreliablePktOut.nMsgs )
    {
      for ( PlayerIter i = lsPlayers.begin(); i != lsPlayers.end(); ++i )
      {
        err = sendto( sUDP, (char *)&unreliablePktOut, sizeof( NetPacket ), 
                      0, (sockaddr *)&i->saAddr, sizeof( SOCKADDR_IN ) );
        ErrCheck( err, "Sending unreliable message failed" );
      }

      unreliablePktOut.nMsgs = 0;
    }

  CleanupPlayers();
  }
}

// -----------------------------------------------------------------------------
// Pops packets from packet window queue & resets resend timer
// -----------------------------------------------------------------------------
void Networking_impl::AdjustPacketWindow( const NetPacket &pktIn, PlayerIter &iPlayer )
{
  if ( iPlayer == lsPlayers.end() || iPlayer->qPktWnd.empty() )
    return;

  bool bAdjusted = false;

  while ( iPlayer->qPktWnd.front() <= pktIn )
  {
    iPlayer->qPktWnd.pop();
    bAdjusted = true;

    if ( iPlayer->qPktWnd.empty() )
      break;
  }

  if ( bAdjusted )
    iPlayer->dwResendTimer = timeGetTime();
}

// -----------------------------------------------------------------------------
// Manages a lagging player
// -----------------------------------------------------------------------------
void Networking_impl::ManageLagger( PlayerIter &iPlayer )
{
    // send all players a message saying player is being booted
  NetMessage msgBoot;
  msgBoot.mType = MSG_BOOT;
  memcpy( msgBoot.data, &( *iPlayer ), sizeof( PlayerInfo ) );
  PushMessage( msgBoot );

    // remove the player from list
  qToDrop.push( iPlayer );
}

// -----------------------------------------------------------------------------
// Resends messages in message window when timer is up
// -----------------------------------------------------------------------------
void Networking_impl::ResendPackets( const PlayerIter &iPlayer ) const
{
  int err;

    // check timer
  if ( timeGetTime() - iPlayer->dwResendTimer > MAX_WAIT_TIME )
  {
    size_t nLen = iPlayer->qPktWnd.size();
    for ( size_t i = 0; i < nLen; ++i )
    {
      err = sendto( sUDP, (char *)&iPlayer->qPktWnd.c[i], sizeof( NetPacket ),
                    0, (sockaddr *)&( iPlayer->saAddr ), sizeof( SOCKADDR_IN ) );

      ErrCheck( err, "Resending message(s) failed." );
    }

    iPlayer->dwResendTimer = timeGetTime();
  }
}

// -----------------------------------------------------------------------------
// Send checkup message if idle time reached MAX_IDLE_TIME
// -----------------------------------------------------------------------------
void Networking_impl::IdleCheck()
{
  if ( timeGetTime() - dwIdleTimer > MAX_IDLE_TIME && qMsgsOut.empty() )
  {
    NetMessage msgCheck;
    msgCheck.mType = MSG_CHECK;
    PushMessage( msgCheck );
    dwIdleTimer = timeGetTime();
  }
}

// -----------------------------------------------------------------------------
// Remove and read each message in queue
// -----------------------------------------------------------------------------
void Networking_impl::TranslateMessages()
{
  while ( !qMsgsIn.empty() )
  {
    NetMessage msgIn = qMsgsIn.front();

    SOCKADDR_IN saPlayer;
    saPlayer.sin_family       = AF_INET;
    saPlayer.sin_port         = htons( PORT );
    memcpy( &saPlayer.sin_addr.s_addr, &msgIn.addrFrom, sizeof( UINT ) );

    switch ( msgIn.mType )
    {
    //  case MSG_DATA:
    //  {
    //    Message< TypeList1( RawStorage< MAX_DATA_LEN > ) > *msg = NEW(
    //      Message< TypeList1( RawStorage< MAX_DATA_LEN > ) >, ( ) );
				//if ( msgIn.nSize > MAX_DATA_LEN )
				//	ASSERT( "You received a network message that tried to send too much data" );
    //    memcpy( msg, msgIn.data, msgIn.nSize );

    //    const EntityID *receiverID = entityIDTable.GetID( msgIn.dest );
    //    if ( receiverID != NULL )
    //    {
				//  GameEntity* receiver = GetEntityByID( *receiverID );
    //      if ( receiver != NULL )
				//  {
    //        msg->Send( receiver );
				//  }
    //      if ( bHost )
    //        PushMessage( msgIn, !msg->toResend, msg->reliable );
    //    }
    //  }
    //  break;

      case MSG_TEXT:
      {
        netStrings.push( (char *) msgIn.data );
        if ( bHost )
          PushMessage( msgIn, true );
      }
      break;

      case MSG_JOIN:
      {
        PlayerIter found = GetPlayer( saPlayer );
        if ( bHost )
        {
          if ( bOpenSession )
          {
            if ( found == lsPlayers.end() )
              lsPlayers.push_back( Player( (char *)msgIn.data, saPlayer ) );
            NetMessage msgOut;
            msgOut.mType = MSG_JOIN;
            if ( !CopyNamesToBuf( msgOut.data, sizeof( msgOut.data ) ) )
              ReportError( "Names could not fit in message" );
            PushMessage( msgOut );
          }
          else
          {
            // send rejection message
          }
        }
        else
        {
          if ( !CopyBufToNames( msgIn.data, sizeof( msgIn.data ) ) )
            ReportError( "Not all names contained in buffer or no terminator found" );
        }
      }
      break;

      case MSG_LEAVE:
      {
        if ( bHost )
          BootPlayer( GetPlayer( msgIn.addrFrom )->szName );
      }
      break;

      case MSG_PLAYERS:
      {
        if ( bHost )
        {
          ReportError( "Received player list update as host..." );
        }
        else
        {
          if ( !CopyBufToNames( msgIn.data, sizeof( msgIn.data ) ) )
            ReportError( "Not all names contained in buffer or no terminator found" );
        }
      }
      break;

      case MSG_HOSTING:
      {
        if ( gsMode == GS_JOIN )
        {
          if ( GetPlayer( saPlayer ) == lsPlayers.end() )
          {
            Player player;
            strcpy( player.szName, (char *)msgIn.data );
            player.saAddr = saPlayer;
            strcpy( player.szTime, (char *)msgIn.data + strlen( player.szName ) + 1 );
            lsPlayers.push_back( player );
          }
        }
      }
      break;

      case MSG_BOOT:
      {
        if ( bHost )
        {
          ReportError( "Received boot message as host..." );
        }
        else
        {
          CopyBufToNames( msgIn.data, sizeof( msgIn.data ) );
          StringVecIt found = std::find( playerNames.begin(), playerNames.end(), entMe.h_name );
          if ( found == playerNames.end() )
          {
            Reset();
            return;
          }
        }
      }
      break;
    }

    qMsgsIn.pop();
  }
}

// -----------------------------------------------------------------------------
// Clear data for next game
// -----------------------------------------------------------------------------
void Networking_impl::Reset()
{
  lsPlayers.clear();
  qMsgsIn.c.clear();
  qMsgsOut.c.clear();
  qToDrop.c.clear();
  entityIDTable.Clear();
  reliablePktOut.nIndex   = 0;
  reliablePktOut.nLRIndex = 0;
  reliablePktOut.nMsgs    = 0;
  unreliablePktOut.nMsgs  = 0;
  nLSIndex = 255;
  bHost    = false;
  bJoining = false;
  gsMode   = GS_MENU;
}

// -----------------------------------------------------------------------------
// Maintain list of available games
// -----------------------------------------------------------------------------
void Networking_impl::BrowseGames()
{
  bHost  = false;
  gsMode = GS_JOIN;
}

// -----------------------------------------------------------------------------
// Join a game
// -----------------------------------------------------------------------------
void Networking_impl::JoinGame( const PlayerInfo &player )
{
    // set host
  lsPlayers.clear();
  Player host( player );
  lsPlayers.push_back( host );

    // send host a join message
  NetMessage msgOut;
  msgOut.mType = MSG_JOIN;
  strcpy( (char *)msgOut.data, entMe.h_name ); // TODO: Replace with real name
  qMsgsOut.c.clear();
  PushMessage( msgOut, false, false );

    // reset joining info
  nJoinAttempt      = 0;
  dwLastJoinAttempt = timeGetTime();
  bJoining          = true;
}

// -----------------------------------------------------------------------------
// Leave current session
// -----------------------------------------------------------------------------
void Networking_impl::LeaveGame()
{
  if ( bHost )
  {
  }
  else
  {
    NetMessage leaveMsg;
    leaveMsg.mType = MSG_LEAVE;
    PushMessage( leaveMsg );
  }
}

// -----------------------------------------------------------------------------
// Host a game
// -----------------------------------------------------------------------------
void Networking_impl::HostGame()
{
  bHost        = true;
  bOpenSession = true;
  gsMode       = GS_SESSION;

  time_t curTime = time( NULL );
  tm *pTime      = localtime( &curTime );
  std::stringstream ssTime;
  ssTime << std::setfill('0') << std::setw(2) << pTime->tm_hour << ":"
                              << std::setw(2) << pTime->tm_min << ":"
                              << std::setw(2) << pTime->tm_sec;
  creationTime = ssTime.str();
  playerNames.push_back( entMe.h_name );
}

// -----------------------------------------------------------------------------
// Get player by sockaddr
// -----------------------------------------------------------------------------
Networking_impl::PlayerIter Networking_impl::GetPlayer( const SOCKADDR_IN &addr )
{
  Player dummy;
  dummy.saAddr = addr;
  PlayerIter player = std::find( lsPlayers.begin(), lsPlayers.end(), dummy );
  return player;
}

// -----------------------------------------------------------------------------
// Get player by addr
// -----------------------------------------------------------------------------
Networking_impl::PlayerIter Networking_impl::GetPlayer( const UINT addr )
{
  Player dummy;
  dummy.saAddr.sin_addr.s_addr = addr;
  PlayerIter player = std::find( lsPlayers.begin(), lsPlayers.end(), dummy );
  return player;
}

// -----------------------------------------------------------------------------
// Build the packet from the outgoing message queue
// -----------------------------------------------------------------------------
void Networking_impl::BuildPackets()
{
    // Debug info
  nMaxMsgs = max( nMaxMsgs, qMsgsOut.size() );

  MessageQueueIt i = qMsgsOut.c.begin();
  while ( i != qMsgsOut.c.end() )
  {
    if ( i->reliable && reliablePktOut.nMsgs < MAX_MESSAGES )
    {
      reliablePktOut.Messages[reliablePktOut.nMsgs++] = *i;
      qMsgsOut.c.erase( i++ );
    }
    else if ( !i->reliable && unreliablePktOut.nMsgs < MAX_MESSAGES )
    {
      unreliablePktOut.Messages[unreliablePktOut.nMsgs++] = *i;
      qMsgsOut.c.erase( i++ );
    }
    else
    {
      i++;
    }
  }
}

// -----------------------------------------------------------------------------
// Remove players in drop list
// -----------------------------------------------------------------------------
void Networking_impl::CleanupPlayers()
{
  bool update = ( bHost && !qToDrop.empty() );
  while ( !qToDrop.empty() )
  {
    PushText( "<" + std::string( qToDrop.front()->szName ) + " has left the game>" );
    lsPlayers.erase( qToDrop.front() );
    qToDrop.pop();
  }
  if ( update )
  {
    NetMessage msgPlayers;
    msgPlayers.mType = MSG_PLAYERS;
    CopyNamesToBuf( msgPlayers.data, sizeof( msgPlayers.data ) );
    PushMessage( msgPlayers );
  }
}

// -----------------------------------------------------------------------------
// Returns true if hosting, false otherwise
// -----------------------------------------------------------------------------
bool Networking_impl::Hosting() const
{
  return bHost;
}

// -----------------------------------------------------------------------------
// Returns the index for the message identified by 'type'. If no message of
// this type is found in the packet, -1 will be returned.
// -----------------------------------------------------------------------------
inline int Networking_impl::Contains( const NetPacket &pkt, NET_MSG type )
{
  for ( int i = 0; i < pkt.nMsgs; ++i )
  {
    if ( pkt.Messages[i].mType == type )
      return i;
  }

  return INDEX_NOT_FOUND;
}

// -----------------------------------------------------------------------------
// True if networking should be the only thing updating
// -----------------------------------------------------------------------------
bool Networking_impl::AmWaiting() const
{
  return bWaiting;
}

// -----------------------------------------------------------------------------
// Pauses / unpauses graphics and physics updates
// -----------------------------------------------------------------------------
void Networking_impl::ToggleWaitState()
{
  bWaiting = !bWaiting;
}

// -----------------------------------------------------------------------------
// Send a MSG_DONE message
// -----------------------------------------------------------------------------
void Networking_impl::SendDoneMessage()
{
  static DWORD lastSend = 0;

  DWORD time = timeGetTime();

  if ( time - lastSend > DONE_MESSAGE_COOLDOWN )
  {
    NetMessage msgDone;
    msgDone.mType = MSG_READY;
    PushMessage( msgDone, false, false );
    lastSend = time;
  }
}

// -----------------------------------------------------------------------------
// Adds an entity to entity lookup table
// Returns false if entity was already found in table
// -----------------------------------------------------------------------------
bool Networking_impl::AddEntity( const EntityIDTable::EntityID &id, 
                                 const EntityIDTable::CreationKey &key, bool mother )
{
  return entityIDTable.Add( id, key, mother );
}

// --------------------------------------------------------------------------------
// Retrieves the ID Translation Table so other things can look up information in it
//----------------------------------------------------------------------
const EntityIDTable& Networking_impl::GetIDTable()
{
  return entityIDTable;
}

// -----------------------------------------------------------------------------
// Checks for and sends done messages while in waiting state
// -----------------------------------------------------------------------------
void Networking_impl::UpdateWaitState()
{
  if ( bHost )
  {
    if ( waitSet.size() == lsPlayers.size() )
    {
      SendDoneMessage();
      bWaiting = false;
      waitSet.clear();
      Sleep( HOST_SLEEP_DELAY );
    }
  }
  else
  {
    SendDoneMessage();
  }
}

// -----------------------------------------------------------------------------
// Get error messages from networking
// -----------------------------------------------------------------------------
StringVec Networking_impl::GetErrStrs() const
{
  return errStrings;
}

// -----------------------------------------------------------------------------
// Determines whether or not the session should be advertised (if host)
// -----------------------------------------------------------------------------
void Networking_impl::SetSession( bool open )
{
  bOpenSession = open;
}

// -----------------------------------------------------------------------------
// Clears list of players if browsing games
// -----------------------------------------------------------------------------
void Networking_impl::RefreshHosts()
{
  if ( gsMode == GS_JOIN && !bJoining )
  {
    lsPlayers.clear();
  }
}

// -----------------------------------------------------------------------------
// Names of players (used by clients)
// -----------------------------------------------------------------------------
const StringVec &Networking_impl::GetPlayerNames() const
{
  return playerNames;
}

// -----------------------------------------------------------------------------
// Copies names to buffer
// Used by host for updating clients' player list
// -----------------------------------------------------------------------------
int Networking_impl::CopyNamesToBuf( BYTE *buffer, UINT size, const char *skip )
{
  UINT pos = 0;
  *(buffer + pos++)  = (BYTE)playerNames.size();
  strcpy( (char *)( buffer + pos ), entMe.h_name );
  pos += ( (UINT)strlen( entMe.h_name ) + 1 );
  for ( StringVecIt i = playerNames.begin(); i != playerNames.end(); ++i )
  {
    if ( pos >= size )
      return -1;
    if ( ( skip == NULL ) || ( strcmp( i->c_str(), skip ) ) )
    {
      strcpy( (char *)( buffer + pos ), i->c_str() );
      pos += ( (UINT)i->length() + 1 );
    }
  }
  return pos;
}

// -----------------------------------------------------------------------------
// Clears names and copies buffer to names
// Used by client when receiving new info on players
// -----------------------------------------------------------------------------
bool Networking_impl::CopyBufToNames( BYTE *buffer, UINT size )
{
  UINT pos = 0;
  BYTE nNames = *(buffer + pos++);
  playerNames.clear();
  for ( BYTE i = 0; i < nNames; ++i )
  {
    if ( pos >= size )
      return false;
    playerNames.push_back( (char *)( buffer + pos ) );
    pos += ( (UINT)playerNames[i].length() + 1 );
  }
  return true;
}

// -----------------------------------------------------------------------------
// Add error message to error strings
// -----------------------------------------------------------------------------
void Networking_impl::ReportError( const std::string &err )
{
  errStrings.push_back( err );
}

// -----------------------------------------------------------------------------
// Add an address to advertise session to
// Only necessary for addresses not on lan
// -----------------------------------------------------------------------------
void Networking_impl::AddVLANAddr( const std::string &addr )
{
  SOCKADDR_IN saVAddr;
  saVAddr.sin_family      = AF_INET;
  saVAddr.sin_port        = htons( PORT );
  saVAddr.sin_addr.s_addr = inet_addr( addr.c_str() );
  vLANAddrs.push_back( saVAddr );
}

// -----------------------------------------------------------------------------
// Boot a player
// Returns false and does nothing if player was not found or if client
// -----------------------------------------------------------------------------
bool Networking_impl::BootPlayer( const char *name )
{
  if ( bHost )
  {
    PlayerIter i = lsPlayers.begin();
    while ( i != lsPlayers.end() )
    {
      if ( !strcmp( i->szName, name ) )
      {
        NetMessage msgBoot;
        msgBoot.mType = MSG_BOOT;
        msgBoot.nSize = CopyNamesToBuf( msgBoot.data, sizeof( msgBoot.data ), name );
        PushMessage( msgBoot );
        qToDrop.push( i );
        return true;
      }
    }
  }
  return false;
}
