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
/// \file   pbj/window.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Window functions.

#include "pbj/window.h"


namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the GLFW window structure for this window.
///
/// \return The GLFWwindow* structure.
GLFWwindow* Window::getGlfwHandle()
{
   return glfw_window_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the WindowSettings used to create this window.
///
/// \note   The settings in the WindowSettings object do not necessarily
///         represent the capabilities of the window's OpenGL context.  For
///         instance, a 32-bit depth buffer may have been requested, but that
///         does not mean that the context's depth buffer is actually 32 bits.
///
/// \return The WindowSettings object associated with this window.
const WindowSettings& Window::getWindowSettings() const
{
   return window_settings_;
}


///////////////////////////////////////////////////////////////////////////////
/// \brief  Changes the title of the window as seen by the operating system
///         and user.
///
/// \param  title The new title for the window.
void Window::setTitle(const std::string& title)
{
   if (title != title_)
   {
      title_ = title;
      glfwSetWindowTitle(glfw_window_, title.c_str());
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the current title of the window as it was passed to
///         setTitle().
///
/// \return The current title of the window.
const std::string& Window::getTitle() const
{
   return title_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Requests that the operating system move the window to the requested
///         location.
///
/// \details The OS is not required to honor such requests, or to place the
///         window in exactly the location requested.
///
/// \param  position An ivec2 representing the desired desktop coordinates of
///         the top-left corner of the window's client area.
void Window::setPosition(const ivec2& position)
{
   glfwSetWindowPos(glfw_window_, position.x, position.y);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Requests that the operating system move the window to the requested
///         location.
///
/// \details The OS is not required to honor such requests, or to place the
///         window in exactly the location requested.
///
/// \param  x The X-coordinate of the desired position in desktop coordinates
///         of the left edge of the window's client area.
/// \param  y The Y-coordinate of the desired position in desktop coordinates
///         of the top edge of the window's client area.
void Window::setPosition(int x, int y)
{
   glfwSetWindowPos(glfw_window_, x, y);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the current position of the window in desktop
///         coordinates.
///
/// \return An ivec2 representing the desktop coordinates of the top-left
///         corner of the window's client area.
ivec2 Window::getPosition() const
{
   ivec2 position;
   glfwGetWindowPos(glfw_window_, &position.x, &position.y);
   return position;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Requests that the operating system resize the window.
///
/// \details The OS is not required to honor such requests, or to use the
///         exact dimensions requested.
///
///         Resizing the window will usually result in the resizing of the
///         OpenGL context, so both a WindowEvent::resized and
///         WindowEvent::context_resized will be generated by a resize request.
///
/// \note   The requested size is in desktop coordinate units, which on some
///         platforms may be different than pixels.
///
/// \param  size An ivec2 representing the desired dimensions of the window's
///         client area in desktop coordinates.
void Window::setSize(const ivec2& size)
{
   glfwSetWindowSize(glfw_window_, size.x, size.y);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Requests that the operating system resize the window.
///
/// \details The OS is not required to honor such requests, or to use the
///         exact dimensions requested.
///
///         Resizing the window will usually result in the resizing of the
///         OpenGL context, so both a WindowEvent::resized and
///         WindowEvent::context_resized will be generated by a resize request.
///
/// \note   The requested size is in desktop coordinate units, which on some
///         platforms may be different than pixels.
///
/// \param  width The desired witdth of the window's client area in desktop
///         coordinates.
/// \param  height The desired height of the window's client area in desktop
///         coordinates.
void Window::setSize(int width, int height)
{
   glfwSetWindowSize(glfw_window_, width, height);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the current dimensions of the window's client area in
///         desktop coordinates.
///
/// \return An ivec2 representing the dimensions of the client area in desktop
///         coordinates.
ivec2 Window::getSize() const
{
   ivec2 size;
   glfwGetWindowSize(glfw_window_, &size.x, &size.y);
   return size;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the current dimensions of the default framebuffer
///         associated with this window's OpenGL context.
///
/// \note   Unlike getSize(), getContextSize() returns its dimensions in
///         device coordinates (pixels) so this is the value that should be
///         used for eg. \c glViewport().
///
/// \return An ivec2 representing the dimensions of the context's default 
///         framebuffer.
ivec2 Window::getContextSize() const
{
   ivec2 size;
   glfwGetFramebufferSize(glfw_window_, &size.x, &size.y);
   return size;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Shows the window.
///
/// \details Equivalent to setVisible(true).
///
/// \sa     setVisible(bool).
void Window::show()
{
   setVisible(true);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Hides the window.
///
/// \details Equivalent to setVisible(false).
///
/// \sa     setVisible(bool).
void Window::hide()
{
   setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Shows or hides the window.
///
/// \details When a window is created, it is hidden by default.
///
/// \param  visible Whether or not the window should be visible (shown) or not.
void Window::setVisible(bool visible)
{
   if (glfwGetWindowAttrib(glfw_window_, GLFW_VISIBLE) == 0)
   {
      if (visible)
         glfwShowWindow(glfw_window_);
   }
   else
   {
      if (!visible)
         glfwHideWindow(glfw_window_);
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns true if the window is currently visible
///
/// \details An iconified (minimized) window still counts as visible.
///
/// \return \c true if the window is visible.
bool Window::isVisible() const
{
   return glfwGetWindowAttrib(glfw_window_, GLFW_VISIBLE) != 0;
}

void Window::iconify()
{
   setIconified(true);
}

void Window::restore()
{
   setIconified(false);
}

void Window::setIconified(bool iconified)
{
   if (glfwGetWindowAttrib(glfw_window_, GLFW_ICONIFIED) == 0)
   {
      if (iconified)
         glfwIconifyWindow(glfw_window_);
   }
   else
   {
      if (!iconified)
         glfwRestoreWindow(glfw_window_);
   }
}

bool Window::isIconified() const
{
   return glfwGetWindowAttrib(glfw_window_, GLFW_ICONIFIED) != 0;
}

bool Window::isFocused() const
{
   return glfwGetWindowAttrib(glfw_window_, GLFW_FOCUSED) != 0;
}


void Window::requestClose()
{
   glfwSetWindowShouldClose(glfw_window_, 1);
}

void Window::cancelClose()
{
   glfwSetWindowShouldClose(glfw_window_, 0);
}

bool Window::isClosePending() const
{
   return glfwWindowShouldClose(glfw_window_) != 0;
}


bool Window::isResizable() const
{
   return glfwGetWindowAttrib(glfw_window_, GLFW_RESIZABLE) != 0;
}

bool Window::isDecorated() const
{
   return glfwGetWindowAttrib(glfw_window_, GLFW_DECORATED) != 0;
}

int Window::getContextVersionMajor() const
{
   return glfwGetWindowAttrib(glfw_window_, GLFW_CONTEXT_VERSION_MAJOR) != 0;
}

int Window::getContextVersionMinor() const
{
   return glfwGetWindowAttrib(glfw_window_, GLFW_CONTEXT_VERSION_MINOR);
}

bool Window::isContextCurrent() const
{
   return glewGetContext() == &glew_context_;
}

void Window::makeContextCurrent()
{
   if (glewGetContext())
   {
      current_context_stack.push_back(current_context);
   }

   if (glewGetContext() != &glew_context_)
   {
      if (current_context != this)
      {
         glfwMakeContextCurrent(glfw_window_);
         current_context = this;
      }
      detail::current_glew_context = &glew_context_;
      detail::current_context_state = &context_state_;
   }
}



void Window::registerListener(IWindowListener& listener, const Id& event_type)
{
   std::vector<IWindowListener*>& listeners = listeners_[event_type];
   listeners.push_back(&listener);
}

bool Window::registerListenerChecked(IWindowListener& listener, const Id& event_type)
{
   std::vector<IWindowListener*>& listeners = listeners_[event_type];
   
   for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
      if (*i == &listener)
         return false;

   listeners.push_back(&listener);
   return true;

}

bool Window::unregisterListener(IWindowListener& listener, const Id& event_type)
{
   auto i(listeners_.find(event_type));
   if (i != listeners_.end())
   {
      std::vector<IWindowListener*>& listeners = i->second;

      for (auto j(listeners.begin()), end(listeners.end()); j != end; ++j)
      {
         if (*j == &listener)
         {
            listeners.erase(j);
            if (listeners.empty())
            {
               // the vector is now empty, so remove it from the map
               listeners_.erase(i);
            }

            return true;
         }
      }
   }
   return false;
}

int Window::unregisterListenerChecked(IWindowListener& listener, const Id& event_type)
{
   int removed_count(0);

   auto i(listeners_.find(event_type));
   if (i != listeners_.end())
   {
      std::vector<IWindowListener*>& listeners = i->second;

      for (auto j(listeners.begin()), end(listeners.end()); j != end;)
      {
         if (*j == &listener)
         {
            j = listeners.erase(j);
            ++removed_count;
         }
         else
            ++j;
      }

      if (listeners.empty())
      {
         // the vector is now empty, so remove it from the map
         listeners_.erase(i);
      }
   }
   return removed_count;
}

Window::Window(const WindowSettings& window_settings)
    window_settings_(window_settings);
{
    int width, height;
    int refresh_rate;
    ivec3 color_bits;

    const bool fullscreen_exclusive = (window_settings.mode & 0x8) != 0;
    const bool fullscreen_windowed = (window_settings.mode & 0x4) != 0;
    const bool undecorated = (window_settings.mode & 0x2) != 0;
    const bool fixed_size = (window_settings.mode & 0x1) != 0;

    ivec2 position = window_settings.position;

    width = window_settings.size.x;
    height = window_settings.size.y;

    color_bits.r = window_settings.red_bits;
    color_bits.g = window_settings.green_bits;
    color_bits.b = window_settings.blue_bits;

    refresh_rate = window_settings.refresh_rate;

    // Get monitor info
    GLFWmonitor* monitor = nullptr;
    if (fullscreen_exclusive || fullscreen_windowed)
    {
        // Fullscreen mode (exclusive or windowed) - use monitor_index

        if (window_settings.monitor_index > 0)
        {
            int monitor_count;
            GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
            if (window_settings.monitor_index < monitor_count)
                monitor = monitors[monitor_count];
        }
    }
    else
    {
        // Windowed mode - find monitor that our window will be on

        ivec2 center = window_settings.position + window_settings.size / 2;

        int monitor_count;
        GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
      
        for (int i = 0; i < monitor_count; ++i)
        {
            GLFWmonitor* mon = monitors[i];
            ivec2 offset;
            glfwGetMonitorPos(mon, &offset.x, &offset.y);
            const GLFWvidmode* mode = glfwGetVideoMode(mon);

            if (center.x >= offset.x && center.x < offset.x + mode->width &&
                center.y >= offset.y && center.y < offset.y + mode->height)
            {
                monitor = mon;
                break;
            }
        }
    }

    if (!monitor)
        monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* monitor_mode = glfwGetVideoMode(monitor);

    
    if (!fullscreen_exclusive)
    {
        if (fullscreen_windowed)
        {
            width = monitor_mode->width;
            height = monitor_mode->height;

            glfwGetMonitorPos(monitor, &position.x, &position.y);

            color_bits.r = monitor_mode->redBits;
            color_bits.g = monitor_mode->greenBits;
            color_bits.b = monitor_mode->blueBits;
        }

        // if not fullscreen exclusive, we don't provide monitor
        monitor = nullptr;
        refresh_rate = 0;
    }


    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, 0);

    glfwWindowHint(GLFW_DECORATED, undecorated ? 0 : 1);
    glfwWindowHint(GLFW_RESIZABLE, fixed_size ? 0 : 1);

    glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
    glfwWindowHint(GLFW_SRGB_CAPABLE, window_settings.srgb_capable ? 1 : 0);
    glfwWindowHint(GLFW_RED_BITS, color_bits.r);
    glfwWindowHint(GLFW_GREEN_BITS, color_bits.g);
    glfwWindowHint(GLFW_BLUE_BITS, color_bits.b);
    glfwWindowHint(GLFW_ALPHA_BITS, window_settings.alpha_bits);
    glfwWindowHint(GLFW_DEPTH_BITS, window_settings.depth_bits);
    glfwWindowHint(GLFW_STENCIL_BITS, window_settings.stencil_bits);
    glfwWindowHint(GLFW_SAMPLES, window_settings.msaa_level);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   

    glfw_window_ = glfwCreateWindow(width, height, "bengine", monitor, nullptr);

    if (fullscreen_exclusive)
    {
        if (window_settings.use_custom_gamma)
            glfwSetGamma(monitor, window_settings.custom_gamma);
    }
    else if (fullscreen_windowed || !window_settings.system_positioned)
    {
        glfwSetWindowPos(glfw_window_, position.x, position.y);
    }

    int vsync = window_settings.v_sync;
    if (vsync < 0)
    {
        if (!(glfwExtensionSupported("WGL_EXT_swap_control_tear") ||
                glfwExtensionSupported("GLX_EXT_swap_control_tear")))
        {
            vsync = abs(vsync);
        }
    }

    glfwSwapInterval(vsync);

    glfwSetWindowUserPointer(glfw_window_, this);

    glfwSetWindowPosCallback(glfw_window_, glfwMoved_);
    glfwSetWindowSizeCallback(glfw_window_, glfwResized_);
    glfwSetFramebufferSizeCallback(glfw_window_, glfwContextResized_);
    glfwSetWindowCloseCallback(glfw_window_, glfwCloseRequested_);
    glfwSetWindowRefreshCallback(glfw_window_, glfwRepaintRequested_);
    glfwSetWindowFocusCallback(glfw_window_, glfwFocusChanged_);
    glfwSetWindowIconifyCallback(glfw_window_, glfwIconStateChanged_);

    glewInit();
}

Window::~Window()
{
   if (window_settings_.save_position_on_close)
   {
      glfwGetWindowPos(glfw_window_, &window_settings_.position.x, &window_settings_.position.y);
      glfwGetWindowSize(glfw_window_, &window_settings_.size.x, &window_settings_.size.y);
      wnd::updateSavedPosition(window_settings_);
   }

   // popCurrentContext() should already have been called if necessary;
   // the context should not be active, nor exist anywhere on the context stack.

   glfwDestroyWindow(glfw_window_);
}

void Window::fireMoved_()
{
   const Id& type = WindowEvent::moved;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowMoved(evt);
   }
}

void Window::fireResized_()
{
   const Id& type = WindowEvent::resized;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowResized(evt);
   }
}

void Window::fireContextResized_()
{
   const Id& type = WindowEvent::context_resized;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowContextResized(evt);
   }
}

void Window::fireCloseRequested_()
{
   const Id& type = WindowEvent::close_requested;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowCloseRequested(evt);
   }
}
   
void Window::fireRepaintRequested_()
{
   const Id& type = WindowEvent::repaint_requested;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowRepaintRequested(evt);
   }
}

void Window::fireFocusGained_()
{
   const Id& type = WindowEvent::focus_gained;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowFocusGained(evt);
   }
}

void Window::fireFocusLost_()
{
   const Id& type = WindowEvent::focus_lost;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowFocusLost(evt);
   }
}

void Window::fireIconified_()
{
   const Id& type = WindowEvent::iconified;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowIconified(evt);
   }
}

void Window::fireRestored_()
{
   const Id& type = WindowEvent::restored;
   auto i(listeners_.find(type));
   if (i != listeners_.end())
   {
      WindowEvent evt(type, getHandle());
      std::vector<IWindowListener*>& listeners(i->second);
      for (auto i(listeners.begin()), end(listeners.end()); i != end; ++i)
         (*i)->onWindowRestored(evt);
   }
}

void Window::glfwMoved_(GLFWwindow* window, int x, int y)
{
   Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
   wnd->fireMoved_();
}
 	
void Window::glfwResized_(GLFWwindow* window, int width, int height)
{
   Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
   wnd->fireResized_();
}

void Window::glfwContextResized_(GLFWwindow* window, int width, int height)
{
   Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
   wnd->fireContextResized_();
}

void Window::glfwCloseRequested_(GLFWwindow* window)
{
   Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
   wnd->fireCloseRequested_();
}

void Window::glfwRepaintRequested_(GLFWwindow* window)
{
   Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
   wnd->fireRepaintRequested_();
}

void Window::glfwFocusChanged_(GLFWwindow* window, int state)
{
   Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
   if (state)
      wnd->fireFocusGained_();
   else
      wnd->fireFocusLost_();
}

void Window::glfwIconStateChanged_(GLFWwindow* window, int state)
{
   Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
   if (state)
      wnd->fireIconified_();
   else
      wnd->fireRestored_();
}


void pushCurrentContext(Window& window)
{
   window.makeContextCurrent();
}

Window* popCurrentContext()
{
   if (!current_context_stack.empty())
   {
      Window* new_context_window = current_context_stack.back();
      current_context_stack.pop_back();

      if (glewGetContext() != &new_context_window->glew_context_)
      {
         glfwMakeContextCurrent(new_context_window->glfw_window_);
         detail::current_glew_context = &new_context_window->glew_context_;
         detail::current_context_state = &new_context_window->context_state_;
      }
      
      return new_context_window;
   }
   else
   {
      detail::current_glew_context = nullptr;
      detail::current_context_state = nullptr;

      return nullptr;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// Retrieves a pointer to the window object whose OpenGL context is current,
/// or nullptr if there is no current context.
Window* getCurrentContext()
{
   return glewGetContext() ? current_context : nullptr;
}

void clearContextStack()
{
   current_context_stack.clear();

   detail::current_glew_context = nullptr;
   detail::current_context_state = nullptr;
}

} // namespace be::wnd
} // namespace be
