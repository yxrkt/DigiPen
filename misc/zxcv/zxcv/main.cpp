#include <iostream>
#include <winsock2.h>

int main()
{
  SOCKADDR_IN sa;
  std::cout << sizeof( sa.sin_addr.s_addr ) << std::endl;
  std::cout << sizeof( sa.sin_addr ) << std::endl;

  return 0;
}
