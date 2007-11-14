#include <sstream>
#include <process.h>
#include <math.h>

#include "BezierWnd.h"

// ============================================================================
// Constructor for main window
// ============================================================================
CBezierWnd::CBezierWnd() : m_bRunning( true ), 
                           m_Grabbed( m_Points.end() ), 
                           m_bDrawPoly( false ),
                           m_bBernstein( true )
{
  if ( !Create( NULL, "Bezier Curve Canvas" ) )
    MessageBox( "Creating window failed.", NULL, MB_ICONERROR );

  RECT rcWnd;
  GetClientRect( &rcWnd );
  m_DimWnd.width  = rcWnd.right - rcWnd.left;
  m_DimWnd.height = rcWnd.bottom - rcWnd.top - TOOLBAR_HEIGHT;

  BITMAPINFOHEADER bmpinfo = { sizeof( BITMAPINFOHEADER ), 
                               m_DimWnd.width, m_DimWnd.height, 
                               1, 32, BI_RGB, 0L, 0L, 0L, 0, 0 };

  m_BitmapDC    = CreateCompatibleDC( CreateDC( "Display", 0, 0, 0 ) );
	m_Bitmap      = CreateDIBSection( m_BitmapDC, ( BITMAPINFO *) &bmpinfo, 
                                    DIB_RGB_COLORS, ( void **) &m_Surface, 
                                    NULL, NULL );

	SelectObject( m_BitmapDC, m_Bitmap );
  memset( m_Surface, 0xFF, sizeof( int ) * m_DimWnd.width * m_DimWnd.height );


  pThis = this;
  AfxBeginThread( sUpdate, NULL );
}

CBezierWnd::~CBezierWnd()
{
  //delete m_Surface;
}

bool CBezierWnd::bDone = false;
HANDLE CBezierWnd::hMutex;
CBezierWnd *CBezierWnd::pThis;
unsigned CBezierWnd::sUpdate( void *pMyID )
{
  while ( pThis->m_bRunning )
    pThis->Update();

  bDone = true;
  return 0;
}

