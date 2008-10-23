#include <windows.h>
#include <psapi.h>
#include <string>

#define MAX_PROCESSES 300

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
{
  DWORD processIDs[MAX_PROCESSES];
  bool done = false;

  while ( !done )
  {
    DWORD nBytesUsed;
    EnumProcesses( processIDs, sizeof( processIDs ), &nBytesUsed );

    DWORD nProcesses = nBytesUsed / sizeof( DWORD );

    for ( DWORD i = 0; i < nProcesses; ++i )
    {
      HANDLE hProc = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, processIDs[i] );
      if ( hProc )
      {
        char szImageName[MAX_PATH];
        ZeroMemory( szImageName, MAX_PATH );
        GetProcessImageFileName( hProc, szImageName, MAX_PATH );
        if ( std::string( szImageName ).find( "proquota.exe" ) != std::string::npos )
        {
          STARTUPINFO si;
          ZeroMemory( &si, sizeof( si ) );
          si.cb = sizeof( si );

          PROCESS_INFORMATION pi;
          ZeroMemory( &pi, sizeof( pi ) );

          CreateProcess( NULL, "taskkill /IM proquota.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
          done = true;
          break;
        }
      }
    }

    Sleep( 10 );
  }

  return 0;
}
