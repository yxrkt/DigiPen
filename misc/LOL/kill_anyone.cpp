#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

#include <windows.h>
#include <stdio.h>

BOOL SetPrivilege( HANDLE hToken,           // token handle
                   LPCTSTR Privilege,       // Privilege to enable/disable
                   BOOL bEnablePrivilege ); // TRUE to enable.  FALSE to disable

void DisplayError( LPTSTR szAPI );

int main( int argc, char *argv[] )
{
  HANDLE hProcess;
  HANDLE hToken;
  int dwRetVal = RTN_OK; // assume success from main()

  // show correct usage for kill
  if ( argc != 2 )
  {
    fprintf( stderr, "Usage: %s [ProcessId]\n", argv[0] );
    return RTN_USAGE;
  }

  if ( !OpenThreadToken( GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken ) )
  {
    if ( GetLastError() == ERROR_NO_TOKEN )
    {
      if ( !ImpersonateSelf( SecurityImpersonation ) )
        return RTN_ERROR;

      if( !OpenThreadToken( GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken ) )
      {
        DisplayError("OpenThreadToken");
        return RTN_ERROR;
      }
    }
    else
    {
      return RTN_ERROR;
    }
  }

  // enable SeDebugPrivilege
  if( !SetPrivilege( hToken, SE_DEBUG_NAME, TRUE ) )
  {
    DisplayError( "SetPrivilege" );

    // close token handle
    CloseHandle( hToken );

    // indicate failure
    return RTN_ERROR;
  }

  // open the process
  if( ( hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, atoi(argv[1] ) ) ) == NULL )
  {
    DisplayError( "OpenProcess" );
    return RTN_ERROR;
  }

  // disable SeDebugPrivilege
  SetPrivilege( hToken, SE_DEBUG_NAME, FALSE );

  if ( !TerminateProcess( hProcess, 0xFFFFFFFF ) )
  {
    DisplayError( "TerminateProcess" );
    dwRetVal = RTN_ERROR;
  }

  // close handles
  CloseHandle( hToken );
  CloseHandle( hProcess );

  return dwRetVal;
}

BOOL SetPrivilege2( HANDLE hToken,          // token handle
                   LPCTSTR Privilege,      // Privilege to enable/disable
                   BOOL bEnablePrivilege ) // TRUE to enable.  FALSE to disable
{
  TOKEN_PRIVILEGES tp;
  LUID luid;
  TOKEN_PRIVILEGES tpPrevious;
  DWORD cbPrevious = sizeof( TOKEN_PRIVILEGES );

  if ( !LookupPrivilegeValue( NULL, Privilege, &luid ) ) return FALSE;

  // 
  // first pass.  get current privilege setting
  // 
  tp.PrivilegeCount           = 1;
  tp.Privileges[0].Luid       = luid;
  tp.Privileges[0].Attributes = 0;

  AdjustTokenPrivileges( hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious );

  if ( GetLastError() != ERROR_SUCCESS ) return FALSE;

  // 
  // second pass.  set privilege based on previous setting
  // 
  tpPrevious.PrivilegeCount       = 1;
  tpPrevious.Privileges[0].Luid   = luid;

  if ( bEnablePrivilege )
  {
    tpPrevious.Privileges[0].Attributes |= ( SE_PRIVILEGE_ENABLED );
  }
  else
  {
    tpPrevious.Privileges[0].Attributes ^= ( SE_PRIVILEGE_ENABLED &
    tpPrevious.Privileges[0].Attributes );
  }

  AdjustTokenPrivileges( hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL );

  if ( GetLastError() != ERROR_SUCCESS ) return FALSE;

  return TRUE;
}

BOOL SetPrivilege( HANDLE hToken,           // token handle
                   LPCTSTR Privilege,       // Privilege to enable/disable
                   BOOL bEnablePrivilege )  // TRUE to enable. FALSE to disable
{
  TOKEN_PRIVILEGES tp = { 0 };
  // Initialize everything to zero
  LUID luid;
  DWORD cb = sizeof( TOKEN_PRIVILEGES );

  if ( !LookupPrivilegeValue( NULL, Privilege, &luid ) )
    return FALSE;

  tp.PrivilegeCount = 1;
  tp.Privileges[0].Luid = luid;

  if ( bEnablePrivilege )
  {
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  }
  else
  {
    tp.Privileges[0].Attributes = 0;
  }

  AdjustTokenPrivileges( hToken, FALSE, &tp, cb, NULL, NULL );

  if ( GetLastError() != ERROR_SUCCESS )
    return FALSE;

  return TRUE;
}

void DisplayError( LPTSTR szAPI )  // pointer to failed API name
{
  LPTSTR MessageBuffer;
  DWORD dwBufferLength;

  fprintf( stderr, "%s() error!\n" , szAPI );

  if ( dwBufferLength = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                                       NULL, GetLastError(), GetSystemDefaultLangID(),
                                       (LPTSTR) &MessageBuffer, 0, NULL ) )
  {
    DWORD dwBytesWritten;

    // 
    // Output message string on stderr
    // 
    WriteFile( GetStdHandle(STD_ERROR_HANDLE),
               MessageBuffer,
               dwBufferLength,
               &dwBytesWritten,
               NULL );

    // 
    // free the buffer allocated by the system
    // 
    LocalFree(MessageBuffer);
  }
}
