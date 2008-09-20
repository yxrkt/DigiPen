#include <windows.h>

#define TEMP_ID 5720
// types
typedef LRESULT ( WINAPI *SENDMESSAGE )( HWND, UINT, WPARAM, LPARAM );

// classes & structs
struct InjData
{
  SENDMESSAGE fnSendMessage;
};

// prototypes
DWORD WINAPI ThreadFunc( InjData *pData );
void SetPrivileges();

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int )
{
  SetPrivileges();

  HANDLE hProc = OpenProcess( PROCESS_ALL_ACCESS, FALSE, TEMP_ID );

  void *pMemory = VirtualAllocEx( hProc, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE );

  if ( pMemory == NULL )
    MessageBox( NULL, "allocating memory failed =(", "Damn", MB_ICONERROR );

  InjData injData;

  SIZE_T bytesWritten;
  if ( !WriteProcessMemory( hProc, pMemory, &injData, sizeof( InjData ), &bytesWritten ) )
    MessageBox( NULL, "copying data to memory failed =(", "Damn", MB_ICONERROR );

  //TerminateProcess( hProc, 0xFFFFFFFF );

  return 0;
}

void SetPrivileges()
{
  HANDLE hToken;

  OpenThreadToken( GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken );

  TOKEN_PRIVILEGES tp;
  memset( &tp, 0, sizeof( tp ) );
  LUID luid;

  LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &luid );

  tp.PrivilegeCount = 1;
  tp.Privileges[0].Luid = luid;

  tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

  AdjustTokenPrivileges( hToken, FALSE, &tp, sizeof( TOKEN_PRIVILEGES ), NULL, NULL );
}

DWORD WINAPI ThreadFunc( InjData *pData )
{
  return 0;
}
