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

InputController::keyAllListeners InputController::_keyAllListeners;
InputController::keyListeners InputController::_keyDownListeners;
InputController::keyListeners InputController::_keyHeldListeners;
InputController::keyListeners InputController::_keyUpListeners;

InputController::scrollListeners InputController::_scrollListeners;

InputController::mouseMotionListeners InputController::_mouseMotionListeners;
InputController::dragListeners InputController::_leftDragListeners;
InputController::dragListeners InputController::_rightDragListeners;
InputController::dragListeners InputController::_middleDragListeners;

InputController::mouseButtonAnyListeners InputController::_mouseButtonAnyListeners;
InputController::mouseButtonListeners InputController::_leftButtonClickListeners;
InputController::mouseButtonListeners InputController::_leftButtonDoubleClickListeners;
InputController::mouseButtonListeners InputController::_leftButtonDownListeners;
InputController::mouseButtonListeners InputController::_leftButtonUpListeners;

InputController::mouseButtonListeners InputController::_rightButtonClickListeners;
InputController::mouseButtonListeners InputController::_rightButtonDownListeners;
InputController::mouseButtonListeners InputController::_rightButtonUpListeners;

InputController::mouseButtonListeners InputController::_middleButtonClickListeners;
InputController::mouseButtonListeners InputController::_middleButtonDownListeners;
InputController::mouseButtonListeners InputController::_middleButtonUpListeners;

InputController::charListeners InputController::_charListeners;

