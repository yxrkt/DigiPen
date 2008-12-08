#pragma once

#include <assert.h>

#define SAFE_RELEASE( p ) if ( p ) { p->Release(); p = NULL; }
#define SAFE_DELETE( p )  if ( p ) { delete p; p = NULL; }
#define SAFE_DELETE_ARRAY( p ) if ( p ) { delete [] p; p = NULL; }
#define ASSERTHR( hr ) assert( SUCCEEDED( hr ) )

#define GET_RED( c ) ( ( c & 0x00FF0000 ) >> 16 )
#define GET_GREEN( c ) ( ( c & 0x0000FF00 ) >> 8 )
#define GET_BLUE( c ) ( ( c & 0x000000FF ) )
