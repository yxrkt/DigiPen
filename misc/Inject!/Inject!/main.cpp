#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#define DATA_CHUNK_SIZE 1024
#define CODE_CHUNK_SIZE 1024


#define ASSERT( _expr, _msg )\
  if ( !_expr ){\
  MessageBox( NULL, _msg, "Damn", MB_ICONERROR );\
  return 0;}

// types
typedef LRESULT ( WINAPI *SENDMESSAGEFN )( HWND, UINT, WPARAM, LPARAM );
typedef int ( WINAPI *MESSAGEBOXFN )( HWND, LPCSTR, LPCSTR, UINT );

// globals
HWND     g_hWnd;

BOOL CALLBACK FindNotepadProc( HWND hWnd, LPARAM lParam )
{
  char szWindowName[MAX_PATH];
  GetWindowText( hWnd, szWindowName, sizeof( szWindowName ) );
  if ( !strcmp( szWindowName, "Untitled - Notepad" ) )
  {
    GetWindowThreadProcessId( hWnd, (LPDWORD)lParam );
    g_hWnd = hWnd;
    return false;
  }
  return true;
}

// classes & structs
struct InjData
{
  SENDMESSAGEFN fnSendMessage;
  MESSAGEBOXFN  fnMessageBox;

  HWND hWnd;
  char szString[100];
};

// prototypes
void SetPrivileges();

static DWORD WINAPI ThreadFunc( InjData *pData )
{
  pData->fnSendMessage( pData->hWnd, WM_GETTEXT, (WPARAM)100, (LPARAM)pData->szString );
  //while ( true )
  //  pData->fnMessageBox( NULL, pData->szString, pData->szString, MB_OK );

  //while ( true ) ; // Works!!

  return 0;
}
static void DummyFunc() {}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int )
{
  HANDLE    hProc;
  HANDLE    hThread;
  PBYTE     pData;
  PBYTE     pCode;
  InjData   injData;
  DWORD     pID;

  ASSERT( !EnumWindows( &FindNotepadProc, (LPARAM)&pID ), "notepad not found!" );

  injData.fnSendMessage = &SendMessageA;
  injData.fnMessageBox  = &MessageBoxA;
  injData.hWnd          = g_hWnd;
  strcpy( injData.szString, "omg hai2u!!1!" );

  // Step 1: Get full access to remote process
  SetPrivileges();
  hProc = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pID );
  ASSERT( hProc != NULL, "opening process with full access rights failed =(" );

  // Step 2: Allocate block of memory for data in remote process
  pData = (PBYTE)VirtualAllocEx( hProc, NULL, DATA_CHUNK_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
  ASSERT( pData != NULL, "allocating memory for data failed =(" );

  // Step 3: Copy data to remote process
  ASSERT( WriteProcessMemory( hProc, pData, &injData, sizeof( InjData ), NULL ), "copying data to memory failed =(" );

  // Step 4: Allocate block of memory for code in remote process
  pCode = (PBYTE)VirtualAllocEx( hProc, NULL, CODE_CHUNK_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
  ASSERT( pCode != NULL, "allocating memory for code failed =(" );

  // Step 5: Copy function to remote process
#pragma warning( disable : 4311 )
  ASSERT( WriteProcessMemory( hProc, pCode, &ThreadFunc, (SIZE_T)&DummyFunc - (SIZE_T)&ThreadFunc, NULL ), "copying function to memory failed =(" );
#pragma warning( default : 4311 )

  // Step 6: Create remote thread!
  hThread = CreateRemoteThread( hProc, NULL, 0, (LPTHREAD_START_ROUTINE)pCode, pData, 0, NULL );
  ASSERT( hThread != NULL, "creating thread failed =(" )

  // Step 7: Wait for thread to terminate
  //WaitForSingleObject( hThread, INFINITE );


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

