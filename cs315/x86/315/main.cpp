#include <iostream>


int Findmax( int *array, int arrayLen )
{
  __asm
  {
    mov   ebx,array
    mov   eax,[ebx]
    mov   ecx,arrayLen

  loop_findmax:
    cmp   eax,[ebx]
    jg    loop_max_end
    mov   eax,[ebx]

  end_findmax_end:
    add   ebx,4h
    loop  loop_max
  }
}

int main()
{
  int array[] = { 10, 4, 55, 66, 19, 5 };
  std::cout << Findmax( array, 6 ) << std::endl;

  return 0;
}
