#pragma once

#include "Cloth.h"

#define PHYSICS Physics::Instance()

class Physics
{
  public:
    static Physics *Instance( void );
    friend class Cloth;

    void Initialize( void );
    void Update( void );

  private:
    void UpdateCloths( void );

  private:
    Physics( void );
    Physics( const Physics &rhs );
    void operator =( const Physics &rhs );
    ~Physics( void );
};
