#include "CS460Project.h"
#include "Spline.h"
#include "GlobalTime.h"

#define CURVE_PRECISION 20
#define ANIM_RATIO      .01f
#define EASE_IN_OUT_PCT .166f

CS460Project *CS460Project::CS460Proj = NULL;

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
CS460Project::~CS460Project() {}

// =============================================================================
// ! Initialize.
// =============================================================================
void CS460Project::Initialize()
{
  //HRESULT hr;

  Gfx->Initialize( hWndMain_ );
  Gfx->animCallback = AnimCallback;
  //hr = D3DXCreateTextureFromFile( Gfx->GetDevice(), ( std::string( ASSETS_DIR ) + 
  //                                "Floor.jpg" ).c_str(), &Gfx->pFloorTex );
  //ASSERT( SUCCEEDED( hr ), "Texture not found" );
  Gfx->LoadAnimatedModel( "run_inPlace.x" );
  LPFRAME pFrameCur = (LPFRAME)D3DXFrameFind( Gfx->AnimatedModels.front().GetFrameRoot(), "shoulder_l" );
  AddNamedFrames( pFrameCur );
  Gfx->LoadStaticModel( "IceChar_RunAnimation.X", D3DXVECTOR3( 0.f, 60.f, -30.f ), .50f );

  AnimatedModel &model = Gfx->AnimatedModels.front();
  D3DXMatrixScaling( &model.MatScale, .15f, .15f, .15f );

  AnimatedModel::Sphere bs = model.GetBS();

  float angle = D3DX_PI + .125f * D3DX_PI;
  float dist  = 10.f * bs.radius;

  Gfx->MainCam.lookAt  = bs.center;
  Gfx->MainCam.eye     = D3DXVECTOR3( dist * cos( angle ), 4.f * bs.radius, dist * sin( angle ) );
}

// =============================================================================
// ! Updates modules.
// =============================================================================
void CS460Project::Update()
{
  GlobalTime::Update();

  // Draw Floor
  D3DCOLOR floorColor = D3DCOLOR_XRGB( 100, 50, 35 );
  ColoredVertex floorQuad[] = 
  {
    ColoredVertex( -250.f, 0.f, -250.f, floorColor ),// 0.f, 0.f ),
    ColoredVertex(  250.f, 0.f, -250.f, floorColor ),// 1.f, 0.f ),
    ColoredVertex(  250.f, 0.f,  250.f, floorColor ),// 1.f, 1.f ),
    ColoredVertex( -250.f, 0.f,  250.f, floorColor )//, 0.f, 1.f )
  };
  //Gfx->AddQuad( floorQuad );
  Gfx->AddPolyline( floorQuad, 4 );

  // Update Spline
  if ( Gfx->controlPoints.size() > 2 )
  {
    UpdateSpline();
    UpdateModel();
  }

  Gfx->Update();
}

