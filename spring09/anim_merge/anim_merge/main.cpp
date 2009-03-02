#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

#define ASSERT( c, msg ) if ( !(c) ) { std::cout << "** ERROR: " << msg << " **" << std::endl; return -1; }
#define SAFE_DELETE_ARRAY( p ) if ( p != NULL ) { delete [] p; p = NULL; }

template <typename T>
int MovePastNext( T &stream, const std::string &str );

template <typename T>
int ReadAnimSet( char *&buf, T &stream, int pos, const std::string &name );

int main( int argc, char *argv[] )
{
  ASSERT( argc > 2, "merge [main file].x [anim file].x [name]" );

  std::fstream mainFile( argv[1] );
  ASSERT( mainFile.is_open(), std::string( "unable to open " ) + argv[1] + " file" );

  std::ifstream animFile( argv[2] );
  ASSERT( mainFile.is_open(), std::string( "unable to open " ) + argv[2] + " file" );

  std::string setName;
  std::string baseName;

  // find the animation set in the anim file
  MovePastNext( animFile, "AnimationSet" );
  int readPos = MovePastNext( animFile, "AnimationSet" );
  animFile >> baseName;
  setName  = ( argc > 3 ) ? argv[3] : baseName;
  animFile.seekg( readPos );

  std::string header( "AnimationSet " + baseName );
  int headerLen = (int)header.length() + 1;

  char *animSet;
  int animSetSize = ReadAnimSet( animSet, animFile, readPos + headerLen, setName );

  int headerPos = -1;
  int writePos  = -1;
  int backupPos =  0;

  // find position to insert animation set
  MovePastNext( mainFile, "AnimationSet" );
  while ( !mainFile.eof() )
  {
    headerPos = MovePastNext( mainFile, "AnimationSet" );
    if ( headerPos == -1 ) break;
    std::string buf("");
    mainFile >> buf;
    if ( buf == setName )
    {
      backupPos = MovePastNext( mainFile, "AnimationSet" );
      break;
    }
  }

  char *backup     = NULL;
  int   backupSize = 0;
  int   endOfFile  = 0;

  header    = "AnimationSet " + setName;
  headerLen = (int)header.length() + 1;

  writePos  = headerPos + headerLen;

  if ( headerPos == -1 )
  {
    mainFile.clear();
    mainFile.seekp( 0, std::ios_base::end );
  }
  else
  {
    mainFile.seekg( 0, std::ios_base::end );
    endOfFile  = (int)mainFile.tellg();
    backupSize = endOfFile - backupPos;
    backup = new char[backupSize];
    mainFile.seekg( backupPos );
    mainFile.read( backup, backupSize );
    mainFile.clear(); // failbit will most likely be set from last read
    mainFile.seekp( headerPos );
  }

  char newLine = '\n';
  mainFile.write( &newLine, sizeof( newLine ) );
  mainFile.write( animSet, animSetSize );
  mainFile.write( &newLine, sizeof( newLine ) );
  mainFile.write( &newLine, sizeof( newLine ) );

  if ( backup )
  {
    int last = backupSize - 1;
    while ( backup[last--] != '}' ) ;
    mainFile.write( backup, last + 2 );

    int curPos = mainFile.tellp();
    if ( curPos < endOfFile )
    {
      int nEmptyBytes = endOfFile - curPos + 1;
      char *emptySpace = new char[nEmptyBytes];
      memset( emptySpace, ' ', nEmptyBytes );
      mainFile.write( emptySpace, nEmptyBytes );
      SAFE_DELETE_ARRAY( emptySpace );
    }
  }

  std::cout << "done" << std::endl;

  mainFile.close();
  animFile.close();

  SAFE_DELETE_ARRAY( animSet );
  SAFE_DELETE_ARRAY( backup );

  return 0;
}


template <typename T>
int MovePastNext( T &stream, const std::string &str )
{
  int strSize      = (int)str.size();
  int strSizeLess1 = strSize - 1;

  char buf[64];
  memset( buf, 0, sizeof( buf ) );

  while ( !stream.eof() )
  {
    if ( buf[0] == str[0] )
    {
      stream.read( buf + 1, (int)strSizeLess1 );
      if ( stream.eof() ) break;
      buf[strSize] = '\0';
      if ( !strcmp( buf, str.c_str() ) ) break;
    }
    stream.read( buf, 1 );
  }

  return stream.eof() ? -1 : (int)stream.tellg() - strSize;
}

template <typename T>
int ReadAnimSet( char *&buf, T &stream, int pos, const std::string &name )
{
  bool firstLevel = true;

  int level = 0;

  char cBuf;

  stream.seekg( pos );

  while ( !stream.eof() )
  {
    stream.read( &cBuf, 1 );
    if ( cBuf == '{' )
    {
      level++;
      firstLevel = false;
    }
    else if ( cBuf == '}' )
    {
      level--;
    }

    if ( !firstLevel && level == 0 ) break;
  }

  std::string header( "AnimationSet " + name );
  int headerLen = (int)header.length() + 1;
  int size = (int)stream.tellg() - pos;

  buf = new char[size + headerLen];
  memset( buf, 0, size + headerLen );
  stream.seekg( pos );
  memcpy( buf, header.c_str(), headerLen );
  buf[headerLen - 1] = ' ';
  stream.read( buf + headerLen, size );

  for ( int last = headerLen + size - 1; last > 0; --last )
  {
      if ( buf[last] == '}' )
        return last + 1;
  }

  return -1;
}
