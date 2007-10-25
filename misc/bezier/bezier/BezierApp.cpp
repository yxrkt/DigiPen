#include "BezierApp.h"
#include "BezierWnd.h"

// ============================================================================
// Constructor for app
// ============================================================================
BOOL CBezierApp::InitInstance()
{
  m_pMainWnd = new CBezierWnd;
  m_pMainWnd->ShowWindow( SW_SHOW );
  return TRUE;
}

// ============================================================================
// Instance
// ============================================================================
CBezierApp MyApp;
