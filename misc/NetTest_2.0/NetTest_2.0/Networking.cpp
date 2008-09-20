/*!*************************************************************************************************
    @file   Networking.cpp
    @author Alex Serio
    @par    aserio\@digipen.edu
    @par    Block Breakers
    @par    by Digital Dreams and DigiPen Institute of Technology
    @date   September 5, 2008
    
    @brief  Networking Engine class implementation.

    @par    All content copyright © 2008-2009 DigiPen(USA) Corporation. All rights reserved.
***************************************************************************************************/

#include "NetworkingPCH.h"
#include "Networking.h"

NetworkingEngine::NetworkingEngine()
  : nLSIndex( 255 )
  , gsMode( GS_MENU )
  , bHost( false )
  , nMaxMsgs( 0 )
  , bWaiting( false )
  , bJoining( false )
  , nJoinAttempt( 0 )
{
  int err;

    // Initialize Winsock
  WSADATA wsadata;
  err = WSAStartup( MAKEWORD( 2, 2 ), &wsadata );
  ErrCheck( err, "Initializing Winsock failed." );

    // Create the socket
  sUDP = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
  if ( sUDP == INVALID_SOCKET )
  {
    err = WSAGetLastError();
    std::stringstream ssErrMsg;
    ssErrMsg << err << ": " << "Creating socket failed.";
    MessageBoxA( NULL, ssErrMsg.str().c_str(), "Winsock Error", MB_ICONERROR );
    exit( 0 );
  }

    // Bind socket to an available IP address
  SOCKADDR_IN saBind;
  saBind.sin_family       = AF_INET;
  saBind.sin_port         = htons( PORT );
  saBind.sin_addr.s_addr  = INADDR_ANY;

  err = bind( sUDP, ( const sockaddr *) &saBind, sizeof( saBind ) );
  ErrCheck( err, "Binding socket failed." );

    // Set socket to non-blocking
  unsigned long u1 = 1;
  err = ioctlsocket( sUDP, FIONBIO, &u1 );
  ErrCheck( err, "Setting socket to non-blocking failed." );

    // Enable broadcasts
  bool bBroadcast = true;
  err = setsockopt( sUDP, SOL_SOCKET, SO_BROADCAST, ( const char *) &bBroadcast, sizeof( bBroadcast ) );
  ErrCheck( err, "Enabling broadcasts failed." );

    // Initialize sockaddr for sending broadcasts
  saBroadcast.sin_family      = AF_INET;
  saBroadcast.sin_port        = htons( PORT );
  saBroadcast.sin_addr.s_addr = INADDR_BROADCAST;

    // Create sockaddr for user
  SetMySA();
}

// =============================================================================
// Destructor: Does nothing!
// =============================================================================
NetworkingEngine::~NetworkingEngine() {}

// =============================================================================
// Updates networking!
// =============================================================================
void NetworkingEngine::Update()
{
  UpdateDebug();
  ReceivePackets();
  TranslateMessages();

  switch ( gsMode )
  {
    case GS_JOIN:
      UpdateJoinMenu();
      break;

    case GS_SESSION:
      UpdateSession();
      break;
  }

  SendMessages();
}

// -----------------------------------------------------------------------------
// Update player list and advertise session (if host)
// -----------------------------------------------------------------------------
void NetworkingEngine::UpdateSession()
{
  if ( bHost && bOpenSession )
    AdvertiseSession();

  if ( bWaiting )
    UpdateWaitState();

  IdleCheck();
}

