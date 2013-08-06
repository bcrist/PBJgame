///////////////////////////////////////////////////////////////////////////////
/// \file   be/_gl.h
/// \author Benjamin Crist
///
/// \brief  Includes OpenGL/GLEW/GLFW headers

#ifndef PBJ_GL_H_
#define PBJ_GL_H_

#include <GL/glew.h>
#include <GL/glfw3.h>

namespace pbj {

inline const char* getGlErrorString(GLenum error)
{
    switch (error)
    {
        case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
        case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
        case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
        default:                               return "";
    }
}

} // namespace pbj

#endif
