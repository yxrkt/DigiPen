//-----------------------------------------------------------------------------
#ifndef BEZIERWND_H
#define BEZIERWND_H
//-----------------------------------------------------------------------------


#include <set>
#include <list>
#include <vector>

#include "stdafx.h"
#include "BezierPoint.h"

#define COLOR(r,g,b) ((BYTE)b)|((WORD)g<<8)|((DWORD)r<<16)
typedef unsigned Color32;

class CBezierWnd : public CFrameWnd
{
    // constants
  static const int HANDLE_RANGE   = 4;
  static const int TOOLBAR_HEIGHT = 24;

  public:
    CBezierWnd();
    ~CBezierWnd();

    void Update();
    static unsigned sUpdate( void *pMyID );
    static CBezierWnd *pThis;
    static HANDLE hMutex;
    static bool bDone;

  protected:
      // types
    typedef std::list< CPoint2D > PointList;
    typedef PointList::iterator PointListIt;
    typedef PointList::const_iterator PointListItC;
    typedef std::set< CBezierPoint > PointSet;
    typedef PointSet::iterator PointSetIt;
    typedef std::vector< CEdit > CEditVec;

      // data
    PointSet m_Points;
    HDC m_BitmapDC;
    HBITMAP m_Bitmap;
    unsigned *m_Surface;
    struct { int width, height; } m_DimWnd;
    bool m_bRunning;
    PointSetIt m_Grabbed;
    bool m_bDrawPoly;
    bool m_bBernstein;
    CEditVec Labels;

      // windows
    CToolBar m_Toolbar;

      // methods
    void Draw();
    CPoint2D GetBezierPoint( const PointList &points, float t );
    void SetPixel( int x, int y, Color32 c = COLOR( 0, 0, 0 ) );
    void DrawHandle( const CPoint2D &point );
    void DrawLine( const CPoint2D &start, const CPoint2D &finish );
    CPoint2D Bernstein( const PointList &points, float t );
    CPoint2D deCasteljau( const PointList &points, float t );
    int Choose( int lhs, int rhs ) const;
    int Factorial( int lhs ) const;
    void DrawLabel( const CPoint2D &point );

      // messages
    DECLARE_MESSAGE_MAP();

    int OnCreate( LPCREATESTRUCT );
    void OnMouseMove( UINT nFlags, CPoint point );
    void OnLButtonDown( UINT nFlags, CPoint point );
    void OnMButtonDown( UINT nFlags, CPoint point );
    void OnMButtonUp( UINT nFlags, CPoint point );
    void OnSize( UINT nType, int cx, int cy );
    void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
    void OnClose();

      // On Command
    void OnHidePolygon();
    void OnShowPolygon();
    void OnDeCasteljau();
    void OnBernstein();
};

#endif