// ============================================================================
// Updates window's bitmaps
// ============================================================================
void CBezierWnd::Update()
{
  BitBlt( GetDC()->m_hDC, 
          0, 
          TOOLBAR_HEIGHT, 
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
    // Draw control point handles and labels
  for ( PointSetIt i = m_Points.begin(); i != m_Points.end(); ++i )
  {
    DrawHandle( *i );
    DrawLabel( *i );
  }

  if ( m_Points.size() < 3)
    return;

    // Draw curve
  PointList lPoints;
  for ( PointSetIt i = m_Points.begin(); i != m_Points.end(); ++i )
    lPoints.push_back( *i );

  CPoint2D p2DStart =  *m_Points.begin();
  float fStep = 1.f / ( 10.f * ( float ) lPoints.size() );
  for ( float i = 0.f; i <= 1.f; i+= fStep )
  {

    CPoint2D p2D = GetBezierPoint( lPoints, i );
    DrawLine( p2DStart, p2D );
    p2DStart = p2D;
  }
  DrawLine( p2DStart, lPoints.back() );

    // Draw control polygon
  if ( m_bDrawPoly )
  {
    PointSetIt i = m_Points.begin(), iPrev = i++;
    do {
      DrawLine( *iPrev, *i );
      iPrev = i++;
    } while ( i != m_Points.end() );
  }
}

// ============================================================================
// Get point on the curve at time t
// ============================================================================
CPoint2D CBezierWnd::GetBezierPoint( const PointList &points, float t )
{
  return m_bBernstein ? Bernstein( points, t ) : deCasteljau( points, t );
}

// ============================================================================
// Get point using Bernstein polynomials
// ============================================================================
CPoint2D CBezierWnd::Bernstein( const PointList &points, float t )
{
  CPoint2D final( 0.f, 0.f  );
  int nDegree = (int) points.size() - 1;
  PointListItC pointIter = points.begin();

  for ( int i = 0; i <= nDegree; ++i )
  {
    float coef = (float) Choose( nDegree, i ) * pow( ( 1.f - t ), nDegree - i ) * pow( t, i );
    final += ( coef * ( *pointIter ) );
    ++pointIter;
  }

  return final;
}

// ============================================================================
// Get point using de Casteljau's algorithm
// ============================================================================
CPoint2D CBezierWnd::deCasteljau( const PointList &points, float t )
{
  size_t nPoints = points.size();
  if ( nPoints > 1 )
  {
    PointList next_points;
    PointListItC i = points.begin(), iPrev = i++;

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
// Mathematical choose function
// ============================================================================
int CBezierWnd::Choose( int lhs, int rhs ) const
{
  if ( rhs == 0 || lhs == rhs )
    return 1;

  return Factorial( lhs ) / ( Factorial( rhs ) * Factorial( lhs - rhs ) );
}

// ============================================================================
// Mathematical choose function
// ============================================================================
int CBezierWnd::Factorial( int lhs ) const
{
  int final = lhs;
  for ( int i = 2; i < lhs; ++i )
    final *= i;
  return final;
}

// ============================================================================
// Draws circles around a control point
// ============================================================================
void CBezierWnd::DrawHandle( const CPoint2D &point )
{
  int x = point.GetXi();
  int y = point.GetYi();

  SetPixel( x - 1, y - 2 );
  SetPixel( x , y - 2 );
  SetPixel( x + 1, y - 2 );
  SetPixel( x - 1, y + 2 );
  SetPixel( x , y + 2 );
  SetPixel( x + 1, y + 2 );

  SetPixel( x - 2, y - 1 );
  SetPixel( x - 2, y );
  SetPixel( x - 2, y + 1 );
  SetPixel( x + 2, y - 1 );
  SetPixel( x + 2, y );
  SetPixel( x + 2, y + 1 );
}

// ============================================================================
// Draws text labeling a control point
// ============================================================================
void CBezierWnd::DrawLabel( const CPoint2D &point )
{
}

// ============================================================================
// Draws a line between two points
// ============================================================================
void CBezierWnd::DrawLine( const CPoint2D &start, const CPoint2D &finish )
{
  if ( m_Points.size() < 2 )
    return;

  float dx  = finish.x - start.x;
  float dy  = finish.y - start.y;
	float m   = dy / dx;
	float m_i = 1.f / m;
	int xstep = dx > 0 ? 1 : -1;
	int ystep = dy > 0 ? 1 : -1;
	if ( m >= -1 && m <= 1 )
  {
    float y = start.y + .5f;
    int iX  = finish.GetXi();
    for ( int x = start.GetXi(); x != iX; x += xstep )
    {
			y += ( dx > 0 ? m : -m );
			SetPixel( x, (int) y );
		}
	}

	else
  {
    float x = start.x + .5f;
    int iY  = finish.GetYi();
    for ( int y = start.GetYi(); y != iY; y += ystep )
    {
			x += ( dy > 0 ? m_i : -m_i );
			SetPixel( (int) x, y );
		}
	}
}

// ============================================================================
// Change the color of a pixel
// ============================================================================
void CBezierWnd::SetPixel( int x, int y, Color32 c )
{
  if ( x < 0 || x >= m_DimWnd.width || y < 1 || y >= m_DimWnd.height )
    return;
  //m_Surface[ ( y * m_DimWnd.width ) + x ] = c; // right handed
  m_Surface[ ( ( m_DimWnd.height - y ) * m_DimWnd.width ) + x ] = c; // left handed
}

unsigned CBezierPoint::nCurrIndex = 0;

// ============================================================================
// Message map and definitions for main window
// ============================================================================
BEGIN_MESSAGE_MAP( CBezierWnd, CFrameWnd )
  ON_WM_CREATE()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONUP()
  ON_WM_SIZE()
  ON_WM_CLOSE()
  ON_WM_CHAR()

  ON_COMMAND( ID_HIDE_POLYGON, OnHidePolygon )
  ON_COMMAND( ID_SHOW_POLYGON, OnShowPolygon )
  ON_COMMAND( ID_DECASTELJAU, OnDeCasteljau )
  ON_COMMAND( ID_BERNSTEIN, OnBernstein )
END_MESSAGE_MAP()

// Window is created
int CBezierWnd::OnCreate( LPCREATESTRUCT )
{
  if ( !m_Toolbar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE
       | CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) )
  {
    AfxMessageBox( "Creating toolbar failed." );
    return -1;
  }

  if ( !m_Toolbar.LoadToolBar( IDR_TOOLBAR1 ) )
  {
    AfxMessageBox( "Loading toolbar failed." );
    return -1;
  }

  m_Toolbar.EnableDocking( CBRS_ALIGN_ANY );
  EnableDocking( CBRS_ALIGN_ANY );
  DockControlBar( &m_Toolbar );

  return 0;
}

// Moving mouse
void CBezierWnd::OnMouseMove( UINT nFlags, CPoint point )
{
  point.y -= TOOLBAR_HEIGHT;
  if ( m_Grabbed != m_Points.end() )
  {
    m_Grabbed->x = ( float ) point.x;
    m_Grabbed->y = ( float ) point.y;
  }
}

// Left clicking
void CBezierWnd::OnLButtonDown( UINT nFlags, CPoint point )
{
  point.y -= TOOLBAR_HEIGHT;
  m_Points.insert( CBezierPoint( point ) );

  //CEdit label;
  //label.SetParent( this );
  //std::stringstream ss;
  //ss << "p_" << m_Points.size();
  //label.SetWindowTextA( ss.str().c_str() );
  //CEdit l2(label);
}

// Middle clicking
void CBezierWnd::OnMButtonDown( UINT nFlags, CPoint point )
{
  point.y -= TOOLBAR_HEIGHT;
  for ( PointSetIt i = m_Points.begin(); i != m_Points.end(); ++i )
  {
    if ( abs( i->GetXi() - point.x ) < HANDLE_RANGE && abs( i->GetYi() - point.y ) < HANDLE_RANGE )
    {
      m_Grabbed = i;
      break;
    }
  }
}

// Releasing middle click
void CBezierWnd::OnMButtonUp( UINT nFlags, CPoint point )
{
  m_Grabbed = m_Points.end();
}

// Resizing window
void CBezierWnd::OnSize( UINT nType, int cx, int cy )
{
  m_DimWnd.width  = cx;
  m_DimWnd.height = cy - TOOLBAR_HEIGHT;
}

// Pressing a key
void CBezierWnd::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
  switch ( nChar )
  {
    case 99:
      m_Points.clear();
      break;
  }
}

// End update thread
void CBezierWnd::OnClose()
{
  m_bRunning = false;
  while ( !bDone )
    ; // wait for update thread to finish
  this->DestroyWindow();
}

//*****************************************************************************
// WM_COMMAND messages

// hide polygon button is pressed
void CBezierWnd::OnHidePolygon()
{
  m_bDrawPoly = false;
}

// show polygon button is pressed
void CBezierWnd::OnShowPolygon()
{
  m_bDrawPoly = true;
}

// draw curve using de Casteljau's algorithm
void CBezierWnd::OnDeCasteljau()
{
  m_bBernstein = false;
}

// draw curve using bernstein polynomials
void CBezierWnd::OnBernstein()
{
  m_bBernstein = true;
}
