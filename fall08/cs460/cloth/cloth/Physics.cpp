#include "Physics.h"


Physics *Physics::Instance( void )
{
  static Physics physics;
  return &physics;
}

Physics::Physics( void )
{
}

Physics::~Physics( void )
{
}

void Physics::Initialize( void )
{
}

void Physics::Update( void )
{
  UpdateCloths();
}

void Physics::UpdateCloths( void )
{
  AutoList< Cloth >::iter_type i = AutoList< Cloth >::List.begin();
  for ( ; i != AutoList< Cloth >::List.end(); ++i )
  {
    Cloth *pCloth = *i;
    // update vertices
    // ...
    pCloth->UpdateNormals();
  }
}
