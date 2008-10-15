#pragma once

#include <d3dx9.h>
#include <fstream>
#include <queue>
#include <string>
#include <map>

#include "ASSERT.h"

typedef std::queue< std::string > StringQueue;

struct KeyFrame
{
  int         tick;
  D3DXMATRIX  matrix;
};

typedef std::vector< KeyFrame >   KeyFrameVec;

struct AnimationKey
{
  int         type;
  KeyFrameVec keyFrames;
};

struct Animation
{
  std::string   name;
  AnimationKey  animKey;
};

typedef std::vector< Animation > AnimationVec;
typedef std::map< std::string, Animation > AnimationMap;

struct AnimationSet
{
  std::string   name;
  AnimationVec  anims;
  AnimationMap  animMap;
};

class XFileParser
{
  public:
    XFileParser( const std::string &fileName );
    ~XFileParser();

    bool GetNextAnimationSet( AnimationSet &animSet );
    std::ifstream &XFile;

  private:
    std::ifstream   xFile;
    StringQueue     stringQueue;

  private:
    bool FindNext( const std::string &token );
    bool GetNextString( std::string &token );

  private:
    static int depth;

  private:
    static bool IsSeperator( const char &a )
    {
      if ( a == '{' )
        depth++;
      else if ( a == '}' )
        depth--;
      return ( a == '{' || a == '}' || a == ';'|| a == ',' );
    }
};

