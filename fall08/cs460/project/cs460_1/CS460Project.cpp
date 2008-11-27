#include "CS460Project.h"
#include "Spline.h"

#define CURVE_PRECISION 20
#define ANIM_RATIO      .01f
#define EASE_IN_OUT_PCT .166f

// =============================================================================
// ! Constructor.
// =============================================================================
CS460Project::CS460Project( HINSTANCE hInstance )
: modelPos( 0.f )
, modelSpeed( 0.f )
, maxSpeed( 100.f )
{
  hInstance_ = hInstance;
  CreateMainWindow( "CS460 Project 1", 800, 600 );
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

  graphics.Initialize( hWndMain_ );
  //hr = D3DXCreateTextureFromFile( graphics.GetDevice(), ( std::string( ASSETS_DIR ) + 
  //                                "Floor.jpg" ).c_str(), &graphics.pFloorTex );
  //ASSERT( SUCCEEDED( hr ), "Texture not found" );
  graphics.LoadAnimatedModel( std::string( ASSETS_DIR ) + "run_inPlace.x" );
  pFrameShoulder = (LPFRAME)D3DXFrameFind( graphics.AnimatedModels.front().GetFrameRoot(), "shoulder_l" );

  AnimatedModel &model = *graphics.AnimatedModels.begin();
  D3DXMatrixScaling( &model.MatScale, .15f, .15f, .15f );

  AnimatedModel::Sphere bs = model.GetBS();

  float angle = D3DX_PI + .125f * D3DX_PI;
  float dist  = 10.f * bs.radius;

  graphics.MainCam.lookAt  = bs.center;
  graphics.MainCam.eye     = D3DXVECTOR3( dist * cos( angle ), 4.f * bs.radius, dist * sin( angle ) );
}

// =============================================================================
// ! Updates modules.
// =============================================================================
void CS460Project::Update()
{
  // Draw Floor
  D3DCOLOR floorColor = D3DCOLOR_XRGB( 100, 50, 35 );
  ColoredVertex floorQuad[] = 
  {
    ColoredVertex( -250.f, 0.f, -250.f, floorColor ),// 0.f, 0.f ),
    ColoredVertex(  250.f, 0.f, -250.f, floorColor ),// 1.f, 0.f ),
    ColoredVertex(  250.f, 0.f,  250.f, floorColor ),// 1.f, 1.f ),
    ColoredVertex( -250.f, 0.f,  250.f, floorColor )//, 0.f, 1.f )
  };
  //graphics.AddQuad( floorQuad );
  graphics.AddPolyline( floorQuad, 4 );

  // Update Spline
  if ( graphics.controlPoints.size() > 2 )
  {
    UpdateSpline();
    UpdateModel();
  }

  graphics.Update();
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
  graphics.Cleanup();
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
          //graphics.IncDecAnimSpeed( true );
          break;

        case VK_SUBTRACT:
          maxSpeed = max( maxSpeed - 25.f, 0.f );
          //graphics.IncDecAnimSpeed( false );
          break;

        case 'C':
          graphics.controlPoints.clear();
          break;

        case VK_ESCAPE:
          PostQuitMessage( 0 );
          break;
      }
      break;
    }

    case WM_LBUTTONDOWN:
    {
      D3DXVECTOR3 isectD3D = graphics.IsectGroundPlane( (int)(short)LOWORD( lParam ), 
                                                        (int)(short)HIWORD( lParam ) );
      ColoredVertex isect( isectD3D );
      isect.color = D3DCOLOR_XRGB( 0, 255, 0 );
      VertVec &points = graphics.controlPoints;
      if ( std::find( points.begin(), points.end(), isect ) == points.end() )
      {
        points.push_back( isect );
      }
      break;
    }

    case WM_MBUTTONDOWN:
    {
      SetCapture( hWndMain_ );
      clickPos.x      = (int)(short)LOWORD( lParam );
      clickPos.y      = (int)(short)HIWORD( lParam );
      startEyePos     = graphics.MainCam.eye;
      startLookatPos  = graphics.MainCam.lookAt;
      break;
    }

    case WM_MOUSEMOVE:
    {
      int x = (int)(short)LOWORD( lParam );
      int y = (int)(short)HIWORD( lParam );

      Camera &cam = graphics.MainCam;

      if ( wParam & MK_MBUTTON )
      {
        if ( wParam & MK_CONTROL  )
        {
          cam.eye = startEyePos + ( (float)( clickPos.y - y ) / 1000.f ) * ( cam.lookAt - cam.eye );
        }
        else
        {
          D3DXVECTOR3 newPos      = graphics.IsectGroundPlane( x, y );
          D3DXVECTOR3 clickIsect  = graphics.IsectGroundPlane( clickPos.x, clickPos.y );
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
      if ( graphics.Ready )
        graphics.Update();
      break;

    default:
      return DefWindowProc( hWndMain_, msg, wParam, lParam );
  }

  return 0L;
}

// --

void CS460Project::UpdateSpline()
{
  distToTime.clear();

  curveLen = 0.f;
  VertVec &points = graphics.controlPoints;
  ColoredVertex vertBegin = points.front();
  D3DXVECTOR3 vertEnd;
  distToTime[0.f] = 0.f;
  float fStep = 1.f / (float)( CURVE_PRECISION * points.size() );
  for ( float t = fStep; t < 1.f; t += fStep )
  {
    GetSplinePoint( vertEnd, points, t );
    graphics.AddLine( vertBegin, vertEnd );
    curveLen += Dist( vertBegin, vertEnd );
    distToTime[curveLen] = t;
    vertBegin = vertEnd;
  }
  graphics.AddLine( vertBegin, points.back() );
  curveLen += Dist( vertBegin, vertEnd );
  distToTime[curveLen] = 1.f;
}

void CS460Project::UpdateModel()
{
  static DWORD lastTick = timeGetTime();
  VertVec &points = graphics.controlPoints;

  float easeTime = ( curveLen * EASE_IN_OUT_PCT ) / ( maxSpeed / 2.f );
  float accel = maxSpeed / easeTime;
  float tick  = (float)( timeGetTime() - lastTick ) / 1000.f;
  lastTick    = timeGetTime();

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
  D3DXMatrixTranslation( &graphics.AnimatedModels.front().MatTrans, pos.x, pos.y, pos.z );
  
  // orient him forward
  D3DXVECTOR3 orient;
  GetSplineFirstDeriv( orient, points, t );
  D3DXVECTOR3 aligned( 0.f, 0.f, -1.f ), axis;
  D3DXVec3Normalize( &orient, &orient );
  float angle = acos( D3DXVec3Dot( &aligned, &orient ) );
  D3DXVec3Cross( &axis, &aligned, &orient );
  D3DXVec3Normalize( &axis, &axis );
  D3DXMatrixRotationAxis( &graphics.AnimatedModels.front().MatRot, &axis, angle );

  // set his animation speed
  graphics.AnimatedModels.front().AnimSpeed = modelSpeed * ANIM_RATIO;
}
