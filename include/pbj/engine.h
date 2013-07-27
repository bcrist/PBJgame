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
/// \file   pbj/engine.h
/// \author Benjamin Crist
///
/// \brief  pbj::Engine class header.

#ifndef PBJ_ENGINE_H_
#define PBJ_ENGINE_H_

#include "be/id.h"
#include "pbj/_pbj.h"
#include "pbj/window.h"
#include "pbj/gfx/built_ins.h"
#include "pbj/gfx/batcher.h"

#include <memory>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief Manages global engine objects.
/// \details Only one engine should be created per process.  Attempts to create
///        multiple engines will result in an exception.
class Engine
{
public:
   Engine();
   ~Engine();

   Window* getWindow() const;

   gfx::Batcher& getBatcher();

   const gfx::BuiltIns& getBuiltIns() const;

private:
    gfx::Batcher batcher_;

    std::unique_ptr<Window> window_;
    std::unique_ptr<gfx::BuiltIns> built_ins_;

   Engine(const Engine&);
   void operator=(const Engine&);
};

Engine& getEngine();

} // namespace pbj

#endif
