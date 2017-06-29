/**
* Copyright (C) 2017 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#include "Settings\Settings.h"
#include "Dllmain\Dllmain.h"
#include "wrapper.h"
#include "dsound.h"
#include "Utils\Utils.h"

struct dinput_dll
{
	HMODULE dll = nullptr;
	FARPROC DirectInputCreateA = jmpaddr;
	FARPROC DirectInputCreateEx = jmpaddr;
	FARPROC DirectInputCreateW = jmpaddr;
	FARPROC DllCanUnloadNow = jmpaddr;
	FARPROC DllGetClassObject = jmpaddr;
	FARPROC DllRegisterServer = jmpaddr;
	FARPROC DllUnregisterServer = jmpaddr;
} dinput;

__declspec(naked) void  FakeDirectInputCreateA() { _asm { jmp[dinput.DirectInputCreateA] } }
__declspec(naked) void  FakeDirectInputCreateEx() { _asm { jmp[dinput.DirectInputCreateEx] } }
__declspec(naked) void  FakeDirectInputCreateW() { _asm { jmp[dinput.DirectInputCreateW] } }
//__declspec(naked) void  FakeDllCanUnloadNow() { _asm { jmp [dinput.DllCanUnloadNow] } }			 // <---  Shared with dsound.dll
//__declspec(naked) void  FakeDllGetClassObject() { _asm { jmp [dinput.DllGetClassObject] } }		 // <---  Shared with dsound.dll
__declspec(naked) void  FakeDllRegisterServer() { _asm { jmp[dinput.DllRegisterServer] } }
__declspec(naked) void  FakeDllUnregisterServer() { _asm { jmp[dinput.DllUnregisterServer] } }

void LoadDinput()
{
	// Load real dll
	dinput.dll = LoadDll(dtype.dinput);

	// Load dll functions
	if (dinput.dll)
	{
		dinput.DirectInputCreateA = GetFunctionAddress(dinput.dll, "DirectInputCreateA", jmpaddr);
		dinput.DirectInputCreateEx = GetFunctionAddress(dinput.dll, "DirectInputCreateEx", jmpaddr);
		dinput.DirectInputCreateW = GetFunctionAddress(dinput.dll, "DirectInputCreateW", jmpaddr);
		Set_DllCanUnloadNow(GetFunctionAddress(dinput.dll, "DllCanUnloadNow", jmpaddr));
		Set_DllGetClassObject(GetFunctionAddress(dinput.dll, "DllGetClassObject", jmpaddr));
		dinput.DllRegisterServer = GetFunctionAddress(dinput.dll, "DllRegisterServer", jmpaddr);
		dinput.DllUnregisterServer = GetFunctionAddress(dinput.dll, "DllUnregisterServer", jmpaddr);
	}
}