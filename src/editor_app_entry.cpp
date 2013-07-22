// Copyright (c) 2013 PBJ^2 Productions
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////
/// \file   editor_app_entry.cpp
/// \author Benjamin Crist
///
/// \brief  Application entry point.
/// \details Parses command line parameters, initializes game engine, loads
///         configuration data, and starts editor.

// Auto-link with libraries
#ifdef _WIN32
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "openal32.lib")
#pragma comment (lib, "alut.lib")
#ifdef DEBUG
#pragma comment (lib, "glew32sd.lib")
#pragma comment (lib, "glfw3sd.lib")
#pragma comment (lib, "assimpsd.lib")
#else
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glfw3s.lib")
#pragma comment (lib, "assimps.lib")
#endif // DEBUG
#endif // _WIN32

// Is this a unit testing build?
#ifdef PBJ_TEST
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#else

#include "pbj/engine.h"

#include "pbj/gfx/built_ins.h"
#include "pbj/gfx/texture_font_text.h"
#include "pbj/gfx/shader_program.h"
#include "pbj/gfx/built_ins.h"

#include <iostream>
#include <fstream>
#include <random>

#if defined(_WIN32) && !defined(DEBUG)
#include <windows.h>
int main(int argc, char* argv[]);

///////////////////////////////////////////////////////////////////////////////
/// \brief  Win32 API entry point; redirects to main().
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
   return main(__argc, __argv);
}

#endif // defined(_WIN32) && !defined(DEBUG)


///////////////////////////////////////////////////////////////////////////////
/// \brief  Application entry point
/// \details Parses any command line arguments, then initializes game engine.
int main(int argc, char* argv[])
{
#ifdef BE_CRT_BUILD
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef DEBUG
   int verbosity = pbj::VAll;
#else
   int verbosity = pbj::VErrorsAndWarnings;
#endif
   std::ofstream cerr_log_file;
   std::string cerr_log("pbjed.log");

   if (argc != 1)
   {
      std::cout << "PBJgame " << PBJ_VERSION_MAJOR << '.' << PBJ_VERSION_MINOR << " (" << __DATE__ " " __TIME__ << ')' << std::endl
                << PBJ_COPYRIGHT << std::endl;
      return 1;
   }
   
   // Set the appropriate verbosity level
   be::setVerbosity(verbosity);

   // Redirect PBJ_LOG_STREAM to a log file if not in DEBUG mode
#ifndef DEBUG
   if (cerr_log.length() > 0)
   {
      PBJ_LOG(pbj::VNotice) << "Redirecting log to " << cerr_log << PBJ_LOG_END;
      cerr_log_file.open(cerr_log, std::ofstream::trunc);
      PBJ_LOG_STREAM.rdbuf(cerr_log_file.rdbuf());

      PBJ_LOG(pbj::VInfo) << "Starting Editor..." << PBJ_LOG_NL
                          << " PBJgame Version: " << PBJ_VERSION_MAJOR << '.' << PBJ_VERSION_MINOR << PBJ_LOG_NL
                          << " Build Date: " <<  __DATE__ " " __TIME__ << PBJ_LOG_END;
   }
#endif

   // Initialize game engine
   pbj::Engine engine;

   pbj::gfx::TextureFontText text(engine.getBuiltIns().getTextureFont(pbj::Id("TextureFont.default")), "ABCDEFG");
   pbj::mat4 transform = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f);


   GLuint vao;
   GLuint vbo;
   GLuint ibo;
   
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ibo);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   pbj::vec2 verts[3] = { pbj::vec2(0, 0), pbj::vec2(0, -1), pbj::vec2(1, 0) };
   glBufferData(GL_ARRAY_BUFFER, sizeof(pbj::vec2)*3, verts, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0); 


   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   GLubyte indices[3] = { 0, 1, 2 };
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 3, indices, GL_STATIC_DRAW);
   

   while (true)
    {
        glfwPollEvents();

        pbj::Window* wnd = engine.getWindow();

        if (!wnd || wnd->isClosePending())
            break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(engine.getBuiltIns().getProgram(pbj::Id("ShaderProgram.TextureFontText")).getGlId());

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0);

        GLenum gl_error;
        while ((gl_error = glGetError()) != GL_NO_ERROR)
        {
            PBJ_LOG(pbj::VWarning) << "OpenGL error while rendering frame!" << PBJ_LOG_NL
                                   << "Error Code: " << gl_error << PBJ_LOG_NL
                                   << "     Error: " << pbj::getGlErrorString(gl_error) << PBJ_LOG_END;
        }

        //text.draw(transform);

        glfwSwapBuffers(wnd->getGlfwHandle());

       // GLenum gl_error;
        while ((gl_error = glGetError()) != GL_NO_ERROR)
        {
            PBJ_LOG(pbj::VWarning) << "OpenGL error while rendering frame!" << PBJ_LOG_NL
                                   << "Error Code: " << gl_error << PBJ_LOG_NL
                                   << "     Error: " << pbj::getGlErrorString(gl_error) << PBJ_LOG_END;
        }
    }
};

#endif
