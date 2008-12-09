#include <sstream>

#include "CS460Proj.h"
#include "Graphics.h"
#include "Input.h"


CS460Proj *CS460Proj::Instance( void )
{
  static CS460Proj cs460Proj;
  return &cs460Proj;
}

CS460Proj::CS460Proj( void )
{
}

CS460Proj::~CS460Proj( void )
{
  SAFE_DELETE( m_pCloth );
}

int CS460Proj::Run( void )
{
  MSG msg;
  ZeroMemory( &msg, sizeof( MSG ) );
  while ( msg.message != WM_QUIT )
  {
    Update();

    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }

  return (int)msg.wParam;
}

HWND CS460Proj::GetWindow( void )
{
  return m_hWndMain;
}

void CS460Proj::Initialize( HINSTANCE hInstance )
{
  m_hInstance = hInstance;
  CreateWindow( "CS460Proj" );

  PHYSICS->Initialize();
  GRAPHICS->Initialize();
  INPUT->Initialize();

  m_targetCamLookat = D3DXVECTOR3( 0.f, 0.f, 0.f );
  m_targetCamPos    = D3DXVECTOR3( 0.f, 400.f, -500.f );
  m_isCamMoving     = false;
  m_camMoveTPF      = 10.f;
  m_zoomSpeed       = 4.f;
  m_scrollZoomSpeed = .25f;

  m_pCloth      = new Cloth( 220.f, 220.f, 32, 32 );
  m_pPhysCloth  = new PhysCloth( m_pCloth );
  m_pSphere     = new Sphere( D3DXVECTOR3( 0.f, -250.f, 0.f ), 100.f, 16, 32 );
  m_pPhysCloth->GetVertexInfo( m_pPhysCloth->resX - 1, 0 ).locked = true;
  m_pPhysCloth->GetVertexInfo( m_pPhysCloth->resX - 1, m_pPhysCloth->resY - 1 ).locked = true;
  m_pPhysCloth->GetVertexInfo( 0, 0 ).locked = true;
  m_pPhysCloth->GetVertexInfo( 0, m_pPhysCloth->resY - 1 ).locked = true;
}

void CS460Proj::Update( void )
{
  // FPS
  static DWORD lastTick = timeGetTime();
  DWORD tick = timeGetTime();
  static std::stringstream ssTime;
  ssTime << "FPS: " << ( 1000.f / (float)( tick - lastTick ) ) << std::endl;
  ssTime << "Mouse Pos: { " << INPUT->GetMousePos().x << ", " << INPUT->GetMousePos().y << " }" << std::endl;
  GRAPHICS->WriteText( ssTime.str().c_str(), 10, 10 );
  ssTime.str( "" );
  lastTick = tick;

  // Message log
  RECT rcWnd;
  GetClientRect( m_hWndMain, &rcWnd );
  GRAPHICS->WriteText( m_messageLog.str(), rcWnd.right - 250, 10 );

  UpdateMouseInputCam();

  INPUT->Update();
  UpdateCam();
  PHYSICS->Update();
  GRAPHICS->Render();
}

