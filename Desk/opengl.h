#pragma comment(lib, "opengl32.lib")
#include <gl/gl.h>

void APIENTRY det_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void APIENTRY det_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void APIENTRY det_glCullFace(GLenum mode);
