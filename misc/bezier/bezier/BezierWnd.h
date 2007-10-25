//-----------------------------------------------------------------------------
#ifndef BEZIERWND_H
#define BEZIERWND_H
//-----------------------------------------------------------------------------

#include <afxwin.h>
#include <set>
#include <list>

#include "BezierPoint.h"

#define COLOR(r,g,b) ((BYTE)b)|((WORD)g<<8)|((DWORD)r<<16)
typedef unsigned Color32;

class CBezierWnd : public CFrameWnd
{
  public:
    CBezierWnd();

    void Update();
    static void sUpdate( CBezierWnd *pThis );


  protected:
      // data
    typedef std::list< CPoint2D > PointList;
    typedef PointList::const_iterator PointListIt;

    typedef std::set< CBezierPoint > PointSet;
    typedef PointSet::iterator PointSetIt;
    PointSet m_points;

    HDC m_BitmapDC;
    HBITMAP m_Bitmap;
    unsigned *m_Surface;
    struct { int width; int height; } m_DimWnd;

      // methods
    void Draw();
    CPoint2D GetBezierPoint( const PointList &points, float t );
    void SetPixel( int x, int y, Color32 c = COLOR( 0, 0, 0 ) );

      // messages
    DECLARE_MESSAGE_MAP();

    void OnMouseMove( UINT nFlags, CPoint point );
    void OnLButtonDown( UINT nFlags, CPoint point );
    void OnSize( UINT nType, int cx, int cy );
    void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
    void OnPaint();
};

#endif