// -----------------------------------------------------------------------------
// Update list of hosts
// -----------------------------------------------------------------------------
void NetworkingEngine::UpdateJoinMenu()
{
  if ( !bJoining )
  {
    UpdateHostList();
  }
  else
  {
    if ( nJoinAttempt < MAX_JOIN_ATTEMPTS )
    {
      DWORD time = timeGetTime();
      if ( time - dwLastJoinAttempt > MAX_JOIN_WAIT_TIME )
      {
        dwLastJoinAttempt = time;
        nJoinAttempt++;
        NetMessage msgJoin;
        msgJoin.mType = MSG_JOIN;
        msgJoin.nSize = 0;
        strcpy( (char *)msgJoin.data, entMe.h_name );;
        PushMessage( msgJoin );
      }
    }
    else
    {
      std::stringstream ssErr;
      ssErr << "Joining game hosted by " << lsPlayers.front().szName << " ("
            << lsPlayers.front().GetAddr().c_str() << ") failed with no response";
      errStrings.push_back( ssErr.str() );
      lsPlayers.clear();
      bJoining = false;
    }
  }
}

// -----------------------------------------------------------------------------
// Gets list of players
//  - Menu          --> N/A
//  - Join Menu     --> hosts
//  - Lobby / Game  --> players
// -----------------------------------------------------------------------------
NetworkingEngine::PlayerInfoList NetworkingEngine::GetPlayers() const
{
  PlayerInfoList lsPlayerInfo;

  for ( PlayerIterC i = lsPlayers.begin(); i != lsPlayers.end(); ++i )
    lsPlayerInfo.push_back( *i );

  return lsPlayerInfo;
}

// =============================================================================

// -----------------------------------------------------------------------------
// Error checker
// -----------------------------------------------------------------------------
void NetworkingEngine::ErrCheck( int err, const char *msg ) const
{

  if ( err == SOCKET_ERROR )
  {
    err = WSAGetLastError();
    if ( err == WSAEWOULDBLOCK )
      return;

    std::stringstream ss;
    ss << err << ": " << msg;
    MessageBoxA( NULL, ss.str().c_str(), "Winsock Error", MB_ICONERROR );
    exit( 0 );
  }
}

