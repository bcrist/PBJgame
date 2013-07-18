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
#ifndef INPUT_CONTROLLER_H_
#include "pbj/input_controller.h"
#endif

using namespace pbj;

bool InputController::_initialized = false;
GLFWwindow* InputController::_window = 0;

bool InputController::_leftDown = false;
bool InputController::_rightDown = false;
bool InputController::_middleDown = false;
bool InputController::_leftClicked = false;

I32 InputController::_leftMods = 0;
I32 InputController::_rightMods = 0;
I32 InputController::_middleMods = 0;

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

//I think I want to nest the action check inside the switch instead.  Just
//trying to get something down for right now.
void InputController::raiseMouseButtonEvent(GLFWwindow* win, I32 button, I32 action, I32 mods)
{
	if(action == GLFW_PRESS)
	{
		switch(button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			_leftDown = true;
			_leftMods = mods;
			raiseMouseLeftDownEvent(mods);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			_rightDown = true;
			_rightMods = mods;
			raiseMouseRightDownEvent(mods);
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			_middleDown = true;
			_middleMods = mods;
			raiseMouseMiddleDownEvent(mods);
			break;
		default:
			break;
		}
	}
	else //action==GLFW_RELEASE
	{
		switch(button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
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
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			_rightMods = mods;
			raiseMouseRightClickEvent(mods);
			raiseMouseRightUpEvent(mods);
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			_middleMods = mods;
			raiseMouseMiddleClickEvent(mods);
			raiseMouseMiddleUpEvent(mods);
			break;
		default:
			break;
		}
	}
}

void InputController::raiseMouseMotionEvent(GLFWwindow* win, F64 x, F64 y)
{
	if(_leftDown) { raiseMouseMotionLeftHeldEvent(x, y, _leftMods); }
	else if(_rightDown) { raiseMouseMotionRightHeldEvent(x, y, _rightMods); }
	else if(_middleDown) { raiseMouseMotionMiddleHeldEvent(x, y, _middleMods); }
}

void InputController::raiseScrollEvent(GLFWwindow* win, F64 xOffset, F64 yOffset)
{
}

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
}

void InputController::raiseCharInputEvent(GLFWwindow* win, U32 character)
{
}