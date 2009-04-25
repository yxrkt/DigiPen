#include "CS460Project.h"
#include "Spline.h"
#include "GlobalTime.h"


#define CURVE_PRECISION 20
#define ANIM_RATIO      .01f
#define EASE_IN_OUT_PCT .166f

CS460Project *CS460Project::CS460Proj = NULL;

AnimatedModel::RENDER_FLAG renderState = AnimatedModel::RENDER_BONES;

// =============================================================================
// ! Constructor.
// =============================================================================
CS460Project::CS460Project( HINSTANCE hInstance )
: modelPos( 0.f )
, modelSpeed( 0.f )
, maxSpeed( 100.f )
{
  SAFE_DELETE( CS460Proj );
  hInstance_ = hInstance;
  CreateMainWindow( "CS460 Project 1", 800, 600 );
  CS460Proj = this;
}

// =============================================================================
// ! Destructor.
// =============================================================================
CS460Project::~CS460Project( void )
{
}

// =============================================================================
// ! Initialize.
// =============================================================================
void CS460Project::Initialize( void )
{
  //HRESULT hr;

  Gfx->Initialize( hWndMain_ );
  Gfx->animCallback = AnimCallback;

  Gfx->LoadAnimatedModel( "run_inPlace.x" );
  Gfx->Update();

  AddJoints( (LPFRAME)D3DXFrameFind( Gfx->AnimatedModels.front().GetFrameRoot(), "shoulder_l" ) );
  int nLast = (int)joints.size() - 1;
  for ( int i = 0; i <= nLast; ++i )
  {
    if ( i != 0 )
      joints[i].SetParent( &joints[i - 1] );
    if ( i != nLast )
      joints[i].SetChild( &joints[i + 1] );
  }

  objHeight = 90.f;

  Gfx->LoadStaticModel( "hammer.X", D3DXVECTOR3( 0.f, objHeight, -30.f ), .15f );

  AnimatedModel &model = Gfx->AnimatedModels.front();
  D3DXMatrixScaling( &model.MatScale, .15f, .15f, .15f );

  m_targetCamLookat = D3DXVECTOR3( 0.f, 0.f, 0.f );
  m_targetCamPos    = D3DXVECTOR3( 0.f, 400.f, -700.f );
  m_isCamMoving     = false;
  m_camMoveTPF      = 10.f;
  m_zoomSpeed       = 4.f;
}

// =============================================================================
// ! Updates modules.
// =============================================================================
void CS460Project::Update( void )
{
  GlobalTime::Update();

  memcpy( prevState, curState, sizeof( curState ) );
  GetKeyboardState( curState );

  UpdateMouseInputCam();
  UpdateCam();

  // Draw Floor
  ColoredVertex floorQuad[] = 
  {
    ColoredVertex( -250.f, 0.f, -250.f, D3DCOLOR_COLORVALUE( 0.f, .5f, 1.f, 1.f ) ),
    ColoredVertex(  250.f, 0.f, -250.f, D3DCOLOR_COLORVALUE( 1.f, .5f, 0.f, 1.f ) ),
    ColoredVertex(  250.f, 0.f,  250.f, D3DCOLOR_COLORVALUE( .5f, 0.f, 1.f, 1.f ) ),
    ColoredVertex( -250.f, 0.f,  250.f, D3DCOLOR_COLORVALUE( .5f, 1.f, 0.f, 1.f ) )
  };
  Gfx->AddQuad( floorQuad );
  //Gfx->AddPolyline( floorQuad, 4 );

  // Update Spline
  if ( Gfx->controlPoints.size() > 2 )
  {
    UpdateSpline();
    UpdateModel();
  }

  DrawHelpText();

  Gfx->Update();
}