// -----------------------------------------------------------------------------
// Set the user's SOCKADDR_IN
// -----------------------------------------------------------------------------
void NetworkingEngine::SetMySA()
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
void NetworkingEngine::AdvertiseSession() const
{
  if ( timeGetTime() - dwAdTimer >= BROADCAST_COOLDOWN )
  {
    int err;

    // create packet
    NetMessage msgHost;
    msgHost.mType = MSG_HOSTING;
    msgHost.addrFrom = saMyAddr.sin_addr.s_addr;
    std::string name( entMe.h_name );
    strcpy( (char *)msgHost.data, name.c_str() );
    strcpy( (char *)msgHost.data + name.length() + 1, creationTime.c_str() );
    msgHost.nSize = (SHORT)name.length() + 1 + (SHORT)creationTime.length() + 1;

    // load packet into message
    NetPacket pktHost;
    pktHost.nMsgs = 1;
    pktHost.PushMessage( msgHost );

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
void NetworkingEngine::UpdateHostList()
{

}

// -----------------------------------------------------------------------------
// Break received packet into messages to be stored in qPktsIn
// -----------------------------------------------------------------------------
void NetworkingEngine::ExtractMessages( const NetPacket &pkt )
{
    // push packets into queue
  for ( BYTE i = 0; i < pkt.nMsgs; ++i )
  {
      // ignore messages relayed back from host
    if ( pkt[i]->addrFrom != saMyAddr.sin_addr.s_addr )
      qMsgsIn.push( *pkt[i] );
  }
}

// -----------------------------------------------------------------------------
// Determines if an index succeeds another (for wrap around problem)
// -----------------------------------------------------------------------------
bool NetworkingEngine::IsNext( const BYTE first, const BYTE second ) const
{
  BYTE temp = first + 1;
  return ( second == temp ) ? true : false;
}

// -----------------------------------------------------------------------------
// Add a message to the outgoing packet
// -----------------------------------------------------------------------------
void NetworkingEngine::PushMessage( const NetMessage &msg )
{
  ASSERT( msg.nSize != UNSPECIFIED_SIZE, "No message size specified!" );
  NetMessage msgOut = msg;
  if ( !( msgOut.flags & NetMessage::FLAG_RELAY_BACK ) )
    msgOut.addrFrom = saMyAddr.sin_addr.s_addr;
  qMsgsOut.push( msgOut );
}

// -----------------------------------------------------------------------------
// Retrieve packets from queue
// -----------------------------------------------------------------------------
void NetworkingEngine::ReceivePackets()
{
  int err = 0;

  NetPacket pktIn;
  SOCKADDR_IN saFrom;
  int nFromLen = sizeof( SOCKADDR_IN );
  int nIndex = 0;

  // receive messages until a WSAEWOULDBLOCK or another error occurs
  while ( err != SOCKET_ERROR )
  {
    err = recvfrom( sUDP, (char *)&pktIn, sizeof( NetPacket ), 0, (sockaddr *)&saFrom, &nFromLen );

    // break on socket error (most likely WSAEWOULDBLOCK)
    if ( err == SOCKET_ERROR ) continue;

    // verify message header
    if ( pktIn.nHeader != MESSAGE_HEADER ) continue;

    PlayerIter iPlayer = GetPlayer( saFrom );

    // if message comes from someone not in current session
    if ( iPlayer == lsPlayers.end() )
    {
      // if a join message
      nIndex = pktIn.GetIndexOfFirst( MSG_JOIN );
      if ( nIndex != INDEX_NOT_FOUND )
      {
        if ( bHost )
        {
          qMsgsIn.push( *pktIn[nIndex] );
          TranslateMessages();
          continue;
        }
      }
      // otherwise, ignore it if not an ad message
      else if ( pktIn.GetIndexOfFirst( MSG_HOSTING ) == INDEX_NOT_FOUND )
        continue;
    }

    // if join message from host
    if ( ( pktIn.GetIndexOfFirst( MSG_JOIN ) != INDEX_NOT_FOUND ) && bJoining )
    {
      lsPlayers.front().nLRIndex = pktIn.nIndex - 1;
      gsMode                     = GS_SESSION;
      bJoining                   = false;
    }

    // ignore messages if one was missed
    if ( !pktIn[0]->flags & NetMessage::FLAG_RELIABLE || IsNext( iPlayer->nLRIndex, pktIn.nIndex ) )
    {
      ExtractMessages( pktIn );
      if ( pktIn[0]->flags & NetMessage::FLAG_RELIABLE )
        iPlayer->nLRIndex = pktIn.nIndex;
    }

    AdjustPacketWindow( pktIn, iPlayer );
  }

  ErrCheck( err, "Receiving message failed." );
}

// -----------------------------------------------------------------------------
// Send the outgoing packets if necessary
// -----------------------------------------------------------------------------
void NetworkingEngine::SendMessages()
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

      reliablePktOut.Clear();
    }
    
    // unreliable messages
    if ( unreliablePktOut.nMsgs )
    {
      for ( PlayerIter i = lsPlayers.begin(); i != lsPlayers.end(); ++i )
      {
        err = sendto( sUDP, (char *)&unreliablePktOut, unreliablePktOut.Size(), 
                      0, (sockaddr *)&i->saAddr, sizeof( SOCKADDR_IN ) );
        ErrCheck( err, "Sending unreliable message failed" );
      }

      unreliablePktOut.Clear();
    }

  CleanupPlayers();
  }
}

// -----------------------------------------------------------------------------
// Pops packets from packet window queue & resets resend timer
// -----------------------------------------------------------------------------
void NetworkingEngine::AdjustPacketWindow( const NetPacket &pktIn, PlayerIter &iPlayer )
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
void NetworkingEngine::ManageLagger( PlayerIter &iPlayer )
{
    // send all players a message saying player is being booted
  NetMessage msgBoot;
  msgBoot.mType = MSG_BOOT;
  msgBoot.nSize = sizeof( PlayerInfo );
  memcpy( msgBoot.data, &( *iPlayer ), sizeof( PlayerInfo ) );
  PushMessage( msgBoot );

    // remove the player from list
  qToDrop.push( iPlayer );
}

