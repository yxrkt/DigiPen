// =============================================================================
#ifndef CS460PROJECT_H
#define CS460PROJECT_H
// =============================================================================

#include <hash_map>

#include "WindowBase.h"
#include "Graphics.h"

#define KEYHIT( key ) ( GetAsyncKeyState( key ) & 1 )
#define KEYDOWN( key ) ( GetAsyncKeyState( key ) & 0x8000 )

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
    void UpdateMouseInputCam( void );
    void UpdateCam( void );
    void GeneratePath( const D3DXVECTOR3 &begin, const D3DXVECTOR3 &end );
    void AddJoints( const LPFRAME pRoot );

    static void AnimCallback( void );

    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

  private:
    void DrawHelpText( void );

    void RotationTest( void );
    void WorldToLocalTest( void );
    void StepByStep( void );
    void ExecuteCCD( void );

    FloatFloatMap   distToTime;
    float           modelPos;
    float           modelSpeed;
    float           maxSpeed;
    float           curveLen;
    float           objHeight;

    struct { int x, y; } clickPos;
    D3DXVECTOR3     startEyePos;
    D3DXVECTOR3     startLookatPos;
    JointVec        joints;

    D3DXVECTOR3     m_prevCamPos;
    D3DXVECTOR3     m_prevCamLookAt;
    D3DXVECTOR3     m_midClickBegin;
    D3DXVECTOR3     m_targetCamPos;
    D3DXVECTOR3     m_targetCamLookat;
    D3DXVECTOR3     m_mousePos;
    float           m_camMoveTPF;
    float           m_zoomSpeed;
    bool            m_isCamMoving;

    byte prevState[256];
    byte curState[256];

    bool KeyDown( int key );
    bool KeyHit( int key );
    bool KeyReleased( int key );
};

#endif