void CS460Project::DrawHelpText( void )
{
  Gfx->WriteText( "Click M.1 to move target" );
  Gfx->WriteText( "Hold  M.3 to pan camera" );
  Gfx->WriteText( "Hold  Alt+M.3 to rotate camera" );
  Gfx->WriteText( "Hold  Ctrl+Alt+M.3 to zoom camera" );
  Gfx->WriteText( "Press 'P' to toggle pause" );
  Gfx->WriteText( "Press 'R' to toggle between bones & meshes" );
  Gfx->WriteText( "Press '+' to increase running speed" );
  Gfx->WriteText( "Press '-' to decrease running speed" );
  Gfx->WriteText( "Press 'Esc' to exit" );

  Gfx->WriteText( "\n  Mouse (M)  " );
  Gfx->WriteText( " ___________ " );
  Gfx->WriteText( "/   |   |   \\" );
  Gfx->WriteText( "| 1 | 3 | 2 |" );
  Gfx->WriteText( "|___|___|___|" );
  Gfx->WriteText( "|   |   |   |" );
  Gfx->WriteText( "|   |   |   |" );
  Gfx->WriteText( "|   |   |   |" );
  Gfx->WriteText( "|___|___|___|" );
}

// =============================================================================
// ! Run application.
// =============================================================================
int CS460Project::Run( void )
{
  MSG msg;
  ZeroMemory( &msg, sizeof( MSG ) );

  Initialize();

  while ( msg.message != WM_QUIT )
  {
    Update();

    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }

  Cleanup();

  return (int)msg.wParam;
}

// =============================================================================
// ! Cleanup.
// =============================================================================
void CS460Project::Cleanup( void )
{
  UnregisterClass( szClassName, hInstance_ );
  Gfx->Cleanup();
}

// =============================================================================
// ! Window procedure.
// =============================================================================
LRESULT CS460Project::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch ( msg )
  {
    case WM_KEYDOWN:
    {
      switch( wParam )
      {
        case VK_ADD: case VK_OEM_PLUS:
          maxSpeed += 25.f;
          break;

        case VK_SUBTRACT: case VK_OEM_MINUS:
          maxSpeed = std::max( maxSpeed - 25.f, 0.f );
          break;

        case 'C':
          //Gfx->controlPoints.clear();
          break;

        case 'P':
          if ( !Gfx->IsPaused() )
          { 
            GlobalTime::Pause();
            Gfx->PauseAnims();
          }
          else
          {
            GlobalTime::Unpause();
            Gfx->PlayAnims();
          }
          break;

        case 'R':
          renderState = (AnimatedModel::RENDER_FLAG)( ( renderState + 1 ) % AnimatedModel::RENDER_ALL );
          break;

        case VK_ESCAPE:
          PostQuitMessage( 0 );
          break;
      }
      break;
    }

    case WM_LBUTTONDOWN:
    {
      D3DXVECTOR3 isectD3D = Gfx->IsectGroundPlane( (int)(short)LOWORD( lParam ), 
                                                    (int)(short)HIWORD( lParam ) );
      Gfx->StaticModels.front().Pos   = isectD3D;
      Gfx->StaticModels.front().Pos.y = objHeight;
      GeneratePath( D3DXVECTOR3( 0.f, 0.f, 0.f ), isectD3D );
      Gfx->PlayAnims();
      GlobalTime::Unpause();
      modelPos = 0.f;
      break;
    }

    case WM_MOUSEMOVE:
    {
      m_mousePos.x = (float)(int)(short)LOWORD( lParam );
      m_mousePos.y = (float)(int)(short)HIWORD( lParam );
      break;
    }

    case WM_MBUTTONDOWN:
    {
      SetCapture( hWndMain_ );
      break;
    }

    case WM_MBUTTONUP:
    {
      ReleaseCapture();
      break;
    }

    case WM_SYSKEYDOWN:
      return 0;

    case WM_DESTROY:
      PostQuitMessage( 0 );
      break;

    case WM_SIZE:
      if ( Gfx->Ready )
        Gfx->Update();
      break;

    default:
      return DefWindowProc( hWndMain_, msg, wParam, lParam );
  }

  return 0L;
}

// --

