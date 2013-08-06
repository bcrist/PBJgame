///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/input_controller.h
/// \author Peter Bartosch
/// \date   2013-07-18
/// \brief  Intermediate class for input handling
/// \details InputController is a class consisting of static methods and
///         variables for grabbing GLFW input events and passing them to
///         registered observers.  While GLFW does support more input events
///         than those listed here, we are not concerned with joysticks at the
///         moment and therefore do not register to hear them.  This is setup
///         for use in only one window.  I do not anticipate that being an
///         issue.  The double-click detection will work poorly without having
///         a timer implemented.  C'est la vie.  Furthermore, I think the way
///         listeners are implemented can be improved.

#ifndef PBJ_INPUT_CONTROLLER_H_
#define PBJ_INPUT_CONTROLLER_H_

#include <functional>
#include <vector>

//I think I'm doing the include wrong here.  At this point using it to get off
//intellisense's radar.
//#include "GL/glfw3.h"
#include "pbj/_gl.h"

#include "pbj/_pbj.h"

namespace pbj {

class InputController
{
public:
	typedef std::function<void(I32, I32, I32, I32)> keyAllListener;
	typedef std::function<void(I32, I32, I32)> keyListener;
	typedef std::function<void(I32, I32, I32)> mouseButtonAnyListener;
	typedef std::function<void(I32)> mouseButtonListener;
	typedef std::function<void(F64, F64)> scrollListener;
	typedef std::function<void(F64, F64)> mouseMotionListener;
	typedef std::function<void(F64, F64, I32)> dragListener;
	typedef std::function<void(U32)> charListener;

	static void init(GLFWwindow* win);

	static void raiseMouseButtonEvent(GLFWwindow*, I32, I32, I32); ///< Callback registered with GLFW
	static void raiseMouseMotionEvent(GLFWwindow*, F64, F64); ///< Callback registered with GLFW
	static void raiseScrollEvent(GLFWwindow*, F64, F64); ///< Callback registered with GLFW
	static void raiseKeyboardEvent(GLFWwindow*, I32, I32, I32, I32); ///< Callback registered with GLFW
	static void raiseCharInputEvent(GLFWwindow*, U32); ///< Callback registered with GLFW
	
	//Specific left mouse click events
	static void raiseMouseLeftClickEvent(I32);
	static void raiseMouseLeftDoubleClickEvent(I32);
	static void raiseMouseLeftDownEvent(I32);
	static void raiseMouseLeftUpEvent(I32);
	
	//Specific right mouse click events
	static void raiseMouseRightClickEvent(I32);
	static void raiseMouseRightDownEvent(I32);
	static void raiseMouseRightUpEvent(I32);
	
	//Specific middle mouse click events
	static void raiseMouseMiddleClickEvent(I32);
	static void raiseMouseMiddleDownEvent(I32);
	static void raiseMouseMiddleUpEvent(I32);
	
	//Specific mouse drag events
	static void raiseMouseMotionLeftHeldEvent(F64, F64, I32);
	static void raiseMouseMotionRightHeldEvent(F64, F64, I32);
	static void raiseMouseMotionMiddleHeldEvent(F64, F64, I32);

	//Specific key events
	static void raiseKeyDownEvent(I32, I32, I32); ///< Event for key down
	static void raiseKeyHeldEvent(I32, I32, I32); ///< Event for key held
	static void raiseKeyUpEvent(I32, I32, I32); ///< Event for key being released
	
	//Registration methods
	static U32 registerKeyAllListener(keyAllListener);
	static U32 registerKeyDownListener(keyListener);
	static U32 registerKeyHeldListener(keyListener);
	static U32 registerKeyUpListener(keyListener);

	static U32 registerMouseButtonAnyListener(mouseButtonAnyListener);
	static U32 registerMouseLeftClickListener(mouseButtonListener);
	static U32 registerMouseLeftDoubleClickListener(mouseButtonListener);
	static U32 registerMouseLeftDownListener(mouseButtonListener);
	static U32 registerMouseLeftUpListener(mouseButtonListener);

	static U32 registerMouseRightClickListener(mouseButtonListener);
	static U32 registerMouseRightDownListener(mouseButtonListener);
	static U32 registerMouseRightUpListener(mouseButtonListener);

