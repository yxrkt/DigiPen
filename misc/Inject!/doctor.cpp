#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string>

#define DATA_CHUNK_SIZE 1024
#define CODE_CHUNK_SIZE 1024

#define ASSERT( expr, msg )\
  if ( !expr ){\
  MessageBox( NULL, msg, "Damn", MB_ICONERROR );\
  exit( -1 );}

typedef LRESULT ( WINAPI *SENDMESSAGEFN )( HWND, UINT, WPARAM, LPARAM );

struct StageData
{
  int width, height;
};

struct InjData
{
  SENDMESSAGEFN       fnSendMessage;
  HWND                hWnd;

  StageData           stageData;
};

enum MESSAGE_TYPE { DATA_PTR, FUNC_PTR };

static DWORD ThreadFunc( InjData *pData )
{
  pData->fnSendMessage( pData->hWnd, WM_APP, (WPARAM)DATA_PTR, (LPARAM)&pData->stageData );

  return 0;
}
static void DummyFunc() {}

BOOL CALLBACK FindPatient( HWND hWnd, LPARAM hWndPtr )
{
  char szWindowText[MAX_PATH];
  GetWindowTextA( hWnd, szWindowText, sizeof( szWindowText ) );
  if ( !strcmp( szWindowText, "patient" ) )
  {
    *(HWND *)hWndPtr = hWnd;
    return FALSE;
  }
  return TRUE;
}

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // data
  HANDLE    hProc;
  HANDLE    hThread;
  PBYTE     pData;
  PBYTE     pCode;
  InjData   injData;
  StageData stageData;
  DWORD     pID;
  HWND      hWnd;

  // prepare data to be injected
  injData.fnSendMessage = &SendMessageA;
  stageData.width       = 10;
  stageData.height      = 10;

  // get pID and handle to window named 'patient'
  ASSERT( !EnumWindows( &FindPatient, (LPARAM)&hWnd ), "patient not found =(" );
  GetWindowThreadProcessId( hWnd, &pID );

  // Step 1: Get full access to remote process
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

  return 0;
}