U32 InputController::_next_listener_id = 0;
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
		it->second(button, action, mods);
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
		it->second(x, y);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseScrollEvent(GLFWwindow* win, F64 xOffset, F64 yOffset)
{
	for(scrollListeners::iterator it=_scrollListeners.begin();
		it!=_scrollListeners.end();
		it++)
		it->second(xOffset, yOffset);
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
		it->second(key, scancode, action, mods);
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
		it->second(key, scancode, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseKeyHeldEvent(I32 key, I32 scancode, I32 mods)
{
	for(keyListeners::iterator it=_keyHeldListeners.begin();
		it!=_keyHeldListeners.end();
		it++)
		it->second(key, scancode, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseKeyUpEvent(I32 key, I32 scancode, I32 mods)
{
	for(keyListeners::iterator it=_keyUpListeners.begin();
		it!=_keyUpListeners.end();
		it++)
		it->second(key, scancode, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMotionLeftHeldEvent(F64 x, F64 y, I32 mods)
{
	for(dragListeners::iterator it=_leftDragListeners.begin();
		it!=_leftDragListeners.end();
		it++)
		it->second(x, y, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMotionRightHeldEvent(F64 x, F64 y, I32 mods)
{
	for(dragListeners::iterator it=_rightDragListeners.begin();
		it!=_rightDragListeners.end();
		it++)
		it->second(x, y, mods);
}

///////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMotionMiddleHeldEvent(F64 x, F64 y, I32 mods)
{
	for(dragListeners::iterator it=_middleDragListeners.begin();
		it!=_middleDragListeners.end();
		it++)
		it->second(x, y, mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonClickListeners.begin();
		it!=_leftButtonClickListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftDoubleClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonDoubleClickListeners.begin();
		it!=_leftButtonDoubleClickListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftDownEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonDownListeners.begin();
		it!=_leftButtonDownListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseLeftUpEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_leftButtonUpListeners.begin();
		it!=_leftButtonUpListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseRightClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_rightButtonClickListeners.begin();
		it!=_rightButtonClickListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseRightDownEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_rightButtonDownListeners.begin();
		it!=_rightButtonDownListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseRightUpEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_rightButtonUpListeners.begin();
		it!=_rightButtonUpListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMiddleClickEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_middleButtonClickListeners.begin();
		it!=_middleButtonClickListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMiddleDownEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_middleButtonDownListeners.begin();
		it!=_middleButtonDownListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
void InputController::raiseMouseMiddleUpEvent(I32 mods)
{
	for(mouseButtonListeners::iterator it=_middleButtonUpListeners.begin();
		it!=_middleButtonUpListeners.end();
		it++)
		it->second(mods);
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerKeyAllListener(keyAllListener l)
{
    U32 id = _next_listener_id++;
	_keyAllListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerKeyDownListener(keyListener l)
{
    U32 id = _next_listener_id++;
	_keyDownListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerKeyHeldListener(keyListener l)
{
    U32 id = _next_listener_id++;
	_keyHeldListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerKeyUpListener(keyListener l)
{
    U32 id = _next_listener_id++;
	_keyUpListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseButtonAnyListener(mouseButtonAnyListener l)
{
    U32 id = _next_listener_id++;
	_mouseButtonAnyListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseLeftClickListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_leftButtonClickListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseLeftDoubleClickListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_leftButtonDoubleClickListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseLeftDownListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_leftButtonDownListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseLeftUpListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_leftButtonUpListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseRightClickListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_rightButtonClickListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseRightDownListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_rightButtonDownListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseRightUpListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_rightButtonUpListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseMiddleClickListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_middleButtonClickListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseMiddleDownListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_middleButtonDownListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseMiddleUpListener(mouseButtonListener l)
{
    U32 id = _next_listener_id++;
	_middleButtonUpListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseMotionListener(mouseMotionListener l)
{
    U32 id = _next_listener_id++;
	_mouseMotionListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseMotionLeftHeldListener(dragListener l)
{
    U32 id = _next_listener_id++;
	_leftDragListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseMotionRightHeldListener(dragListener l)
{
    U32 id = _next_listener_id++;
	_rightDragListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerMouseMotionMiddleHeldListener(dragListener l)
{
    U32 id = _next_listener_id++;
	_middleDragListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerScrollListener(scrollListener l)
{
    U32 id = _next_listener_id++;
	_scrollListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 InputController::registerCharInputListener(charListener l)
{
    U32 id = _next_listener_id++;
	_charListeners.push_back(std::make_pair(id, l));
    return id;
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelKeyAllListener(U32 id)
{
    auto& c = _keyAllListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelKeyDownListener(U32 id)
{
    auto& c = _keyDownListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelKeyHeldListener(U32 id)
{
    auto& c = _keyHeldListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelKeyUpListener(U32 id)
{
    auto& c = _keyUpListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseButtonAnyListener(U32 id)
{
    auto& c = _mouseButtonAnyListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseLeftClickListener(U32 id)
{
    auto& c = _leftButtonClickListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseLeftDoubleClickListener(U32 id)
{
    auto& c = _leftButtonDoubleClickListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseLeftDownListener(U32 id)
{
    auto& c = _leftButtonDownListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseLeftUpListener(U32 id)
{
    auto& c = _leftButtonUpListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseRightClickListener(U32 id)
{
    auto& c = _rightButtonClickListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseRightDownListener(U32 id)
{
    auto& c = _rightButtonDownListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseRightUpListener(U32 id)
{
    auto& c = _rightButtonUpListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseMiddleClickListener(U32 id)
{
    auto& c = _middleButtonClickListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseMiddleDownListener(U32 id)
{
    auto& c = _middleButtonDownListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseMiddleUpListener(U32 id)
{
    auto& c = _middleButtonUpListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseMotionListener(U32 id)
{
    auto& c = _mouseMotionListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseMotionLeftHeldListener(U32 id)
{
    auto& c = _leftDragListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseMotionRightHeldListener(U32 id)
{
    auto& c = _rightDragListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelMouseMotionMiddleHeldListener(U32 id)
{
    auto& c = _middleDragListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelScrollListener(U32 id)
{
    auto& c = _scrollListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::cancelCharInputListener(U32 id)
{
    auto& c = _charListeners;
    for (auto i = c.begin(), end = c.end(); i != end; ++i)
    {
        if (i->first == id)
        {
            c.erase(i);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InputController::destroy()
{
    _initialized = false;
    _window = 0;

    _leftDown = false;
    _rightDown = false;
    _middleDown = false;
    _leftClicked = false;

    _leftMods = 0;
    _rightMods = 0;
    _middleMods = 0;

    _keyAllListeners.clear();
    _keyDownListeners.clear();
    _keyHeldListeners.clear();
    _keyUpListeners.clear();

    _scrollListeners.clear();

    _mouseMotionListeners.clear();
    _leftDragListeners.clear();
    _rightDragListeners.clear();
    _middleDragListeners.clear();

    _mouseButtonAnyListeners.clear();
    _leftButtonClickListeners.clear();
    _leftButtonDoubleClickListeners.clear();
    _leftButtonDownListeners.clear();
    _leftButtonUpListeners.clear();

    _rightButtonClickListeners.clear();
    _rightButtonDownListeners.clear();
    _rightButtonUpListeners.clear();

    _middleButtonClickListeners.clear();
    _middleButtonDownListeners.clear();
    _middleButtonUpListeners.clear();

    _charListeners.clear();

    _next_listener_id = 0;
}

} // namespace pbj
