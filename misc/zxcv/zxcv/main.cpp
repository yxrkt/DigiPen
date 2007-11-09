#include <iostream>
#include <string>

int main()
{
  std::string str( "LOL DAVID O'DAY LOL DAVID O'DAY LOL DAVID O'DAY LOL DAVID O'DAY LOL DAVID O'DAY" );

  std::cout << "using member size:    " << str.size() << std::endl
            << "using sizeof:         " << sizeof( str ) << std::endl
            << "size of empty string: " << sizeof( std::string );

  return 0;
}
