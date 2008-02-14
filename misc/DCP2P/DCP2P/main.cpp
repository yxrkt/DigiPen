#include <iostream>
#include <conio.h>
#include <winsock2.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <time.h>


// constants
const unsigned long   MESSAGE_HEADER  = 0xababcdcd;
const unsigned long   MESSAGE_DONE    = 0xcdcdabab;
const unsigned short  PORT            = 8010;
const unsigned long   SIZE_OF_MESSAGE = 1024;
const unsigned long   MAX_BUFFER_SIZE = 1018;
const unsigned long   ADDR_STRING_LEN = 15;
const unsigned long   KIBIBYTE        = 1024;
const unsigned long   MEBIBYTE        = 1024 * 1024;
const unsigned long   HEADER_SIZE     = 6;

// globals
SOCKET sTCP;

// function prototypes
bool ErrCheck( int err, unsigned line );
void SetupWinsock();
void DisplaySplash();
void WaitForFile();
void SendFileTo();
std::string GetFileName( std::string path );

// structs
struct Message
{
  Message() : id( MESSAGE_HEADER ) {}

  void copy( BYTE *data, unsigned short size )
  {
    this->size = size;
    memcpy( buffer, data, size );
  }

  unsigned long id;
  unsigned short size;
  BYTE buffer[MAX_BUFFER_SIZE];
};

// entry point
int main()
{
  int err;
  char key = '\0';

  SetConsoleTitle( "Direct Connect P2P" );

  SetupWinsock();
  DisplaySplash();

  do {
    if ( _kbhit() )
    {
      key = (char) _getch();

      switch ( key )
      {
        case '1':
          WaitForFile();
          break;
        case '2':
          SendFileTo();
          break;
      }
    }
    Sleep( 1 );
  } while ( key != 'q' );
  
  err = closesocket( sTCP );
  ErrCheck( err, 69 );

  err = WSACleanup();
  ErrCheck( err, 80 );

  return 0;
}

// Error checking for winsock
bool ErrCheck( int err, unsigned line )
{
  if ( err == SOCKET_ERROR )
  {
    err = WSAGetLastError();

    if ( err == WSAEWOULDBLOCK ) return true;

    std::cout << "winsock error " << err << " (line "<< line << ")" << std::endl;
    exit( -1 );
  }

  return false;
}

// Setup winsock
void SetupWinsock()
{
  int err;

  WSADATA wsadata;
  err = WSAStartup( MAKEWORD( 2, 2 ), &wsadata );
  ErrCheck( err, 108 );

  sTCP = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

  SOCKADDR_IN saBind;
  saBind.sin_family       = AF_INET;
  saBind.sin_port         = htons( PORT );
  saBind.sin_addr.s_addr  = INADDR_ANY;

  err = bind( sTCP, (sockaddr *) &saBind, sizeof( saBind ) );
  ErrCheck( err, 118 );

  unsigned long u1 = 1;
  err = ioctlsocket( sTCP, FIONBIO, &u1 );
  ErrCheck( err, 122 );
}


// Display splash
void DisplaySplash()
{
  std::cout << "Welcome to Direct Connect P2P!" << std::endl
            << "[1] Accept incoming file(s)" << std::endl
            << "[2] Send a file to..." << std::endl
            << "[q] Quit program" << std::endl
            << std::endl;
}

// Waiting for incoming files
void WaitForFile()
{
  bool first = true;

  int err;
  std::ofstream outFile;
  Message msg;
  std::string file;
  SOCKET sRecv;

  err = listen( sTCP, SOMAXCONN );
  ErrCheck( err, 149 );

  SOCKADDR_IN saConnect;
  int saSize;

  std::cout << "waiting for connection..." << std::endl;

  while ( 1 )
  {
    sRecv = accept( sTCP, (sockaddr *) &saConnect, &saSize );
    if ( sRecv == INVALID_SOCKET )
    {
      err = WSAGetLastError();
      if ( err != WSAEWOULDBLOCK )
      {
        std::cout << "error accepting (" << err << ")" << std::endl;
        exit( -1 );
      }
    }

    else
    {
      std::cout << "connection made" << std::endl;
      break;
    }
  }

  while ( msg.id != MESSAGE_DONE )
  {
    err = recv( sRecv, (char *) &msg, SIZE_OF_MESSAGE, 0 );
    if ( ErrCheck( err, 179 ) ) continue;

    if ( msg.id != MESSAGE_HEADER && msg.id != MESSAGE_DONE )
    {
      std::cout << "rejected" << std::endl;
      std::cout << std::hex << msg.id << " vs " << std::hex << MESSAGE_HEADER << std::endl;
      continue;
    }

    if ( first )
    {
      std::cout << "creating the file" << std::endl;
      file = (char *) msg.buffer;
      outFile.open( (char *) msg.buffer, std::ios::binary );
      if ( !outFile )
      {
        std::cout << "failed to create file \'" << file << "\'" << std::endl;
        exit( -1 );
      }
      first = false;
    }

    else
    {
      std::cout << "writing to file" << std::endl;
      outFile.write( (char *) msg.buffer, msg.size );
    }

    Sleep( 1 );
  }

  outFile.close();

  std::cout << "\'" << file << "\' successfully received" << std::endl;
  std::cout << std::endl;

  DisplaySplash();
}

