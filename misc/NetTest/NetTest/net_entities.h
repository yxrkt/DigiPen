//------------------------------------------------------------------------------
#ifndef NETENTITIES_H
#define NETENTITIES_H
//------------------------------------------------------------------------------

#include <hash_map>

// entity lookup table
class EntityIDTable
{
  public:
      // machine independent entity identification
    struct CreationKey
    {
      CreationKey( DWORD _time = 0, unsigned _addr = 0, unsigned _fseq = 1 )
        : time( _time ), addr( _addr ), fseq( _fseq ) {}
      inline bool operator <( const CreationKey &rhs ) const
      {
        return ( addr <  rhs.addr ) || ( addr == rhs.addr && time < rhs.time ) ||
               ( addr == rhs.addr && time == rhs.time && fseq < rhs.fseq );
      }

      DWORD     time; // creation time
      unsigned  addr; // creator's address
      unsigned  fseq; // n'th in creation sequence of frame
    };

      // other public types
    typedef int EntityID;

  private:
      // hasher for creation key
    struct ckHasher : public stdext::hash_compare<CreationKey>
    {
      size_t operator()( const CreationKey &key ) const
      {
        return (size_t)( key.time * key.addr * ( key.fseq + 1 ) );
      }
      bool operator()( const CreationKey &lhs, const CreationKey &rhs ) const
      {
        return lhs < rhs;
      }
    };

      // private types
    typedef stdext::hash_map<CreationKey, EntityID, ckHasher> KeyIDMap;
    typedef KeyIDMap::iterator KeyIDMapIt;
    typedef KeyIDMap::const_iterator KeyIDMapItC;
    typedef std::pair<CreationKey, EntityID> KeyIDPair;
    typedef stdext::hash_map<EntityID, CreationKey> IDKeyMap;
    typedef IDKeyMap::iterator IDKeyMapIt;
    typedef IDKeyMap::const_iterator IDKeyMapItC;
    typedef std::pair<EntityID, CreationKey> IDKeyPair;

  public:
    EntityIDTable();
    ~EntityIDTable();
    bool Add( const EntityID &id, const CreationKey &key, bool mother = false );
    const EntityID *GetID( const CreationKey &key ) const;
    const CreationKey *GetKey( const EntityID &id ) const;
    void Clear();

  private:
    KeyIDMap   keyToID; // map creation keys to entity ids
    IDKeyMap   idToKey; // map entity ids to creation keys
    IDKeyPair *pMother; // entity that remains after clear is called
};



#endif