void CS460Proj::UpdateMouseInputCam( void )
{
  Camera &camera = GRAPHICS->GetMainCam();
  const D3DXVECTOR3 &screenPos = INPUT->GetMousePos();
  D3DXVECTOR3 isect;
  GRAPHICS->IsectPlane( &isect, screenPos.x, screenPos.y );

  bool altDown  = ( INPUT->IsKeyHeld( VK_MENU ) );
  bool altHit   = ( INPUT->IsKeyHit( VK_MENU ) );
  bool altRel   = ( INPUT->IsKeyReleased( VK_MENU ) );
  bool ctrlDown = ( INPUT->IsKeyHeld( VK_CONTROL ) );
  bool ctrlHit  = ( INPUT->IsKeyHit( VK_CONTROL ) );
  bool ctrlRel  = ( INPUT->IsKeyReleased( VK_CONTROL ) );

  // panning
  if ( INPUT->IsMouseHit( MIDDLE_BUTTON ) )
  {
    m_midClickBegin   = screenPos;
    m_prevCamPos      = camera.Eye;
    m_prevCamLookAt   = camera.LookAt;
  }
  else if ( INPUT->IsMouseHeld( MIDDLE_BUTTON ) )
  {
    if ( altRel || altHit || ctrlHit || ctrlRel )
    {
      m_midClickBegin   = screenPos;
      m_prevCamPos      = camera.Eye;
      m_prevCamLookAt   = camera.LookAt;
    }
    else if ( !altDown && !ctrlDown ) // pan
    {
      D3DXVECTOR3 prevPt;
      GRAPHICS->IsectPlane( &prevPt, m_midClickBegin.x, m_midClickBegin.y );

      D3DXVECTOR3 trans = prevPt - isect;
      m_targetCamPos    = m_prevCamPos + trans;
      m_targetCamLookat = m_prevCamLookAt + trans;
      m_camMoveTPF = 3.5f;
    }
    else if ( altDown && !ctrlDown )  // rotate
    {
      float coef  = .008f;
      float yaw   = -coef * ( m_midClickBegin.x - screenPos.x );
      float pitch = -coef * ( m_midClickBegin.y - screenPos.y );

      D3DXVECTOR3 look( m_prevCamPos - m_prevCamLookAt );
      D3DXVECTOR3 lookXZ( look.x, 0.f, look.z );
      float dist     = D3DXVec3Length( &look );
      float projDist = D3DXVec3Length( &lookXZ );
      float denom    = dist * projDist;
      D3DXVECTOR3 newCamPos( 0.f, 0.f, -dist );
      float curPitch = ( look.y > 0.f ? 1.f : -1.f ) * acos( D3DXVec3Dot( &look, &lookXZ ) / denom );
      float curYaw   = ( lookXZ.x > 0.f ? -1.f : 1.f ) * acos( D3DXVec3Dot( &lookXZ, &newCamPos ) / denom );

      D3DXMATRIX rotPitch, rotYaw, rotFinal;
      D3DXMatrixRotationX( &rotPitch, curPitch + pitch );
      D3DXMatrixRotationY( &rotYaw, curYaw + yaw );
      D3DXMatrixMultiply( &rotFinal, &rotPitch, &rotYaw );

      D3DXVec3TransformCoord( &newCamPos, &newCamPos, &rotFinal );
      newCamPos += m_prevCamLookAt;

      m_targetCamPos = newCamPos;
      m_camMoveTPF = 7.5f;
    }
    else if ( altDown && ctrlDown )   // zoom
    {
      float scroll = m_zoomSpeed * ( m_midClickBegin.y - screenPos.y );
      D3DXVECTOR3 dir( m_prevCamLookAt - m_prevCamPos );
      D3DXVec3Normalize( &dir, &dir );
      m_targetCamPos = m_prevCamPos + scroll * dir;
      m_camMoveTPF = 10.f;
    }
  }
  else
  {
    m_isCamMoving = false;
  }

  // mouse scroll
  if ( !m_isCamMoving )
  {
    //static float totalScroll = 0.f;
    //float scroll = m_scrollZoomSpeed * (float)INPUT->GetMouseScrollDelta();
    //totalScroll += scroll;
    //if ( totalScroll )
    //{
    //  D3DXVECTOR3 dir( camera.LookAt - camera.Eye );
    //  D3DXVec3Normalize( &dir, &dir );
    //  m_prevCamPos = camera.Eye;
    //  m_targetCamPos = camera.Eye + ( totalScroll * dir );
    //  m_camMoveTPF = 5.f;
    //  m_isCamMoving = false;
    //}
    //if ( !scroll )
    //{
    //  totalScroll = 0.f;
    //}
  }
}

void CS460Proj::UpdateCam( void )
{
  D3DXVECTOR3 &curPos    = GRAPHICS->GetMainCam().Eye;
  D3DXVECTOR3 &curLookat = GRAPHICS->GetMainCam().LookAt;

  if ( curPos == m_targetCamPos )
    return;

  D3DXVECTOR3 dir( m_targetCamPos - curPos );
  float dist = D3DXVec3Length( &dir );

  D3DXVECTOR3 dirLook( m_targetCamLookat - curLookat );
  float distLook = D3DXVec3Length( &dirLook );

  if ( dist < .50f )
  {
    curPos    = m_targetCamPos;
    curLookat = m_targetCamLookat;
    return;
  }

  float slowZone = 1.f;
  float speed     = ( ( dist     > slowZone ) ? ( dist     / m_camMoveTPF ) : ( slowZone / m_camMoveTPF ) );
  float speedLook = ( ( distLook > slowZone ) ? ( distLook / m_camMoveTPF ) : ( slowZone / m_camMoveTPF ) );

  D3DXVec3Normalize( &dir, &dir );
  D3DXVec3Normalize( &dirLook, &dirLook );
  dir       *= speed;
  dirLook   *= speedLook;
  curPos    += dir;
  curLookat += dirLook;
}

LRESULT CS460Proj::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
  if ( msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST )
    INPUT->HandleMessage( msg, wParam, lParam );

  switch ( msg )
  {
    case WM_DESTROY:
      PostQuitMessage( 0 );
      break;

    case WM_SYSKEYDOWN:
      break;

    default:
      return DefWindowProc( m_hWndMain, msg, wParam, lParam );
  }

  return 0L;
}
