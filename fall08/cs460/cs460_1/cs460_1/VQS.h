#pragma once

#include <d3dx9.h>

class VQS
{
  public:
    static void Interpolate( VQS &vqsOut, const VQS &vqs0, const VQS &vqs1, float t );

  public:
    VQS( D3DXVECTOR3 _v = D3DXVECTOR3(), D3DXQUATERNION _q = D3DXQUATERNION(), float _s = 0 );
    VQS( const VQS &rhs );
    ~VQS();

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
