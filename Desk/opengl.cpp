#include <windows.h>
#include "opengl.h"

// Current state of enabled features
bool fog = 0;
BYTE wallhack = 0;
bool keydown = 0;
BYTE chams = 0;
bool night = 0;
bool whitewalls = 0;

/*!
*	We hook glTexCoordPointer to deal with hotkeys, no fog, night-mode, white-walls, and entity wallhack.
*
*	No fog: CoD uses GL_FOG to display fog. glDisable( GL_FOG ) disables the engine's fog.
*	Night-mode: By blending all textures together, textures are rendered as black on display.
*	White-walls: By setting the texture mode to GL_DECAL, texture colors are ignored and displayed as white.
*	Wallhack: Setting the depth function to GL_ALWAYS displays all textures on the same level.
*/
void APIENTRY det_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	// Checking for hotkey presses.
	if(GetAsyncKeyState(VK_NUMPAD1) && keydown == 0)
	{
		wallhack++;
		if(wallhack > 2)		wallhack = 0;
		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD3) && keydown == 0)
	{
		if(fog == 0)       fog = 1;
		else			   fog = 0;
		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD2) && keydown == 0)
	{
		chams++;
		if(chams > 5)	   chams = 0;
		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD4) && keydown == 0)
	{
		if(night == 0)		night = 1;
		else				night = 0;
		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD5) && keydown == 0)
	{
		if(whitewalls == 0)		whitewalls = 1;
		else					whitewalls = 0;
		keydown = 1;
	}
	else if(!GetAsyncKeyState(VK_NUMPAD3) && (!GetAsyncKeyState(VK_NUMPAD2)) && (!GetAsyncKeyState(VK_NUMPAD1)) && (!GetAsyncKeyState(VK_NUMPAD4)) && (!GetAsyncKeyState(VK_NUMPAD5)))
		keydown = 0;

	// No fog
	if(fog)
		glDisable(GL_FOG);

	// Night-mode
	if(night)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND); 
	else if(!night || !whitewalls)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// White-walls
	if(whitewalls)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Wallhack
	if(wallhack == 1)
	{
		glDepthFunc(GL_LEQUAL);
		if(size == 2)
		{
			if(type == GL_FLOAT)
			{
				if(stride == 0)
				{
					glDepthFunc(GL_ALWAYS);
				}
			}
		}
	}
	glTexCoordPointer(size, type, stride, pointer);
}