void CS460Project::GeneratePath( const D3DXVECTOR3 &begin, const D3DXVECTOR3 &end )
{
  AnimatedModel &dude = Gfx->AnimatedModels.front();

  VertVec &points = Gfx->controlPoints;
  points.clear();

  points.push_back( begin );

  // get look vector
  D3DXVECTOR3 vecOrient = D3DXVECTOR3( 0.f, 0.f, -1.f );

  D3DXVECTOR3 dir( end - begin );
  float dist = D3DXVec3Length( &dir );
  D3DXVec3Normalize( &dir, &dir );

  points.push_back( begin + ( vecOrient * dist ) );

  D3DXMATRIX matRot;
  D3DXVECTOR3 thirdPtOffset, crossOut;
  D3DXVec3Cross( &crossOut, points[1].ToLPD3DXVECTOR3(), &dir );
  float angle = .5f * acos( D3DXVec3Dot( &vecOrient, &dir ) );
  if ( crossOut.y < 0.f )
    angle = -angle;
  D3DXMatrixRotationY( &matRot, angle );
  D3DXVec3TransformCoord( &thirdPtOffset, points.back().ToLPD3DXVECTOR3(), &matRot );

  points.push_back( thirdPtOffset );

  points.push_back( end );
}

void CS460Project::UpdateSpline( void )
{
  distToTime.clear();

  curveLen = 0.f;
  VertVec &points = Gfx->controlPoints;
  ColoredVertex vertBegin = points.front();
  D3DXVECTOR3 vertEnd;
  distToTime[0.f] = 0.f;
  float fStep = 1.f / (float)( CURVE_PRECISION * points.size() );
  for ( float t = fStep; t < 1.f; t += fStep )
  {
    GetSplinePoint( vertEnd, points, t );
    Gfx->AddLine( vertBegin, vertEnd );
    curveLen += Dist( vertBegin, vertEnd );
    distToTime[curveLen] = t;
    vertBegin = vertEnd;
  }
  Gfx->AddLine( vertBegin, points.back() );
  curveLen += Dist( vertBegin, vertEnd );
  distToTime[curveLen] = 1.f;
}

void CS460Project::UpdateModel( void )
{
  //if ( Gfx->IsPaused() ) return;

  VertVec &points = Gfx->controlPoints;

  float easeTime = ( curveLen * EASE_IN_OUT_PCT ) / ( maxSpeed / 2.f );
  float accel = maxSpeed / easeTime;
  float tick  = (float)( GlobalTime::GetStep() ) / 1000.f;

  float easeInDist  = curveLen * EASE_IN_OUT_PCT;
  float easeOutDist = curveLen - easeInDist;

  float velDif = accel * tick;

  if ( modelPos < easeInDist )
    modelSpeed = std::min( modelSpeed + velDif, maxSpeed );
  else if ( modelPos >= easeOutDist )
    modelSpeed = std::max( modelSpeed - velDif, velDif );
  else
    modelSpeed = maxSpeed;

  modelPos += modelSpeed * tick;

  if ( modelPos > curveLen )
    modelPos = curveLen;

  FloatFloatMap::iterator low  = distToTime.begin();
  FloatFloatMap::iterator high = low;
  for ( FloatFloatMap::iterator i = distToTime.begin(); i != distToTime.end(); ++i )
  {
    if ( i->first > modelPos )
    {
      high = i;
      break;
    }
    low = i;
  }

  float u = ( modelPos - low->first ) / ( high->first - low->first );
  float t = low->second + u * ( high->second - low->second );

  if ( modelPos == curveLen )
  {
    modelPos = 0.f;
    modelSpeed = 0.f;
  }

  // move the model
  D3DXVECTOR3 pos;
  GetSplinePoint( pos, points, t );
  D3DXMatrixTranslation( &Gfx->AnimatedModels.front().MatTrans, pos.x, pos.y, pos.z );
  
  // orient him forward
  D3DXVECTOR3 orient;
  GetSplineFirstDeriv( orient, points, t );
  D3DXVECTOR3 aligned( 0.f, 0.f, -1.f ), axis;
  D3DXVec3Normalize( &orient, &orient );
  float angle = acos( D3DXVec3Dot( &aligned, &orient ) );
  D3DXVec3Cross( &axis, &aligned, &orient );
  D3DXVec3Normalize( &axis, &axis );
  D3DXMatrixRotationAxis( &Gfx->AnimatedModels.front().MatRot, &axis, angle );

  // set his animation speed
  Gfx->AnimatedModels.front().AnimSpeed = modelSpeed * ANIM_RATIO;
}

