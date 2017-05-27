/*!
*	A multihack for CoD1 that features a wallhack, chams, full-bright, no-scope, no-fog and internal ui.
*
*	Originally written 2008/02/23 by attilathedud.
*/

#include <windows.h>
#include <detours.h>
#include "opengl.h"

/*!
*	The LoadLibrary hooks is responsible for detecting when OpenGL is loaded and detouring 
*	glDrawElements, glViewport, and glTexCoordPointer.
*/
HMODULE (WINAPI *orig_LoadLibrary) ( LPCSTR lpFileName );
HMODULE WINAPI LoadLibrary_Hook ( LPCSTR lpFileName ) 
{
	HMODULE hM = orig_LoadLibrary( lpFileName );
	if ( strstr( lpFileName, "opengl32.dll" ) ) 
	{
		DetourFunction((unsigned char*)GetProcAddress(hM, "glDrawElements"), (unsigned char*)att_glDrawElements);
		DetourFunction((unsigned char*)GetProcAddress(hM, "glViewport"), (unsigned char*)att_glViewport);
		DetourFunction((unsigned char*)GetProcAddress(hM, "glTexCoordPointer"), (unsigned char*)att_glTexCoordPointer);
	}

	return orig_LoadLibrary( lpFileName );
}

// On attach, detour LoadLibrary to a custom function so that we can hook the needed gl functions. 
// Store the original address in orig_LoadLibrary.
void attach_Hook()
{
	DetourFunction((LPBYTE) LoadLibraryA, (LPBYTE) LoadLibrary_Hook );
	__asm mov [ orig_LoadLibrary ], eax;
}

bool APIENTRY DllMain(DWORD hInstance, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		// While we are in the process of attaching our hook, make sure no processes are 
		// attempting to access external references to the old endpoint.
		DisableThreadLibraryCalls((HINSTANCE)hInstance);
		attach_Hook();
	}	
	else if(reason == DLL_PROCESS_DETACH)
		// On detach, delete the lists allocated to hold our font.
		glDeleteLists(1000, 256);
	else
	{
		MessageBox(0, "Could not attach", "idkmybffCoD", 0);
		return false;
	}

	return true;
}