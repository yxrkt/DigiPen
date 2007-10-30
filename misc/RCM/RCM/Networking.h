#include <winsock2.h>

class Networking
{
  public:
    static const UINT MAX_DATA_LEN = 260;
    static const UINT MAX_PACKETS  = 4;
    static const USHORT PORT       = 80;

    enum PACKET_TYPE 
    { 
      PK_TEXT, 
      PK_JOIN
    };

    struct SPacket
    {
      BYTE nType;
      BYTE data[ MAX_DATA_LEN ];
    };

    struct SMessage
    {
      USHORT nId;
    };

    Networking();
    ~Networking();

    void PushPkt();
    void Update();

  private:
    void Init();
    void ErrCheck( int err );

    SOCKET sUDP;
};
