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
///
/// \todo   Add support for creating maximized windows (non-fullscreen)
class Window
{
   friend class Engine;
   friend struct std::default_delete<Window>;

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

    U32 registerMoveListener(const MoveListener& listener);
    U32 registerResizeListener(const ResizeListener& listener);
    U32 registerContextResizeListener(const ContextResizeListener& listener);
    U32 registerCloseRequestListener(const CloseRequestListener& listener);
    U32 registerRepaintRequestListener(const RepaintRequestListener& listener);
    U32 registerFocusChangeListener(const FocusChangeListener& listener);
    U32 registerStateChangeListener(const StateChangeListener& listener);

    void cancelMoveListener(U32 id);
    void cancelResizeListener(U32 id);
    void cancelContextResizeListener(U32 id);
    void cancelCloseRequestListener(U32 id);
    void cancelRepaintRequestListener(U32 id);
    void cancelFocusChangeListener(U32 id);
    void cancelStateChangeListener(U32 id);

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

   std::vector<std::pair<U32, MoveListener> > move_listeners_;
   std::vector<std::pair<U32, ResizeListener> > resize_listeners_;
   std::vector<std::pair<U32, ContextResizeListener> > context_resize_listeners_;
   std::vector<std::pair<U32, CloseRequestListener> > close_request_listeners_;
   std::vector<std::pair<U32, RepaintRequestListener> > repaint_request_listeners_;
   std::vector<std::pair<U32, FocusChangeListener> > focus_change_listeners_;
   std::vector<std::pair<U32, StateChangeListener> > state_change_listeners_;

   U32 next_listener_id_;

   WindowSettings window_settings_;
   std::string title_;
   GLFWwindow* glfw_window_;
   
   Window(const Window&);
   void operator=(const Window&);
};

} // namespace pbj

#endif
