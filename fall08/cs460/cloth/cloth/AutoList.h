#pragma once

#include <list>

#define FOR_EACH_AUTO( _type, _pValue ) \
  AutoList< _type >::iter_type __iter__ = AutoList< _type >::List.begin();\
  _type *_pValue = !AutoList< _type >::List.empty() ? ( *__iter__ ) : NULL;\
  for ( ; __iter__ != AutoList< _type >::List.end(); ++__iter__\
        , _pValue = __iter__ != AutoList< _type >::List.end() ? ( *__iter__ ) : NULL )


template <typename T>
class AutoList
{
  public:
    typedef std::list< T *>                    list_type;
    typedef typename list_type::iterator       iter_type;
    typedef typename list_type::const_iterator citer_type;

    static list_type List;

  protected:
    AutoList( void )
    {
      id = nextID++;
      List.push_back( static_cast< T *>( this ) );
    }
    ~AutoList( void )
    {
      List.remove( static_cast< T *>( this ) );
    }

  private:
    bool operator ==( const AutoList< T > &rhs )
    {
      return ( id == rhs.id );
    }

    static int nextID;
    int id;
};

template <typename T>
typename AutoList< T >::list_type AutoList< T >::List;

template <typename T>
int AutoList< T >::nextID = 0;
