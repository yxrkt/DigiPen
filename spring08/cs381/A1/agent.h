#ifndef AGENT_H
#define AGENT_H

#include <map>

#include "definitions.h"

enum NODE_TYPE { OPEN, WALL, UNVISITED };

// Square in room
struct Node
{
  Node() : type( UNVISITED ), dist( 0x4FFFFFFF ), hits( 0 ) {}

  NODE_TYPE type;
  int dist;
  int hits;
};

// Map of nodes in room
typedef std::map< int, std::map< int, Node > > RoomMap;

// Vacuum cleaner agent
class Agent
{
  static const int RETREAT_SCORE      = 70;
  static const int RETREAT_TICKER     = 150;
  static const int UNINITIALIZED      = -1;

  public:
    Agent( int );
    Action GetAction( Percept p );
    void PrintDebug();
    
  private:
    Heading GetRetreatOrient();
    Action OrientAndMoveTo( Heading dir );
    int GetFaceX( int dir = UNINITIALIZED ); // dir is of type Heading
    int GetFaceY( int dir = UNINITIALIZED ); // dir is of type Heading
    Action FindBestNode();
    Action Turn( Action dir, bool twice = false );
    void ChooseBest( Node &best, const Node &cur, int &bestDir, int dir );
    void UpdateDist( int x, int y );
  
    RoomMap   room;
    Heading   facing;
    bool      first;
    bool      retreating;
    bool      turned;
    bool      sucked;
    int       score;
    int       maxScore;
    int       toTurn;
    int       ticker;
    int       x;
    int       y;
};

#endif

