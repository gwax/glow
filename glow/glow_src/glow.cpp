/*
===============================================================================

	FILE:  glow.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Basic classes in GLOW
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
	COPYRIGHT:
	
		Copyright (C) 1997-2000  Daniel Azuma  (dazuma@kagi.com)
		
		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2.1 of the License, or (at your option) any later version.
		
		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
		GNU Lesser General Public License for more details.
		
		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 0.95  (27 March 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
		7 March 2000 -- DA -- Fixed off-by-one bug in GlowMenu::RemoveItem()
	
===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <cstring>

#ifndef GLOW__H
	#include "glow.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowSubwindowParams GlowSubwindowParams::defaults(true);

GlowSubwindowParams::GlowSubwindowParams()
{
	*this = defaults;
}

GlowSubwindowParams::GlowSubwindowParams(bool)
{
	x = 0;
	y = 0;
	width = -1;
	height = -1;
	eventMask = Glow::noEvents;
	inactiveEventMask = Glow::noEvents;
	mode = Glow::rgbBuffer | Glow::doubleBuffer;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWindowParams GlowWindowParams::defaults(true);

GlowWindowParams::GlowWindowParams()
{
	*this = defaults;
}

GlowWindowParams::GlowWindowParams(bool)
{
	x = -1;
	y = -1;
	width = 100;
	height = 100;
	eventMask = Glow::noEvents;
	inactiveEventMask = Glow::noEvents;
	mode = Glow::rgbBuffer | Glow::doubleBuffer;
	title = "";
	iconTitle = 0;
}


/*
===============================================================================
	GLUT-integration idle function receiver
===============================================================================
*/

class Glow_IdleFuncReceiver :
	public GlowIdleReceiver
{
	friend class Glow;
	
	private:
	
		inline Glow_IdleFuncReceiver(
			void (*func)());
	
	protected:
	
		virtual void OnMessage(
			const GlowIdleMessage& message);
	
	private:
	
		void (*_funcPtr)();
};


inline Glow_IdleFuncReceiver::Glow_IdleFuncReceiver(
	void (*func)())
{
	_funcPtr = func;
}


void Glow_IdleFuncReceiver::OnMessage(
	const GlowIdleMessage& message)
{
	_funcPtr();
}


/*
===============================================================================
	Private globals for Glow
===============================================================================
*/

GLOW_STD::map<int, GlowSubwindow*> Glow::_windowRegistry;
GLOW_STD::map<int, GlowMenu*> Glow::_menuRegistry;

GLOW_STD::map<GlowComponent*, bool> Glow::_activateNotifyList;
GLOW_STD::map<GlowComponent*, bool> Glow::_closeList;
void (*Glow::_widgetNotifier)() = 0;
unsigned long Glow::_clock = 0;

GlowSubwindow* Glow::_menuWindow = 0;
int Glow::_menuXClick = -1;
int Glow::_menuYClick = -1;
bool Glow::_menuInUse = false;

GLOW_STD::vector<GlowWindow*> Glow::_modalWindows;

TSender<const GlowIdleMessage&> Glow::_idleSender;
GLOW_STD::map<int, TSender<const GlowTimerMessage&>*> Glow::_timerSenders;
int Glow::_nextTimerID = 1;

Glow_IdleFuncReceiver* Glow::_idleFuncReceiver = 0;


/*
===============================================================================
	Methods for Glow
===============================================================================
*/

double Glow::Version()
{
	return GLOW_VERSION;
}


void Glow::_RemoveWindow(
	int windowNum)
{
	// Find entry in registry
	_WindowRegistryIterator iter = _windowRegistry.find(windowNum);
	if (iter != _windowRegistry.end())
	{
		// Are there modal windows?
		if (!_modalWindows.empty())
		{
			GlowSubwindow* wind = (*iter).second;
			// Is window to remove the current top modal window?
			if (wind == _modalWindows.back())
			{
				PopModalWindow();
			}
			else
			{
				// Is window to remove in the modal window stack at all?
				for (GLOW_STD::vector<GlowWindow*>::iterator miter = _modalWindows.begin();
					miter != _modalWindows.end(); ++miter)
				{
					if (*miter == wind)
					{
						_modalWindows.erase(miter);
						break;
					}
				}
			}
		}
		_windowRegistry.erase(iter);
	}
}


/*
===============================================================================
	Idle functions
===============================================================================
*/

void Glow::RegisterIdle(
	GlowIdleReceiver* idle)
{
	GLOW_DEBUGSCOPE("Glow::RegisterIdle");
	
	if (!_idleSender.IsBoundTo(idle))
	{
		_idleSender.Bind(idle);
		if (_idleSender.NumReceivers() == 1)
		{
			::glutIdleFunc(_IdleFunc);
		}
	}
}


void Glow::UnregisterIdle(
	GlowIdleReceiver* idle)
{
	GLOW_DEBUGSCOPE("Glow::UnregisterIdle");
	
	if (_idleSender.IsBoundTo(idle))
	{
		_idleSender.Unbind(idle);
		if (_idleSender.NumReceivers() == 0)
		{
			::glutIdleFunc(0);
		}
	}
}


