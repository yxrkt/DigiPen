/*!*************************************************************************************************
    @file   Networking.h
    @author Alex Serio
    @par    aserio\@digipen.edu
    @par    Block Breakers
    @par    by Digital Dreams and DigiPen Institute of Technology
    @date   September 5, 2008
    
    @brief  This is the class interface for the Network engine.

    @par    All content copyright © 2008-2009 DigiPen(USA) Corporation. All rights reserved.
***************************************************************************************************/

#pragma once

#include "NetworkingPCH.h"
#include "EntityTable.h"

class NetworkingEngine
{
    // constants
  static const USHORT MAX_NAME_LEN            = 32;
  static const USHORT MESSAGE_HEADER          = 0xB10C;
  static const USHORT PORT                    = 8003;
  static const USHORT MAX_DATA_LEN            = 256;
  static const DWORD  BROADCAST_COOLDOWN      = 1000;
  static const UINT   WINDOW_SIZE             = 20;
  static const DWORD  MAX_WAIT_TIME           = 250;
  static const DWORD  MAX_IDLE_TIME           = 2000;
  static const int    INDEX_NOT_FOUND         = -1;
  static const DWORD  DONE_MESSAGE_COOLDOWN   = 250;
  static const DWORD  HOST_SLEEP_DELAY        = 30;
  static const USHORT MAX_JOIN_ATTEMPTS       = 10;
  static const DWORD  MAX_JOIN_WAIT_TIME      = 1000;
  static const USHORT MAX_MESSAGE_DATA        = 1000;
  static const SHORT  UNSPECIFIED_SIZE        = -1;

// -----------------------------------------------------------------------------
  public:
      // Defines the type of message
    enum NET_MSG
    {
      MSG_INVALID,
      MSG_HOSTING,
      MSG_JOIN,
      MSG_LEAVE,
      MSG_BOOT,
      MSG_CHECK,
      MSG_DATA,
      MSG_TEXT,
      MSG_READY,
      MSG_PLAYERS
    };

      // Game state enum
    enum GAME_STATE
    {
      GS_MENU,
      GS_JOIN,
      GS_SESSION
    };

      // high level player info
    struct PlayerInfo
    {
      static const UINT TIME_STRING_SIZE = 8;

      char szName[MAX_NAME_LEN];
      SOCKADDR_IN saAddr;
      char szTime[TIME_STRING_SIZE];

      std::string GetAddr() const;
    };

    typedef std::list< PlayerInfo > PlayerInfoList;
    typedef PlayerInfoList::iterator PlayerILIter;

      // ctor/dtor
    NetworkingEngine();
    ~NetworkingEngine();

      // public interface
    void Update();
    void Reset();
    void BrowseGames();
    void HostGame();
    void JoinGame( unsigned index );
    void JoinGame( const PlayerInfo &player );
    void LeaveGame();
    void SetSession( bool open );
    void RefreshHosts();
    const StringVec &GetPlayerNames() const;
    PlayerInfoList GetPlayers() const;
    bool Hosting() const;
    void AddVLANAddr( const std::string &addr );
    bool BootPlayer( const char *name );

      // ugly templated stuff
    template < typename T >
    void PushMessage( const T &msg, EntityIDTable::EntityID dest );

      // other
    void PushText( const std::string &text );
    StringQueue &GetText();
    GAME_STATE GetMode() const;
    const hostent &GetMe() const;
    StringQueue GetDbgStrs() const;
    StringVec GetErrStrs() const;
    bool AmWaiting() const;
    void ToggleWaitState();
    bool AddEntity( const EntityIDTable::EntityID &id, const EntityIDTable::CreationKey &key, bool mother );
    const EntityIDTable &GetIDTable();

// -----------------------------------------------------------------------------
  private:
      // message struct
    struct NetMessage
    {
      NetMessage();

      enum FLAGS { FLAG_RELIABLE = 1 << 0, FLAG_RELAY_BACK = 1 << 1 };

      //EntityIDTable::CreationKey dest;

      SHORT nSize;                        // size of data -- MUST BE FIRST
      BYTE flags;                         // 0 reliable, 1 relay
      NET_MSG mType;                      // message identifier
      UINT addrFrom;                      // sender's address
      BYTE data[MAX_DATA_LEN];            // message's contents
    };

    static const USHORT MESSAGE_HEADER_SIZE = sizeof( NetMessage ) - MAX_DATA_LEN;

      // packet struct
    struct NetPacket
    {
      NetPacket();
      bool PushMessage( const NetMessage &msg );
      int Size() const;
      void Clear();
      int GetIndexOfFirst( NET_MSG type );
      bool operator <=( const NetPacket &pktIn ) const;
      NetMessage *operator []( unsigned index );
      const NetMessage *operator []( unsigned index ) const;

