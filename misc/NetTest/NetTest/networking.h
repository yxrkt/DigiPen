#include "precompiled.h"

//-----------------------------------------------------------------------------
#ifndef NETWORKING_H
#define NETWORKING_H
//-----------------------------------------------------------------------------

#include "singleton.h"
#include "net_entities.h"

#define Networking Singleton< Networking_impl >::Instance()

  // Defines the type of packet
enum NET_MSG
{
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


// =============================================================================
// WinSock networking class
// =============================================================================
class Networking_impl
{
    // constants
  static const USHORT MAX_NAME_LEN            = 32;
  static const USHORT MESSAGE_HEADER          = 0xFADE;
  static const BYTE   MAX_MESSAGES            = 4;
  static const USHORT PORT                    = 8003;
  static const UINT   MAX_DATA_LEN            = 256;
  static const DWORD  BROADCAST_COOLDOWN      = 1000;
  static const UINT   WINDOW_SIZE             = 20;
  static const DWORD  MAX_WAIT_TIME           = 250;
  static const DWORD  MAX_IDLE_TIME           = 2000;
  static const int    INDEX_NOT_FOUND         = -1;
  static const DWORD  DONE_MESSAGE_COOLDOWN   = 250;
  static const DWORD  HOST_SLEEP_DELAY        = 30;
  static const USHORT MAX_JOIN_ATTEMPTS       = 10;
  static const DWORD  MAX_JOIN_WAIT_TIME      = 1000;

// -----------------------------------------------------------------------------
  public:
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
    typedef std::queue< std::string > StringQueue;

      // ctor/dtor
    Networking_impl();
    ~Networking_impl();

      // public interface
    void Update();
    void Reset();
    void BrowseGames();
    void HostGame();
    void JoinGame( const PlayerInfo &player );
    void LeaveGame();
    void SetSession( bool open );
    void RefreshHosts();
    const StringVec &GetPlayerNames() const;
    PlayerInfoList GetPlayers() const;
    bool Hosting() const;

      // ugly templated stuff
    template < typename TList >
    void PushMessage( Message< TList > *pMsg, EntityIDTable::EntityID dest );

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

      UINT nSize;
      EntityIDTable::CreationKey dest;

      NET_MSG mType;
      UINT addrFrom;
      bool reliable;
      BYTE data[MAX_DATA_LEN];
    };

      // packet struct
    struct NetPacket
    {
      NetPacket();
      int Size() const;
      bool operator <=( const NetPacket &pktIn ) const;

      USHORT nHeader;                     // program identifier
      BYTE nIndex;                        // index of this msg
      BYTE nLRIndex;                      // index of last recvd msg
      BYTE nMsgs;                         // number of packets
      NetMessage Messages[MAX_MESSAGES];  // data being sent
      #define msgFront Messages[0]
    };

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

      // state updaters
    void UpdateJoinMenu();
    void UpdateSession();

      // others

    void AdjustPacketWindow( const NetPacket &pkt, PlayerIter &iPlayer );
    void AdvertiseSession() const;
    void BuildPackets();
    void CleanupPlayers();
    inline int Contains( const NetPacket &pkt, NET_MSG type );
    bool CopyBufToNames( BYTE *buffer, UINT size );
    bool CopyNamesToBuf( BYTE *buffer, UINT size );
    void ErrCheck( int err, const char *msg ) const;
    void ExtractMessages( const NetPacket &pkt );
    PlayerIter GetPlayer( const UINT addr );
    PlayerIter GetPlayer( const SOCKADDR_IN &addr );
    void IdleCheck();
    bool IsNext( const BYTE first, const BYTE second ) const;
    void ManageLagger( PlayerIter &iPlayer );
    void PushMessage( const NetMessage &msg, bool relay = false, bool reliable = true );
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
    StringVec     playerNames;             // player names

    mutable DWORD dwAdTimer;              // timer used for advertising session
    mutable DWORD dwIdleTimer;            // timer used for sending checkup Messages
    mutable DWORD dwLastJoinAttempt;      // time of last join attempt

      // debug data
    StringQueue   netStrings;
    StringQueue   debugStrings;
    StringVec     errStrings;
    UINT          nMaxMsgs;
    UAddrSet      waitSet;
};

#include "net_templates.cpp"

#endif