void Glow::SetIdleFunc(
	void (*func)())
{
	GLOW_DEBUGSCOPE("Glow::SetIdleFunc");
	
	if (func == 0)
	{
		if (_idleFuncReceiver != 0)
		{
			UnregisterIdle(_idleFuncReceiver);
			delete _idleFuncReceiver;
			_idleFuncReceiver = 0;
		}
	}
	else
	{
		if (_idleFuncReceiver != 0)
		{
			_idleFuncReceiver->_funcPtr = func;
		}
		else
		{
			_idleFuncReceiver = new Glow_IdleFuncReceiver(func);
			RegisterIdle(_idleFuncReceiver);
		}
	}
}


/*
===============================================================================
	Timer functions
===============================================================================
*/

int Glow::RegisterTimer(
	unsigned int msecs,
	GlowTimerReceiver* timer)
{
	GLOW_DEBUGSCOPE("Glow::RegisterTimer");
	
	int timerID = _nextTimerID;
	while (_timerSenders.find(timerID) != _timerSenders.end() || timerID == 0)
	{
		++timerID;
	}
	_nextTimerID = timerID + 1;
	TSender<const GlowTimerMessage&>* sender = new TSender<const GlowTimerMessage&>;
	sender->Bind(timer);
	_timerSenders[timerID] = sender;
	::glutTimerFunc(msecs, _TimerFunc, timerID);
	return timerID;
}


void Glow::UnregisterTimer(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::UnregisterTimer");
	
	GLOW_STD::map<int, TSender<const GlowTimerMessage&>*>::iterator iter = _timerSenders.find(id);
	if (iter != _timerSenders.end())
	{
		delete (*iter).second;
		_timerSenders.erase(iter);
	}
}


/*
===============================================================================
	Callbacks
===============================================================================
*/

void Glow::_TimerFunc(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::_TimerFunc");
	
	++_clock;
	GLOW_STD::map<int, TSender<const GlowTimerMessage&>*>::iterator iter = _timerSenders.find(id);
	if (iter != _timerSenders.end())
	{
		TSender<const GlowTimerMessage&>* sender = (*iter).second;
		_timerSenders.erase(iter);
		sender->Send(GlowTimerMessage(id));
		delete sender;
	}
	_ExecuteDeferred();
}


void Glow::_DisplayFunc()
{
	GLOW_DEBUGSCOPE("Glow::_DisplayFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->_BroadcastPaint();
		window->_FinishRender();
	}
	_ExecuteDeferred();
}


void Glow::_ReshapeFunc(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("Glow::_ReshapeFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->_width = width;
		window->_height = height;
		window->OnReshape(width, height);
	}
	_ExecuteDeferred();
}


void Glow::_KeyboardFunc(
	unsigned char key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_KeyboardFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnKeyboard(int(key), ::glutGetModifiers(), x, y);
	}
	_ExecuteDeferred();
}


void Glow::_MouseFunc(
	int button,
	int state,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_MouseFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	int modifiers = ::glutGetModifiers();
	if (window != 0)
	{
		if (state == GLUT_DOWN)
		{
			window->OnMouseDown(button, x, y, modifiers);
		}
		else if (state == GLUT_UP)
		{
			window->OnMouseUp(button, x, y, modifiers);
		}
	}
	_ExecuteDeferred();
}


void Glow::_MotionFunc(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_MotionFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnMouseDrag(x, y);
	}
	_ExecuteDeferred();
}


void Glow::_PassiveMotionFunc(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_PassiveMotionFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnMouseMotion(x, y);
	}
	_ExecuteDeferred();
}


void Glow::_VisibilityFunc(
	int state)
{
	GLOW_DEBUGSCOPE("Glow::_VisibilityFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		if (state == GLUT_VISIBLE)
		{
			window->OnVisible();
		}
		else if (state == GLUT_NOT_VISIBLE)
		{
			window->OnInvisible();
		}
	}
	_ExecuteDeferred();
}


void Glow::_EntryFunc(
	int state)
{
	GLOW_DEBUGSCOPE("Glow::_EntryFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		if (state == GLUT_ENTERED)
		{
			window->OnMouseEnter();
		}
		else if (state == GLUT_LEFT)
		{
			window->OnMouseExit();
		}
	}
	_ExecuteDeferred();
}


void Glow::_SpecialFunc(
	int key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_SpecialFunc");
	
	++_clock;
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnKeyboard(key+Glow::specialKeyOffset,
			::glutGetModifiers(), x, y);
	}
	_ExecuteDeferred();
}


void Glow::_MenuStatusFunc(
	int status,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_MenuStatusFunc");
	
	++_clock;
	if (status == GLUT_MENU_IN_USE)
	{
		_menuWindow = ResolveWindow(::glutGetWindow());
		_menuXClick = x;
		_menuYClick = y;
		_menuInUse = true;
		if (_menuWindow != 0)
		{
			_menuWindow->OnMenuDown(x, y);
		}
	}
	else if (status == GLUT_MENU_NOT_IN_USE)
	{
		if (_menuWindow != 0)
		{
			_menuWindow->OnMenuUp();
		}
		_menuInUse = false;
	}
	_ExecuteDeferred();
}


