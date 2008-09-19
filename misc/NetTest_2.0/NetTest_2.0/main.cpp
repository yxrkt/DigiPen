
#include <conio.h>

#include "Networking.h"


enum GAME_STATE { EXIT, MENU, JOIN, SESSION };

void GetName( char *name, int max );
GAME_STATE UpdateMenu( bool &entry );
GAME_STATE UpdateJoin( bool &entry );
GAME_STATE UpdateSession( bool &entry );

const UINT MAX_NAME_LEN = 20;
const UINT MAX_TEXT_LEN = 256;

NetworkingEngine NETWORKING;
char g_name[MAX_NAME_LEN];
char g_text[MAX_TEXT_LEN];

int main()
{
  NETWORKING.AddVLANAddr( "192.168.1.110" );
  NETWORKING.AddVLANAddr( "192.168.1.107" );

  GAME_STATE state  = MENU;
  bool entry        = true;;

  GetName( g_name, MAX_NAME_LEN );

  bool done = false;

  while ( !done )
  {
    GAME_STATE prevState = state;

    switch ( state )
    {
      case MENU:
        state = UpdateMenu( entry );
      break;

      case JOIN:
        state = UpdateJoin( entry );
      break;

      case SESSION:
        state = UpdateSession( entry );
      break;

      case EXIT:
        done = true;
      break;
    }

    if ( state != prevState )
      entry = true;
  }

  return 0;
}

// Retrieve player name
void GetName( char *name, int max )
{
  bool done = false;
  while ( !done )
  {
    std::cout << "Name: ";
    fgets( name, max, stdin );
    done = ( strlen( name ) > 1 );
    std::cout << std::endl;
  }

  name[strlen(name) - 1] = 0;
}

// Update the menu
GAME_STATE UpdateMenu( bool &entry )
{
  if ( entry )
  {
    std::cout << "Welcome to NetTest 2.0, " << g_name << "!" << std::endl
              << "[1]Host Game" << std::endl
              << "[2]Join Game" << std::endl
              << "[3]Exit"      << std::endl;

    entry = false;
  }

  char key = 0;
  if ( _kbhit() )
  {
    key = (char)_getch();
    switch ( key )
    {
      case VK_NUMPAD1: case '1':
        NETWORKING.HostGame();
        return SESSION;
      break;

      case VK_NUMPAD2: case '2':
        NETWORKING.BrowseGames();
        return JOIN;
      break;

      case VK_NUMPAD3: case '3':
        return EXIT;
      break;
    }
  }

  return MENU;
}

// Update list of possible hosts
GAME_STATE UpdateJoin( bool &entry )
{
  if ( entry )
  {
    std::cout << std::endl << "JOIN MENU (Esc to exit)" << std::endl;
    entry = false;
  }

  // update host list
  static DWORD lastUpdate = 0;
  DWORD time = timeGetTime();
  NetworkingEngine::PlayerInfoList hosts = NETWORKING.GetPlayers();
  if ( time - lastUpdate >= 5000 )
  {
    if ( !hosts.empty() )
    {
      std::cout << std::endl << "Available Games: " << std::endl;
      unsigned index = 1;
      for ( NetworkingEngine::PlayerILIter i = hosts.begin(); i != hosts.end(); ++i )
      {
        std::cout << "[" << index << "]" << i->szName << std::endl;
      }
    }
  }

  if ( _kbhit() )
  {
    char key = (char)_getch();
    if ( key >= '1' && key < '1' + (char)hosts.size() )
    {
      NETWORKING.JoinGame( (int)key );
      return SESSION;
    }
    else if ( key == VK_ESCAPE )
    {
      std::cout << std::endl;
      NETWORKING.Reset();
      return MENU;
    }
  }

  return JOIN;
}

// Update chat session
GAME_STATE UpdateSession( bool &entry )
{
  if ( entry )
  {
    std::cout << std::endl << "CHAT LOBBY (Esc to exit)" << std::endl;
    entry = false;
  }

  // post messages from network
  StringQueue &recvd( NETWORKING.GetText() );
  while ( !recvd.empty() )
  {
    std::cout << recvd.front() << std::endl;
    recvd.pop();
  }

  // get your shiz somehow...
  if ( _kbhit() )
  {
    char key = (char)_getch();
    if ( key == VK_ESCAPE )
    {
      std::cout << std::endl;
      return MENU;
    }
    else if ( key == VK_RETURN )
    {
      std::cout << "> ";
      fgets( g_text, MAX_TEXT_LEN, stdin );
      std::stringstream message;
      message << g_name << ": " << g_text;
      NETWORKING.PushText( message.str() );
      std::cout << message.str();
    }
  }

  return SESSION;
}