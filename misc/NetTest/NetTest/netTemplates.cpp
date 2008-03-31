#include "precompiled.h"

#ifndef NETTEMPLATES_CPP
#define NETTEMPLATES_CPP

// =============================================================================
// Pushes a game message onto message queue. 
// Requires all types to not have pointers.
// =============================================================================
template < typename TList >
void Networking_impl::PushMessage( Message< TList > *pMsg, EntityIDTable::EntityID dest )
{
  // Translate ID to CreationKey

  //const EntityIDTable::CreationKey *destKey = entityIDTable.GetKey( dest );
  //if ( destKey == NULL )
  //{
  //  ASSERT( "There is no CreationKey associated with this EntityID" );
  //  return;
  //}
  //  // create net message
  //NetMessage netMsg;
  //netMsg.mType    = MSG_DATA;
  //netMsg.addrFrom = saMyAddr.sin_addr.s_addr;

  //  // add data for game message
  //netMsg.nSize = sizeof( Message< TList > );
  //netMsg.dest  = *destKey;
  //memcpy( netMsg.Data, pMsg, netMsg.nSize );

  //PushMessage( netMsg, !pMsg->toResend, pMsg->reliable );
}

#endif
