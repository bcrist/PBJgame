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

#include "pbj/input_controller.h"

namespace pbj {

bool InputController::_initialized = false;
GLFWwindow* InputController::_window = 0;

bool InputController::_leftDown = false;
bool InputController::_rightDown = false;
bool InputController::_middleDown = false;
bool InputController::_leftClicked = false;

I32 InputController::_leftMods = 0;
I32 InputController::_rightMods = 0;
I32 InputController::_middleMods = 0;

InputController::keyAllListeners InputController::_keyAllListeners = keyAllListeners();
InputController::keyListeners InputController::_keyDownListeners = keyListeners();
InputController::keyListeners InputController::_keyHeldListeners = keyListeners();
InputController::keyListeners InputController::_keyUpListeners = keyListeners();

InputController::scrollListeners InputController::_scrollListeners = scrollListeners();

InputController::mouseMotionListeners InputController::_mouseMotionListeners = mouseMotionListeners();
InputController::dragListeners InputController::_leftDragListeners = dragListeners();
InputController::dragListeners InputController::_rightDragListeners = dragListeners();
InputController::dragListeners InputController::_middleDragListeners = dragListeners();

InputController::mouseButtonAnyListeners InputController::_mouseButtonAnyListeners = mouseButtonAnyListeners();
InputController::mouseButtonListeners InputController::_leftButtonClickListeners = mouseButtonListeners();
InputController::mouseButtonListeners InputController::_leftButtonDoubleClickListeners = mouseButtonListeners();
InputController::mouseButtonListeners InputController::_leftButtonDownListeners = mouseButtonListeners();
InputController::mouseButtonListeners InputController::_leftButtonUpListeners = mouseButtonListeners();

InputController::mouseButtonListeners InputController::_rightButtonClickListeners = mouseButtonListeners();
InputController::mouseButtonListeners InputController::_rightButtonDownListeners = mouseButtonListeners();
InputController::mouseButtonListeners InputController::_rightButtonUpListeners = mouseButtonListeners();

InputController::mouseButtonListeners InputController::_middleButtonClickListeners = mouseButtonListeners();
InputController::mouseButtonListeners InputController::_middleButtonDownListeners = mouseButtonListeners();
InputController::mouseButtonListeners InputController::_middleButtonUpListeners = mouseButtonListeners();
///////////////////////////////////////////////////////////////////////////////
void InputController::init(GLFWwindow* win)
{
	if(!_initialized)
	{
		_window = win;
		glfwSetMouseButtonCallback(_window, raiseMouseButtonEvent);
		glfwSetCursorPosCallback(_window, raiseMouseMotionEvent);
		glfwSetScrollCallback(_window, raiseScrollEvent);
		glfwSetKeyCallback(_window, raiseKeyboardEvent);
		glfwSetCharCallback(_window, raiseCharInputEvent);
		_initialized = true;
	}
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseButtonEvent(GLFWwindow* win, I32 button, I32 action, I32 mods)
{
	switch(button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if(action == GLFW_PRESS)
		{
			_leftDown = true;
			_leftMods = mods;
			raiseMouseLeftDownEvent(mods);
		}
		else //GLFW_RELEASE
		{
			if(_leftClicked)
			{
				raiseMouseLeftDoubleClickEvent(mods);
				_leftClicked = false;
			}
			else
			{
				_leftClicked = true;
			}
			_leftMods = mods;
			raiseMouseLeftClickEvent(mods);
			raiseMouseLeftUpEvent(mods);
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if(action == GLFW_PRESS)
		{
			_rightDown = true;
			_rightMods = mods;
			raiseMouseRightDownEvent(mods);
		}
		else //GLFW_RELEASE
		{
			_rightMods = mods;
			raiseMouseRightClickEvent(mods);
			raiseMouseRightUpEvent(mods);
		}
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		if(action == GLFW_PRESS)
		{
			_middleDown = true;
			_middleMods = mods;
			raiseMouseMiddleDownEvent(mods);
		}
		else //GLFW_RELEASE
		{
			_middleMods = mods;
			raiseMouseMiddleClickEvent(mods);
			raiseMouseMiddleUpEvent(mods);
		}
		break;
	default:
		break;
	}
	
	for(mouseButtonAnyListeners::iterator it=_mouseButtonAnyListeners.begin();
		it!=_mouseButtonAnyListeners.end();
		it++)
		(*it)(button, action, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMotionEvent(GLFWwindow* win, F64 x, F64 y)
{
	if(_leftDown) { raiseMouseMotionLeftHeldEvent(x, y, _leftMods); }
	else if(_rightDown) { raiseMouseMotionRightHeldEvent(x, y, _rightMods); }
	else if(_middleDown) { raiseMouseMotionMiddleHeldEvent(x, y, _middleMods); }
	
	for(mouseMotionListeners::iterator it=_mouseMotionListeners.begin();
		it!=_mouseMotionListeners.end();
		it++)
		(*it)(x, y);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseScrollEvent(GLFWwindow* win, F64 xOffset, F64 yOffset)
{
	for(scrollListeners::iterator it=_scrollListeners.begin();
		it!=_scrollListeners.end();
		it++)
		(*it)(xOffset, yOffset);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseKeyboardEvent(GLFWwindow*, I32 key, I32 scancode, I32 action, I32 mods)
{
	switch(action)
	{
	case GLFW_PRESS:
		raiseKeyDownEvent(key, scancode, mods);
		break;
	case GLFW_REPEAT:
		raiseKeyHeldEvent(key, scancode, mods);
		break;
	case GLFW_RELEASE:
		raiseKeyUpEvent(key, scancode, mods);
		break;
	default:
		break;
	}
	for(keyAllListeners::iterator it=_keyAllListeners.begin();
		it!=_keyAllListeners.end();
		it++)
		(*it)(key, scancode, action, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseCharInputEvent(GLFWwindow* win, U32 character)
{
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseKeyDownEvent(I32 key, I32 scancode, I32 mods)
{
	for(keyListeners::iterator it=_keyDownListeners.begin();
		it!=_keyDownListeners.end();
		it++)
		(*it)(key, scancode, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseKeyHeldEvent(I32 key, I32 scancode, I32 mods)
{
	for(keyListeners::iterator it=_keyHeldListeners.begin();
		it!=_keyHeldListeners.end();
		it++)
		(*it)(key, scancode, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseKeyUpEvent(I32 key, I32 scancode, I32 mods)
{
	for(keyListeners::iterator it=_keyUpListeners.begin();
		it!=_keyUpListeners.end();
		it++)
		(*it)(key, scancode, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMotionLeftHeldEvent(F64 x, F64 y, I32 mods)
{
	for(dragListeners::iterator it=_leftDragListeners.begin();
		it!=_leftDragListeners.end();
		it++)
		(*it)(x, y, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMotionRightHeldEvent(F64 x, F64 y, I32 mods)
{
	for(dragListeners::iterator it=_rightDragListeners.begin();
		it!=_rightDragListeners.end();
		it++)
		(*it)(x, y, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMotionMiddleHeldEvent(F64 x, F64 y, I32 mods)
{
	for(dragListeners::iterator it=_middleDragListeners.begin();
		it!=_middleDragListeners.end();
		it++)
		(*it)(x, y, mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonClickListeners.begin();
		it!=_leftButtonClickListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftDoubleClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonDoubleClickListeners.begin();
		it!=_leftButtonDoubleClickListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftDownEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonDownListeners.begin();
		it!=_leftButtonDownListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftUpEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonUpListeners.begin();
		it!=_leftButtonUpListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseRightClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_rightButtonClickListeners.begin();
		it!=_rightButtonClickListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseRightDownEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_rightButtonDownListeners.begin();
		it!=_rightButtonDownListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseRightUpEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_rightButtonUpListeners.begin();
		it!=_rightButtonUpListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMiddleClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_middleButtonClickListeners.begin();
		it!=_middleButtonClickListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMiddleDownEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_middleButtonDownListeners.begin();
		it!=_middleButtonDownListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMiddleUpEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_middleButtonUpListeners.begin();
		it!=_middleButtonUpListeners.end();
		it++)
		(*it)(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerKeyAllListener(keyAllListener l)
{
	_keyAllListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerKeyDownListener(keyListener l)
{
	_keyDownListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerKeyHeldListener(keyListener l)
{
	_keyHeldListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerKeyUpListener(keyListener l)
{
	_keyUpListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseButtonAnyListener(mouseButtonAnyListener l)
{
	_mouseButtonAnyListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseLeftClickListener(mouseButtonListener l)
{
	_leftButtonClickListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseLeftDoubleClickListener(mouseButtonListener l)
{
	_leftButtonDoubleClickListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseLeftDownListener(mouseButtonListener l)
{
	_leftButtonDownListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseLeftUpListener(mouseButtonListener l)
{
	_leftButtonUpListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseRightClickListener(mouseButtonListener l)
{
	_rightButtonClickListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseRightDownListener(mouseButtonListener l)
{
	_rightButtonDownListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseRightUpListener(mouseButtonListener l)
{
	_rightButtonUpListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseMiddleClickListener(mouseButtonListener l)
{
	_middleButtonClickListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseMiddleDownListener(mouseButtonListener l)
{
	_middleButtonDownListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseMiddleUpListener(mouseButtonListener l)
{
	_middleButtonUpListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseMotionListener(mouseMotionListener l)
{
	_mouseMotionListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseMotionLeftHeldListener(dragListener l)
{
	_leftDragListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseMotionRightHeldListener(dragListener l)
{
	_rightDragListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerMouseMotionMiddleHeldListener(dragListener l)
{
	_middleDragListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::registerScrollListener(scrollListener l)
{
	_scrollListeners.push_back(l);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::destroy()
{
//	if(_window != 0)
//		delete _window;
}

} // namespace pbj
