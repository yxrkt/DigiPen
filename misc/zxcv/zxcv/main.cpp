#include <iostream>

struct DAVID_LOL
{
  short s;
  int i;
  long l;
};


int main()
{
  DAVID_LOL LOLDAVID;
  struct { short S; int I; long L; } david_LOL;
  std::cout << (typeid(LOLDAVID) == typeid(DAVID_LOL) ? true : false) << std::endl;

  return 0;
}
