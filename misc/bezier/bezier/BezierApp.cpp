#include "BezierApp.h"
#include "BezierWnd.h"

// ============================================================================
// Constructor for app
// ============================================================================
BOOL CBezierApp::InitInstance()
{
    // Make sure common controls dll starts
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof( INITCOMMONCONTROLSEX );
  InitCtrls.dwICC  = ICC_WIN95_CLASSES;
  if ( !InitCommonControlsEx( &InitCtrls ) )
    AfxMessageBox( "Initializing common controls failed." );

    // Create main window
  m_pMainWnd = new CBezierWnd;
  m_pMainWnd->ShowWindow( SW_SHOW );
  return TRUE;
}

// ============================================================================
// Instance
// ============================================================================
CBezierApp MyApp;
