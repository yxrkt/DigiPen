/////////////////////////////////////////////////
//
// File: "DirectInput.h"
//
// Author: Jason Jurecka
//
// Creation Date: 02/18/03
//
// Purpose: The DirectInput class
/////////////////////////////////////////////////

#ifndef _DIRECTINPUT_H_
#define _DIRECTINPUT_H_

#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#define KEYS 256

#ifndef MSBOX
#define MSBOX(a)	MessageBox(m_hwnd, a, "ERROR", MB_OK);
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	if(p) {p->Release(); p=NULL;}
#endif

class DirectInput  
{
private:
	//to make it singleton
	static DirectInput* pInstance;

	HWND            m_hwnd;		//	The Main Window Handle.
	HINSTANCE		m_hInstance;
	//Direct input vars		
	LPDIRECTINPUT8			m_diObj;
	LPDIRECTINPUTDEVICE8	m_diKeyboard;
	LPDIRECTINPUTDEVICE8	m_diMouse;

	char					m_keyboard[KEYS];
	DIMOUSESTATE2			m_diMouseState;

	int m_nX, m_nY;	//mouse position variables

	////////////////////////////////////////////////
	//
	// Function: DirectInput
	//
	// Last Modified: 02/18/03
	//
	// Input: none
	//
	// Output: none
	//
	// Returns: none
	//
	// Purpose: Constructor 
	////////////////////////////////////////////////
	DirectInput()
	{
		//Set up all the vars with default values
		m_diObj = 0;
		m_diKeyboard =0;
		m_diMouse =0;
		memset(&m_keyboard, 0, sizeof(m_keyboard));
		m_nX = 0;
		m_nY = 0;
		m_hwnd = NULL;
		m_hInstance = NULL;
	}

	////////////////////////////////////////////////
	//
	// Function: ~DirectInput
	//
	// Last Modified: 02/18/03
	//
	// Input: none
	//
	// Output: none
	//
	// Returns: none
	//
	// Purpose: Destructor 
	////////////////////////////////////////////////
	virtual ~DirectInput(){}

	////////////////////////////////////////////////
	//
	// Function: DirectInput
	//
	// Last Modified: 02/18/03
	//
	// Input: di    The guy to mimick.	
	//
	// Output: none
	//
	// Returns: none
	//
	// Purpose:	Copy Constructor
	////////////////////////////////////////////////
	DirectInput(const DirectInput &di){}

	////////////////////////////////////////////////
	//
	// Function: =
	//
	// Last Modified: 02/18/03
	//
	// Input: di    The thing to be like.	
	//
	// Output: none
	//
	// Returns: none
	//
	// Purpose:	Assigment operator
	//////////////////////////////////////////////// 
	DirectInput & operator=(const DirectInput &di){ return *this;}	

public:

	////////////////////////////////////////////////
	//
	// Function:GetInstance
	//
	// Last Modified: 02/18/03
	//
	// Input:	none
	//
	// Output:	none
	//
	// Returns:	DirectInput*	pointer to the class variable
	//
	// Purpose:	To make the class a singleton
	////////////////////////////////////////////////
	static DirectInput* GetInstance()
	{
		//If no instance exists
		if(pInstance == 0)
		{	//Make one!!!!!
			pInstance = new DirectInput;
		}
		
		//return where it came from
		return pInstance;
	}

	////////////////////////////////////////////////
	//
	// Function:DeleteInstance
	//
	// Last Modified: 02/10/03
	//
	// Input:	none
	//
	// Output:	none
	//
	// Returns:	void
	//
	// Purpose:	Delete the instance of the class
	////////////////////////////////////////////////
	static void DeleteInstance(void)
	{
		//if there is an instance
		if(pInstance)
		{
			//Delete it and set the pointer to NULL
			delete pInstance;
			pInstance = NULL;
		}
	}

	////////////////////////////////////////////////
	//
	// Function: InitDirectInput
	//
	// Last Modified: 02/18/03
	//
	// Input: inst	the instace of the app
	//		  hwnd	the handle to the window
	//
	// Output: none
	//
	// Returns: void
	//
	// Purpose: Set up Direct input
	////////////////////////////////////////////////
	void InitDirectInput(HINSTANCE inst, HWND hwnd);

	////////////////////////////////////////////////
	//
	// Function: ShutdownDirectInput
	//
	// Last Modified: 02/18/03
	//
	// Input: void
	//
	// Output: none
	//
	// Returns: void
	//
	// Purpose: Clean up all the dynamic memory and objects
	////////////////////////////////////////////////
	void ShutdownDirectInput(void);

	////////////////////////////////////////////////
	//
	// Function: CreateMouseDevice
	//
	// Last Modified: 02/18/03
	//
	// Input: dwFlags	the flags to use when creating
	//
	// Output: none
	//
	// Returns: void
	//
	// Purpose: Create a Mouse device to get input
	////////////////////////////////////////////////
	void CreateMouseDevice(DWORD dwFlags);

	////////////////////////////////////////////////
	//
	// Function: CreateKeyboardDevice
	//
	// Last Modified: 02/18/03
	//
	// Input: dwFlags	the flags to use when creating
	//
	// Output: none
	//
	// Returns: void
	//
	// Purpose: Create a Keyboard device to get input
	////////////////////////////////////////////////
	void CreateKeyboardDevice(DWORD dwFlags);

	////////////////////////////////////////////////
	//
	// Function: DIKeyboardHandler
	//
	// Last Modified: 02/18/03
	//
	// Input: dkcode		the key you want to check
	//
	// Output: reacts to keyboard state
	//
	// Returns: bool whether to continue or not
	//
	// Purpose: Check state and react correctly
	////////////////////////////////////////////////
	bool DIKeyboardHandler(DWORD dkcode);

	////////////////////////////////////////////////
	//
	// Function: DIMouseHandler
	//
	// Last Modified: 02/18/03
	//
	// Input: void
	//
	// Output: reacts to Mouse state
	//
	// Returns: point	the x and y coordinates
	//
	// Purpose: Check state and react correctly
	////////////////////////////////////////////////
	POINT DIMouseHandler(void);

	////////////////////////////////////////////////
	//
	// Function: DIMouseHandler
	//
	// Last Modified: 02/18/03
	//
	// Input: button the button to check
	//
	// Output: reacts to Mouse state
	//
	// Returns: bool return true if button was pressed
	//
	// Purpose: Check state and react correctly
	////////////////////////////////////////////////
	bool DIMouseButtonHandler(int button);
};

#endif 