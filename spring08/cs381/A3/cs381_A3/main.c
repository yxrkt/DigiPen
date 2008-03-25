#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "clips/clips.h"

#define SIZE 4
#define COLORS 8

/*this is the nsequence you are trying to guess*/
unsigned char secret[SIZE] = { 6, 1, 7, 7 };

/*number of steps/attempts*/
unsigned char count = 0;

/*done flag*/
unsigned char done = 0;

void guess( DATA_OBJECT_PTR val )
{
	unsigned char guess[SIZE], i, j;
	unsigned char usedposition_guess[SIZE] = { 0 };
	unsigned char usedposition_secret[SIZE] = { 0 };
	unsigned char bulls = 0;
	unsigned char cows = 0;
	char assert_buffer[12];
	++count;

	for ( i = 0; i < SIZE; ++i )
  {
		guess[i] = (unsigned char)RtnLong( i + 1 );
	}

	for ( i = 0; i < SIZE; ++i )
  {
		if ( secret[i] == guess[i] )
    { 
			++bulls; 
			usedposition_secret[i] = 1;
			usedposition_guess[i] = 1;
		}
	}
	if ( bulls == SIZE )
    done=1;

	for ( i = 0; i < SIZE; ++i )
  {
		for ( j = 0; j < SIZE; ++j )
    {
			if ( usedposition_guess[i] == 0 && usedposition_secret[j] == 0 && secret[j] == guess[i] )
      {
				++cows;
				usedposition_secret[j] = 1;
				usedposition_guess[i] = 1;
			}
		}
	}
	sprintf( assert_buffer, "(bulls %i)", bulls );
	AssertString( assert_buffer ); /*built-in version*/
	sprintf( assert_buffer, "(cows %i)", cows );
	AssertString( assert_buffer ); /*built-in version*/
}

int main()
{
	DATA_OBJECT rtn;
	char assert_buffer[32], key;

	InitializeEnvironment();

	/* define function AFTER InitializeEnvironment */
	DefineFunction( "guess", 'v', PTIF guess, "guess" );

	Load( "rules.clp" );
	Reset();
	sprintf( assert_buffer, "(seq-size %i)", SIZE );
	AssertString( assert_buffer ); /*built-in version*/
	sprintf( assert_buffer, "(num-colors %i)", COLORS );
	AssertString( assert_buffer ); /*built-in version*/
	Run( -1 );
	FunctionCall( "print_all_facts", "", &rtn );
	printf ( "done %i\n", done );
	printf ( "moves %i\n", count );

  do {
    key = _kbhit() ? _getch() : '\0';
  } while ( key == '\0' );

	return 0;
}

