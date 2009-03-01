#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

#define ASSERT( c, msg ) if ( !(c) ) { std::cout << "** ERROR: " << msg << " **" << std::endl; return -1; }

template <typename T>
int MovePastNext( T &stream, const std::string &str );

template <typename T>
int ReadAnimSet( char *&buf, T &stream, int pos );

int main( int argc, char *argv[] )
{
  ASSERT( argc > 2, "merge [main file].x [anim file].x [name]" );

  std::fstream mainFile( argv[1] );
  ASSERT( mainFile.is_open(), std::string( "unable to open " ) + argv[1] + " file" );

  std::ifstream animFile( argv[2] );
  ASSERT( mainFile.is_open(), std::string( "unable to open " ) + argv[2] + " file" );

  std::string setName;

  // find the animation set in the anim file
  MovePastNext( animFile, "AnimationSet" );
  int readPos = MovePastNext( animFile, "AnimationSet" );
  if ( argc > 3 )
    setName = argv[3];
  else
    animFile >> setName;
  animFile.seekg( readPos );

  char *animSet;
  int animSetSize = ReadAnimSet( animSet, animFile, readPos );

  int writePos = -1;

  // find position to insert animation set
  MovePastNext( mainFile, "AnimationSet" );
  while ( !mainFile.eof() )
  {
    writePos = MovePastNext( mainFile, "AnimationSet" );
    if ( writePos == -1 ) break;
    std::string buf;
    mainFile >> buf;
    if ( buf == setName )
    {
      writePos = MovePastNext( mainFile, "AnimationSet" );
      mainFile.seekg( writePos );
      break;
    }
  }

  char *backup     = NULL;
  int   backupSize = 0;

  if ( mainFile.eof() )
  {
    mainFile.clear();
    mainFile.seekp( 0, std::ios_base::end );
  }
  else
  {
    backupSize = (int)mainFile.tellg() - writePos + 1;
    backup = new char[backupSize];
    mainFile.seekg( writePos );
    mainFile.read( backup, backupSize );
    mainFile.seekp( writePos );
  }

  mainFile.write( animSet, animSetSize );
  
  if ( backup )
    mainFile.write( backup, backupSize );

  std::cout << "done" << std::endl;

  return 0;
}


template <typename T>
int MovePastNext( T &stream, const std::string &str )
{
  size_t strSize      = str.size();
  size_t strSizeLess1 = strSize - 1;
  int pos = stream.tellg();

  char buf[64];
  memset( buf, 0, sizeof( buf ) );

  while ( !stream.eof() )
  {
    if ( buf[0] == str[0] )
    {
      stream.read( buf + 1, (int)strSizeLess1 );
      buf[strSize] = '\0';
      if ( !strcmp( buf, str.c_str() ) ) break;
    }
    stream.read( buf, 1 );
  }

  return stream.eof() ? -1 : stream.tellg();
}

template <typename T>
int ReadAnimSet( char *&buf, T &stream, int pos )
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

  int size = (int)stream.tellg() - pos;

  buf = new char[size];
  stream.seekg( pos );
  stream.read( buf, size );

  return size;
}
