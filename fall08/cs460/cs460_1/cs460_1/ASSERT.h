#pragma once

#include <windows.h>

#define MESSAGEOK( msg ) MessageBoxA( NULL, msg, "Message", MB_OK );

template<typename T> struct _hackForAssert                    //
{                                                             //
	static bool _true_;                                       //
	static bool _false_;                                      //
};                                                            //
#define _globalForAssert(_var) (_hackForAssert<int>::_var)    //  little hack for header variables
                                                              //
bool _globalForAssert(_true_) = true;                         //
bool _globalForAssert(_false_) = false;                       //
                                                              //
#define _True (_globalForAssert(_true_))                      //
#define _False (_globalForAssert(_false_))                    //

// awesome assert
#if defined(DEBUG) | defined(_DEBUG)
#define ASSERT(_expr,_msg)\
	           do{if(!(_expr)){\
	           MessageBoxA(NULL, (_msg), "ASSERT", MB_OK);\
	           DebugBreak();}}\
	           while(_False)

#else
#define ASSERT(_expr,_msg)\
	           sizeof( (_expr || _msg) )
#endif

