#include <ctime> // for random seed
#include <cmath>
#include "agent.h"
#include <iostream>
#include <sstream>

// 
Agent::Agent( int )
  : facing( NORTH )
  , first( true )
  , retreating( false )
  , turned( false )
  , sucked( false )
  , score( 0 )
  , maxScore( 0 )
  , toTurn( 0 )
  , ticker( 0 )
{
}

// get vacuum's next move
Action Agent::GetAction( Percept p )
{
  // on home node
  if ( p.home )
  {
    if ( first )
    {
      x = 0;
      y = 0;
      
      room[x][y].type = OPEN;
      room[x][y].dist = 0;
      room[x][y].hits = 1;
      first = false;
      return FORWARD;
    }
    
    if ( retreating ) return SHUTOFF;
  }
  
  // if returning home...
  if ( retreating )
  {
    // orient if not facing right direction
    return OrientAndMoveTo( GetRetreatOrient() );
  }  
  
  // otherwise...
  
  if ( p.bump )
  {
    room[x + GetFaceX()][y + GetFaceY()].type = WALL;
    
    return FindBestNode();
  }
  
  else
  {
    if ( turned || sucked ) return FindBestNode();
    
    // last action was a move; update position and node
    room[x += GetFaceX()][y += GetFaceY()].type = OPEN;
    room[x][y].hits++;
    UpdateDist( x, y );
    
    // clean this potentially new node if dirty
    if ( p.dirt )
    {
      sucked = true;
      score += 20;
      ticker = 0;
      if ( score >= RETREAT_SCORE ) retreating = true;
      return SUCK;
    }
    
    return FindBestNode();
  }
}

// Gets orientation for node closest to home
Heading Agent::GetRetreatOrient()
{
  std::pair<int, Heading> dest;
  dest.first = UNINITIALIZED;
  
  // check above
  if ( room[x][y - 1].type == OPEN )
  {
    dest.first  = room[x][y - 1].dist;
    dest.second = NORTH;
  }
  
  // check right
  if ( room[x + 1][y].type == OPEN  )
  {
    if ( room[x + 1][y].dist < dest.first || dest.first == UNINITIALIZED )
    {
      dest.first  = room[x + 1][y].dist;
      dest.second = EAST;
    }
  }
  
  // check down
  if ( room[x][y + 1].type == OPEN  )
  {
    if ( room[x][y + 1].dist < dest.first || dest.first == UNINITIALIZED )
    {
      dest.first  = room[x][y + 1].dist;
      dest.second = SOUTH;
    }
  }
  
  // check left
  if ( room[x - 1][y].type == OPEN  )
  {
    if ( room[x - 1][y].dist < dest.first || dest.first == UNINITIALIZED )
    {
      dest.first  = room[x - 1][y].dist;
      dest.second = WEST;
    }
  }
  
  return dest.second;
}

// Used only when retreating
Action Agent::OrientAndMoveTo( Heading dir )
{
  // if already oriented in direction, move to node
  if ( facing == dir )
  {
    y += GetFaceY();
    x += GetFaceX();
    
    return FORWARD;
  }
  
  // otherwise, turn
  if ( ( facing + 3 ) % 4 == dir )
  {
    return Turn( LEFT );
  }
  
  return Turn( RIGHT );
}

// get x direction offset for dir
int Agent::GetFaceX( int dir )
{
  if ( dir == UNINITIALIZED ) dir = facing;
  if ( dir == EAST ) return 1;
  if ( dir == WEST ) return -1;
  return 0;
}

// get y direction offset for dir
int Agent::GetFaceY( int dir )
{
  if ( dir == UNINITIALIZED ) dir = facing;
  if ( dir == NORTH ) return -1;
  if ( dir == SOUTH ) return 1;
  return 0;
}

// turn towards or move to best nearby node (not coming home)
Action Agent::FindBestNode()
{
  sucked = false;
  score--;
  if ( ++ticker == RETREAT_TICKER )
  {
    retreating = true;
    return Turn( LEFT );
  }
  
  if ( toTurn )
  {
    int temp = toTurn;
    toTurn   = 0;
    return Turn( (Action) temp );
  }
  
  int leftDir   = ( facing + 3 ) % 4;
  int rightDir  = ( facing + 1 ) % 4;
  int crossDir  = ( facing + 2 ) % 4;
  
  Node best, cur;
  int dir = facing;
  turned = false;
  
  // node ahead
  cur = room[x + GetFaceX()][y + GetFaceY()];
  if ( cur.type == UNVISITED ) return FORWARD;
  else best = room[x + GetFaceX()][y + GetFaceY()];

  // left node
  cur = room[x + GetFaceX( leftDir )][y + GetFaceY( leftDir )];
  if ( cur.type == UNVISITED ) return Turn( LEFT );
  else ChooseBest( best, cur, dir, leftDir );
  
  // right node
  cur = room[x + GetFaceX( rightDir )][y + GetFaceY( rightDir )];
  if ( cur.type == UNVISITED ) return Turn( RIGHT );
  else ChooseBest( best, cur, dir, rightDir );
  
  // cross node
  cur = room[x + GetFaceX( crossDir )][y + GetFaceY( crossDir )];
  if ( cur.type == UNVISITED ) return Turn( RIGHT );  
  else ChooseBest( best, cur, dir, crossDir );
  
  // return appropriate action
  if ( dir == facing )    return FORWARD;
  if ( dir == leftDir )   return Turn( LEFT );
  if ( dir == rightDir )  return Turn( RIGHT );
  return Turn( RIGHT, true ); // crossDir
}

