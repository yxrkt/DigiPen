/*!*************************************************************************************************
    @file   EntityTable.h
    @author Alex Serio
    @par    aserio\@digipen.edu
    @par    Block Breakers
    @par    by Digital Dreams and DigiPen Institute of Technology
    @date   September 6, 2008
    
    @brief  This is the implementation for the entity translation table class for networking.

    @par    All content copyright © 2008-2009 DigiPen(USA) Corporation. All rights reserved.
***************************************************************************************************/

#include "NetworkingPCH.h"
#include "EntityTable.h"

EntityIDTable::EntityIDTable() : pMother( NULL ) {}

EntityIDTable::~EntityIDTable() { if ( pMother ) delete pMother; }

// returns true if entity was inserted, false if entity had already been inserted
bool EntityIDTable::Add( const EntityIDTable::EntityID &id, const EntityIDTable::CreationKey &key, bool mother )
{
  if ( mother ) pMother = new IDKeyPair( id, key );
  return keyToID.insert( KeyIDPair( key, id ) ).second &&
         idToKey.insert( IDKeyPair( id, key ) ).second;
}

// returns NULL if no id matched with key
const EntityIDTable::EntityID *EntityIDTable::GetID( const EntityIDTable::CreationKey &key ) const
{
  KeyIDMapItC found = keyToID.find( key );
  if ( found == keyToID.end() )
    return NULL;
  return &found->second;
}

// returns NULL if no key matched with id
const EntityIDTable::CreationKey *EntityIDTable::GetKey( const EntityIDTable::EntityID &id ) const
{
  IDKeyMapItC found = idToKey.find( id );
  if ( found == idToKey.end() )
    return NULL;
  return &found->second;
}

// clear id table
void EntityIDTable::Clear()
{
  keyToID.clear();
  idToKey.clear();

  if ( pMother )
    Add( pMother->first, pMother->second );
}
