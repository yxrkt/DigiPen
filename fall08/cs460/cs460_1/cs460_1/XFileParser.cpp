#include "XFileParser.h"


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
    if ( !ReadToken( strBuf ) )
      return false;
  } while ( strBuf == token );
  return true;
}

bool XFileParser::ReadToken( std::string &token )
{
  return true;
}