void Glow::_MenuFunc(
	int value)
{
	GLOW_DEBUGSCOPE("Glow::_MenuFunc");
	
	++_clock;
	GlowMenu* menu = ResolveMenu(::glutGetMenu());
	if (menu != 0)
	{
		menu->OnHit(value, _menuWindow, _menuXClick, _menuYClick);
	}
	_ExecuteDeferred();
}


void Glow::_IdleFunc()
{
	GLOW_DEBUGSCOPE("Glow::_IdleFunc");
	
	++_clock;
	_idleSender.Send(GlowIdleMessage());
	_ExecuteDeferred();
}


/*
===============================================================================
	Finalizers (deferred execution)
===============================================================================
*/

void Glow::_ExecuteDeferred()
{
	GLOW_DEBUGSCOPE("Glow::_ExecuteDeferred");
	
	// Widget notifier
	if (_widgetNotifier != 0)
	{
		_widgetNotifier();
	}
	
	// Deferred activation and deactivation
	for (GLOW_STD::map<GlowComponent*, bool>::iterator iter = _activateNotifyList.begin();
		iter != _activateNotifyList.end(); ++iter)
	{
		if ((*iter).second)
		{
			(*iter).first->OnActivate();
		}
		else
		{
			(*iter).first->OnDeactivate();
		}
	}
	_activateNotifyList.clear();
	
	// Deferred deletion of components
	while (!_closeList.empty())
	{
		delete (*(_closeList.begin())).first;
	}
}


/*
===============================================================================
	Modal window management
===============================================================================
*/

void Glow::PushModalWindow(
	GlowWindow* wind)
{
	GLOW_DEBUGSCOPE("Glow::PushModalWindow");
	
	if (_modalWindows.empty())
	{
		for (_WindowRegistryIterator iter = _windowRegistry.begin();
			iter != _windowRegistry.end(); iter++)
		{
			GlowSubwindow* subwind = (*iter).second;
			if (subwind->IsTopLevel() && subwind != wind)
			{
				subwind->_BroadcastStandby(false, false);
			}
		}
	}
	else
	{
		_modalWindows.back()->_BroadcastStandby(false, false);
		wind->_BroadcastStandby(true, false);
	}
	_modalWindows.push_back(wind);
	wind->Raise();
}


void Glow::PopModalWindow()
{
	GLOW_DEBUGSCOPE("Glow::PopModalWindow");
	
	GlowWindow* wind = _modalWindows.back();
	_modalWindows.pop_back();
	if (_modalWindows.empty())
	{
		for (_WindowRegistryIterator iter = _windowRegistry.begin();
			iter != _windowRegistry.end(); iter++)
		{
			GlowSubwindow* subwind = (*iter).second;
			if (subwind->IsTopLevel() && subwind != wind)
			{
				subwind->_BroadcastStandby(true, false);
			}
		}
	}
	else
	{
		wind->_BroadcastStandby(false, false);
		_modalWindows.back()->_BroadcastStandby(true, false);
		_modalWindows.back()->Raise();
	}
}


/*
===============================================================================
	Methods for GlowComponent
===============================================================================
*/

void GlowComponent::Init(
	GlowComponent* parent)
{
	GLOW_DEBUGSCOPE("GlowComponent::Init");
	
	_parent = parent;
	_activeState = 1;
	if (_parent != 0)
	{
		_parent->_AddChild(this);
		if (!_parent->IsActive())
		{
			_activeState = 2;
		}
	}
}


GlowComponent::~GlowComponent()
{
	GLOW_DEBUGSCOPE("GlowComponent::~GlowComponent");
	
	KillChildren();
	if (_parent != 0)
	{
		_parent->_RemoveChild(this);
	}
	GLOW_STD::map<GlowComponent*, bool>::iterator iter =
		Glow::_activateNotifyList.find(this);
	if (iter != Glow::_activateNotifyList.end())
	{
		Glow::_activateNotifyList.erase(iter);
	}
	iter = Glow::_closeList.find(this);
	if (iter != Glow::_closeList.end())
	{
		Glow::_closeList.erase(iter);
	}
}


void GlowComponent::Close()
{
	GLOW_DEBUGSCOPE("GlowComponent::Close");
	
	GLOW_STD::map<GlowComponent*, bool>::iterator iter =
		Glow::_closeList.find(this);
	if (iter == Glow::_closeList.end())
	{
		Glow::_closeList.insert(
			GLOW_STD::pair<GlowComponent* const, bool>(this, true));
	}
}


