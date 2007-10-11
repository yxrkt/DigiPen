#include <winsock2.h>
#include <iostream>

  // constants
unsigned short PORT = 8003;
unsigned short MESSAGE_ID = 0xABCD;
unsigned MAX_MESSAGE_LEN = 260;

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

struct SMessage
{
  SMessage() : id(MESSAGE_ID) {}

  const unsigned short id;
  BYTE buf[260];
};

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
  SOCKADDR_IN saBroadcast;
  saBroadcast.sin_family        = AF_INET;
  saBroadcast.sin_port          = htons(PORT);
  saBroadcast.sin_addr.s_addr   = INADDR_BROADCAST;

  bool bBroadcast = true;
  err = setsockopt(sUDP, SOL_SOCKET, SO_BROADCAST, (char *) &bBroadcast, sizeof(bool));
  ErrCheck(err);

  // send a test message over internet to self
  unsigned long ia = inet_addr("66.192.186.99");
  SOCKADDR_IN saExternal;
  saExternal.sin_family       = AF_INET;
  saExternal.sin_port         = htons(PORT);
  saExternal.sin_addr         = *(in_addr *) &ia;

  SMessage msg;
  strcpy((char *) msg.buf, "SUCCESS");
  err = sendto(sUDP, (char *) &msg, sizeof(SMessage), 0, (sockaddr *) &saExternal, sizeof(SOCKADDR_IN));
  ErrCheck(err);

  while (1)
  {
    SMessage msgIn;
    sockaddr saFrom;
    int nFromLen = sizeof(sockaddr);
    //memset(msgIn.buf, '\0', MAX_MESSAGE_LEN);

    err = recvfrom(sUDP, (char *) &msgIn, sizeof(SMessage), 0, &saFrom, &nFromLen);
    ErrCheck(err);

    if (msgIn.id == MESSAGE_ID)
    {
      std::cout << (char *) msgIn.buf << std::endl;
      break;
    }
  }

  return 0;
}
