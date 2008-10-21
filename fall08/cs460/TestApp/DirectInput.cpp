/////////////////////////////////////////////////
//
// File: "DirectInput.cpp"
//
// Author: Jason Jurecka
//
// Creation Date: 02/18/03
//
// Purpose: The DirectInput class
/////////////////////////////////////////////////

#include "DirectInput.h"

DirectInput* DirectInput::pInstance = 0;


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
void DirectInput::InitDirectInput(HINSTANCE inst, HWND hwnd)
{
	m_hInstance = inst;
	m_hwnd = hwnd;

	//create the DI object
	if(FAILED(DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
					(void **) &m_diObj, NULL)))
	{
		MSBOX("Failed to Create Direct Input Object.");
		return;
	}
}


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
void DirectInput::CreateMouseDevice(DWORD dwFlags)
{
	//create device
	if(FAILED(m_diObj->CreateDevice(GUID_SysMouse, &m_diMouse, NULL)))
	{
		MSBOX("Failed to Create DI Mouse Device.");
		return;
	}

	//set data format for the Mouse
	if(FAILED(m_diMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MSBOX("Failed to set Data fromat for DI Mouse device.");
		return;
	}

	//set coop level
	if(FAILED(m_diMouse->SetCooperativeLevel(m_hwnd, dwFlags)))
	{
		MSBOX("Failed to set coop level for DI mouse device.");
		return;
	}

	//Acquire the device
	if(FAILED(m_diMouse->Acquire()))
	{
		MSBOX("Failed to Acquire Mouse Device");
		return;
	}
}

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
void DirectInput::CreateKeyboardDevice(DWORD dwFlags)
{
	//create device
	if(FAILED(m_diObj->CreateDevice(GUID_SysKeyboard, &m_diKeyboard, NULL)))
	{
		MSBOX("Failed to Create DI keyboard Device.");
		return;
	}

	//set data format for the keyboard
	if(FAILED(m_diKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MSBOX("Failed to set Data fromat for DI keyboard device.");
		return;
	}

	//set coop level
	if(FAILED(m_diKeyboard->SetCooperativeLevel(m_hwnd, dwFlags)))
	{
		MSBOX("Failed to set coop level for DI keyboard device.");
		return;
	}

	//acquire device
	if(FAILED(m_diKeyboard->Acquire()))
	{
		MSBOX("Failed to Acquire keyboard Device");
		return;
	}
}

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
void DirectInput::ShutdownDirectInput(void)
{
	//if there is a mouse
	if(m_diMouse)
	{
		//Smack it down
		m_diMouse->Unacquire();
		SAFE_RELEASE(m_diMouse);
	}

	//if there is a keyboard
	if(m_diKeyboard)
	{
		//Bash it to hell
		m_diKeyboard->Unacquire();
		SAFE_RELEASE(m_diKeyboard);
	}
	
	//Release the object into the wild
	SAFE_RELEASE(m_diObj);
	// you are free now object
}

////////////////////////////////////////////////
//
// Function: DIKeyboardHandler
//
// Last Modified: 02/18/03
//
// Input: dkcode	the key you want to check
//
// Output: reacts to keyboard state
//
// Returns: bool whether to continue or not
//
// Purpose: Check state and react correctly
////////////////////////////////////////////////
bool DirectInput::DIKeyboardHandler(DWORD dkcode)
{
	HRESULT hr;
	//Clear out the keyboard array
	memset(&m_keyboard, 0, sizeof(m_keyboard));
	//Get the scoop from the keyboard
	if(FAILED(hr = m_diKeyboard->GetDeviceState(sizeof(m_keyboard),(LPVOID) &m_keyboard)))
	{
		//make sure we dont lose the device
		//if so reacquiter the device
		if(hr == DIERR_INPUTLOST)
			m_diKeyboard->Acquire();
	}

	//If the key is pressed
	if(m_keyboard[dkcode] & 0x80)
		return true;

	return false;
}

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
POINT DirectInput::DIMouseHandler(void)
{
	POINT p;

	HRESULT hr;
	//Clear out the mouse structure
	memset(&m_diMouseState, 0, sizeof(m_diMouseState));
	//Get the scoop from the mouse
	if(FAILED(hr = m_diMouse->GetDeviceState(sizeof(m_diMouseState),(LPVOID) &m_diMouseState)))
	{
		//make sure we dont lose the device
		//if so reacquiter the device
		if(hr == DIERR_INPUTLOST)
			m_diMouse->Acquire();
	}

	p.x = m_diMouseState.lX;
	p.y = m_diMouseState.lY;

	return p;
}



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
bool DirectInput::DIMouseButtonHandler(int button)
{
	if(m_diMouseState.rgbButtons[button] & 0x80)
		return true;

	return false;
}