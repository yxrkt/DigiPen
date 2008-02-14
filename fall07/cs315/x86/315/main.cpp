//-----------------------------------------------------------------------------
// Alex Serio
// CS 315A
// 10-24-07
//-----------------------------------------------------------------------------

#include <iostream>
#include <time.h>
#include <sstream>

void timeTest( unsigned nTests );
void BubbleSort( int *array, int arrayLen, bool C = false );

// ============================================================================
// Entry point
// ============================================================================
int main()
{
  int array[] = { 10, 4, 55, 66, 19, 5 };
  int nSize   = 6;

  //BubbleSort( array, nSize, true );
  BubbleSort( array, nSize );

  for ( int i = 0; i < nSize; ++i )
    std::cout << array[i] << std::endl;

  //timeTest( 4 );

  return 0;
}

// ============================================================================
// Bubble Sort -- By default, runs in assembly
// ============================================================================
void BubbleSort( int *array, int arrayLen, bool C )
{
  bool bSwapped;

    // C implementation
  if ( C )
  {
    int iTemp;
    do {
      bSwapped = false;
      for ( int i = 0; i < arrayLen - 1; ++i )
      {
        if ( array[i] > array[i + 1] )
        {
          iTemp = array[i];
          array[i] = array[i + 1];
          array[i + 1] = iTemp;
          bSwapped = true;
        }
      }
    } while ( bSwapped );

    return;
  }

    // Assembly implementation
  __asm
  {
  loop_bubble_sort:

    mov   ebx,array
    mov   bSwapped,0
    mov   ecx,arrayLen
    sub   ecx,1

    _loop_bubble_sort:
      mov   eax,[ebx]
      cmp   eax,[ebx + 4]
      jl    _end_loop_bubble_sort

      mov   edx,[ebx]
      mov   eax,[ebx + 4]
      mov   [ebx],eax
      mov   [ebx + 4],edx
      mov   bSwapped,1

    _end_loop_bubble_sort:
      add   ebx,4h
      loop  _loop_bubble_sort

  ;end_bubble_sort_loop:
    mov   al,0
    cmp   bSwapped,al
    jne   loop_bubble_sort
  }
}

// ============================================================================
// Compares run-time for C bubble sort with ASM bubble sort
// ============================================================================
void timeTest( unsigned nTests )
{
  for ( unsigned i = 0; i < nTests; ++i )
  {
    unsigned BIG_NUMBER = 1000000;

    std::stringstream ssTest;
    ssTest << "Test " << i << std::endl;
    std::cout << ssTest.str().c_str();

    int nSize = 6;
    time_t using_c, using_asm, start;

      // first test
    start = clock();
    for ( unsigned i = 0; i < BIG_NUMBER; ++i )
    {
      int intArray[] = { 10, 4, 55, 66, 19, 5 };
      BubbleSort( intArray, nSize, true );
    }

    using_c = clock() - start;

    start = clock();
    for ( unsigned i = 0; i < BIG_NUMBER; ++i )
    {
      int intArray[] = { 10, 4, 55, 66, 19, 5 };
      BubbleSort( intArray, nSize );
    }

    using_asm = clock() - start;

    std::cout << "Using C:   " << using_c << std::endl;
    std::cout << "Using ASM: " << using_asm << std::endl;

      // reverse order
    start = clock();
    for ( unsigned i = 0; i < BIG_NUMBER; ++i )
    {
      int intArray[] = { 10, 4, 55, 66, 19, 5 };
      BubbleSort( intArray, nSize );
    }

    using_asm = clock() - start;

    start = clock();
    for ( unsigned i = 0; i < BIG_NUMBER; ++i )
    {
      int intArray[] = { 10, 4, 55, 66, 19, 5 };
      BubbleSort( intArray, nSize, true );
    }

    using_c = clock() - start;

    std::cout << "Using C:   " << using_c << std::endl;
    std::cout << "Using ASM: " << using_asm << std::endl;
    std::cout << std::endl;
  }
}