// Sending a file to waiting app
void SendFileTo()
{
  int err;
  std::string file;
  //std::ifstream inFile;
  FILE *inFile;
  unsigned nFileSize;

  char path[MAX_PATH];
  std::cout << "file to send: ";
  std::cin.getline( path, MAX_PATH );

  file = GetFileName( path );

  inFile = fopen( path, "rb" );
  if ( !inFile )
  {
    perror( path );
    return;
  }

  char addr[ADDR_STRING_LEN];
  std::cout << "address to send to: ";
  std::cin.getline( addr, ADDR_STRING_LEN );

  SOCKADDR_IN saDest;
  saDest.sin_family       = AF_INET;
  saDest.sin_port         = htons( PORT );
  saDest.sin_addr.s_addr  = inet_addr( addr );

  err = connect( sTCP, (sockaddr *) &saDest, sizeof( saDest ) );
  ErrCheck( err, 251 );

  std::cout << "connecting..." << std::endl;

  bool done = false;
  fd_set fdWrite, fdExcept;

  do {

    FD_ZERO( &fdWrite );
    FD_ZERO( &fdExcept );

    TIMEVAL tv = { 0 };

    FD_SET( sTCP, &fdWrite );
    FD_SET( sTCP, &fdExcept );

    err = select( 0, NULL, &fdWrite, &fdExcept, &tv );
    ErrCheck( err, 269 );

    // optionally... check if socket is accepted
    // ...

    // check to see if socket is writable
    if ( FD_ISSET( sTCP, &fdWrite ) )
      done = true;

  } while ( !done );

  Message msg;
  strcpy( (char *) msg.buffer, file.c_str() );

  err = send( sTCP, (char *) &msg, (int) strlen( (char *) msg.buffer ) + HEADER_SIZE + 1, 0 );
  ErrCheck( err, 284 );

  // get file size
  fseek( inFile, 0, SEEK_END );
  nFileSize = ftell( inFile );
  rewind( inFile );

  // set proper display of size
  std::stringstream sizeStream;
  if ( nFileSize < KIBIBYTE )
    sizeStream << nFileSize << " b)";
  else if ( nFileSize < KIBIBYTE * KIBIBYTE )
    sizeStream << (float) nFileSize / (float) KIBIBYTE << " Kb)";
  else
    sizeStream << (float) nFileSize / ( (float) KIBIBYTE * (float) KIBIBYTE ) << " Mb)";

  std::cout << "sending file" << file << " (" << sizeStream.str() << std::endl;

  unsigned nSizeLeft = nFileSize;
  clock_t start = clock();

  while ( !feof( inFile ) )
  {
    msg.size = (unsigned short) fread( (char *) msg.buffer, 1, MAX_BUFFER_SIZE, inFile );
    if ( msg.size < MAX_BUFFER_SIZE ) msg.id = MESSAGE_DONE;
    err = send( sTCP, (char *) &msg, (int) msg.size + HEADER_SIZE, 0 );
    ErrCheck( err, 315 );
    do {
      std::cout << "loop" << std::endl;
      fd_set fdWrite;
      FD_ZERO( &fdWrite );
      FD_SET( sTCP, &fdWrite );
      TIMEVAL tv = { 0 };
      err = select( 0, NULL, &fdWrite, NULL, &tv );
      ErrCheck( err, 317 );
    } while ( !FD_ISSET( sTCP, &fdWrite ) );

    Sleep( 0 );
  }

  clock_t totalTime = clock() - start;
  float kbps = 1000.f * (float) nFileSize / ( (float) totalTime * (float) KIBIBYTE );
  std::cout << "total time = " << totalTime << "ms (" << kbps << "Kb/s)" << std::endl;

  fclose( inFile );

  std::cout << "sending of file \'" << file << "\' completed" << std::endl;
  std::cout << std::endl;

  DisplaySplash();
}

// Get name of file from path
std::string GetFileName( std::string path )
{
  std::string::size_type stopper, temp;
  stopper = path.find_last_of( "\\" );
  temp    = path.find_last_of( "/" );
  if ( temp != std::string::npos )
  {
    if ( stopper == std::string::npos || temp > stopper )
      stopper = temp;
  }

  if ( stopper != std::string::npos )
    path.erase( 0, stopper + 1 );

  return path;
}
