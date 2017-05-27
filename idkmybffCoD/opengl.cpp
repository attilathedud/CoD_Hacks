/*!
*	Contains all our OpenGL hooked functions along with several helper functions to render text and ui.
*
*	Also contains all the state variables for our hack. Should be abstracted to a class.
*/

#include <windows.h>
#include "opengl.h"

// Toggles
bool menu = 0, menuDrawn = 0, keydown = 0, text = 0, wallhack = 0, scope = 0, scopeDrawn = 0, fog = 0, chamsDrawn = 0;

// Menu location
float y1 = 100,y2 = 120, MAX_INDEX = 200, MIN_INDEX = 100;

// Current location of pointer, chams value, and wallcolor value.
BYTE index = 0, chams = 0, wallcolor = 0;

// Device context of CoD in order to render text.
HDC hDc = 0;

/*!
*	A helper function to render text to the screen. Uses raster positions to place the text over the 
*	world along with bitmap fonts to render the text.
*/
void drawText(char* text, float x, float y, float R, float G, float B, float A)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glColor4f(R,G,B,A);
	glRasterPos2f(x, y);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

/*!
*	Helper function that renders a box of a given color.
*/
void drawBox(float x1, float y1, float x2, float y2, float R, float G, float B, float A)
{
	glColor4f(R,G,B,A);
	glBegin(GL_QUADS);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();
}

/*! 
*	Helper function that renders a line of a given color and width.
*/
void drawLine(float x1, float y1, float x2, float y2, float width, float R, float G, float B, float A)
{
	glColor4f(R,G,B,A);
	glLineWidth(width);

	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}
	
/*!
*	Helper function that abstracts away code for the menu display. The menu is made up of a title
*	bar box, a line, and a main box that contains all the text and options. Also draws the box
*	representing our current selection.
*/
void drawMenu()
{
	drawBox(100, y1, 300, y2,0, 0, 1, .5f);
	drawBox(100,80, 300, 100, .2f, 0, 0.2f, .5f);
	drawBox(100,100,300,200,0,0,0,.5f);
	drawLine(100,100,300,100,1,1,1,1,1);
	drawText("idkmybffCoD :: ^attilathedud^", 100, 93, 1, 1, 0, .5f);
	drawText("Wallhack", 100, 113, 1, 1, 0, .5f);		
	if(wallhack)
		drawText("On", 275, 113, 0, 1, 0, .5f);
	else
		drawText("Off", 275, 113, 1, 0, 0, .5f);
	drawText("Chams", 100, 133, 1, 1, 0, .5f);
	switch(chams)
	{
		case 0:		drawText("Off", 275, 133, 1, 0, 0, .5f);		break;
		case 1:		drawText("All*", 275, 133, 1, 0, 0, .5f);		break;
		case 2:		drawText("All*", 275, 133, 0, 1, 0, .5f);		break;
		case 3:		drawText("All*", 275, 133, 0, 0, 1,	.5f);		break;
		case 4:		drawText("All*", 275, 133, 1, 0, 1, .5f);		break;
	}
	drawText("Full Bright", 100, 153, 1, 1, 0, .5f);
	switch(wallcolor)
	{
		case 0:		drawText("Off", 275, 153, 1, 0, 0, .5f);		break;
		case 1:		drawText("Black", 260, 153, .1f, .1f, .1f, .5f);		break;
		case 2:		drawText("White", 260, 153, 1, 1, 1, .5f);		break;
	}
	drawText("No Scope", 100, 173, 1, 1, 0, .5f);
	if(scope)
		drawText("On", 275, 173, 0, 1, 0, .5f);
	else
		drawText("Off", 275, 173, 1, 0, 0, .5f);
	drawText("No Fog", 100, 193, 1, 1, 0, .5f);
	if(fog)
		drawText("On", 275, 193, 0, 1, 0, .5f);
	else
		drawText("Off", 275, 193, 1, 0, 0, .5f);
}

