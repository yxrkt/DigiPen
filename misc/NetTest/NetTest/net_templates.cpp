#include "precompiled.h"

#ifndef NETTEMPLATES_CPP
#define NETTEMPLATES_CPP

#include "networking.h"

// =============================================================================
// Pushes a game message onto message queue. 
// Requires all types to not have pointers.
// =============================================================================
template < typename T >
void Networking_impl::PushMessage( const T &msg, EntityIDTable::EntityID dest )
{
  // Translate ID to CreationKey

  const EntityIDTable::CreationKey *destKey = entityIDTable.GetKey( dest );
  if ( destKey == NULL )
  {
    ASSERT( "There is no CreationKey associated with this EntityID" );
    return;
  }

  NetMessage netMsg;
  netMsg.mType    = MSG_DATA;
  netMsg.addrFrom = saMyAddr.sin_addr.s_addr;
  netMsg.nSize    = sizeof( msg );
  netMsg.dest     = *destKey;
  memcpy( netMsg.data, &msg, netMsg.nSize );

  PushMessage( netMsg, msg.toResend, msg.reliable );
}

#endif
