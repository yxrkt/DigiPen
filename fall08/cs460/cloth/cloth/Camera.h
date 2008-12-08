#pragma once

struct Camera
{
  public:
    Camera( const D3DXVECTOR3 &_Eye    = D3DXVECTOR3( 0.f, 0.f, -100.f ), 
            const D3DXVECTOR3 &_LookAt = D3DXVECTOR3( 0.f, 0.f, 0.f ), 
            const D3DXVECTOR3 &_Up     = D3DXVECTOR3( 0.f, 1.f, 0.f ),
            float _Near = 10.f, float _Far = 5000.f, float _FovY = D3DX_PI / 4.f )
    : Eye( _Eye )
    , LookAt( _LookAt )
    , Up( _Up )
    , Near( _Near )
    , Far( _Far )
    , FovY( _FovY )
    {}

    ~Camera( void ) {}

    D3DXVECTOR3 Eye;
    D3DXVECTOR3 LookAt;
    D3DXVECTOR3 Up;
    float       Near;
    float       Far;
    float       FovY;
  };