#include <windows.h>
#include "opengl.h"

bool wallhack;
bool keydown = 0;

/*!
*	If we are drawing an entity, size = 2, type = GL_FLOAT, and stride = 0. All other cases we are
*	drawing a static object.
*/
void APIENTRY att_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{

	// Allow us to toggle with Numpad 1.
	if(GetAsyncKeyState(VK_NUMPAD1) && keydown == 0)
	{
		wallhack = !wallhack;
		keydown = 1;
	}

	else if(!GetAsyncKeyState(VK_NUMPAD1))
		keydown = 0;

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