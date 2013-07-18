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
/// \file   /include/pbj/input_controller.h
/// \author Peter Bartosch
/// \date 2013-07-18
/// \brief  Intermediate class for input handling
/// \details InputController is a class consisting of static methods and
///          variables for grabbing GLFW input events and passing them to
///          registered observers.  While GLFW does support more input events
///          than those listed here, we are not concerned with joysticks at the
///          moment and therefore do not register to hear them.  This is setup
///          for use in only one window.  I do not anticipate that being an
///          issue.  The double-click detection will work poorly without having
///          a timer implemented.  C'est la vie.
#ifndef INPUT_CONTROLLER_H_
#define INPUT_CONTROLLER_H_

//I think I'm doing the include wrong here.  At this point using it to get off
//intellisense's radar.
#include "../vc11/include/GL/glfw3.h"
#include "_pbj.h"

namespace pbj
{
class InputController
{
public:
	static void init(GLFWwindow* win);

	static void raiseMouseButtonEvent(GLFWwindow*, I32, I32, I32); ///< Callback registered with GLFW
	static void raiseMouseMotionEvent(GLFWwindow*, F64, F64); ///< Callback registered with GLFW
	static void raiseScrollEvent(GLFWwindow*, F64, F64); ///< Callback registered with GLFW
	static void raiseKeyboardEvent(GLFWwindow*, I32, I32, I32, I32); ///< Callback registered with GLFW
	static void raiseCharInputEvent(GLFWwindow*, U32); ///< Callback registered with GLFW

	//Specific key events
	static void raiseKeyDownEvent(I32, I32, I32); ///< Event for key down
	static void raiseKeyHeldEvent(I32, I32, I32); ///< Event for key held
	static void raiseKeyUpEvent(I32, I32, I32); ///< Event for key being released

	//Specific mouse drag events
	static void raiseMouseMotionLeftHeldEvent(F64, F64, I32);
	static void raiseMouseMotionRightHeldEvent(F64, F64, I32);
	static void raiseMouseMotionMiddleHeldEvent(F64, F64, I32);
	
	//Specific left mouse click events
	static void raiseMouseLeftClickEvent(I32);
	static void raiseMouseLeftDoubleClickEvent(I32);
	static void raiseMouseLeftDownEvent(I32);
	static void raiseMouseLeftUpEvent(I32);
	
	//Specific right mouse click events
	static void raiseMouseRightClickEvent(I32);
	static void raiseMouseRightDownEvent(I32);
	static void raiseMouseRightUpEvent(I32);
	
	//Sepcific middle mouse click events
	static void raiseMouseMiddleClickEvent(I32);
	static void raiseMouseMiddleDownEvent(I32);
	static void raiseMouseMiddleUpEvent(I32);
	
private:
	static bool _initialized;
	static GLFWwindow* _window;

	static bool _leftDown;
	static bool _rightDown;
	static bool _middleDown;
	static bool _leftClicked; ///< Used to track double click
	static I32 _leftMods;
	static I32 _rightMods;
	static I32 _middleMods;

};

bool InputController::_initialized = false;
} //namespace pbj
#endif