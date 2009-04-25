/////////////////////////////////////////////////
//
// File: "Macros.h"
//
// Author: Jason Jurecka
//
// Creation Date: June 9, 2003
//
// Purpose: made this file to store all the macros
/////////////////////////////////////////////////

#ifndef _MACROS_H_
#define _MACROS_H_

#include <algorithm>

//Delete an Array safely
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }

//Delete an object pointer
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }

//Release an object pointer
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

template < typename T>
static inline T clamp( const T &n, const T &lo, const T &hi )
{
  return std::min( std::max( lo, n ), hi );
}

#endif