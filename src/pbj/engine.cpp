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
/// \file   pbj/engine.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Engine functions.

#include "pbj/engine.h"
#include "pbj/_gl.h"
#include "pbj/sw/sandwich_open.h"

#include <thread>
#include <cassert>
#include <iostream>

namespace pbj {
namespace {

Engine* process_engine_ = nullptr;

// GLFW error handler
void glfwError(int error, const char* description)
{
    PBJ_LOG(VError) << "GLFW Error!" << PBJ_LOG_NL
                    << "       Code: " << error << PBJ_LOG_NL
                    << "Description: " << description << PBJ_LOG_END;
}

} // namespace be::(anon)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Engine constructor.
///
/// \details Only one Engine may be constructed.  It should be created as a
///         local variable in main().
Engine::Engine()
{
    if (process_engine_)
        throw std::runtime_error("Engine already initialized!");

    process_engine_ = this;

    glfwSetErrorCallback(glfwError);

    if (!glfwInit())
        PBJ_LOG(VError) << "GLFW could not be initialized!" << PBJ_LOG_END;

    sw::readDirectory("./");

    std::shared_ptr<sw::Sandwich> config_sandwich;
    config_sandwich = sw::open(Id("__pbjconfig__"));

    Id window_settings_id;
    std::string window_title;

#ifdef PBJ_EDITOR
    window_settings_id = Id("__editor__");
    window_title = "PBJ Editor";
#elif defined(PBJ_SERVER)
    window_settings_id = Id("__server__");
    window_title = "PBJ Server";
#else
    window_settings_id = Id("__client__");
    window_title = "PBJ Client";
#endif    

    WindowSettings window_settings;

    window_settings.msaa_level = 4;

    if (config_sandwich)
        window_settings = loadWindowSettings(*config_sandwich, window_settings_id);

    

    Window* wnd = new Window(window_settings);
    window_.reset(wnd);

    built_ins_.reset(new gfx::BuiltIns());

    wnd->setTitle(window_title);
    
    PBJ_LOG(VInfo) << glGetString(GL_VERSION) << PBJ_LOG_END;

    wnd->show();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor.
Engine::~Engine()
{
    window_.reset();
    built_ins_.reset();
    glfwTerminate();
}

Window* Engine::getWindow() const
{
    return window_.get();
}

const gfx::BuiltIns& Engine::getBuiltIns() const
{
    return *built_ins_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the engine object.
///
/// \details If no engine object exists, this will result in an assertion
///         failure.
///
/// \return The process' engine object.
Engine& getEngine()
{
    assert(process_engine_);
    return *process_engine_;
}

} // namespace pbj
