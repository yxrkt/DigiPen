/*!*************************************************************************************************
    @file   SubClasses.h
    @author Alex Serio
    @par    aserio\@digipen.edu
    @par    Block Breakers
    @par    by Digital Dreams and DigiPen Institute of Technology
    @date   September 6, 2008
    
    @brief  Networking Engine sub classes.

    @par    All content copyright © 2008-2009 DigiPen(USA) Corporation. All rights reserved.
***************************************************************************************************/

#include "NetworkingPCH.h"
#include "Networking.h"

// -----------------------------------------------------------------------------
// Packet class
// -----------------------------------------------------------------------------
NetworkingEngine::NetPacket::NetPacket()
  : nHeader( MESSAGE_HEADER )
  , nMsgs( 0 )
  , nBytesUsed( 0 )
{
  memset( messages, 0, sizeof( messages ) );
}

bool NetworkingEngine::NetPacket::operator <=( const NetPacket &pktIn ) const
{
  int l = (int)this->nIndex, r = (int)pktIn.nLRIndex;
  return ( ( abs( l - r ) <= WINDOW_SIZE ) ? ( l <= r ) : ( l > r ) );
}

void NetworkingEngine::NetPacket::Clear()
{
  nMsgs      = 0;
  nBytesUsed = 0;
  memset( messages, 0, sizeof( messages ) );
}

bool NetworkingEngine::NetPacket::PushMessage( const NetMessage &msg )
{
  USHORT nMsgSize = MESSAGE_HEADER_SIZE + msg.nSize;
  
  if ( nMsgSize > ( MAX_MESSAGE_DATA - nBytesUsed ) )
    return false;

  memcpy( messages + nBytesUsed, &msg, nMsgSize );
  nBytesUsed = nBytesUsed + nMsgSize; // += generates a warning with unsigned shorts...
  nMsgs++;
  
  return true;
}

int NetworkingEngine::NetPacket::GetIndexOfFirst( NET_MSG type )
{
  for ( int i = 0; i < nMsgs; ++i )
  {
    if ( operator[]( i )->mType == type )
      return i;
  }

  return INDEX_NOT_FOUND;
}

int NetworkingEngine::NetPacket::Size() const
{
  return (int)( PACKET_HEADER_SIZE + nBytesUsed );
}

NetworkingEngine::NetMessage *NetworkingEngine::NetPacket::operator []( unsigned index )
{
  BYTE *ptr = messages;
  for ( unsigned i = 0; i < nMsgs; ++i )
  {
    if ( i == index )
    {
      return (NetMessage *)ptr;
    }
    ptr += *( (SHORT *)ptr ) + MESSAGE_HEADER_SIZE;
  }
  return NULL;
}

const NetworkingEngine::NetMessage *NetworkingEngine::NetPacket::operator []( unsigned index ) const
{
  return (const NetMessage *)( (NetPacket *)this )->operator []( index );
}

// -----------------------------------------------------------------------------
// Player Class
// -----------------------------------------------------------------------------
NetworkingEngine::Player::Player() : nLRIndex( 0 ), dwResendTimer( 0 )
{
}

NetworkingEngine::Player::Player( const PlayerInfo &player )
  : nLRIndex( 0 ), dwResendTimer( 0 )
{
  saAddr = player.saAddr;
  strcpy( szName, player.szName );
}

NetworkingEngine::Player::Player( const char *name, const SOCKADDR_IN &addr )
{
  strcpy( szName, name );
  saAddr        = addr;
  nLRIndex      = 0;
  dwResendTimer = 0;
}

bool NetworkingEngine::Player::operator ==( const Player &rhs )
{
  return !memcmp( &saAddr.sin_addr, &rhs.saAddr.sin_addr, sizeof( rhs.saAddr.sin_addr ) );
}

std::string NetworkingEngine::PlayerInfo::GetAddr() const
{
  return std::string( inet_ntoa( saAddr.sin_addr ) );
}

// -----------------------------------------------------------------------------
// Message Class
NetworkingEngine::NetMessage::NetMessage()
  : nSize( UNSPECIFIED_SIZE )
  , mType( MSG_INVALID )
  , addrFrom( 0 )
  , flags( 0 )
{
  memset( data, 0, sizeof( data ) );
}