/*!
*	We hook glDrawElements to deal with player wallhacks and chams.
*
*	Wallhack: We use a list of counts to detect player models. On detection, disable the depth
*	test and set the color to full to enable behind-wall detection.
*	Chams: On indices presence, set the color mode to our desired color.
*/
void APIENTRY det_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	glDrawElements(mode, count, type, indices);

	// Wallhack and behind-wall detection
	if(wallhack == 2)
	{
		// The count list is originally from wurzil
	   if(count == 6702 || count == 4482 || count == 3078 || count == 1902 ||		//germans
       count == 8892 || count == 6672 || count == 2928 || count == 1908 ||
       count == 6186 || count == 3966 || count == 2562 || count == 1386 ||
       count == 3306 || count == 1182 || count == 8502 || count == 1572 ||
       count == 4914 || count == 2694 || count == 1110 || count == 642  ||
       count == 3306 || count == 1182 || count == 3078 || count == 6606 ||
       count == 4158 || count == 1578 || count == 4158 || count == 6378 ||
       count == 2754 || count == 1650 || count == 1080 || count == 8892 ||
       count == 8310 || count == 6090 || count == 2562 || count == 2928 ||
       count == 5298 || count == 1644 || count == 510  || count == 1080 ||
       count == 8310 || count == 6090 || count == 2562 || count == 1458 ||
       count == 7932 || count == 5712 || count == 2184 || count == 6606 ||
       count == 3078 || count == 1974 || count == 6282 || count == 1650 ||
       count == 3588 || count == 1644 || count == 1008 || count == 3114 ||
       count == 990  || count == 1572 || count == 4722 || count == 2502 ||
       count == 918  || count == 450  || count == 2790 || count == 570  ||
       count == 8700 || count == 6480 || count == 2736 || count == 1716 ||           //russians
	   count == 8352 || count == 6132 || count == 2310 || count == 522  ||
       count == 8340 || count == 6120 || count == 1986 || count == 972  ||
       count == 3996 || count == 1458 || count == 888  || count == 678  ||           //brits
	   count == 5268 || count == 1590 || count == 486  || count == 5100 ||
       count == 1572 || count == 7320 || count == 5100 || count == 1572 ||
       count == 522  || count == 7488 || count == 5268 || count == 1590 || 
       count == 486  || count == 7062 || count == 4842 || count == 432  ||			 //americans
	   count == 516  || count == 2640 || count == 852  || count == 426  ||
       count == 2820 || count == 1032 || count == 606  || count == 426  ||
       count == 6642 || count == 4422 || count == 2028 || count == 1482 ||
       count == 6552 || count == 4332 || count == 1938 || count == 1392 ||
       count == 2442 || count == 852  || count == 6498 || count == 3594 ||
       count == 5412 || count == 3192 || count == 798  || count == 3030 ||
       count == 1440 || count == 1014 || count == 3690 || count == 3504 ||
       count == 1716 || count == 1290 || count == 6696 || count == 4476 ||
       count == 2082 || count == 1536 || count == 1296 || count == 708  ||
       count == 3006 || count == 1218 || count == 792  || count == 1884 ||
       count == 1338 || count == 1296 || count == 708  || count == 1098 ||
       count == 510  || count == 84   || count == 720  || count == 3138 ||
       count == 1350 || count == 924  || count == 3708 || count == 1110 ||
       count == 606  || count == 684  || count == 4074 || count == 1476 ||
       count == 972  || count == 606)
		{
		       if(count == 810  || count == 576  || count == 2682 || count == 2220 ||	//not players
			   count == 462  || count == 432  || count == 2124 || count == 1272 ||
			   count == 588  || count == 318  || count == 324  || count == 114  ||
			   count == 420  || count == 270  || count == 690  || count == 450  ||
			   count == 606  || count == 180  || count == 210  || count == 1380 ||
			   count == 348  || count == 336  || count == 366  || count == 216  ||
			   count == 354  || count == 966  || count == 168  || count == 234  ||
			   count == 560  || count == 948  || count == 7824 || count == 132  ||
			   count == 138  || count == 144  || count == 378  || count == 960  ||
			   count == 1104 || count == 360  || count == 1656 || count == 4134 ||
			   count == 4134 || count == 4134 || count == 3672 || count == 414  ||
			   count == 384  || count == 150  || count == 372  || count == 3672 ||
			   count == 4134 || count == 2109 || count == 5757)
			   {
				   glEnable(GL_DEPTH_TEST);
			   }

			   if(indices == NULL || mode == GL_QUADS)
				   glEnable(GL_DEPTH_TEST);

				glDisable(GL_DEPTH_TEST);
				glEnable(GL_COLOR_MATERIAL);
				glColor3f((float)(rand()%10), (float)(rand()%10), (float)(rand()%10));
				glDrawElements(mode, count, type, indices);
				glEnable(GL_DEPTH_TEST);
				glColor3f(1,1,1);
				glDisable(GL_COLOR_MATERIAL);

		}

	}

	// Chams
	if(chams > 0)
	{
		if(indices == NULL || mode == GL_QUADS)
		{
			glDisable(GL_COLOR_MATERIAL);
		}
		else
		{
			glEnable(GL_COLOR_MATERIAL);
			switch(chams)
			{
			case 1: glColor3f(.7f,.1f,.1f); break;
			case 2: glColor3f(.2f,.7f,.2f); break;
			case 3: glColor3f(.2f,.2f,.7f); break;
			case 4: glColor3f(.7f,.1f,.4f);	break;
			case 5: glColor3f(0,0,0);		break;
			}
			glDrawElements(mode, count, type, indices);
			glColor3f(1, 1, 1);
			glDisable(GL_COLOR_MATERIAL);
		}
	}
}