void GlowComponent::_BroadcastPaint()
{
	GLOW_DEBUGSCOPE("GlowComponent::_BroadcastPaint");
	
	if (OnBeginPaint())
	{
		for (ChildIterator iter = _children.begin(); iter != _children.end(); iter++)
		{
			if (dynamic_cast<GlowSubwindow*>(*iter) == 0)
			{
				(*iter)->_BroadcastPaint();
			}
		}
	}
	OnEndPaint();
}


void GlowComponent::_BroadcastStandby(
	bool activating,
	bool first)
{
	GLOW_DEBUGSCOPE("GlowComponent::_BroadcastStandby");
	
	bool changing = false;
	
	if (activating && (_activeState == 2 || (first && _activeState == 0)))
	{
		changing = true;
		_activeState = 1;
	}
	else if (!activating && _activeState == 1)
	{
		changing = true;
		_activeState = (first ? 0 : 2);
	}
	if (changing)
	{
		GlowSubwindow* subwindow = dynamic_cast<GlowSubwindow*>(this);
		if (subwindow != 0)
		{
			subwindow->_EventsForActivation(activating);
		}
		GLOW_STD::map<GlowComponent*, bool>::iterator iter =
			Glow::_activateNotifyList.find(this);
		if (iter != Glow::_activateNotifyList.end())
		{
			if (activating != (*iter).second)
			{
				Glow::_activateNotifyList.erase(iter);
			}
		}
		else
		{
			Glow::_activateNotifyList.insert(
				GLOW_STD::pair<GlowComponent* const, bool>(this, activating));
		}
		for (ChildIterator iter = _children.begin(); iter != _children.end(); iter++)
		{
			(*iter)->_BroadcastStandby(activating, false);
		}
		WhichWindow()->Refresh();
	}
}


void GlowComponent::Activate()
{
	GLOW_DEBUGSCOPE("GlowComponent::Activate");
	
	if (_activeState == 0)
	{
		if ((IsTopLevel() && (Glow::NumModalWindows() == 0 || Glow::TopModalWindow() == this)) ||
			(!IsTopLevel() && Parent()->IsActive()))
		{
			_BroadcastStandby(true, true);
		}
		else
		{
			_activeState = 2;
		}
	}
}


void GlowComponent::Deactivate()
{
	GLOW_DEBUGSCOPE("GlowComponent::Deactivate");
	
	if (_activeState == 1)
	{
		_BroadcastStandby(false, true);
	}
	else if (_activeState == 2)
	{
		_activeState = 0;
	}
}


GlowSubwindow* GlowComponent::WhichWindow()
{
	return _parent->WhichWindow();
}


GlowWindow* GlowComponent::ToplevelWindow()
{
	return (_parent == 0) ? static_cast<GlowWindow*>(this) : _parent->ToplevelWindow();
}


void GlowComponent::KillChildren()
{
	GLOW_DEBUGSCOPE("GlowComponent::KillChildren");
	
	ChildIterator iter;
	while ((iter = _children.begin()) != _children.end())
	{
		delete (*iter);
	}
}


bool GlowComponent::OnBeginPaint()
{
	return true;
}


void GlowComponent::OnEndPaint()
{
}


void GlowComponent::OnActivate()
{
}


void GlowComponent::OnDeactivate()
{
}


/*
===============================================================================
	Methods for GlowSubwindow
===============================================================================
*/


void GlowSubwindow::Init(
	GlowComponent* parent,
	const GlowSubwindowParams& params)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Init");
	
	GLOW_DEBUG(params.width < parentWindowSize || params.width == 0,
		"Subwindow width too small");
	GLOW_DEBUG(params.height < parentWindowSize || params.height == 0,
		"Subwindow height too small");
	
	GlowComponent::Init(parent);
	::glutInitDisplayMode(params.mode);
	_width = params.width;
	if (_width == parentWindowSize)
	{
		_width = parent->WhichWindow()->Width() - params.x;
	}
	_height = params.height;
	if (_height == parentWindowSize)
	{
		_height = parent->WhichWindow()->Height() - params.y;
	}
	_windowNum = ::glutCreateSubWindow(parent->WhichWindow()->_windowNum,
		params.x, params.y, _width, _height);
	_eventMask = params.eventMask;
	_inactiveEventMask = params.inactiveEventMask;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_RegisterCallbacks(_eventMask);
	_saveCursor = GLUT_CURSOR_INHERIT;
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((params.mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
}


void GlowSubwindow::Init(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	int mode,
	int eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Init");
	
	GLOW_DEBUG(width < parentWindowSize || width == 0,
		"Subwindow width too small");
	GLOW_DEBUG(height < parentWindowSize || height == 0,
		"Subwindow height too small");
	
	GlowComponent::Init(parent);
	::glutInitDisplayMode(mode);
	if (width == parentWindowSize)
	{
		width = parent->WhichWindow()->Width() - x;
	}
	if (height == parentWindowSize)
	{
		height = parent->WhichWindow()->Height() - y;
	}
	_windowNum = ::glutCreateSubWindow(parent->WhichWindow()->_windowNum,
		x, y, width, height);
	_width = width;
	_height = height;
	_eventMask = eventMask;
	_inactiveEventMask = GlowSubwindowParams::defaults.inactiveEventMask;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_RegisterCallbacks(_eventMask);
	_saveCursor = GLUT_CURSOR_INHERIT;
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
}


GlowSubwindow::~GlowSubwindow()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::~GlowSubwindow");
	
	KillChildren();
	Glow::_RemoveWindow(_windowNum);
	::glutDestroyWindow(_windowNum);
}


