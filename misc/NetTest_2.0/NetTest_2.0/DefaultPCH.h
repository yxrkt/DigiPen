/**********************************************************************************************//*!
\file	DefaultPCH.h
\par	Project:
		Block Breakers II
\par	Team:
		Digital Dreams
\par	Copyright:
		All content copyright © 2008-2009 DigiPen(USA) Corporation. All rights reserved.
\author	Justin Chambers
\par	Email:
		jchamber\@digipen.edu
\date	Created: 07/01/2008
		Revised: 08/17/2008
\brief  Default header for precompiled headers throughout the projects
*//***********************************************************************************************/
#pragma once

#ifndef	_CRT_SECURE_NO_DEPRECATE        //
#define	_CRT_SECURE_NO_DEPRECATE        // Disable deprecation warnings
#endif                                  //

#include <windows.h>                    //
#include <stdio.h>                      //
#include <stdarg.h>                     //
#include <varargs.h>                    //
#include <io.h>                         //
#include <iostream>                     // Standard and Windows includes
#include <sstream>                      //
#include <string>                       //
#include <time.h>                       //
#include <math.h>                       //
#include <fstream>                      //
#include <fcntl.h>                      //

#include <boost/shared_ptr.hpp>         //
#include <boost/bind.hpp>               // Boost stuff
#pragma warning( disable: 4244 )        // 'argument' : conversion from 'long double' to 'double', possible loss of data
#include <boost/functional/hash.hpp>    //
#pragma warning( default: 4244 )        //

#include <list>                         //
#include <queue>                        //
#include <vector>                       //
#include <map>                          // Container and Container Algorithms
#include <set>                          //
#include <algorithm>                    //

using std::string;                      //
using std::fstream;                     //
using std::list;                        //
using std::vector;                      //
using std::map;                         //
using std::queue;                       // Shortcuts to save some typing
using std::priority_queue;              //
using std::set;                         //
using std::stringbuf;                   //
using std::stringstream;                //
using std::ostream;                     //
using std::istream;                     //
using std::cout;                        //
using std::cin;                         //
using std::endl;                        //

#include "AssertBB.h"                   //

/*************************************************************************************************/
