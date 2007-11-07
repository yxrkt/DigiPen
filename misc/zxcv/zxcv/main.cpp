#include <iostream>

int main()
{
  char input = 'r';

  while ( input != 'q' )
  {
    float t1, t2, t3;

    std::cout << "t1 = ";
    std::cin >> t1;

    std::cout << "t2 = ";
    std::cin >> t2;

    std::cout << "t3 = ";
    std::cin >> t3;

    float x = ( -2.f * t1 * t2 * t3 ) + ( t1 * t2 + t2 * t3 + t3 * t1 );
    float y = -( t1 * t2 + t2 * t3 + t3 * t1) + ( t1 + t2 + t3 );

    std::cout << "(" << x << ", " << y << ")" << std::endl;

    std::cout << std::endl << "[r] to restart, [q] to quit>";
    std::cin >> input;
  }

  return 0;
}