void GlowSubwindow::_RegisterCallbacks(
	int eventMask)
{
	::glutDisplayFunc(Glow::_DisplayFunc);
	::glutReshapeFunc(Glow::_ReshapeFunc);
	
	if (eventMask & Glow::keyboardEvents)
	{
		::glutKeyboardFunc(Glow::_KeyboardFunc);
		::glutSpecialFunc(Glow::_SpecialFunc);
	}
	else
	{
		::glutKeyboardFunc(0);
		::glutSpecialFunc(0);
	}
	if (eventMask & Glow::mouseEvents)
	{
		::glutMouseFunc(Glow::_MouseFunc);
	}
	else
	{
		::glutMouseFunc(0);
	}
	if (eventMask & Glow::dragEvents)
	{
		::glutMotionFunc(Glow::_MotionFunc);
	}
	else
	{
		::glutMotionFunc(0);
	}
	if (eventMask & Glow::motionEvents)
	{
		::glutPassiveMotionFunc(Glow::_PassiveMotionFunc);
	}
	else
	{
		::glutPassiveMotionFunc(0);
	}
	if (eventMask & Glow::visibilityEvents)
	{
		::glutVisibilityFunc(Glow::_VisibilityFunc);
	}
	else
	{
		::glutVisibilityFunc(0);
	}
	if (eventMask & Glow::focusEvents)
	{
		::glutEntryFunc(Glow::_EntryFunc);
	}
	else
	{
		::glutEntryFunc(0);
	}
	if (eventMask & Glow::menuEvents)
	{
		int saveMenu = ::glutGetMenu();
		if (_leftMenu != 0)
		{
			::glutSetMenu(_leftMenu->_menuNum);
			::glutAttachMenu(Glow::leftButton);
		}
		if (_centerMenu != 0)
		{
			::glutSetMenu(_centerMenu->_menuNum);
			::glutAttachMenu(Glow::middleButton);
		}
		if (_rightMenu != 0)
		{
			::glutSetMenu(_rightMenu->_menuNum);
			::glutAttachMenu(Glow::rightButton);
		}
		if (saveMenu != 0)
		{
			::glutSetMenu(saveMenu);
		}
	}
	else
	{
		if (_leftMenu != 0)
		{
			::glutDetachMenu(Glow::leftButton);
		}
		if (_centerMenu != 0)
		{
			::glutDetachMenu(Glow::middleButton);
		}
		if (_rightMenu != 0)
		{
			::glutDetachMenu(Glow::rightButton);
		}
	}
}


GlowSubwindow* GlowSubwindow::WhichWindow()
{
	return this;
}


bool GlowSubwindow::OnBeginPaint()
{
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}


void GlowSubwindow::OnReshape(
	int width,
	int height)
{
	::glViewport(0, 0, width, height);
}


void GlowSubwindow::OnMouseEnter()
{
}


void GlowSubwindow::OnMouseExit()
{
}


void GlowSubwindow::OnMouseDown(
	int button,
	int x,
	int y,
	int modifiers)
{
}


void GlowSubwindow::OnMouseUp(
	int button,
	int x,
	int y,
	int modifiers)
{
}


void GlowSubwindow::OnMenuDown(
	int x,
	int y)
{
}


void GlowSubwindow::OnMenuUp()
{
}


void GlowSubwindow::OnMouseMotion(
	int x,
	int y)
{
}


void GlowSubwindow::OnMouseDrag(
	int x,
	int y)
{
}


void GlowSubwindow::OnKeyboard(
	int key,
	int modifiers,
	int x,
	int y)
{
}


void GlowSubwindow::OnVisible()
{
}


void GlowSubwindow::OnInvisible()
{
}


void GlowSubwindow::OnDirectMenuHit(
	const GlowMenuMessage& message)
{
}


void GlowSubwindow::SetEventMask(
	int eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetEventMask");
	
	_eventMask = eventMask;
	if (IsActive())
	{
		int saveWind = ::glutGetWindow();
		::glutSetWindow(_windowNum);
		_RegisterCallbacks(eventMask);
		if (saveWind != 0)
		{
			::glutSetWindow(saveWind);
		}
	}
}


void GlowSubwindow::SetInactiveEventMask(
	int eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetInactiveEventMask");
	
	_inactiveEventMask = eventMask;
	if (!IsActive())
	{
		int saveWind = ::glutGetWindow();
		::glutSetWindow(_windowNum);
		_RegisterCallbacks(eventMask);
		if (saveWind != 0)
		{
			::glutSetWindow(saveWind);
		}
	}
}