// =============================================================================
// ! Run application.
// =============================================================================
int CS460Project::Run()
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
void CS460Project::Cleanup()
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
        case VK_ADD:
          maxSpeed += 25.f;
          //Gfx->IncDecAnimSpeed( true );
          break;

        case VK_SUBTRACT:
          maxSpeed = max( maxSpeed - 25.f, 0.f );
          //Gfx->IncDecAnimSpeed( false );
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
      Gfx->StaticModels.front().Pos = isectD3D;
      Gfx->StaticModels.front().Pos.y = 45.f;
      GeneratePath( D3DXVECTOR3( 0.f, 0.f, 0.f ), isectD3D );
      Gfx->PlayAnims();
      GlobalTime::Unpause();
      //ColoredVertex isect( isectD3D );
      //isect.color = D3DCOLOR_XRGB( 0, 255, 0 );
      //VertVec &points = Gfx->controlPoints;
      //if ( std::find( points.begin(), points.end(), isect ) == points.end() )
      //{
      //  points.push_back( isect );
      //}
      break;
    }

    case WM_MBUTTONDOWN:
    {
      SetCapture( hWndMain_ );
      clickPos.x      = (int)(short)LOWORD( lParam );
      clickPos.y      = (int)(short)HIWORD( lParam );
      startEyePos     = Gfx->MainCam.eye;
      startLookatPos  = Gfx->MainCam.lookAt;
      break;
    }

    case WM_MOUSEMOVE:
    {
      int x = (int)(short)LOWORD( lParam );
      int y = (int)(short)HIWORD( lParam );

      Camera &cam = Gfx->MainCam;

      if ( wParam & MK_MBUTTON )
      {
        if ( wParam & MK_CONTROL  )
        {
          cam.eye = startEyePos + ( (float)( clickPos.y - y ) / 1000.f ) * ( cam.lookAt - cam.eye );
        }
        else
        {
          D3DXVECTOR3 newPos      = Gfx->IsectGroundPlane( x, y );
          D3DXVECTOR3 clickIsect  = Gfx->IsectGroundPlane( clickPos.x, clickPos.y );
          D3DXVECTOR3 panTrans    = clickIsect - newPos;
          cam.eye                 = startEyePos + panTrans;
          cam.lookAt              = startLookatPos + panTrans;
        }
      }
      break;
    }

    case WM_MBUTTONUP:
    {
      ReleaseCapture();
      break;
    }

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

  points[1].color = D3DCOLOR_XRGB( 255, 0, 0 );
  points[2].color = D3DCOLOR_XRGB( 0, 0, 255 );
}

void CS460Project::UpdateSpline()
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

void CS460Project::UpdateModel()
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
    modelSpeed = min( modelSpeed += velDif, maxSpeed );
  else if ( modelPos >= easeOutDist )
    modelSpeed = max( modelSpeed - velDif, velDif );
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

void CS460Project::AddNamedFrames( const LPFRAME pRoot )
{
  armFrames.push_back( pRoot );

  LPFRAME pCurChild = (LPFRAME)pRoot->pFrameFirstChild;
  while ( pCurChild )
  {
    if ( pCurChild->Name && strlen( pCurChild->Name ) )
    {
      AddNamedFrames( pCurChild );
      break;
    }
    pCurChild = (LPFRAME)pCurChild->pFrameSibling;
  }
}

void CS460Project::AnimCallback( void )
{
  /*

  int nFrames = (int)CS460Proj->armFrames.size();
  for ( int i = 0; i < nFrames; ++i )
  {
    D3DXVECTOR3 vecScale, vecTrans;
    D3DXQUATERNION quatRot;
    D3DXMATRIX matTrans, matRot, matScale;
    D3DXMatrixDecompose( &vecScale, &quatRot, &vecTrans, &CS460Proj->armFrames[i]->TransformationMatrix );
    D3DXMatrixTranslation( &matTrans, vecTrans.x, vecTrans.y, vecTrans.z );
    D3DXMatrixRotationQuaternion( &matRot, &quatRot );
    D3DXMatrixScaling( &matScale, vecScale.x, vecScale.y, vecScale.z );

    D3DXMatrixMultiply( &CS460Proj->armFrames[i]->TransformationMatrix, &matScale, &matRot );
    D3DXMatrixMultiply( &CS460Proj->armFrames[i]->TransformationMatrix, &CS460Proj->armFrames[i]->TransformationMatrix, &matTrans );
  }
  
  /*/
  static bool       showSolution = false;
  static MatrixVec  matricesOut;

  if ( !showSolution )
  {
    if ( Gfx->CCD( &matricesOut, &CS460Proj->armFrames, Gfx->StaticModels.front().Pos, NULL ) )
    {
      Gfx->PauseAnims();
      GlobalTime::Pause();
      showSolution = true;
    }
  }

  if ( showSolution )
  {
    int nFrames = (int)CS460Proj->armFrames.size();
    for ( int i = 0; i < nFrames; ++i )
    {
      CS460Proj->armFrames[i]->TransformationMatrix = matricesOut[i];
    }
  }

  //*/
}