/*!
*	The glDrawElements hook is responsible for rendering the menu, initializing text, no-scope, 
*	wallhack, full-bright, and chams.
*/
void APIENTRY att_glDrawElements(GLenum mode,  GLsizei count,  GLenum type,  const GLvoid *indices)
{
	if(menu)
	{
		if(!menuDrawn)
		{
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
			glLoadIdentity();
			drawMenu();
			glPopMatrix();
			glEnable(GL_TEXTURE_2D);
			menuDrawn = 1;
		}
	}

	// If our text isn't initialized, get the current context, create a bitmap font and store
	// it in a list.
	if(!text)
	{
		hDc = wglGetCurrentDC();
		wglUseFontBitmaps(hDc, 0, 256, 1000);
		glListBase(1000);
		text = 1;
	}

	// No-scope works by drawing a 0x0 box with no alpha and distrupting the scope blur rendering.
	if(scope)
	{
		if(!scopeDrawn)						
		{
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
			glLoadIdentity();
			drawBox(0,0,0,0,1,1,1,0);
			glPopMatrix();
			glEnable(GL_TEXTURE_2D);
			scopeDrawn = 1;
		}
	}

	// Wallhack works by setting the depth to GL_ALWAYS before rendering the entity
	if(wallhack)
	{
		if( mode == GL_TRIANGLES && indices != NULL)
		{
			glDepthFunc(GL_ALWAYS);
			glDrawElements(mode, count, type, indices);
			glDepthFunc(GL_LEQUAL);
		}
	}

	// Full-bright works by bluring the texture mode on environmental pieces into a solid color.
	if(wallcolor)
	{
			if(wallcolor == 1)
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND); 
			}
			else if(wallcolor == 2)
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			}
	}
	else if(!wallcolor)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			
	// Chams works by modifying the color before rendering the entity.
	if(!chams && !chamsDrawn)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(.6f,.6f,.6f);
		glDrawElements(mode, count, type, indices);
		glDisable(GL_COLOR_MATERIAL);
		chamsDrawn = 1;
	}
	else if(chams > 0 && indices != NULL && mode != GL_QUADS && !chamsDrawn)
	{
		glEnable(GL_COLOR_MATERIAL);
		switch(chams)
		{
			case 1:   glColor3f(.6f,.2f,.2f);	break;
			case 2:	  glColor3f(.2f,.6f,.2f);	break;
			case 3:	  glColor3f(.2f,.2f,.6f);	break;
			case 4:	  glColor3f(.6f,.2f,.6f);	break;
		}
		glDrawElements(mode, count, type, indices);
		glDisable(GL_COLOR_MATERIAL);
		chamsDrawn = 1;
	}

	glDrawElements(mode, count, type, indices);
}

/*!
*	The glViewport hook is responsible for dealing with input and menu navigation via the numpad.
*/
void APIENTRY att_glViewport(GLint x,  GLint y,  GLsizei width,  GLsizei height)
{
	if(GetAsyncKeyState(VK_NUMPAD0) && keydown == 0)
	{
		menu = !menu;
		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD2) && menu == true && keydown == 0)
	{
		y1 += 20; y2 += 20; index++;
		if(y2 > MAX_INDEX)
		{
			y1 = MIN_INDEX;
			y2 = MIN_INDEX+20;
			index = 0;
		}

		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD8) && menu == true && keydown == 0)
	{
		y1 -= 20; y2 -= 20; index--;
		if(y1 < MIN_INDEX)
		{
			y1 = MAX_INDEX-20;
			y2 = MAX_INDEX;
			index = 4;
		}

		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD6) && menu == true && keydown == 0)
	{
		if(index == 0)
			wallhack = !wallhack;
		else if(index == 1)
		{
			if(chams < 4)
				chams++;
			else
				chams = 0;
			chamsDrawn = false;
		}
		else if(index == 2)
		{
			if(wallcolor < 2)
				wallcolor++;
			else
				wallcolor = 0;
		}
		else if(index == 3)
			scope = !scope;
		else if(index == 4)
			fog = !fog;
		keydown = 1;
	}
	else if(GetAsyncKeyState(VK_NUMPAD4) && menu == true && keydown == 0)
	{
		if(index == 0)
			wallhack = !wallhack;
		else if(index == 1)
		{
			if(chams > 0)
				chams--;
			else
				chams = 4;
			chamsDrawn = false;
		}
		else if(index == 2)
		{
			if(wallcolor > 0)
				wallcolor--;
			else
				wallcolor = 2;
		}
		else if(index == 3)
			scope = !scope;
		else if(index == 4)
			fog = !fog;
		keydown = 1;
	}
	else if(!(GetAsyncKeyState(VK_NUMPAD0) || GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState(VK_NUMPAD8) || GetAsyncKeyState(VK_NUMPAD6) || GetAsyncKeyState(VK_NUMPAD4)))
		keydown = 0;

	menuDrawn = false;
	scopeDrawn = false;

	glViewport(x, y, width, height);
}

/*!
*	glTexCoordPointer hook is responsible for disabling fog.
*/
void APIENTRY att_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	if(fog)
		glDisable(GL_FOG);

	glTexCoordPointer(size, type, stride, pointer);
}
