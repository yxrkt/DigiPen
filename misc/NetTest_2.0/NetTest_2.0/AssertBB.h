/**********************************************************************************************//*!
\file	AssertBB.h
\par	Project:
		Block Breakers II
\par	Team:
		Digital Dreams
\par	Copyright:
		All content copyright © 2008-2009 DigiPen(USA) Corporation. All rights reserved.
\author	Justin Chambers
\par	Email:
		jchamber\@digipen.edu
\date	Created: 08/16/2008
		Revised: 08/16/2008
\brief  Custom assert that breaks to the actual line where the problem exists
*//***********************************************************************************************/
#pragma once

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