// set turning bool on and return action
Action Agent::Turn( Action dir, bool twice )
{
  turned = true;
  if ( twice ) toTurn = dir;
  facing = (Heading) ( ( dir == LEFT ) ? ( ( facing + 3 ) % 4 ) : ( ( facing + 1 ) % 4 ) );
  
  return dir;
}

// look for nodes in order of:
//    unexplored
//    least visited
//    farthest from home
void Agent::ChooseBest( Node &best, const Node &cur, int &bestDir, int dir )
{
  bool replace = false;
  
  if ( cur.type == OPEN )
  {
    if ( cur.hits < best.hits ) replace = true;
    if ( cur.hits == best.hits && cur.dist > best.dist ) replace = true;
    if ( best.type == WALL ) replace = true;
  }
  
  if ( replace )
  {
    best = cur;
    bestDir = dir;
  }
}

// recursively update distances
void Agent::UpdateDist( int nx, int ny )
{
  int min = UNINITIALIZED;
  Node cur;
  
  // get dist value for current node
  cur = room[nx][ny - 1]; // check north
  if ( ( cur.type == OPEN ) && ( cur.dist + 1 < min || min == UNINITIALIZED ) )
    min = cur.dist + 1;
  cur = room[nx + 1][ny]; // check east
  if ( ( cur.type == OPEN ) && ( cur.dist + 1 < min || min == UNINITIALIZED ) )
    min = cur.dist + 1;
  cur = room[nx][ny + 1]; // check south
  if ( ( cur.type == OPEN ) && ( cur.dist + 1 < min || min == UNINITIALIZED ) )
    min = cur.dist + 1;
  cur = room[nx - 1][ny]; // check west
  if ( ( cur.type == OPEN ) && ( cur.dist + 1 < min || min == UNINITIALIZED ) )
    min = cur.dist + 1;
  
  // set dist
  room[nx][ny].dist = min;
  
  // adjust neighboring nodes' values if needed
  cur = room[nx][ny - 1]; // north
  if ( ( cur.type == OPEN ) && ( cur.dist > min + 1 ) )
    UpdateDist( nx, ny - 1 );
  cur = room[nx + 1][ny]; // east
  if ( ( cur.type == OPEN ) && ( cur.dist > min + 1 ) )
    UpdateDist( nx + 1, ny );
  cur = room[nx][ny + 1]; // south
  if ( ( cur.type == OPEN ) && ( cur.dist > min + 1 ) )
    UpdateDist( nx, ny + 1 );
  cur = room[nx - 1][ny]; // west
  if ( ( cur.type == OPEN ) && ( cur.dist > min + 1 ) )
    UpdateDist( nx - 1, ny );
}

void Agent::PrintDebug()
{
  static int curFace = UNINITIALIZED;
  if ( curFace == UNINITIALIZED ) curFace = facing;
  
  Node cur = room[x][y];
  Node north = room[x][y - 1];
  Node east  = room[x + 1][y];
  Node south = room[x][y + 1];
  Node west  = room[x - 1][y];
  
  std::string dir;
  if ( curFace == 0 ) dir = "north";
  else if ( curFace == 1 ) dir = "east";
  else if ( curFace == 2 ) dir = "south";
  else if ( curFace == 3 ) dir = "west";

  std::cout << "* Agent internal info *:" << std::endl
            << "------------------------" << std::endl
            << "position:   " << x << ", " << y << std::endl
            << "facing:     " << dir << std::endl
            << "toTurn:     " << ( toTurn != 0 ) << std::endl
            << "ticks:      " << ticker << "/" << RETREAT_TICKER << std::endl
            << "cur dist:   " << cur.dist << std::endl
            << "north dist: " << north.dist << std::endl
            << "east dist:  " << east.dist << std::endl
            << "south dist: " << south.dist << std::endl
            << "west dist:  " << west.dist << std::endl;
  
  curFace = facing;
}

