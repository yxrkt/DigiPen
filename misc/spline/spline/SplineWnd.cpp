#include "SplineWnd.h"


BEGIN_MESSAGE_MAP( SplineWnd, CFrameWnd )
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_CLOSE()
END_MESSAGE_MAP()

SplineWnd *SplineWnd::pThis;

SplineWnd::SplineWnd() : done( false )
{
  Create( NULL, "Spline Tester 1.0" );
  ShowWindow( SW_SHOW );

  pThis = this;
  AfxBeginThread( &SplineWnd::sUpdate, NULL );
}

UINT SplineWnd::sUpdate( LPVOID )
{
  while ( !pThis->done )
    pThis->Update();

  AfxEndThread( 0 );
  return NULL;
}

void SplineWnd::Update()
{
  DrawPts();
}

void SplineWnd::DrawPts()
{
  //SetPixel( x - 1, y - 2 );
  //SetPixel( x , y - 2 );
  //SetPixel( x + 1, y - 2 );
  //SetPixel( x - 1, y + 2 );
  //SetPixel( x , y + 2 );
  //SetPixel( x + 1, y + 2 );

  //SetPixel( x - 2, y - 1 );
  //SetPixel( x - 2, y );
  //SetPixel( x - 2, y + 1 );
  //SetPixel( x + 2, y - 1 );
  //SetPixel( x + 2, y );
  //SetPixel( x + 2, y + 1 );
}

void SplineWnd::OnLButtonDown( UINT nFlags, CPoint point )
{
  static UINT count = 0;
  points[count++] = Point2D( (float) point.x, (float) point.y );
}

void SplineWnd::OnLButtonUp( UINT nFlags, CPoint point )
{
  AfxMessageBox( "This will prompt for vel and accel" );
}

void SplineWnd::OnClose()
{
  done = true;
  DestroyWindow();
}