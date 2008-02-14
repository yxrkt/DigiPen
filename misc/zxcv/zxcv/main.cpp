#include <iostream>

class B {};
class D : public B {};

class Base
{
  public:
    Base()
    {
      pFoo[0] = &Base::foo1;
      pFoo[1] = &Base::foo2;
    }

    void CallMyFoos()
    {
      B b;
      ( this->*pFoo[0] )( &b );
      ( this->*pFoo[1] )( &b );
    }

  private:
    void foo1( B *b )
    {
      std::cout << "foo1 was called" << std::endl;
    }

    void foo2( B *b )
    {
      std::cout << "foo2 was called" << std::endl;
    }

    void foo3( D *d )
    {
    }

    void ( Base::*pFoo[2] )( B * );
};

void Bar()
{
  std::cout << "Bar was called." << std::endl;
}

int main()
{
  void (*pFunc)( void ) = NULL;
  pFunc = Bar;

  Bar();

  Base b;

  b.CallMyFoos();

  std::cout << std::endl;
  return 0;
}
