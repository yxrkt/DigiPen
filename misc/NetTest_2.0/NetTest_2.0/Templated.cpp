#include "NetworkingPCH.h"

#pragma once

#include "Networking.h"

// =============================================================================
// Pushes a game message onto message queue. 
// Requires all types to not have pointers.
// =============================================================================
template < typename T >
void NetworkingEngine::PushMessage( const T &msg, EntityIDTable::EntityID dest )
{
  // Translate ID to CreationKey

  const EntityIDTable::CreationKey *destKey = entityIDTable.GetKey( dest );
  ASSERT( destKey == NULL, "There is no CreationKey associated with this EntityID" );

  NetMessage netMsg;
  netMsg.mType    = MSG_DATA;
  netMsg.addrFrom = saMyAddr.sin_addr.s_addr;
  netMsg.nSize    = sizeof( msg );
  //netMsg.dest     = *destKey;
  if ( msg.toResend ) netMsg.flags |= NetMessage::FLAG_RELAY_BACK;
  if ( msg.reliable ) netMsg.flags |= NetMessage::FLAG_RELIABLE;
  memcpy( netMsg.data, &msg, netMsg.nSize );

  PushMessage( netMsg );
}
