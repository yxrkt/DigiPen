#pragma once

#include <sstream>
#include <queue>

#include "FrameWindow.h"
#include "Cloth.h"
#include "Sphere.h"
#include "Physics.h"

#define CS460PROJ CS460Proj::Instance()

#define FRAME_HISTORY 60

class CS460Proj : public FrameWindow
{
  public:
    static CS460Proj *Instance( void );

    void Initialize( HINSTANCE hInstance );
    int Run( void );
    HWND GetWindow( void );

  private:
    CS460Proj( void );
    CS460Proj( const CS460Proj &rhs );
    void operator =( const CS460Proj &rhs );
    ~CS460Proj( void );

    void Update( void );
    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

    void UpdateMouseInputCam( void );
    void UpdateCam( void );
    int  GetFPS( void );
    void Reset( void );

  private:
    Cloth              *m_pCloth;
    PhysCloth          *m_pPhysCloth;
    Sphere             *m_pSphere;
    std::stringstream   m_messageLog;

    // camera stuff
    bool                m_isCamMoving;
    D3DXVECTOR3         m_midClickBegin;
    D3DXVECTOR3         m_prevCamPos;
    D3DXVECTOR3         m_prevCamLookAt;
    D3DXVECTOR3         m_targetCamPos;
    D3DXVECTOR3         m_targetCamLookat;
    float               m_camMoveTPF;
    float               m_zoomSpeed;
    float               m_scrollZoomSpeed;

};
