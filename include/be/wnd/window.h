// Copyright (c) 2013 Benjamin Crist
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
/// \file   be/wnd/window.h
/// \author Benjamin Crist
///
/// \brief  be::wnd::Window class header.

#ifndef BE_WND_WINDOW_H_
#define BE_WND_WINDOW_H_

#include "be/wnd/detail/context_state.h"
#include "be/wnd/detail/window_entity_listener.h"
#include "be/wnd/window_settings.h"
#include "be/wnd/i_window_listener.h"
#include "be/entity.h"
#include "be/id.h"
#include "be/_math.h"
#include "be/_gl.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace be {
namespace bsc {

class Scene;

} // namespace be::bsc

class Engine;

namespace wnd {

///////////////////////////////////////////////////////////////////////////////
/// \class  Window   be/wnd/window.h "be/wnd/window.h"
///
/// \brief  Represents a window or fullscreen exclusive mode which can be used
///         to display OpenGL graphics.
/// \ingroup window
class Window
{
   friend Window* popCurrentContext();
   friend class Engine;
   friend struct std::default_delete<Window>;

   typedef std::vector< std::unique_ptr<Entity> > EntityContainer;
   typedef std::unordered_map<Id, std::vector<IWindowListener*> > ListenerContainer;
public:
   /// \brief  An iterator over the root entities attached to this window.
   /// \details Compatible with \c std::iterator and standard library algorithms.
   typedef be::detail::EntityChildIterator EntityIterator;
   /// \brief  An iterator over the root entities attached to this window.
   /// \details Compatible with \c std::iterator and standard library algorithms.
   ///         May not be used to modify entities.
   typedef be::detail::EntityConstChildIterator ConstEntityIterator;

   const Handle<Window>& getHandle();
   const ConstHandle<Window>& getHandle() const;

   GLFWwindow* getGlfwHandle();

   const WindowSettings& getWindowSettings() const;

   void render();
   void update(const std::chrono::duration<double>& elapsed);

   void addEntity(std::unique_ptr<Entity>&& entity);
   std::unique_ptr<Entity> releaseEntity(const EntityIterator& it);
   void eraseEntity(const EntityIterator& it);
   void eraseEntities(const EntityIterator& begin, const EntityIterator& end);

   EntityIterator begin();
   EntityIterator end();
   ConstEntityIterator begin() const;
   ConstEntityIterator end() const;

   void setTitle(const std::string& title);
   const std::string& getTitle() const;

   void setPosition(const ivec2& position);
   void setPosition(int x, int y);
   ivec2 getPosition() const;

   void setSize(const ivec2& size);
   void setSize(int width, int height);
   ivec2 getSize() const;
   ivec2 getContextSize() const;

   void show();
   void hide();
   void setVisible(bool visible);
   bool isVisible() const;

   void iconify();
   void restore();
   void setIconified(bool iconified);
   bool isIconified() const;

   bool isFocused() const;

   void requestClose();
   void cancelClose();
   bool isClosePending() const;

   bool isResizable() const;
   bool isDecorated() const;

   int getContextVersionMajor() const;
   int getContextVersionMinor() const;
   int getContextRevision() const;

   bool isDebugContext() const;

   bool isContextCurrent() const;
   void makeContextCurrent();

   void registerListener(IWindowListener& listener, const Id& event_type);
   bool registerListenerChecked(IWindowListener& listener, const Id& event_type);
   bool unregisterListener(IWindowListener& listener, const Id& event_type);
   int unregisterListenerChecked(IWindowListener& listener, const Id& event_type);


private:
   Window(const WindowSettings& window_settings);
   ~Window();

   void fireMoved_();
   void fireResized_();
   void fireContextResized_();
   void fireCloseRequested_();
   void fireRepaintRequested_();
   void fireFocusGained_();
   void fireFocusLost_();
   void fireIconified_();
   void fireRestored_();

   static void glfwMoved_(GLFWwindow* window, int x, int y);
   static void glfwResized_(GLFWwindow* window, int width, int height);
   static void glfwContextResized_(GLFWwindow* window, int width, int height);
   static void glfwCloseRequested_(GLFWwindow* window);
   static void glfwRepaintRequested_(GLFWwindow* window);
   static void glfwFocusChanged_(GLFWwindow* window, int state);
   static void glfwIconStateChanged_(GLFWwindow* window, int state);

   SourceHandle<Window> handle_;
   detail::WindowEntityListener entity_listener_;
   WindowSettings window_settings_;
   float monitor_dpi_;

   std::string title_;

   GLFWwindow* glfw_window_;
   GLEWContext glew_context_;
   detail::ContextState context_state_;

   ListenerContainer listeners_;
   EntityContainer entities_;

   std::vector<bsc::Scene*> scenes_;
   
   Window(const Window&);
   void operator=(const Window&);
};

void pushCurrentContext(const Window& window);
Window* popCurrentContext();

Window* getCurrentContext();
void clearContextStack();

} // namespace be::wnd
} // namespace be

#endif
