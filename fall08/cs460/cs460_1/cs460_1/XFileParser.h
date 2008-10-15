#pragma once

#include <fstream>
#include <queue>
#include <string>

#include "ASSERT.h"

typedef std::queue< std::string > StringQueue;

class XFileParser
{
  public:
    XFileParser( const std::string &fileName );
    ~XFileParser();

    bool FindNext( const std::string &token );
    bool ReadToken( std::string &token );

    std::ifstream &XFile;

  private:
    std::ifstream   xFile;
    StringQueue     tokenQueue;
};

