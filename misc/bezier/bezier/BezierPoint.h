//-----------------------------------------------------------------------------
#ifndef BEZIERPOINT_H
#define BEZIERPOINT_H
//-----------------------------------------------------------------------------

struct CPoint2D
{
  public:
    CPoint2D( float _x = 0.f, float _y = 0.f ) : x( _x ), y ( _y ) {}
    CPoint2D( CPoint point ) : x( ( float ) point.x), y( (float) point.y ) {}

    int GetXi() const
    {
      return int( x + .5f );
    }

    int GetYi() const
    {
      return int( y + .5f );
    }

    CPoint2D operator +( const CPoint2D &rhs ) const
    {
      return CPoint2D( x + rhs.x, y + rhs.y );
    }

    CPoint2D operator -( const CPoint2D &rhs ) const
    {
      return CPoint2D( x - rhs.x, y - rhs.y );
    }

    friend CPoint2D operator *( const float lhs, const CPoint2D &rhs )
    {
      return CPoint2D( lhs * rhs.x, lhs * rhs.y );
    }

    float x;
    float y;
};

class CBezierPoint : public CPoint2D
{
  public:
    CBezierPoint() {}
    CBezierPoint( const CPoint2D &rhs )
    {
      memcpy( this, &rhs, sizeof( CPoint2D ) );
      nIndex = nCurrIndex++;
    }

    bool operator <( const CBezierPoint &rhs ) const
    {
      return ( nIndex < rhs.nIndex ) ? true : false;
    }

    bool operator==( const CBezierPoint &rhs ) const
    {
      return ( nIndex == rhs.nIndex ) ? true : false;
    }

    bool operator>( const CBezierPoint &rhs ) const
    {
      return ( nIndex > rhs.nIndex ) ? true : false;
    }

    unsigned nIndex;
    static unsigned nCurrIndex;
};


#endif
