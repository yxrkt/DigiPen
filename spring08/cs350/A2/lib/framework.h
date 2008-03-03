///////////////////////////////////////////////////////////////////////
// $Id: framework.h 1140 2008-01-11 23:49:58Z gherron $
//
// Provides the framework for graphics projects. Opens a window and
// hooks up all GUI callbacks.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

// Included for Windows
#define NOMINMAX // ???
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <crtdbg.h>

#else

// Includes for Linux
#include <fstream>

#endif


#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>

#include <vector>
#include "math.h"

#include "geomlib.h"

struct Unimplemented {};	   // Marks code to be implemented by students.
