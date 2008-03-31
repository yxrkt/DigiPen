#include "precompiled.h"

#include "networking.h"

// -----------------------------------------------------------------------------
// Packet class
// -----------------------------------------------------------------------------
Networking_impl::NetPacket::NetPacket() : nHeader( MESSAGE_HEADER ), nMsgs( 0 ) {}

bool Networking_impl::NetPacket::operator <=( const NetPacket &pktIn ) const
{
  int l = (int)this->nIndex, r = (int)pktIn.nLRIndex;
  return ( ( abs( l - r ) <= WINDOW_SIZE ) ? ( l <= r ) : ( l > r ) );
}

int Networking_impl::NetPacket::Size() const
{
  int size = sizeof( NetPacket );
  size -= ( ( MAX_MESSAGES - nMsgs ) * sizeof( NetMessage ) );
  return size;
}

// -----------------------------------------------------------------------------
// Player
// -----------------------------------------------------------------------------
Networking_impl::Player::Player() : nLRIndex( 0 ), dwResendTimer( 0 )
{
}

Networking_impl::Player::Player( const PlayerInfo &player )
  : nLRIndex( 0 ), dwResendTimer( 0 )
{
  saAddr = player.saAddr;
  strcpy( szName, player.szName );
}

Networking_impl::Player::Player( const char *name, const SOCKADDR_IN &addr )
{
  strcpy( szName, name );
  saAddr        = addr;
  nLRIndex      = 0;
  dwResendTimer = 0;
}

bool Networking_impl::Player::operator ==( const Player &rhs )
{
  return !memcmp( &saAddr.sin_addr, &rhs.saAddr.sin_addr, sizeof( rhs.saAddr.sin_addr ) );
}

std::string Networking_impl::PlayerInfo::GetAddr() const
{
  return std::string( inet_ntoa( saAddr.sin_addr ) );
}

// -----------------------------------------------------------------------------
// retrieve address in form of a string for PlayerInfo struct
Networking_impl::NetMessage::NetMessage()
{
  memset( this, 0, sizeof( NetMessage ) );
}
