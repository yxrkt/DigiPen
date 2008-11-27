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
  public:
    CS460Project( HINSTANCE hInstance );
    ~CS460Project();

    int Run();

  private:
    void Initialize();
    void Update();
    void Cleanup();

    void UpdateSpline();
    void UpdateModel();

    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

  private:
    Graphics        graphics;
    FloatFloatMap   distToTime;
    float           modelPos;
    float           modelSpeed;
    float           maxSpeed;
    float           curveLen;

    struct { int x, y; } clickPos;
    D3DXVECTOR3     startEyePos;
    D3DXVECTOR3     startLookatPos;
    LPFRAME         pFrameShoulder;
};

#endif