      USHORT  nHeader;                      // program identifier
      BYTE    nIndex;                       // index of this packet
      BYTE    nLRIndex;                     // index of last recvd packet
      BYTE    nMsgs;                        // number of messages in buffer
      USHORT  nBytesUsed;                   // number of bytes used in message buffer
      BYTE    messages[MAX_MESSAGE_DATA];   // data being sent
    };

    static const USHORT PACKET_HEADER_SIZE = sizeof( NetPacket ) - MAX_MESSAGE_DATA;

      // player struct
    struct Player : public PlayerInfo
    {
      std::queue< NetPacket > qPktWnd;
      BYTE nLRIndex;
      DWORD dwResendTimer;

      Player();
      Player( const PlayerInfo &player );
      Player( const char *name, const SOCKADDR_IN &addr );
      bool operator ==( const Player &rhs );
    };

      // typedefs
    typedef std::list< Player > PlayerList;
    typedef PlayerList::iterator PlayerIter;
    typedef PlayerList::const_iterator PlayerIterC;
    typedef std::queue< PlayerIter > PlayerItQueue;
    typedef std::queue< NetMessage, std::list< NetMessage > > MessageQueue;
    typedef std::list< NetMessage >::iterator MessageListIt, MessageQueueIt;
    typedef std::set< unsigned > UAddrSet;
    typedef std::vector< SOCKADDR_IN > SockAddrVec;
    typedef SockAddrVec::const_iterator SockAddrVecItC;

      // state updaters
    void UpdateJoinMenu();
    void UpdateSession();

      // callbacks
    bool ( *_DispatchToGame )( const BYTE *pRawData, UINT nSize, const EntityIDTable::EntityID &dest );
    void ( *_GetMessageInfo )( const BYTE *pRawData, bool &reliable, bool &relay );
    void ( *_PopupMessage )( const std::string & );

      // others
    void AdjustPacketWindow( const NetPacket &pkt, PlayerIter &iPlayer );
    void AdvertiseSession() const;
    void BuildPackets();
    void CleanupPlayers();
    bool CopyBufToNames( BYTE *buffer, UINT size );
    int CopyNamesToBuf( BYTE *buffer, UINT size, const char *skip = NULL );
    void ErrCheck( int err, const char *msg ) const;
    void ExtractMessages( const NetPacket &pkt );
    PlayerIter GetPlayer( const std::string &name );
    PlayerIter GetPlayer( const UINT addr );
    PlayerIter GetPlayer( const SOCKADDR_IN &addr );
    void IdleCheck();
    bool IsNext( const BYTE first, const BYTE second ) const;
    void ManageLagger( PlayerIter &iPlayer );
    void PushMessage( const NetMessage &msg );
    void ReceivePackets();
    void ReportError( const std::string &err );
    void ResendPackets( const PlayerIter &iPlayer ) const;
    void SendDoneMessage();
    void SendMessages();
    void SetMySA();
    void TranslateMessages();
    void UpdateDebug();
    void UpdateHostList();
    void UpdateWaitState();

      // data
    MessageQueue  qMsgsIn;                // queue of received & extracted messages
    MessageQueue  qMsgsOut;               // queue of messages to be sent
    NetPacket     reliablePktOut;         // reliable outgoing packet
    NetPacket     unreliablePktOut;       // unreliable outgoing packet
    PlayerList    lsPlayers;              // list of players
    BYTE          nLSIndex;               // index of last sent message
    SOCKET        sUDP;                   // UDP socket
    SOCKADDR_IN   saMyAddr;               // user's sockaddr
    hostent       entMe;                  // struct w/ useful data of user
    SOCKADDR_IN   saBroadcast;            // sockaddr for broadcasts
    GAME_STATE    gsMode;                 // determines updating method
    bool          bHost;                  // true if hosting
    bool          bWaiting;               // true if only networking should be updated
    PlayerItQueue qToDrop;                // queue of players to remove from game
    EntityIDTable entityIDTable;          // entity lookup table
    DWORD         nJoinAttempt;           // n'th attempt to join
    bool          bJoining;               // true if in the process of joining game
    bool          bOpenSession;           // true if host and advertising game
    std::string   creationTime;           // time of creation for advertising session
    StringVec     playerNames;            // player names
    SockAddrVec   vLANAddrs;              // 'virtual lan' addrs; will 'broadcast' to

    mutable DWORD dwAdTimer;              // timer used for advertising session
    mutable DWORD dwIdleTimer;            // timer used for sending checkup messages
    mutable DWORD dwLastJoinAttempt;      // time of last join attempt

      // debug
    StringQueue   netStrings;
    StringQueue   debugStrings;
    StringVec     errStrings;
    UINT          nMaxMsgs;
    UAddrSet      waitSet;
};

#include "Templated.cpp"
