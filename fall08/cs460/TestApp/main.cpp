/////////////////////////////////////////////////
//
// File: "Main.cpp"
//
// Author: Jason Jurecka
//
// Creation Date: June 5, 2003
//
// Purpose: This is the main application for
//		viewing models
//
// Note: This was written for DirectX version 9
//
//	A lot of the code for the window was taken
//		from the directX SDK sample apps, but changed
//		to work with my model class
/////////////////////////////////////////////////
#pragma comment(lib, "d3d9")
//#pragma comment(lib, "d3dx9dt")
#include <afxwin.h>
#include <afxdlgs.h>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9math.h>
#include "CModel.h"
#include "Macros.h"
#include "DirectInput.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D           = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice     = NULL; // Our rendering device
D3DLIGHT9				g_dLight;				 // A light for the App
CModel*					g_pModel		 = NULL; // A model object to work with
DirectInput*			g_pDI			 = NULL; // DirectInput instance

#define controllist "Escape = Exit\nRight Arrow = Rotate model right\nLeft Arrow = Rotate model left\nUp Arrow = Move model forward\nDown Arrow = Move model backward\nA = Move model up\nD = Move model down\nW = Rotate model up\nS = Rotate model down\nN = Play next animation\nM = Play previous animation\nT = Toggle Wireframe mode\nR = Toggle Culling\nO = Open an X file\nC = Bring up Controls list"

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
                      "TestApp", NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( "TestApp", "X File Model Viewer - Press \'C\' to view the controls", 
                              WS_OVERLAPPEDWINDOW, 0, 0, 800, 600,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

	// Initializes MFC so it will work
	AfxWinInit(hInst, hPrevInstance, lpCmdLine, nCmdShow );
	
	//////////////////////////////////////////////////////////////////////////
    // Initialize Direct3D
	//////////////////////////////////////////////////////////////////////////
	// Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

	g_pDI = DirectInput::GetInstance();
	g_pDI->InitDirectInput(hInst,hWnd);
	g_pDI->CreateKeyboardDevice(DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	BOOL bCulling = TRUE;
	// Culling
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//No lighting
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//BOOL bSimpleShadow = FALSE;
	//Do the alpha blending
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
            
	BOOL bComplexShadow = FALSE;
    // Show the window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );

	D3DXMATRIXA16 matWorld, matYWorld, matXWorld, matTranslate, matUp, matView, matProj, matZWorld;

	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	//Movement variables
	static float fAngle = 0.0f;
	static float fForward = 0.0f;
	static float fUp = 0.0f;
	static float fAngle2 = 0.0f;
	static float fAngle3 = 0.0f;
	static bool bWireframe = false;

	//From the DirectX SDK window for setting up projection matrices
	//
	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, (800.0f/600.0f), 1.0f, 2500.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);	
	
	//Set up the time stuff
	double starttime;
	LARGE_INTEGER nowtime;
	LONGLONG ticks;

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	starttime = (double)time.QuadPart;

	QueryPerformanceFrequency(&time);
	ticks = time.QuadPart;

	// Enter the message loop
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			// Clear the backbuffer and the zbuffer
			g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 
								 D3DCOLOR_XRGB(125,125,125), 1.0f, 0 );
		
			static int iFrames = 0;
			static double sttime = 0.0;
			iFrames++;
			if (iFrames == 100) 
			{
				QueryPerformanceCounter(&nowtime);
				float fps = float(iFrames/((nowtime.QuadPart - sttime)/ticks));
				char cBuff[32];
				sprintf(cBuff,"X File Model Viewer - Press \'C\' to view the controls FPS: %.1f", fps);
				SetWindowText(hWnd,cBuff);
				sttime = (double)nowtime.QuadPart;
				iFrames = 0;
			}
			
			//////////////////////////////////////////////////////////////////////////
			// Setup the world and view
			//////////////////////////////////////////////////////////////////////////

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixIdentity(&matYWorld);
			D3DXMatrixIdentity(&matXWorld);
			D3DXMatrixIdentity(&matZWorld);
			D3DXMatrixIdentity(&matTranslate);
			D3DXMatrixIdentity(&matUp);
			
			//This makes the model rotate Y
  			D3DXMatrixRotationY( &matYWorld, D3DXToRadian(fAngle));
			//This makes the model rotate X
  			D3DXMatrixRotationX( &matXWorld, D3DXToRadian(fAngle2));
			//This makes the model rotate Z
			D3DXMatrixRotationZ( &matZWorld, D3DXToRadian(fAngle3));

			//This moves the stuff around
			D3DXMatrixTranslation(&matTranslate, 0.0f, fUp, fForward);
			
			matWorld = (matYWorld * matXWorld * matZWorld * matTranslate);

			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld);

			D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
			g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

			// Begin the scene
			if(SUCCEEDED(g_pd3dDevice->BeginScene()))
			{
				//////////////////////////////////////////////////////////////////////////
				// Draw the stuff
				//////////////////////////////////////////////////////////////////////////
				
				if(g_pModel)
				{
					//find the time difference
					QueryPerformanceCounter(&nowtime);
					double dtime = ((nowtime.QuadPart - starttime)/ticks);
										
					//Update the model
					g_pModel->Update(dtime);
					starttime = (double)nowtime.QuadPart;
					g_pModel->Draw();
				}

				// End the scene
				g_pd3dDevice->EndScene();
			}

			// Present the backbuffer contents to the display
			g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

			if(bWireframe)
				g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			else
				g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			
			if(bCulling)
				g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			else
				g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			//If we have focus handle the input stuff
			if(GetFocus())
			{
				//Toggle wireframe mode
				if(g_pDI->DIKeyboardHandler(DIK_T))
				{
					Sleep(200);
					bWireframe = !bWireframe;
				}

				if(g_pDI->DIKeyboardHandler(DIK_R))
				{
					Sleep(200);
					bCulling = !bCulling;
				}

				if(g_pDI->DIKeyboardHandler(DIK_ESCAPE))
					break;
				
				if (g_pDI->DIKeyboardHandler(DIK_RIGHT))
					fAngle -= 2.0f;
				if (g_pDI->DIKeyboardHandler(DIK_LEFT))
					fAngle += 2.0f;
				
				if (g_pDI->DIKeyboardHandler(DIK_UP))
					fForward += 1.0f;
				if (g_pDI->DIKeyboardHandler(DIK_DOWN))
					fForward -= 1.0f;
				
				if (g_pDI->DIKeyboardHandler(DIK_A))
					fUp += 0.5f;
				if (g_pDI->DIKeyboardHandler(DIK_D))
					fUp -= 0.5f;
				
				if (g_pDI->DIKeyboardHandler(DIK_W))
					fAngle2 -= 2.0f;
				if (g_pDI->DIKeyboardHandler(DIK_S))
					fAngle2 += 2.0f;

				if (g_pDI->DIKeyboardHandler(DIK_Q))
					fAngle3 -= 2.0f;
				if (g_pDI->DIKeyboardHandler(DIK_E))
					fAngle3 += 2.0f;	
				
				if(g_pModel)
				{
					if (g_pDI->DIKeyboardHandler(DIK_N))
					{
						Sleep(200);
						g_pModel->SetCurrentAnimation(g_pModel->GetCurrentAnimation()+1);
					}

					if (g_pDI->DIKeyboardHandler(DIK_M))
					{
						Sleep(200);
						g_pModel->SetCurrentAnimation(g_pModel->GetCurrentAnimation()-1);
					}					
				}

				//Show the controls list
				if (g_pDI->DIKeyboardHandler(DIK_C))
				{
					Sleep(200);
					MessageBox(hWnd, controllist, "Controls List", MB_OK);
				}

				//O is for open
				if(g_pDI->DIKeyboardHandler(DIK_O))
				{
					//OpenFile
					static char szFilter[] = "X files (*.x)|*.x||";
					
					//Display the Open dialog box.
					CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter);
					
					if(IDOK == dlg.DoModal())
					{
						if(g_pModel)
						{
							//Delete the old
							SAFE_DELETE(g_pModel)
							//make new
							g_pModel = new CModel(g_pd3dDevice);
							//Load the new
							g_pModel->LoadXFile((char*)LPCTSTR(dlg.GetPathName()));

							//Make sure we can see it
							float radius = g_pModel->GetBoundingSphereRadius();
							vEyePt = D3DXVECTOR3(0.0f, radius*2, -(radius*2));
							LPD3DXVECTOR3 ptemp = g_pModel->GetBoundingSphereCenter();
							vLookatPt = D3DXVECTOR3(ptemp->x, ptemp->y, ptemp->z);
							fAngle = 0.0f;
							fForward = 0.0f;
							fUp = 0.0f;
							fAngle2 = 0.0f;
						}
						else
						{
							//make new
							g_pModel = new CModel(g_pd3dDevice);
							//Load the new
							g_pModel->LoadXFile((char*)LPCTSTR(dlg.GetPathName()));

							//Make sure we can see it
							float radius = g_pModel->GetBoundingSphereRadius();
							vEyePt = D3DXVECTOR3(0.0f, radius*2, -(radius*2));
							LPD3DXVECTOR3 ptemp = g_pModel->GetBoundingSphereCenter();
							vLookatPt = D3DXVECTOR3(ptemp->x, ptemp->y, ptemp->z);
							fAngle = 0.0f;
							fForward = 0.0f;
							fUp = 0.0f;
							fAngle2 = 0.0f;
						}
					}
				}
			}
		}
    }
	
	SAFE_DELETE(g_pModel)
	SAFE_RELEASE(g_pd3dDevice)
	SAFE_RELEASE(g_pD3D)
	g_pDI->ShutdownDirectInput();
	g_pDI->DeleteInstance();
	
    UnregisterClass( "TestApp", wc.hInstance );
    return 0;
}