// =============================================================================
#ifndef CS460PROJECT_H
#define CS460PROJECT_H
// =============================================================================

#include <hash_map>

#include "WindowBase.h"
#include "Graphics.h"

//typedef stdext::hash_map< float, float > FloatFloatHashMap;
typedef std::map< float, float > FloatFloatMap;

// =============================================================================
// ! CS460 Framework engine
// =============================================================================
class CS460Project : WindowBase
{
  static CS460Project *CS460Proj;

  public:
    CS460Project( HINSTANCE hInstance );
    ~CS460Project( void );

    int Run( void );

  private:
    void Initialize( void );
    void Update( void );
    void Cleanup( void );

    void UpdateSpline( void );
    void UpdateModel( void );
    void GeneratePath( const D3DXVECTOR3 &begin, const D3DXVECTOR3 &end );
    void AddNamedFrames( const LPFRAME pRoot );

    static void AnimCallback( void );

    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

  private:
    FloatFloatMap   distToTime;
    float           modelPos;
    float           modelSpeed;
    float           maxSpeed;
    float           curveLen;

    struct { int x, y; } clickPos;
    D3DXVECTOR3     startEyePos;
    D3DXVECTOR3     startLookatPos;
    PFrameVec       armFrames;
};

#endif
