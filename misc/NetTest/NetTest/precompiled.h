//-----------------------------------------------------------------------------
#ifndef PRECOMPILED_H
#define PRECOMPILED_H
//-----------------------------------------------------------------------------

#ifndef WINVER                // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501         // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT          // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501   // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <conio.h>
#include <time.h>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include "singleton.h"
#include "message.h"

typedef std::vector<std::string> StringVec;
typedef StringVec::iterator      StringVecIt;
typedef std::queue<std::string>  StringQueue;

#endif
