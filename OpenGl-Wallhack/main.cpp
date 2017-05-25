/*!
*	A wallhack that uses the glTexCoordPointer method to achieve great results without logging counts.
*
*	Originally written 2007/12/23 by attilathedud.
*/
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <detours.h>
#include "opengl.h"

// On Dll attached, detour glTexCoordPointer to our custom function and store the original pointer.
void attach_Hook()
{
	DetourFunction((unsigned char*)glTexCoordPointer, (unsigned char*)att_glTexCoordPointer);
	__asm{ mov [glTexCoordPointer],eax }
}

// On Dll detached, remove the detour.
inline void detach_Hook()
{
	DetourRemove((unsigned char*)glTexCoordPointer, (unsigned char*)att_glTexCoordPointer);
}

bool APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		// While we are in the process of attaching our hook, make sure no processes are 
		// attempting to access external references to the old endpoint.
		DisableThreadLibraryCalls((HINSTANCE)hInstance);
		attach_Hook();
	}	
	else if(reason == DLL_PROCESS_DETACH)
		detach_Hook();
	else
	{
		MessageBox(0, "Could not attach", "dedicus", 0);
		return false;
	}

	return true;
}