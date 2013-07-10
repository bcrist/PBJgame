// Copyright (c) 2013 PBJ Productions
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
#include "be/_gl.h"

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
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor.
Engine::~Engine()
{
   glfwTerminate();
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