void CS460Project::AddJoints( const LPFRAME pRoot )
{
  float bound = D3DXToRadian( 360.f );
  Constraints constraints = { -bound, bound, -bound, bound, -bound, bound }; // no constraints
  Joint joint( pRoot, .15f, constraints ); // .15 is the haxxed scale of the model
  joints.push_back( joint );

  LPFRAME pCurChild = (LPFRAME)pRoot->pFrameFirstChild;
  while ( pCurChild )
  {
    if ( pCurChild->Name && strlen( pCurChild->Name ) )
    {
      AddJoints( pCurChild );
      break;
    }
    pCurChild = (LPFRAME)pCurChild->pFrameSibling;
  }
}

void CS460Project::AnimCallback( void )
{
  if ( CS460Proj->joints.empty() ) return;

  for ( JointVec::iterator i = CS460Proj->joints.begin(); i != CS460Proj->joints.end(); ++i )
  {
    if ( !Gfx->IsPaused() )
      i->Reset();
  }

  /*
  CS460Proj->StepByStep();
  /*/
  CS460Proj->ExecuteCCD();
  //*/
}

void CS460Project::ExecuteCCD( void )
{
  static DWORD pauseTime = 0;
  static float speed     = 2.f;

  if ( Gfx->IsPaused() )
  {
    float t = std::min( speed * (float)( timeGetTime() - pauseTime ) / 1000.f, 1.f );
    for ( JointVec::iterator i = CS460Proj->joints.begin(); i != CS460Proj->joints.end(); ++i )
      i->UpdateFrame( t );
  }
  else
  {
    D3DXVECTOR3 dest( Gfx->StaticModels.front().Pos );

    if ( Gfx->CCD( CS460Proj->joints, dest ) )
    {
      Gfx->PauseAnims();
      GlobalTime::Pause();

      pauseTime = timeGetTime();

      //for ( JointVec::iterator i = CS460Proj->joints.begin(); i != CS460Proj->joints.end(); ++i )
      //  i->UpdateFrame();

    }
  }
}

void CS460Project::StepByStep( void )
{
  if ( !Gfx->IsPaused() )
  {
    Gfx->PauseAnims();
    GlobalTime::Pause();
  }

  D3DXVECTOR3 dest( Gfx->StaticModels.front().Pos );

  JointVec &joints        = CS460Proj->joints;
  static int nLastParent  = (int)joints.size() - 2;
  static int nLast        = nLastParent + 1;

  static int iter         = 0;

  if ( KEYHIT( VK_RETURN ) )
    iter++;
  if ( KEYHIT( VK_BACK ) )
    iter = std::max( iter - 1, 0 );

  for ( int i = 0; i < iter; ++i )
  {
    Joint &joint = joints[nLast - (i % nLast)];
    joint.AlignTo( dest );
    joint.UpdateFrame( 1.f );
  }
}

