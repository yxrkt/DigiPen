#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <sstream>

#include "common.h"

#define DATA_CHUNK_SIZE 1024
#define CODE_CHUNK_SIZE 1024

typedef LRESULT ( WINAPI *SENDMESSAGEFN )( HWND, UINT, WPARAM, LPARAM );
typedef WINBASEAPI HANDLE ( WINAPI *OPENPROCESSFN )( DWORD, BOOL, DWORD );
typedef int ( WINAPI *MESSAGEBOXFN )( HWND, LPCSTR, LPCSTR, UINT );

struct InjData
{
  SENDMESSAGEFN       fnSendMessage;
  OPENPROCESSFN       fnOpenProcess;
  MESSAGEBOXFN        fnMessageBox;

  HWND                hWnd;
  DWORD               docProcId;
  StageData           stageData;
  StageData          *pDocStageData;
  char                msgBoxStr[50];
};

static DWORD ThreadFunc( InjData *pData )
{
  if ( pData->fnOpenProcess( PROCESS_ALL_ACCESS, FALSE, pData->docProcId ) == NULL )
  {
    pData->fnMessageBox( NULL, pData->msgBoxStr, NULL, MB_ICONERROR );
    return -1;
  }

  pData->fnSendMessage( pData->hWnd, WM_APP, (WPARAM)DATA_PTR, (LPARAM)&pData->stageData );
  //pData->fnSendMessage( pData->hWnd, WM_APP + 1, (WPARAM)DATA_PTR, (LPARAM)pData->pDocStageData );

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
  DWORD     pID;
  StageData *pStage = new StageData;

  // for test 2
  pStage->width   = 10;
  pStage->height  = 10;
  pStage->changed = false;

  // prepare data to be injected
  injData.fnSendMessage     = &SendMessageA;
  injData.fnOpenProcess     = &OpenProcess;
  injData.fnMessageBox      = &MessageBoxA;
  injData.stageData.width   = 10;
  injData.stageData.height  = 10;
  injData.pDocStageData     = pStage;
  injData.docProcId         = GetProcessId( GetCurrentProcess() );
  strcpy( injData.msgBoxStr, "Error gaining access to doctor proc" );

  // get pID and handle to window named 'patient'
  ASSERT( !EnumWindows( &FindPatient, (LPARAM)&injData.hWnd ), "patient not found =(" );
  GetWindowThreadProcessId( injData.hWnd, &pID );

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
  ASSERT( hThread != NULL, "creating thread failed =(" );

  int pInt = 666;
  ASSERT( WriteProcessMemory( hProc, pData + 4 + 4 + 4 + 4 + 4, &pInt, sizeof( int ), NULL ), "666 FAILED" );
  //reinterpret_cast<InjData *>( pData )->stageData.width = 666;
  //SendMessage( injData.hWnd, WM_APP, (WPARAM)DATA_PTR, (LPARAM)&injData.stageData );
  //SendMessage( injData.hWnd, WM_APP + 1, (WPARAM)DATA_PTR, (LPARAM)injData.pDocStageData );

  while ( !pStage->changed ) ;

  std::stringstream ssStage;
  ssStage << "width = " << pStage->width << ", height = " << pStage->height;
  MessageBox( NULL, ssStage.str().c_str(), "New Stage Shiz", MB_ICONINFORMATION );

  return 0;
}
