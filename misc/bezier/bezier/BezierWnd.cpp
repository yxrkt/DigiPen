#include <sstream>
#include <process.h>

#include "BezierWnd.h"

// ============================================================================
// Constructor for main window
// ============================================================================
CBezierWnd::CBezierWnd()
{
  if ( !Create( NULL, "Bezier Curve Canvas" ) )
    MessageBox( "Creating window failed.", NULL, MB_ICONERROR );

  RECT rcWnd;
  GetClientRect( &rcWnd );
  m_DimWnd.width  = rcWnd.right - rcWnd.left;
  m_DimWnd.height = rcWnd.bottom - rcWnd.top;

  //m_Surface = new unsigned[ m_DimWnd.right * m_DimWnd.bottom ];
  BITMAPINFOHEADER bmpinfo = { sizeof( BITMAPINFOHEADER ), 
                               m_DimWnd.width, 
                               m_DimWnd.height, 
                               1, 
                               32, 
                               BI_RGB,
                               0L, 
                               0L, 
                               0L, 
                               0, 
                               0
                             };

  m_BitmapDC    = CreateCompatibleDC( CreateDC( "Display", 0, 0, 0 ) );
	m_Bitmap      = CreateDIBSection( m_BitmapDC, 
                                    ( BITMAPINFO *) &bmpinfo, 
                                    DIB_RGB_COLORS, 
                                    ( void **) &m_Surface, 
                                    NULL, 
                                    NULL
                                  );

	SelectObject( m_BitmapDC, m_Bitmap );
  memset( m_Surface, 0xFF, sizeof( int ) * m_DimWnd.width * m_DimWnd.height );

  pThis = this;
  hMutex = CreateMutex( NULL, FALSE, NULL );
  int nThreads = 1;
  _beginthread( sUpdate, 0, &nThreads );
}

HANDLE CBezierWnd::hMutex;
CBezierWnd *CBezierWnd::pThis;
void CBezierWnd::sUpdate( void *pMyID )
{
  while ( pThis )
  {
    WaitForSingleObject( hMutex, INFINITE );
    pThis->Update();
    ReleaseMutex( hMutex );
  }

  CloseHandle( hMutex );
}

// ============================================================================
// Updates window's bitmaps
// ============================================================================
void CBezierWnd::Update()
{
  BitBlt( GetDC()->m_hDC, 
          0, 
          0, 
          m_DimWnd.width, 
          m_DimWnd.height, 
          m_BitmapDC,
          0,
          0,
          SRCCOPY
        );

  memset( m_Surface, 0xFF, sizeof( int ) * m_DimWnd.width * m_DimWnd.height );
  Draw();
}

// ============================================================================
// Draw points and curves
// ============================================================================
void CBezierWnd::Draw()
{
  if ( m_points.size() < 3)
  {
    for ( PointSetIt i = m_points.begin(); i != m_points.end(); ++i )
      SetPixel( i->GetXi(), i->GetYi() );

    return;
  }

  PointList lPoints;
  for ( PointSetIt i = m_points.begin(); i != m_points.end(); ++i )
    lPoints.push_back( *i );

  float fStep = ( 1.f / ( 100.f * ( float ) lPoints.size() ) );
  for ( float i = 0.f; i <= 1.f; i+= fStep )
  {
    CPoint2D p2D = GetBezierPoint( lPoints, i );
    SetPixel( p2D.GetXi(), p2D.GetYi() );
  }
}

// ============================================================================
// Draw points and curves
// ============================================================================
CPoint2D CBezierWnd::GetBezierPoint( const PointList &points, float t )
{
  size_t nPoints = points.size();
  if ( nPoints > 1 )
  {
    PointList next_points;
    PointListIt i = points.begin(), iPrev = i++;

    do {
      next_points.push_back( *iPrev + t * ( *i - *iPrev ) );
      iPrev = i;
    } while ( ++i != points.end() );

    return GetBezierPoint( next_points, t );
  }

  else
    return points.front();
}

// ============================================================================
// Change the color of a pixel
// ============================================================================
void CBezierWnd::SetPixel( int x, int y, Color32 c )
{
  if ( x < 0 || x >= m_DimWnd.width || y < 0 || y >= m_DimWnd.height )
    return;
  //m_Surface[ ( y * m_DimWnd.width ) + x ] = c; // right handed
  m_Surface[ ( ( m_DimWnd.height - y ) * m_DimWnd.width ) + x ] = c; // left handed
}

unsigned CBezierPoint::nCurrIndex = 0;

// ============================================================================
// Message map and definitions for main window
// ============================================================================
BEGIN_MESSAGE_MAP( CBezierWnd, CFrameWnd )
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_SIZE()

  ON_WM_CHAR()
END_MESSAGE_MAP()

// Moving mouse
void CBezierWnd::OnMouseMove( UINT nFlags, CPoint point )
{
}

// Left clicking
void CBezierWnd::OnLButtonDown( UINT nFlags, CPoint point )
{
  m_points.insert( CBezierPoint( point ) );
}

// Moving window
void CBezierWnd::OnSize( UINT nType, int cx, int cy )
{
  m_DimWnd.width  = cx;
  m_DimWnd.height = cy;
}

// Pressing a key
void CBezierWnd::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
  switch ( nChar )
  {
    case 99:
      m_points.clear();
      break;
  }
}

// UpdateWindow
void CBezierWnd::OnPaint()
{
}
