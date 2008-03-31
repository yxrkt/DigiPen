#include "precompiled.h"

#include "networking.h"

// =============================================================================
// Ctor: Initializes winsock, UDP socket, and various other data
// =============================================================================
Networking_impl::Networking_impl()
  : nLSIndex( 255 )
  , gsMode( GS_MENU )
  , bHost( false )
  , nMaxMsgs( 0 )
  , bWaiting( false )
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
    MessageBox( NULL, ssErrMsg.str().c_str(), "Winsock Error", MB_ICONERROR );
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
  saBroadcast.sin_addr.s_addr = inet_addr( "192.168.1.110" );//INADDR_BROADCAST;

    // Create sockaddr for user
  SetMySA();
}

// =============================================================================
// Destructor: Does nothing!
// =============================================================================
Networking_impl::~Networking_impl() {}

// =============================================================================
// Updates networking!
// =============================================================================
void Networking_impl::Update()
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
void Networking_impl::UpdateSession()
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
void Networking_impl::UpdateJoinMenu()
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
        strcpy( (char *)msgJoin.data, entMe.h_name );
        PushMessage( msgJoin, false, false );
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
Networking_impl::PlayerInfoList Networking_impl::GetPlayers() const
{
  PlayerInfoList lsPlayerInfo;

  for ( PlayerIterC i = lsPlayers.begin(); i != lsPlayers.end(); ++i )
    lsPlayerInfo.push_back( *i );

  return lsPlayerInfo;
}
