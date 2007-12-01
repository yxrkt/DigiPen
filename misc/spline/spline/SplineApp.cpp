#include "SplineApp.h"
#include "SplineWnd.h"

BOOL SplineApp::InitInstance()
{
  m_pMainWnd = (CWnd *) new SplineWnd;
  return TRUE;
}

SplineApp app;
