/*!
*   A utility to log the counts of displayed entities to help isolate specific counts of player models.
*
*   Originally written 2017/07/03 by attilathedud.
*/
#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#include <gl/gl.h>
#include <stdio.h>
#include <detours.h>

FILE *countfile;
char path[256]; 

GLsizei prevCount[620] = {0};	
bool countEqual = 0; 
int currentCounts = 0;

void __stdcall att_glDrawElements(GLenum mode,  GLsizei count,  GLenum type,  const GLvoid *indices);

// Create a file to log our counts and then hook glDrawElements.
bool __stdcall DllMain(HANDLE hInstance, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		GetModuleFileName(0, path, sizeof(path));
		for(int i = strlen(path); path[i] != '\\' && i; i--)
			path[i] = 0;
		strcat(path, "COUNTS.TXT");
		countfile = fopen(path, "w");
		DetourFunction((unsigned char*)glDrawElements, (unsigned char*)att_glDrawElements);
		__asm mov [glDrawElements],eax
	}
	else if(reason == DLL_PROCESS_DETACH)
	{
		DetourRemove((unsigned char*)glDrawElements, (unsigned char*)att_glDrawElements);
		fclose(countfile);
	}
	else
		return 0;

	return 1;
}

// Our hook checks that we are drawing a dynamic entity and then logs the amount of indices rendered.
void __stdcall att_glDrawElements(GLenum mode,  GLsizei count,  GLenum type,  const GLvoid *indices)
{
	if(mode == GL_TRIANGLES && indices != NULL && count > 50)
	{
        countEqual = 0;
        
		for(int i = 0; i < currentCounts; i++)
		{
			if(prevCount[i] == count)
				countEqual = 1;
        }
        
		if(!countEqual)
		{
			fprintf(countfile, "%d, ", count);
			prevCount[++currentCounts] = count;
		}
	}

	glDrawElements(mode, count, type, indices);
}
		
