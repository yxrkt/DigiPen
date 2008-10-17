#pragma once

#include <d3dx9.h>

#define FLOAT_EPSILON   0.00001f

class VQS
{
  public:
    static void Interpolate( VQS &vqsOut, const VQS &vqs0, const VQS &vqs1, float t );

  private:
    static void Slerp( D3DXQUATERNION &qOut, const D3DXQUATERNION &q0, const D3DXQUATERNION &q1, float t );

  public:
    VQS( D3DXVECTOR3 _v = D3DXVECTOR3(), D3DXQUATERNION _q = D3DXQUATERNION(), float _s = 0 );
    VQS( const VQS &rhs );
    ~VQS();

    void GetMatrix( D3DXMATRIX &matrix );

    D3DXVECTOR3     &V;
    D3DXQUATERNION  &Q;
    float           &S;

  private:
    D3DXVECTOR3     v;
    D3DXQUATERNION  q;
    float           s;

  private:
    void operator =( const VQS &rhs );
};