	static U32 registerMouseMiddleClickListener(mouseButtonListener);
	static U32 registerMouseMiddleDownListener(mouseButtonListener);
	static U32 registerMouseMiddleUpListener(mouseButtonListener);

	static U32 registerMouseMotionListener(mouseMotionListener);
	static U32 registerMouseMotionLeftHeldListener(dragListener);
	static U32 registerMouseMotionRightHeldListener(dragListener);
	static U32 registerMouseMotionMiddleHeldListener(dragListener);

	//Scroll listener
	static U32 registerScrollListener(scrollListener);

	//Char Input Listener
	static U32 registerCharInputListener(charListener);

    // Registration cancellation methods
    static void cancelKeyAllListener(U32 id);
	static void cancelKeyDownListener(U32 id);
	static void cancelKeyHeldListener(U32 id);
	static void cancelKeyUpListener(U32 id);
	static void cancelMouseButtonAnyListener(U32 id);
	static void cancelMouseLeftClickListener(U32 id);
	static void cancelMouseLeftDoubleClickListener(U32 id);
	static void cancelMouseLeftDownListener(U32 id);
	static void cancelMouseLeftUpListener(U32 id);
	static void cancelMouseRightClickListener(U32 id);
	static void cancelMouseRightDownListener(U32 id);
	static void cancelMouseRightUpListener(U32 id);
	static void cancelMouseMiddleClickListener(U32 id);
	static void cancelMouseMiddleDownListener(U32 id);
	static void cancelMouseMiddleUpListener(U32 id);
	static void cancelMouseMotionListener(U32 id);
	static void cancelMouseMotionLeftHeldListener(U32 id);
	static void cancelMouseMotionRightHeldListener(U32 id);
	static void cancelMouseMotionMiddleHeldListener(U32 id);
	static void cancelScrollListener(U32 id);
	static void cancelCharInputListener(U32 id);


	static void destroy();
private:
	//placing the typedefs here to keep them out of the way
	typedef std::vector<std::pair<U32, mouseButtonAnyListener> > mouseButtonAnyListeners;
	typedef std::vector<std::pair<U32, mouseButtonListener> > mouseButtonListeners;
	typedef std::vector<std::pair<U32, mouseMotionListener> > mouseMotionListeners;
	typedef std::vector<std::pair<U32, dragListener> > dragListeners;
	typedef std::vector<std::pair<U32, scrollListener> > scrollListeners;
	typedef std::vector<std::pair<U32, keyListener> > keyListeners;
	typedef std::vector<std::pair<U32, keyAllListener> > keyAllListeners;
	typedef std::vector<std::pair<U32, charListener> > charListeners;

	static bool _initialized;
	static GLFWwindow* _window;

	static bool _leftDown;
	static bool _rightDown;
	static bool _middleDown;
	static bool _leftClicked; ///< Used to track double click
	static I32 _leftMods;
	static I32 _rightMods;
	static I32 _middleMods;

	static keyAllListeners _keyAllListeners;
	static keyListeners _keyDownListeners;
	static keyListeners _keyHeldListeners;
	static keyListeners _keyUpListeners;

	static scrollListeners _scrollListeners;

	static mouseMotionListeners _mouseMotionListeners;
	static dragListeners _leftDragListeners;
	static dragListeners _rightDragListeners;
	static dragListeners _middleDragListeners;

	static mouseButtonAnyListeners _mouseButtonAnyListeners;
	static mouseButtonListeners _leftButtonClickListeners;
	static mouseButtonListeners _leftButtonDoubleClickListeners;
	static mouseButtonListeners _leftButtonDownListeners;
	static mouseButtonListeners _leftButtonUpListeners;

	static mouseButtonListeners _rightButtonClickListeners;
	static mouseButtonListeners _rightButtonDownListeners;
	static mouseButtonListeners _rightButtonUpListeners;

	static mouseButtonListeners _middleButtonClickListeners;
	static mouseButtonListeners _middleButtonDownListeners;
	static mouseButtonListeners _middleButtonUpListeners;

	static charListeners _charListeners;

    static U32 _next_listener_id;
};

} //namespace pbj

#endif
