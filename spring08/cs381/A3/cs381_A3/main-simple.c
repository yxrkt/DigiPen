#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "clips/clips.h"

#define SIZE 4
#define COLORS 8

/*this is the nsequence you are trying to guess*/
unsigned char secret[SIZE] = { 1, 2, 3, 4 };

/*number of steps/attempts*/
unsigned char count = 0;

/*done flag*/
unsigned char done = 0;

void guess_simple( DATA_OBJECT_PTR val )
{
	unsigned char guess[SIZE], i, j;
	unsigned char usedposition_guess[SIZE]  = { 0 };
	unsigned char usedposition_secret[SIZE] = { 0 };
	unsigned char result[SIZE];
	unsigned char bulls = 0;
	unsigned char cows  = 0;
	char assert_buffer[32];
	++count;

	for ( i = 0; i < SIZE; ++i )
  {
		result[i] = 0;
		guess[i]  = (unsigned char)RtnLong( i + 1 );
	}

	for ( i = 0; i < SIZE; ++i )
  {
		if (secret[i] == guess[i] )
    {
			++bulls; 
			usedposition_secret[i] = 1; 
			usedposition_guess[i]  = 1; 
			result[i] = 2;
		}
	}

	if ( bulls == SIZE )
    done = 1;

	for ( i = 0; i < SIZE; ++i )
  {
		if ( usedposition_guess[i] == 0 )
    { 
			for ( j = 0; j < SIZE; ++j )
      {
				if ( secret[j] == guess[i] && usedposition_secret[j] == 0 )
        {
					++cows;
					usedposition_secret[j] = 1;
					result[i] = 1;
				}
			}
		}
	}

	for ( i = 0; i < SIZE; ++i )
  {
		switch (result[i])
    {
			case 0:
				sprintf( assert_buffer, "(nothing-at %i)", i );
				AssertString( assert_buffer ); /*built-in version*/
				break;
			case 1:
				sprintf( assert_buffer, "(cow-at %i)", i );
				AssertString( assert_buffer ); /*built-in version*/
				break;
			case 2:
				sprintf( assert_buffer, "(bull-at %i)", i );
				AssertString( assert_buffer ); /*built-in version*/
				break;
		}
	}
}

int main()
{
	DATA_OBJECT rtn;
	char assert_buffer[32], key;

	InitializeEnvironment();

	/* define function AFTER InitializeEnvironment */
	DefineFunction( "guess_simple", 'v', PTIF guess_simple, "guess_simple" );

	Load( "rules-simple.clp" );
	Reset();
	sprintf( assert_buffer, "(seq-size %i)", SIZE );
	AssertString( assert_buffer ); /*built-in version*/
	sprintf( assert_buffer, "(num-colors %i)", COLORS );
	AssertString( assert_buffer ); /*built-in version*/
	Run( -1 );
	FunctionCall( "print_all_facts", "", &rtn );
	printf( "done %i\n", done );
	printf( "moves %i\n", count );


  do {
    key = _kbhit() ? _getch() : '\0';
  } while ( key == '\0' );

	return 0;
}