void GlowSubwindow::Refresh()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Refresh");
	
	if (_refreshEnabled)
	{
		int saveWind = ::glutGetWindow();
		::glutSetWindow(_windowNum);
		::glutPostRedisplay();
		if (saveWind != 0)
		{
			::glutSetWindow(saveWind);
		}
	}
}


void GlowSubwindow::Move(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Move");
	
	_clock = Glow::_clock;
	_globalXPos = x;
	_globalYPos = y;
	GlowSubwindow* parent = ParentWindow();
	if (parent != 0)
	{
		_globalXPos += parent->GlobalPositionX();
		_globalYPos += parent->GlobalPositionY();
	}
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutPositionWindow(x, y);
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Reshape(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Reshape");
	
	GLOW_ASSERT(width > 0);
	GLOW_ASSERT(height > 0);
	
	_width = width;
	_height = height;
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutReshapeWindow(width, height);
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Raise()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Raise");
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutPopWindow();
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Lower()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Lower");
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutPushWindow();
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Show()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Show");
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutShowWindow();
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Hide()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Hide");
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutHideWindow();
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::SetCursor(
	int cursor)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetCursor");
	
	_saveCursor = cursor;
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutSetCursor(cursor);
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::SetMenu(
	int button,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetMenu");
	
	GLOW_DEBUG(button!=Glow::leftButton && button!=Glow::middleButton &&
		button!=Glow::rightButton, "Bad button value");
	
	if (button == Glow::leftButton)
	{
		_leftMenu = menu;
	}
	else if (button == Glow::middleButton)
	{
		_centerMenu = menu;
	}
	else //if (button == Glow::rightButton)
	{
		_rightMenu = menu;
	}
	if (menu != 0)
	{
		if ((IsActive() && (_eventMask & Glow::menuEvents) != 0) ||
			(!IsActive() && (_inactiveEventMask & Glow::menuEvents) != 0))
		{
			int saveWind = ::glutGetWindow();
			int saveMenu = ::glutGetMenu();
			::glutSetWindow(_windowNum);
			::glutSetMenu(menu->_menuNum);
			::glutAttachMenu(button);
			if (saveWind != 0)
			{
				::glutSetWindow(saveWind);
			}
			if (saveMenu != 0)
			{
				::glutSetMenu(saveMenu);
			}
		}
	}
	else
	{
		int saveWind = ::glutGetWindow();
		::glutSetWindow(_windowNum);
		::glutDetachMenu(button);
		if (saveWind != 0)
		{
			::glutSetWindow(saveWind);
		}
	}
}


int GlowSubwindow::GlutInfo(
	GLenum key) const
{
	GLOW_DEBUGSCOPE("GlowSubwindow::GlutInfo");
	
	int ret;
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	ret = ::glutGet(key);
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
	return ret;
}


void GlowSubwindow::_EventsForActivation(
	bool activating)
{
	if (activating)
	{
		int saveWind = ::glutGetWindow();
		::glutSetWindow(_windowNum);
		::glutSetCursor(_saveCursor);
		_RegisterCallbacks(_eventMask);
		if (saveWind != 0)
		{
			::glutSetWindow(saveWind);
		}
	}
	else
	{
		int saveWind = ::glutGetWindow();
		::glutSetWindow(_windowNum);
		::glutSetCursor(GLUT_CURSOR_INHERIT);
		_RegisterCallbacks(_inactiveEventMask);
		if (saveWind != 0)
		{
			::glutSetWindow(saveWind);
		}
	}
	Refresh();
}


/*
===============================================================================
	Methods for GlowWindow
===============================================================================
*/

void GlowWindow::Init(
	const GlowWindowParams& params)
{
	GLOW_DEBUGSCOPE("GlowWindow::Init");
	
	GLOW_ASSERT(params.width > 0);
	GLOW_ASSERT(params.height > 0);
	
	GlowComponent::Init(0);
	_eventMask = params.eventMask;
	_inactiveEventMask = params.inactiveEventMask;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_saveCursor = GLUT_CURSOR_INHERIT;
	_width = params.width;
	_height = params.height;
	_title = new char[GLOW_CSTD::strlen(params.title)+1];
	GLOW_CSTD::strcpy(_title, params.title);
	if (params.iconTitle == 0)
	{
		_iconTitle = new char[GLOW_CSTD::strlen(params.title)+1];
		GLOW_CSTD::strcpy(_iconTitle, params.title);
	}
	else
	{
		_iconTitle = new char[GLOW_CSTD::strlen(params.iconTitle)+1];
		GLOW_CSTD::strcpy(_iconTitle, params.iconTitle);
	}
	::glutInitWindowSize(params.width, params.height);
	::glutInitWindowPosition(params.x, params.y);
	::glutInitDisplayMode(params.mode);
	_windowNum = ::glutCreateWindow(_title);
	::glutSetIconTitle(_iconTitle);
	_RegisterCallbacks(params.eventMask);
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((params.mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
	if (Glow::NumModalWindows() != 0)
	{
		_activeState = 2;
		Glow::TopModalWindow()->Raise();
	}
}


void GlowWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	int mode,
	int eventMask)
{
	GLOW_DEBUGSCOPE("GlowWindow::Init");
	
	GLOW_ASSERT(width > 0);
	GLOW_ASSERT(height > 0);
	
	GlowComponent::Init(0);
	_eventMask = eventMask;
	_inactiveEventMask = GlowWindowParams::defaults.inactiveEventMask;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_saveCursor = GLUT_CURSOR_INHERIT;
	_width = width;
	_height = height;
	int len = GLOW_CSTD::strlen(title)+1;
	_title = new char[len];
	GLOW_CSTD::strcpy(_title, title);
	_iconTitle = new char[len];
	GLOW_CSTD::strcpy(_iconTitle, title);
	::glutInitWindowSize(width, height);
	::glutInitWindowPosition(x, y);
	::glutInitDisplayMode(mode);
	_windowNum = ::glutCreateWindow(_title);
	_RegisterCallbacks(eventMask);
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
	if (Glow::NumModalWindows() != 0)
	{
		_activeState = 2;
		Glow::TopModalWindow()->Raise();
	}
}


void GlowWindow::Maximize()
{
	GLOW_DEBUGSCOPE("GlowWindow::Maximize");
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutFullScreen();
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowWindow::Iconify()
{
	GLOW_DEBUGSCOPE("GlowWindow::Iconify");
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	::glutIconifyWindow();
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowWindow::SetTitle(
	const char* name)
{
	GLOW_DEBUGSCOPE("GlowWindow::SetTitle");
	
	GLOW_ASSERT(name != 0);
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	delete[] _title;
	_title = new char[GLOW_CSTD::strlen(name)+1];
	GLOW_CSTD::strcpy(_title, name);
	::glutSetWindowTitle(_title);
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowWindow::SetIconTitle(
	const char* name)
{
	GLOW_DEBUGSCOPE("GlowWindow::SetIconTitle");
	
	GLOW_ASSERT(name != 0);
	
	int saveWind = ::glutGetWindow();
	::glutSetWindow(_windowNum);
	delete[] _iconTitle;
	_iconTitle = new char[GLOW_CSTD::strlen(name)+1];
	GLOW_CSTD::strcpy(_iconTitle, name);
	::glutSetIconTitle(_iconTitle);
	if (saveWind != 0)
	{
		::glutSetWindow(saveWind);
	}
}


/*
===============================================================================
	Methods for GlowMenu
===============================================================================
*/

GlowMenu::GlowMenu()
{
	GLOW_DEBUGSCOPE("GlowMenu::GlowMenu");
	
	_menuNum = ::glutCreateMenu(Glow::_MenuFunc);
	Glow::_AddMenu(this, _menuNum);
	_bindState = bindNormal;
}


GlowMenu::~GlowMenu()
{
	GLOW_DEBUGSCOPE("GlowMenu::~GlowMenu");
	
	Glow::_RemoveMenu(_menuNum);
	::glutDestroyMenu(_menuNum);
	for (ItemIterator iter = _itemData.begin(); iter != _itemData.end(); ++iter)
	{
		delete[] (*iter).label;
		delete[] (*iter).mark;
	}
}


void GlowMenu::OnHit(
	int code,
	GlowSubwindow* window,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowMenu::OnHit");
	
	if (_bindState != bindNone)
	{
		GlowMenuMessage msg;
		msg.code = code;
		msg.menu = this;
		msg.window = window;
		msg.x = x;
		msg.y = y;
		if (_bindState == bindSubwindow)
		{
			window->OnDirectMenuHit(msg);
		}
		else
		{
			_sender.Send(msg);
		}
	}
}


void GlowMenu::_SetupItem(
	int itemNum,
	int untilNum) const
{
	if (untilNum == -1) untilNum = itemNum+1;
	char buf[200];
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	ItemConstIterator iter = _itemData.begin()+itemNum;
	for (int i=itemNum; i<untilNum; ++i)
	{
		buf[0] = 0;
		if ((*iter).mark != 0)
		{
			GLOW_CSTD::strcpy(buf, (*iter).mark);
		}
		GLOW_CSTD::strcat(buf, (*iter).label);
		if ((*iter).subMenu != 0)
		{
			if (i < ::glutGet(GLenum(GLUT_MENU_NUM_ITEMS)))
			{
				::glutChangeToSubMenu(i+1, buf, (*iter).subMenu->_menuNum);
			}
			else
			{
				::glutAddSubMenu(buf, (*iter).subMenu->_menuNum);
			}
		}
		else
		{
			if (i < ::glutGet(GLenum(GLUT_MENU_NUM_ITEMS)))
			{
				::glutChangeToMenuEntry(i+1, buf, (*iter).code);
			}
			else
			{
				::glutAddMenuEntry(buf, (*iter).code);
			}
		}
		++iter;
	}
	
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


void GlowMenu::AddEntry(
	const char* label,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::AddEntry");
	
	Glow_MenuItem item;
	int labellen = GLOW_CSTD::strlen(label);
	item.label = new char[labellen+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = code;
	item.subMenu = 0;
	_itemData.push_back(item);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	::glutAddMenuEntry((char*)label, code);
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


void GlowMenu::AddSubmenu(
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::AddSubmenu");
	
	Glow_MenuItem item;
	int labellen = GLOW_CSTD::strlen(label);
	item.label = new char[labellen+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = 0;
	item.subMenu = menu;
	_itemData.push_back(item);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	::glutAddSubMenu((char*)label, menu->_menuNum);
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


void GlowMenu::InsertEntry(
	int itemNum,
	const char* label,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::InsertEntry");
	GLOW_DEBUG(itemNum<0 || itemNum>int(_itemData.size()),
		"itemNum out of range in GlowMenu::InsertEntry");
	
	Glow_MenuItem item;
	item.label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = code;
	item.subMenu = 0;
	_itemData.insert(_itemData.begin()+itemNum, item);
	
	_SetupItem(itemNum, _itemData.size());
}


void GlowMenu::InsertSubmenu(
	int itemNum,
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::InsertSubmenu");
	GLOW_DEBUG(itemNum<0 || itemNum>int(_itemData.size()),
		"itemNum out of range in GlowMenu::InsertSubmenu");
	
	Glow_MenuItem item;
	item.label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = 0;
	item.subMenu = menu;
	_itemData.insert(_itemData.begin()+itemNum, item);
	
	_SetupItem(itemNum, _itemData.size());
}


void GlowMenu::RemoveItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowMenu::RemoveItem");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	delete[] (*iter).mark;
	_itemData.erase(iter);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	::glutRemoveMenuItem(itemNum+1);
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


int GlowMenu::NextMarkedItem(
	int start) const
{
	GLOW_DEBUGSCOPE("GlowMenu::NextMarkedItem");
	
	GLOW_DEBUG(start<-1, "starting itemnum out of range");
	
	int max = _itemData.size();
	for (int i=start+1; i<max; i++)
	{
		if (_itemData[i].mark != 0)
			return i;
	}
	return -1;
}


void GlowMenu::SetItemLabel(
	int itemNum,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemLabel");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	GLOW_ASSERT(label);
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	_SetupItem(itemNum);
}


void GlowMenu::SetItemMark(
	int itemNum,
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemMark");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).mark;
	if (mark == 0)
	{
		(*iter).mark = 0;
	}
	else
	{
		int marklen = GLOW_CSTD::strlen(mark);
		(*iter).mark = new char[marklen+1];
		GLOW_CSTD::strcpy((*iter).mark, mark);
	}
	_SetupItem(itemNum);
}


void GlowMenu::ToggleItemMark(
	int itemNum,
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowMenu::ToggleItemMark");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	if ((*iter).mark == 0)
	{
		int marklen = GLOW_CSTD::strlen(mark);
		(*iter).mark = new char[marklen+1];
		GLOW_CSTD::strcpy((*iter).mark, mark);
	}
	else
	{
		delete[] (*iter).mark;
		(*iter).mark = 0;
	}
	_SetupItem(itemNum);
}


void GlowMenu::UnmarkItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowMenu::UnmarkItem");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).mark;
	(*iter).mark = 0;
	_SetupItem(itemNum);
}


void GlowMenu::UnmarkAllItems()
{
	GLOW_DEBUGSCOPE("GlowMenu::UnmarkAllItems");
	
	int itemNum = 0;
	for (ItemIterator iter = _itemData.begin(); iter != _itemData.end(); ++iter)
	{
		delete[] (*iter).mark;
		(*iter).mark = 0;
		_SetupItem(itemNum);
		itemNum++;
	}
}


void GlowMenu::ChangeToEntry(
	int itemNum,
	const char* label,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::ChangeToEntry");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	GLOW_ASSERT(label);
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	delete[] (*iter).mark;
	(*iter).mark = 0;
	(*iter).code = code;
	(*iter).subMenu = 0;
	_SetupItem(itemNum);
}


void GlowMenu::SetItemCode(
	int itemNum,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemCode");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	(*iter).code = code;
	(*iter).subMenu = 0;
	_SetupItem(itemNum);
}


void GlowMenu::SetCodesToItemNumbers()
{
	GLOW_DEBUGSCOPE("GlowMenu::SetCodesToItemNumbers");
	
	int mx = _itemData.size();
	for (int i=0; i<mx; ++i)
	{
		if (_itemData[i].subMenu == 0)
		{
			_itemData[i].code = i;
		}
	}
}


void GlowMenu::ChangeToSubmenu(
	int itemNum,
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::ChangeToSubmenu");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	delete[] (*iter).mark;
	(*iter).mark = 0;
	(*iter).code = 0;
	(*iter).subMenu = menu;
	_SetupItem(itemNum);
}


void GlowMenu::SetItemSubmenu(
	int itemNum,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemSubmenu");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	(*iter).code = 0;
	(*iter).subMenu = menu;
	_SetupItem(itemNum);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

