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
/// \file   pbj/window.h
/// \author Benjamin Crist
///
/// \brief  pbj::Window class header.

#ifndef PBJ_WINDOW_H_
#define PBJ_WINDOW_H_

#include "pbj/window_settings.h"
#include "pbj/_math.h"
#include "pbj/_gl.h"
#include "pbj/_pbj.h"

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>

namespace pbj {

class Engine;

///////////////////////////////////////////////////////////////////////////////
/// \class  Window   pbj/window.h "pbj/window.h"
///
/// \brief  Represents a window or fullscreen exclusive mode which can be used
///         to display OpenGL graphics.
class Window
{
   friend class Engine;

public:
    typedef std::function<void(I32, I32)> MoveListener;
    typedef std::function<void(I32, I32)> ResizeListener;
    typedef std::function<void(I32, I32)> ContextResizeListener;
    typedef std::function<void()> CloseRequestListener;
    typedef std::function<void()> RepaintRequestListener;
    typedef std::function<void(bool)> FocusChangeListener;
    typedef std::function<void(bool)> StateChangeListener;

    GLFWwindow* getGlfwHandle();

    const WindowSettings& getWindowSettings() const;

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

    size_t registerMoveListener(const MoveListener& listener);
    size_t registerResizeListener(const ResizeListener& listener);
    size_t registerContextResizeListener(const ContextResizeListener& listener);
    size_t registerCloseRequestListener(const CloseRequestListener& listener);
    size_t registerRepaintRequestListener(const RepaintRequestListener& listener);
    size_t registerFocusChangeListener(const FocusChangeListener& listener);
    size_t registerStateChangeListener(const StateChangeListener& listener);

    void cancelMoveListener(size_t id);
    void cancelResizeListener(size_t id);
    void cancelContextResizeListener(size_t id);
    void cancelCloseRequestListener(size_t id);
    void cancelRepaintRequestListener(size_t id);
    void cancelFocusChangeListener(size_t id);
    void cancelStateChangeListener(size_t id);

private:
    // called from Engine:
   Window(const WindowSettings& window_settings);
   ~Window();

   void fireMoved_(I32 x, I32 y);
   void fireResized_(I32 width, I32 height);
   void fireContextResized_(I32 width, I32 height);
   void fireCloseRequested_();
   void fireRepaintRequested_();
   void fireFocusChanged_(bool focused);
   void fireStateChanged_(bool iconified);

   static void glfwMoved_(GLFWwindow* window, int x, int y);
   static void glfwResized_(GLFWwindow* window, int width, int height);
   static void glfwContextResized_(GLFWwindow* window, int width, int height);
   static void glfwCloseRequested_(GLFWwindow* window);
   static void glfwRepaintRequested_(GLFWwindow* window);
   static void glfwFocusChanged_(GLFWwindow* window, int state);
   static void glfwIconStateChanged_(GLFWwindow* window, int state);

   std::vector<MoveListener> move_listeners_;
   std::vector<ResizeListener> resize_listeners_;
   std::vector<ContextResizeListener> context_resize_listeners_;
   std::vector<CloseRequestListener> close_request_listeners_;
   std::vector<RepaintRequestListener> repaint_request_listeners_;
   std::vector<FocusChangeListener> focus_change_listeners_;
   std::vector<StateChangeListener> state_change_listeners_;

   WindowSettings window_settings_;
   std::string title_;
   GLFWwindow* glfw_window_;
   
   Window(const Window&);
   void operator=(const Window&);
};

} // namespace pbj

#endif
