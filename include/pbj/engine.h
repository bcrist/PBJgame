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
