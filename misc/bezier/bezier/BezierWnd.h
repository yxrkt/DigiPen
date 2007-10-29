//-----------------------------------------------------------------------------
#ifndef BEZIERWND_H
#define BEZIERWND_H
//-----------------------------------------------------------------------------


#include <set>
#include <list>

#include "stdafx.h"
#include "BezierPoint.h"

#define COLOR(r,g,b) ((BYTE)b)|((WORD)g<<8)|((DWORD)r<<16)
typedef unsigned Color32;

class CBezierWnd : public CFrameWnd
{
  static const int HANDLE_RANGE = 4;

  public:
    CBezierWnd();
    ~CBezierWnd();

    void Update();
    static unsigned sUpdate( void *pMyID );
    static CBezierWnd *pThis;
    static HANDLE hMutex;
    static bool bDone;

  protected:
      // data
    typedef std::list< CPoint2D > PointList;
    typedef PointList::const_iterator PointListItC;

    typedef std::set< CBezierPoint > PointSet;
    typedef PointSet::iterator PointSetIt;
    PointSet m_points;

    HDC m_BitmapDC;
    HBITMAP m_Bitmap;
    unsigned *m_Surface;
    struct { int width; int height; } m_DimWnd;
    bool bRunning;
    PointSetIt grabbed;

      // windows
    CToolBar m_Toolbar;

      // methods
    void Draw();
    CPoint2D GetBezierPoint( const PointList &points, float t );
    void SetPixel( int x, int y, Color32 c = COLOR( 0, 0, 0 ) );
    void DrawHandle( const CPoint2D &point );

      // messages
    DECLARE_MESSAGE_MAP();

    void OnMouseMove( UINT nFlags, CPoint point );
    void OnLButtonDown( UINT nFlags, CPoint point );
    void OnMButtonDown( UINT nFlags, CPoint point );
    void OnMButtonUp( UINT nFlags, CPoint point );
    void OnSize( UINT nType, int cx, int cy );
    void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
    void OnPaint();
    void OnClose();
};

#endif