// -----------------------------------------------------------------------------
// Resends messages in message window when timer is up
// -----------------------------------------------------------------------------
void NetworkingEngine::ResendPackets( const PlayerIter &iPlayer ) const
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
void NetworkingEngine::IdleCheck()
{
  if ( timeGetTime() - dwIdleTimer > MAX_IDLE_TIME && qMsgsOut.empty() )
  {
    NetMessage msgCheck;
    msgCheck.mType = MSG_CHECK;
    msgCheck.nSize = 0;
    PushMessage( msgCheck );
    dwIdleTimer = timeGetTime();
  }
}

// -----------------------------------------------------------------------------
// Remove and read each message in queue
// -----------------------------------------------------------------------------
void NetworkingEngine::TranslateMessages()
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
    //    if ( msgIn.nSize > MAX_DATA_LEN )
    //	  ASSERT( "You received a network message that tried to send too much data" );
    //    memcpy( msg, msgIn.data, msgIn.nSize );

    //    const EntityID *receiverID = entityIDTable.GetID( msgIn.dest );
    //    if ( receiverID != NULL )
    //    {
    //      GameEntity* receiver = GetEntityByID( *receiverID );
    //      if ( receiver != NULL )
    //      {
    //        msg->Send( receiver );
    //      }
    //      if ( bHost )
    //        PushMessage( msgIn, !msg->toResend, msg->reliable );
    //    }
    //  }
    //  break;

      case MSG_TEXT:
      {
        netStrings.push( (char *) msgIn.data );
        if ( bHost )
        {
          msgIn.flags |= ( NetMessage::FLAG_RELIABLE | NetMessage::FLAG_RELAY_BACK );
          PushMessage( msgIn );
        }
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
            {
              lsPlayers.push_back( Player( (char *)msgIn.data, saPlayer ) );
              NetMessage msgOut;
              msgOut.mType = MSG_JOIN;
              msgOut.nSize = (SHORT)CopyNamesToBuf( msgOut.data, sizeof( msgOut.data ) );
              if ( msgOut.nSize == 0 )
              {
                ReportError( "Names could not fit in message" );
              }
              PushMessage( msgOut );
            }
            else
            {
              ReportError( "Getting join message from an already accepted client" );
            }
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
void NetworkingEngine::Reset()
{
  lsPlayers.clear();
  qMsgsIn.c.clear();
  qMsgsOut.c.clear();
  qToDrop.c.clear();
  entityIDTable.Clear();
  reliablePktOut.nIndex   = 0;
  reliablePktOut.nLRIndex = 0;
  reliablePktOut.Clear();
  unreliablePktOut.Clear();
  nLSIndex = 255;
  bHost    = false;
  bJoining = false;
  gsMode   = GS_MENU;
}

// -----------------------------------------------------------------------------
// Maintain list of available games
// -----------------------------------------------------------------------------
void NetworkingEngine::BrowseGames()
{
  bHost  = false;
  gsMode = GS_JOIN;
}

// -----------------------------------------------------------------------------
// Join a game using n'th index in player list
// -----------------------------------------------------------------------------
void NetworkingEngine::JoinGame( unsigned index )
{
  PlayerIter player = lsPlayers.begin();
  for ( unsigned i = 0; i < index; ++i )
  {
    ++player;
  }

  JoinGame( PlayerInfo( *player ) );
}

// -----------------------------------------------------------------------------
// Join a game using PlayerInfo struct
// -----------------------------------------------------------------------------
void NetworkingEngine::JoinGame( const PlayerInfo &player )
{
    // set host
  lsPlayers.clear();
  Player host( player );
  lsPlayers.push_back( host );

    // send host a join message
  NetMessage msgOut;
  msgOut.mType = MSG_JOIN;
  strcpy( (char *)msgOut.data, entMe.h_name ); // TODO: Replace with real name
  msgOut.nSize = (SHORT)strlen( (char *)msgOut.data ) + 1;
  qMsgsOut.c.clear();
  PushMessage( msgOut );

    // reset joining info
  nJoinAttempt      = 0;
  dwLastJoinAttempt = timeGetTime();
  bJoining          = true;
}

// -----------------------------------------------------------------------------
// Leave current session
// -----------------------------------------------------------------------------
void NetworkingEngine::LeaveGame()
{
  if ( bHost )
  {
  }
  else
  {
    NetMessage leaveMsg;
    leaveMsg.mType = MSG_LEAVE;
    leaveMsg.nSize = 0;
    PushMessage( leaveMsg );
  }
}

// -----------------------------------------------------------------------------
// Host a game
// -----------------------------------------------------------------------------
void NetworkingEngine::HostGame()
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
NetworkingEngine::PlayerIter NetworkingEngine::GetPlayer( const SOCKADDR_IN &addr )
{
  Player dummy;
  dummy.saAddr = addr;
  PlayerIter player = std::find( lsPlayers.begin(), lsPlayers.end(), dummy );
  return player;
}

// -----------------------------------------------------------------------------
// Get player by addr
// -----------------------------------------------------------------------------
NetworkingEngine::PlayerIter NetworkingEngine::GetPlayer( const UINT addr )
{
  Player dummy;
  dummy.saAddr.sin_addr.s_addr = addr;
  PlayerIter player = std::find( lsPlayers.begin(), lsPlayers.end(), dummy );
  return player;
}

// -----------------------------------------------------------------------------
// Build the packet from the outgoing message queue
// -----------------------------------------------------------------------------
void NetworkingEngine::BuildPackets()
{
    // Debug info
  nMaxMsgs = max( nMaxMsgs, qMsgsOut.size() );
  bool reliableDone = false;

  MessageQueueIt i = qMsgsOut.c.begin();
  while ( i != qMsgsOut.c.end() )
  {
    if ( i->flags & NetMessage::FLAG_RELIABLE && !reliableDone )
    {
      if ( reliablePktOut.PushMessage( *i ) )
        qMsgsOut.c.erase( i++ );
      else
        reliableDone = true;
    }
    else if ( !i->flags & NetMessage::FLAG_RELIABLE )
    {
      if ( unreliablePktOut.PushMessage( *i ) )
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
void NetworkingEngine::CleanupPlayers()
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
    msgPlayers.nSize = (SHORT)CopyNamesToBuf( msgPlayers.data, sizeof( msgPlayers.data ) );
    PushMessage( msgPlayers );
  }
}

// -----------------------------------------------------------------------------
// Returns true if hosting, false otherwise
// -----------------------------------------------------------------------------
bool NetworkingEngine::Hosting() const
{
  return bHost;
}

// -----------------------------------------------------------------------------
// True if networking should be the only thing updating
// -----------------------------------------------------------------------------
bool NetworkingEngine::AmWaiting() const
{
  return bWaiting;
}

// -----------------------------------------------------------------------------
// Pauses / unpauses graphics and physics updates
// -----------------------------------------------------------------------------
void NetworkingEngine::ToggleWaitState()
{
  bWaiting = !bWaiting;
}

// -----------------------------------------------------------------------------
// Send a MSG_DONE message
// -----------------------------------------------------------------------------
void NetworkingEngine::SendDoneMessage()
{
  static DWORD lastSend = 0;

  DWORD time = timeGetTime();

  if ( time - lastSend > DONE_MESSAGE_COOLDOWN )
  {
    NetMessage msgDone;
    msgDone.mType = MSG_READY;
    msgDone.nSize = 0;
    PushMessage( msgDone );
    lastSend = time;
  }
}

// -----------------------------------------------------------------------------
// Adds an entity to entity lookup table
// Returns false if entity was already found in table
// -----------------------------------------------------------------------------
bool NetworkingEngine::AddEntity( const EntityIDTable::EntityID &id, 
                                  const EntityIDTable::CreationKey &key, bool mother )
{
  return entityIDTable.Add( id, key, mother );
}

// --------------------------------------------------------------------------------
// Retrieves the ID Translation Table so other things can look up information in it
// --------------------------------------------------------------------------------
const EntityIDTable &NetworkingEngine::GetIDTable()
{
  return entityIDTable;
}

// -----------------------------------------------------------------------------
// Checks for and sends done messages while in waiting state
// -----------------------------------------------------------------------------
void NetworkingEngine::UpdateWaitState()
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
StringVec NetworkingEngine::GetErrStrs() const
{
  return errStrings;
}

// -----------------------------------------------------------------------------
// Determines whether or not the session should be advertised (if host)
// -----------------------------------------------------------------------------
void NetworkingEngine::SetSession( bool open )
{
  bOpenSession = open;
}

// -----------------------------------------------------------------------------
// Clears list of players if browsing games
// -----------------------------------------------------------------------------
void NetworkingEngine::RefreshHosts()
{
  if ( gsMode == GS_JOIN && !bJoining )
  {
    lsPlayers.clear();
  }
}

// -----------------------------------------------------------------------------
// Names of players (used by clients)
// -----------------------------------------------------------------------------
const StringVec &NetworkingEngine::GetPlayerNames() const
{
  return playerNames;
}

// -----------------------------------------------------------------------------
// Copies names to buffer
// Used by host for updating clients' player list
// -----------------------------------------------------------------------------
int NetworkingEngine::CopyNamesToBuf( BYTE *buffer, UINT size, const char *skip )
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
bool NetworkingEngine::CopyBufToNames( BYTE *buffer, UINT size )
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
void NetworkingEngine::ReportError( const std::string &err )
{
  errStrings.push_back( err );
}

// -----------------------------------------------------------------------------
// Add an address to advertise session to
// Only necessary for addresses not on lan
// -----------------------------------------------------------------------------
void NetworkingEngine::AddVLANAddr( const std::string &addr )
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
bool NetworkingEngine::BootPlayer( const char *name )
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
        msgBoot.nSize = (SHORT)CopyNamesToBuf( msgBoot.data, sizeof( msgBoot.data ), name );
        PushMessage( msgBoot );
        qToDrop.push( i );
        return true;
      }
    }
  }
  return false;
}

