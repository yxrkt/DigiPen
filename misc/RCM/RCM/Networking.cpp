#include <sstream>

#include "Networking.h"

// ctor
Networking::Networking()
{
  Init();
}

// dtor
Networking::~Networking()
{
}

// init
Networking::Init()
{
  int err;

    // winsock startup
  WSADATA wsadata;
  err = WSAStartup( MAKEWORD( 2, 2 ), &wsadata );
  ErrCheck( err );

    // create socket
  sUDP = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

    // bind socket
  SOCKADDR_IN saBind;
  saBind.sin_family       = AF_INET;
  saBind.sin_port         = htons( PORT );
  saBind.sin_addr.s_addr  = INADDR_ANY;

  err = bind( sUDP, ( sockadr * ) &saBind, sizeof( SOCKADDR_IN ) );
  ErrCheck( err );

    // set to non-blocking
  unsigned long u1 = 1;
  err = ioctlsocket( sUDP, FIONBIO, &u1 );
  ErrCheck( err );
}

// err check
Networking::ErrCheck( int err )
{
  if ( err == SOCKET_ERROR)
  {
    err = WSAGetLastError();
    if ( err != WSAEWOULDBLOCK )
    {
      std::stringstream ssErr;
      ssErr << err;
      MessageBox( NULL, ssErr.str().c_str(), NULL, MB_ICONERROR );
      exit( 0 );
    }
  }
}
