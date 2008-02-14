//------------------------------------------------------------------------------
#ifndef SPLINEWND_H
#define SPLINEWND_H
//------------------------------------------------------------------------------


#include <map>

#include "stdafx.h"

// Point struct
struct Point2D
{
  Point2D( float x = 0.f, float y = 0.f ) : x( x ), y( y ) {}

  float x;
  float y;
};

// vector struct
struct Vector2D
{
  Point2D p;
  Point2D q;
};

// =============================================================================
// Window class
// =============================================================================
class SplineWnd : public CFrameWnd
{
  static SplineWnd *pThis;

  public:
    SplineWnd();
    static UINT sUpdate( LPVOID );

  private:
      // types
    typedef std::map< UINT, Point2D > PointMap;
    typedef PointMap::iterator PointMapIt;

    DECLARE_MESSAGE_MAP();

    void OnLButtonDown( UINT nFlags, CPoint point );
    void OnLButtonUp( UINT nFlags, CPoint point );
    void OnClose();

      // misc members
    void Update();
    void DrawPts();

      // data
    bool      done;
    PointMap  points;
};

#endif