void CS460Project::RotationTest( void )
{
  // ROTATION TEST
  if ( !Gfx->IsPaused() )
  {
    Gfx->PauseAnims();
    GlobalTime::Pause();
  }

  float speed = .25f;
  float t     = speed * fmod( (float)timeGetTime(), 1000.f / speed ) / 1000.f;
  float angle = t * ( 2.f * D3DX_PI );

  D3DXMATRIX &transform = CS460Proj->joints[3].GetFrame()->TransformationMatrix;

  D3DXVECTOR3 scale, trans;
  D3DXQUATERNION rot;
  D3DXMatrixDecompose( &scale, &rot, &trans, &transform );

  D3DXMATRIX matRot;
  D3DXMatrixRotationY( &matRot, angle );
  D3DXQuaternionRotationMatrix( &rot, &matRot );

  D3DXMatrixTransformation( &transform, NULL, NULL, &scale, NULL, &rot, &trans );
}

void CS460Project::WorldToLocalTest( void )
{
  // WORLD TO LOCAL SPACE TEST
  if ( !Gfx->IsPaused() )
  {
    Gfx->PauseAnims();
    GlobalTime::Pause();
  }

  Joint &joint = CS460Proj->joints.back();

  D3DXVECTOR3 dest( Gfx->StaticModels.front().Pos );
  float dist = joint.GetWorldDistFrom( dest );

  float det;
  D3DXVECTOR3 tempVec;
  D3DXMATRIX matInverse;
  D3DXMatrixInverse( &matInverse, &det, &joint.GetFrame()->matCombined );
  D3DXVec3TransformCoord( &tempVec, &dest, &matInverse );

  float dist2 = D3DXVec3Length( &tempVec );
  dist2 = dist2;

  // move origin to world
  D3DXVECTOR3 origin( 0.f, 0.f, 0.f );
  D3DXVec3TransformCoord( &origin, &origin, &joint.GetFrame()->matCombined );

  D3DXVec3TransformCoord( &tempVec, &dest, &matInverse );
  D3DXVec3TransformCoord( &tempVec, &tempVec, &joint.GetFrame()->matCombined );

  tempVec = tempVec - origin;
  float dist3 = D3DXVec3Length( &tempVec );
  dist3 = dist3;
}

void CS460Project::UpdateMouseInputCam( void )
{
  Camera &camera = Gfx->MainCam;
  const D3DXVECTOR3 &screenPos = m_mousePos;
  D3DXVECTOR3 isect = Gfx->IsectGroundPlane( (int)screenPos.x, (int)screenPos.y );

  bool altDown  = ( KeyDown( VK_MENU ) );
  bool altHit   = ( KeyHit( VK_MENU ) );
  bool altRel   = ( KeyReleased( VK_MENU ) );
  bool ctrlDown = ( KeyDown( VK_CONTROL ) );
  bool ctrlHit  = ( KeyHit( VK_CONTROL ) );
  bool ctrlRel  = ( KeyReleased( VK_CONTROL ) );

  // panning
  if ( KEYHIT( VK_MBUTTON ) )
  {
    m_midClickBegin   = screenPos;
    m_prevCamPos      = camera.eye;
    m_prevCamLookAt   = camera.lookAt;
  }
  else if ( KEYDOWN( VK_MBUTTON ) )
  {
    if ( altRel || altHit || ctrlHit || ctrlRel )
    {
      m_midClickBegin   = screenPos;
      m_prevCamPos      = camera.eye;
      m_prevCamLookAt   = camera.lookAt;
    }
    else if ( !altDown && !ctrlDown ) // pan
    {
      D3DXVECTOR3 prevPt = Gfx->IsectGroundPlane( (int)m_midClickBegin.x, (int)m_midClickBegin.y );

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

void CS460Project::UpdateCam( void )
{
  D3DXVECTOR3 &curPos    = Gfx->MainCam.eye;
  D3DXVECTOR3 &curLookat = Gfx->MainCam.lookAt;

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

bool CS460Project::KeyDown( int key )
{
  return ( ( curState[key] & 0x80 ) != 0 );
}

bool CS460Project::KeyHit( int key )
{
  return ( !( prevState[key] & 0x80 ) && ( curState[key] & 0x80 ) );
}

bool CS460Project::KeyReleased( int key )
{
  return ( ( prevState[key] & 0x80 ) && !( curState[key] & 0x80 ) );
}