//=============================================================================

// -----------------------------------------------------------------------------
// Update debugging networking strings
// -----------------------------------------------------------------------------
void NetworkingEngine::UpdateDebug()
{
  std::stringstream ssDebug;

  // Reset String
  debugStrings.c.clear();

  // Title
  debugStrings.push( "Networking Debugger" );

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

  if ( gsMode == GS_MENU ) return;

  // Errors
  for ( StringVecIt i = errStrings.begin(); i != errStrings.end(); ++i )
    debugStrings.push( "ERROR: " + *i );

  // Host or client
  std::string strMyRank = bHost ? "(Host)" : "(Client)";
  debugStrings.push( strMyRank.c_str() );

  // Session state if host
  if ( bHost )
  {
    debugStrings.push( bOpenSession ? "Open Session" : "Closed Session" );
  }

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
StringQueue NetworkingEngine::GetDbgStrs() const
{
  return debugStrings;
}

// -----------------------------------------------------------------------------
// get state of network
// -----------------------------------------------------------------------------
NetworkingEngine::GAME_STATE NetworkingEngine::GetMode() const
{
  return gsMode;
}

// -----------------------------------------------------------------------------
// push a text message
// -----------------------------------------------------------------------------
void NetworkingEngine::PushText( const std::string &text )
{
  NetMessage msgOut;
  msgOut.mType = MSG_TEXT;
  strcpy( (char *)msgOut.data, text.c_str() );
  msgOut.nSize = (SHORT)text.size() + 1;

  PushMessage( msgOut );
}

// -----------------------------------------------------------------------------
// return string buffer for text messages
// -----------------------------------------------------------------------------
StringQueue &NetworkingEngine::GetText()
{
  return netStrings;
}

// -----------------------------------------------------------------------------
// return networking info on user
// -----------------------------------------------------------------------------
const hostent &NetworkingEngine::GetMe() const
{
  return entMe;
}
