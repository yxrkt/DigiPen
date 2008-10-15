#include "XFileParser.h"

int XFileParser::depth = 0;

XFileParser::XFileParser( const std::string &fileName )
: XFile( xFile )
, xFile( fileName.c_str() )
{
  ASSERT( xFile.is_open(), "Opening x file failed." );
}

XFileParser::~XFileParser()
{
}

bool XFileParser::FindNext( const std::string &token )
{
  std::string strBuf;
  do {
    xFile >> strBuf;
    if ( xFile.eof() )
      return false;
  } while ( strBuf.find( token ) == std::string::npos );
  return true;
}

bool XFileParser::GetNextString( std::string &token )
{
  while ( stringQueue.empty() )
  {
    std::string strBuf( "" );
    xFile >> strBuf;
    if ( strBuf.empty() )
      return false;

    std::string::iterator curPos = strBuf.begin();
    std::string::iterator curEnd;
    do {
      curEnd = std::find_if( curPos, strBuf.end(), IsSeperator );
      if ( curPos != curEnd )
      {
        stringQueue.push( std::string( curPos, curEnd ) );
      }
      curPos = ( curEnd == strBuf.end() ? curEnd : curEnd + 1 );
    } while ( curPos != strBuf.end() );
  }

  token = stringQueue.front();
  stringQueue.pop();

  return true;
}

bool XFileParser::GetNextAnimationSet( AnimationSet &animSet )
{
  if ( !FindNext( "AnimationSet" ) )
    return false;

  std::string strBuf;
  GetNextString( strBuf );
  animSet.name = strBuf;

  // make an animation
  Animation anim;
  GetNextString( strBuf );
  ASSERT( strBuf == "Animation", "parser error" );
  GetNextString( anim.name );

  AnimationKey animKey;
  FindNext( "AnimationKey" );
  GetNextString( strBuf );
  animKey.type = atoi( strBuf.c_str() );
  GetNextString( strBuf );
  animKey.keyFrames.reserve( atoi( strBuf.c_str() ) );

  size_t nFrames = animKey.keyFrames.capacity();
  for ( size_t i = 0; i < nFrames; ++i )
  {
    KeyFrame keyFrame;
    GetNextString( strBuf );
    keyFrame.tick = atoi( strBuf.c_str() );
    GetNextString( strBuf );  // should be 16
    for ( size_t i = 0; i < 16; ++i )
    {
      xFile.ignore();
      xFile >> keyFrame.matrix.m[i / 4][i % 4];
    }
    animKey.keyFrames.push_back( keyFrame );
  }

  //std::string strBuf;
  //xFile >> strBuf;
  //animSet.name = ( strBuf != '{' ? strBuf : "" );


  //GetNextString( strBuf );

  return true;
}
