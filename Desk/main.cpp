/*!
*	A multihack for CoD1 that features a wallhack, chams, behind-wall detection, night-mode, no fog, and 
*	white-walls. Undetected by PB from 2007/08/22 to ~2009.
*
*	Originally written 2007/08/22 by attilathedud.
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include "detours/detours.h"
#include "opengl.h"

FARPROC (WINAPI *ori_GetProcAddress)(HMODULE hModule, LPCSTR lpProcName);

/*!
*	When CoD looks for the address of glTextCoordPointer and glDrawElements, send it our detoured
*	functions instead of the original ones. 
*/
FARPROC __stdcall det_GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if(HIWORD(lpProcName))
	{
		if(!strcmp(lpProcName, "GetProcAddress"))				return (FARPROC)det_GetProcAddress;
		else if(!strcmp(lpProcName, "glTexCoordPointer"))		return (FARPROC)det_glTexCoordPointer;
		else if(!strcmp(lpProcName, "glDrawElements"))			return (FARPROC)det_glDrawElements;
	}
	return ori_GetProcAddress(hModule, lpProcName);
}

/*!
*	On attach, detour GetProcAddress and store the original address in ori_GetProcAddress.
*/
bool WINAPI DllMain(HINSTANCE hInstance, DWORD cReason, LPVOID lpReserved)
{
	if(cReason == DLL_PROCESS_ATTACH)
	{
		DetourFunction((unsigned char*)GetProcAddress, (unsigned char*)det_GetProcAddress);
		__asm mov [ori_GetProcAddress],eax
	}

	else
		return false;

	return true;
}