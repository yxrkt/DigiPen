#include <winsock2.h>
#include <iostream>

  // constants
unsigned short PORT = 8003;
unsigned short MESSAGE_ID = 0xABCD;
unsigned MAX_MESSAGE_LEN = 260;
SOCKADDR_IN saBroadcast;

  // variables
SOCKET sUDP;

void ErrCheck(int err)
{
  if (err == SOCKET_ERROR)
  {
    err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK)
      return;

    std::cout << err << std::endl;
    exit(0);
  }
}

  // structs
struct SMessage
{
  SMessage() : id(MESSAGE_ID) {}

  const unsigned short id;
  BYTE buf[260];
};

  // forward decls
void Send();
void Receive();

int main()
{
  int err;

  // startup winsock
  WSAData wsadata;
  err = WSAStartup(MAKEWORD(2,2), &wsadata);
  ErrCheck(err);

  // initialize socket
  sUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // bind socket
  SOCKADDR_IN saBind;
  saBind.sin_family       = AF_INET;
  saBind.sin_port         = htons(PORT);
  saBind.sin_addr.s_addr  = INADDR_ANY;

  err = bind(sUDP, (sockaddr *) &saBind, sizeof(SOCKADDR_IN));

  // set to non-blocking
  unsigned long u1 = 1;
  err = ioctlsocket(sUDP, FIONBIO, &u1);
  ErrCheck(err);

  // enable broadcasts
  saBroadcast.sin_family        = AF_INET;
  saBroadcast.sin_port          = htons(PORT);
  saBroadcast.sin_addr.s_addr   = INADDR_BROADCAST;

  bool bBroadcast = true;
  err = setsockopt(sUDP, SOL_SOCKET, SO_BROADCAST, (char *) &bBroadcast, sizeof(bool));
  ErrCheck(err);

  //Send();
  Receive();

  return 0;
}

//-----------------------------------------------------------------------------
// Send a test message
void Send()
{
  int err;

  // send a test message over internet to perry
  unsigned long ia = inet_addr("76.233.11.79");

  SOCKADDR_IN saExternal;
  saExternal.sin_family   = AF_INET;
  saExternal.sin_port     = htons(PORT);
  saExternal.sin_addr     = *(in_addr *) &ia;

  SMessage msg;

  for (unsigned i = 0; i < 10; ++i)
  {
    err = sendto(sUDP, (char *) &msg, sizeof(SMessage), 0, (sockaddr *) &saBroadcast, sizeof(SOCKADDR_IN));
    ErrCheck(err);
  }
}

//-----------------------------------------------------------------------------
// Loop indefinitely until message is received
void Receive()
{
  int err;

  SOCKADDR_IN saFrom;
  int nFromLen = sizeof(SOCKADDR_IN);
  SMessage msgIn;

  while (1)
  {
    err = recvfrom(sUDP, (char *) &msgIn, sizeof(SMessage), 0, (sockaddr *) &saFrom, &nFromLen);
    ErrCheck(err);

    if (err != SOCKET_ERROR)
    {
      if (msgIn.id == MESSAGE_ID)
      {
        std::cout << "Knock knock, Neo." << std::endl;
        break;
      }
    }

    Sleep(1);
  }

  getchar();
}
