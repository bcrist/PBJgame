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

#include "pbj/input_controller.h"
#include "pbj/gfx/built_ins.h"
#include "pbj/gfx/texture_font_text.h"
#include "pbj/gfx/shader_program.h"
#include "pbj/scene/ui_label.h"
#include "pbj/scene/ui_button.h"
#include "pbj/scene/ui_root.h"

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
   
    be::ConstHandle<pbj::gfx::TextureFont> font = engine.getBuiltIns().getTextureFont(pbj::Id("TextureFont.default")).getHandle();

    pbj::InputController::init(engine.getWindow()->getGlfwHandle());

    pbj::scene::UIRoot ui;

    pbj::scene::UIButton* btn = new pbj::scene::UIButton();
    ui.panel.addElement(std::unique_ptr<pbj::scene::UIElement>(btn));
    
    pbj::scene::UILabel* label = new pbj::scene::UILabel();
    ui.panel.addElement(std::unique_ptr<pbj::scene::UIElement>(label));

    btn->setPosition(pbj::vec2(100, 100));
    btn->setDimensions(pbj::vec2(200, 50));
    btn->setText("Hello World");

    pbj::scene::UIButtonStateConfig config;
    config.button_state = pbj::Id("__normal__");
    config.background_color = pbj::color4(1, 0,1, 0.33f);
    config.border_color = pbj::color4(1,0,1,1);
    config.click_callback = []() { PBJ_LOG(pbj::VInfo) << "Button Pressed!" << PBJ_LOG_END; };
    config.font = font;
    config.text_color = pbj::color4(1,1,1,1);
    config.border_width_bottom = 5;
    btn->setStateConfig(config);

    config.button_state = pbj::Id("__hovered__");
    config.border_width_left = 1;
    config.border_width_top = 5;
    config.border_width_right = 1;
    btn->setStateConfig(config);
    

    label->setDimensions(pbj::vec2(640, 480));
    label->setAlign(pbj::scene::UILabel::AlignRight);
    label->setTextScale(pbj::vec2(5.0f, 5.0f));
    label->setTextColor(pbj::color4(0, 0.6f, 1.0f, 1.0f));
    label->setFont(font);
    label->setText("Frame");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
   
    double current_time;
    double last_time = current_time = glfwGetTime();

    while (true)
    {
        glfwPollEvents();

        pbj::Window* wnd = engine.getWindow();

        if (!wnd || wnd->isClosePending())
            break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        last_time = current_time;
        current_time = glfwGetTime();

        label->setText(std::to_string(1000.0 * (current_time - last_time)) + " ms");

        ui.draw();

        engine.getBatcher().draw();

        glfwSwapBuffers(wnd->getGlfwHandle());

        GLenum gl_error;
        while ((gl_error = glGetError()) != GL_NO_ERROR)
        {
            PBJ_LOG(pbj::VWarning) << "OpenGL error while rendering frame!" << PBJ_LOG_NL
                                   << "Error Code: " << gl_error << PBJ_LOG_NL
                                   << "     Error: " << pbj::getGlErrorString(gl_error) << PBJ_LOG_END;
        }
    }

    pbj::InputController::destroy();
};

#endif
