// =============================================================================
#ifndef JOINT_H
#define JOINT_H
// =============================================================================

#include <vector>

#include "DerivedStructs.h"

struct Constraints
{
  float operator []( int index )
  {
    return reinterpret_cast< float *>( this )[index];
  }

  float minX, maxX;
  float minY, maxY;
  float minZ, maxZ;
};

class Joint
{
  public:
    Joint( const LPFRAME pFrame, float scale, const Constraints &constraints );
    ~Joint( void );

    void Reset( void );
    void AlignTo( const D3DXVECTOR3 &world );
    void UpdateFrame( float t = 1.f );
    void SetParent( Joint *pJoint );
    void SetChild( Joint *pChild );

    void    GetWorldPos( D3DXVECTOR3 &out ) const;
    float   GetWorldDistFrom( const D3DXVECTOR3 &world ) const;
    LPFRAME GetFrame( void ) const;
    float   GetScale( void ) const;

  private:
    void UpdateCombined( void );
    void ApplyRotation( const D3DXMATRIX &matRot );

    D3DXMATRIX    m_transformBegin;
    LPFRAME       m_pFrame;
    Constraints   m_constraints;

    Joint        *m_pParent;
    Joint        *m_pChild;

    D3DXVECTOR3   m_end;
    D3DXMATRIX    m_transform;
    D3DXMATRIX    m_combined;
    float         m_scale;
    bool          m_interpolating;
};

typedef std::vector< Joint > JointVec;

#endif
