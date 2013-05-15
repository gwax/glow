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
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 1.1.2dev  (7 August 2000)
	
	CHANGES:
	
		Original release (DA)

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
#include <cstdlib>

#ifndef GLOW__H
	#include "glow.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


typedef GLOW_STD::map<int, GlowSubwindow*>::value_type WindowRegistryEntry_;
typedef GLOW_STD::map<int, GlowMenu*>::value_type MenuRegistryEntry_;
typedef GLOW_STD::map<int, GlowSubwindow*>::iterator WindowRegistryIterator_;
typedef GLOW_STD::map<int, GlowMenu*>::iterator MenuRegistryIterator_;
typedef GLOW_STD::vector<Glow_MenuItem>::iterator MenuItemIterator_;
typedef GLOW_STD::vector<Glow_MenuItem>::const_iterator MenuItemConstIterator_;


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
#ifndef GLOW_OPTION_STRICTGLUT3
	modeString = 0;
#endif
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
	Event filter receivers
===============================================================================
*/

void GlowKeyboardFilter::OnMessage(
	GlowKeyboardData& message)
{
	if (message._continue)
	{
		message._continue = OnFilter(message);
	}
}


void GlowMouseFilter::OnMessage(
	GlowMouseData& message)
{
	if (message._continue)
	{
		message._continue = OnFilter(message);
	}
}


/*
===============================================================================
	Private globals for Glow
===============================================================================
*/

// Map window IDs to GlowSubwindow*s
GLOW_STD::map<int, GlowSubwindow*> Glow::windowRegistry_;
// Map menu IDs to GlowMenu*s
GLOW_STD::map<int, GlowMenu*> Glow::menuRegistry_;

// List of components to activate in deferred activation
GLOW_STD::map<GlowComponent*, bool> Glow::activateNotifyList_;
// List of components to close in deferred delete (bool is unused)
GLOW_STD::map<GlowComponent*, bool> Glow::closeList_;
// Special pointer to a function to call during deferred event time
// used by widget system to implement deferred widget events
void (*Glow::widgetNotifier_)() = 0;
// Each subwindow contains a local clock value, and there's this global
// clock here that gets incremented before every event handler. If local
// clock == global clock, then the subwindow's data members contain the
// correct position. If !=, the subwindow needs to update its knowledge
// of position by querying glutGet.
// This rather convoluted system is necessary to maintain the semantics
// of GLOW state querying in the face of GLUT deferred execution. After
// calling Move(), the GLOW state should reflect the new position (the
// local clock is updated and the data members are set to the new pos
// even though glutGet() would still return the old values.)
// If the window is moved by the user, the local clock will be inc'd
// at the beginning of the next event handling cycle, so GLOW knows that
// it is possible the cached data member values are stale.
unsigned long Glow::clock_ = 0;

// Subwindow that spawned the current menu. Set if a menu is down.
GlowSubwindow* Glow::menuWindow_ = 0;
// Coordinates of the click that spawned the current menu.
int Glow::menuXClick_ = -1;
int Glow::menuYClick_ = -1;
// Is a menu down?
bool Glow::menuInUse_ = false;

// Modal window stack
GLOW_STD::vector<GlowWindow*> Glow::modalWindows_;

// Sender for idle events
TSender<const GlowIdleMessage&> Glow::idleSender_;
// List of senders for timer events. Maps Timer IDs to senders.
GLOW_STD::map<int, TSender<const GlowTimerMessage&>*> Glow::timerSenders_;
// The next timer ID that will be assigned.
int Glow::nextTimerID_ = 1;

// Special idle function receiver implementing GLUT-style idle func
Glow_IdleFuncReceiver* Glow::idleFuncReceiver_ = 0;
// GLUT-style menu status function pointer
void (*Glow::userMenuStatusFunc_)(int status, int x, int y) = 0;

// Global filters for mouse and keyboard events
TSender<GlowMouseData&> Glow::mouseFilters_;
TSender<GlowKeyboardData&> Glow::keyboardFilters_;

// Number of toplevel windows in existence
int Glow::numToplevelWindows_ = 0;
// If true, GLOW will automatically quit when the last toplevel window
// is deleted.
bool Glow::autoQuitting_ = false;

// GLUT seems to have a bug that sometimes causes windows to be "locked
// out" of the worklist (i.e. unable to receive events.) It often happens
// when you re-post a redisplay event from within the same window's
// display callback. This option causes GLOW to defer such re-postings
// until the next event.
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
// Window ID currently in display callback, or 0 for not displaying
int Glow::curDisplayWindow_ = 0;
// Window ID for which we should repost redisplay, or 0 for none.
int Glow::refreshMe_ = 0;
#endif


/*
===============================================================================
	Methods for Glow
===============================================================================
*/

void Glow::Init(
	int& argc,
	char** argv)
{
	::glutInit(&argc, argv);
	::glutMenuStatusFunc(Glow::MenuStatusFunc_);
}


void Glow::MainLoop()
{
	::glutMainLoop();
}


double Glow::Version()
{
	return GLOW_VERSION;
}


int Glow::APIVersion()
{
	return GLOW_APIVERSION;
}


/*
===============================================================================
	Registry functions
===============================================================================
*/

void Glow::AddWindow_(
	GlowSubwindow* window,
	int windowNum)
{
	// Toplevel windows are the only components that have no parent
	if (window->Parent() == 0)
	{
		++numToplevelWindows_;
	}
	windowRegistry_.insert(WindowRegistryEntry_(windowNum, window));
}


void Glow::RemoveWindow_(
	int windowNum)
{
	// Find entry in registry
	WindowRegistryIterator_ iter = windowRegistry_.find(windowNum);
	if (iter != windowRegistry_.end())
	{
		// Update numToplevelWindows
		GlowSubwindow* wind = (*iter).second;
		// Toplevel windows are the only components that have no parent
		if (wind->Parent() == 0)
		{
			--numToplevelWindows_;
		}
		// Are there modal windows?
		if (!modalWindows_.empty())
		{
			// Is window to remove the current top modal window?
			if (wind == modalWindows_.back())
			{
				PopModalWindow();
			}
			else
			{
				// Is window to remove in the modal window stack at all?
				for (GLOW_STD::vector<GlowWindow*>::iterator miter = modalWindows_.begin();
					miter != modalWindows_.end(); ++miter)
				{
					if (*miter == wind)
					{
						modalWindows_.erase(miter);
						break;
					}
				}
			}
		}
		windowRegistry_.erase(iter);
		// Auto-quit
		if (autoQuitting_ && windowRegistry_.empty())
		{
			GLOW_ASSERT(numToplevelWindows_ == 0);
			GLOW_CSTD::exit(0);
		}
	}
}


GlowSubwindow* Glow::ResolveWindow(
	int windowNum)
{
	WindowRegistryIterator_ iter = windowRegistry_.find(windowNum);
	return (iter == windowRegistry_.end()) ? 0 : (*iter).second;
}


void Glow::AddMenu_(
	GlowMenu *menu,
	int menuNum)
{
	menuRegistry_.insert(MenuRegistryEntry_(menuNum, menu));
}


void Glow::RemoveMenu_(
	int menuNum)
{
	menuRegistry_.erase(menuNum);
}


GlowMenu* Glow::ResolveMenu(
	int menuNum)
{
	MenuRegistryIterator_ iter = menuRegistry_.find(menuNum);
	return (iter == menuRegistry_.end()) ? 0 : (*iter).second;
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
	
	// Bind to this receiver if it's not already
	if (!idleSender_.IsBoundTo(idle))
	{
		idleSender_.Bind(idle);
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		if (idleSender_.NumReceivers() == 1 && refreshMe_ == 0)
#else
		if (idleSender_.NumReceivers() == 1)
#endif
		{
			::glutIdleFunc(IdleFunc_);
		}
	}
}


void Glow::UnregisterIdle(
	GlowIdleReceiver* idle)
{
	GLOW_DEBUGSCOPE("Glow::UnregisterIdle");
	
	// Unbind this receiver if it's bound
	if (idleSender_.IsBoundTo(idle))
	{
		idleSender_.Unbind(idle);
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		if (idleSender_.NumReceivers() == 0 && refreshMe_ == 0)
#else
		if (idleSender_.NumReceivers() == 0)
#endif
		{
			::glutIdleFunc(0);
		}
	}
}


void Glow::SetIdleFunc(
	void (*func)())
{
	GLOW_DEBUGSCOPE("Glow::SetIdleFunc");
	
	// GLOW version of glutIdleFunc. 
	if (func == 0)
	{
		// Unregister the GLUT-style idle function receiver
		if (idleFuncReceiver_ != 0)
		{
			UnregisterIdle(idleFuncReceiver_);
			delete idleFuncReceiver_;
			idleFuncReceiver_ = 0;
		}
	}
	else
	{
		// Set the function pointer if the receiver is already registered
		if (idleFuncReceiver_ != 0)
		{
			idleFuncReceiver_->_funcPtr = func;
		}
		else
		// Otherwise register a GLUT-style idle function receiver
		{
			idleFuncReceiver_ = new Glow_IdleFuncReceiver(func);
			RegisterIdle(idleFuncReceiver_);
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
	
	// Get the next available timer ID.
	int timerID = nextTimerID_;
	while (timerSenders_.find(timerID) != timerSenders_.end() || timerID == 0)
	{
		++timerID;
	}
	nextTimerID_ = timerID + 1;
	// Create a sender for this timer
	TSender<const GlowTimerMessage&>* sender = new TSender<const GlowTimerMessage&>;
	sender->Bind(timer);
	timerSenders_[timerID] = sender;
	// Register timer event
	::glutTimerFunc(msecs, TimerFunc_, timerID);
	return timerID;
}


void Glow::UnregisterTimer(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::UnregisterTimer");
	
	// Find the timer sender with this id. Delete it if it exists.
	// The timer function will still be called, but it will notice that
	// this id no longer exists, and it will ignore the callback.
	// This will, of course, fail if the id gets reused. However, I don't
	// think anyone will be scheduling 4 billion timers in that short
	// a space of time.
	GLOW_STD::map<int, TSender<const GlowTimerMessage&>*>::iterator iter = timerSenders_.find(id);
	if (iter != timerSenders_.end())
	{
		delete (*iter).second;
		timerSenders_.erase(iter);
	}
}


/*
===============================================================================
	Callbacks
===============================================================================
*/

// Temporary code that will report re-entry into event handlers
class Glow_ReentryChecker_
{
	public:
		inline Glow_ReentryChecker_()
			throw()
		{
			++entryCounter_;
			GLOW_WARNING(entryCounter_>1, "Event re-entry detected.");
		}
		inline ~Glow_ReentryChecker_()
			throw()
		{
			--entryCounter_;
		}
	private:
		static int entryCounter_;
};
int Glow_ReentryChecker_::entryCounter_ = 0;
#define GLOW_INTERNAL_CHECKEVENTREENTRY() Glow_ReentryChecker_ glow_reentryChecker_
// End temporary code


void Glow::TimerFunc_(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::TimerFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Which timer was invoked?
	GLOW_STD::map<int, TSender<const GlowTimerMessage&>*>::iterator iter = timerSenders_.find(id);
	if (iter != timerSenders_.end())
	{
		// Remove the timer sender, and send the message.
		TSender<const GlowTimerMessage&>* sender = (*iter).second;
		timerSenders_.erase(iter);
		sender->Send(GlowTimerMessage(id));
		delete sender;
		ExecuteDeferred_();
	}
}


void Glow::DisplayFunc_()
{
	GLOW_DEBUGSCOPE("Glow::DisplayFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
	// Paint the window
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	curDisplayWindow_ = ::glutGetWindow();
	if (curDisplayWindow_ != refreshMe_)
	{
		RaiseDeferredRefresh_();
	}
	GlowSubwindow* window = ResolveWindow(curDisplayWindow_);
#else
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
#endif
	if (window != 0)
	{
		window->Paint();
		window->FinishRender_();
		ExecuteDeferred_();
	}
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	curDisplayWindow_ = 0;
#endif
}


void Glow::ReshapeFunc_(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("Glow::ReshapeFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window, set the width and height, and handle the event
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->width_ = width;
		window->height_ = height;
		window->OnReshape(width, height);
		ExecuteDeferred_();
	}
}


void Glow::KeyboardFunc_(
	unsigned char key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::KeyboardFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global keyboard filters
		GlowKeyboardData filterData;
		filterData.subwindow = window;
#ifndef GLOW_OPTION_STRICTGLUT3
		filterData.type = GlowKeyboardData::keyDown;
#endif
		filterData.key = KeyCode(key);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		keyboardFilters_.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			filterData.subwindow->OnKeyboard(filterData.key,
				filterData.x, filterData.y, filterData.modifiers);
		}
		ExecuteDeferred_();
	}
}


#ifndef GLOW_OPTION_STRICTGLUT3
void Glow::KeyboardUpFunc_(
	unsigned char key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::KeyboardUpFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global keyboard filters
		GlowKeyboardData filterData;
		filterData.subwindow = window;
		filterData.type = GlowKeyboardData::keyUp;
		filterData.key = KeyCode(key);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		keyboardFilters_.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			filterData.subwindow->OnKeyboardUp(filterData.key,
				filterData.x, filterData.y, filterData.modifiers);
		}
		ExecuteDeferred_();
	}
}
#endif


void Glow::MouseFunc_(
	int button,
	int state,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::MouseFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global mouse filters
		GlowMouseData filterData;
		filterData.subwindow = window;
		filterData.type = (state == GLUT_DOWN) ?
			GlowMouseData::mouseDown : GlowMouseData::mouseUp;
		filterData.button = MouseButton(button);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		mouseFilters_.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			if (filterData.type == GlowMouseData::mouseDown)
			{
				filterData.subwindow->OnMouseDown(filterData.button,
					filterData.x, filterData.y, filterData.modifiers);
			}
			else if (filterData.type == GlowMouseData::mouseUp)
			{
				filterData.subwindow->OnMouseUp(filterData.button,
					filterData.x, filterData.y, filterData.modifiers);
			}
		}
		ExecuteDeferred_();
	}
}


void Glow::MotionFunc_(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::MotionFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window and invoke event handler
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnMouseDrag(x, y);
		ExecuteDeferred_();
	}
}


void Glow::PassiveMotionFunc_(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::PassiveMotionFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window and invoke event handler
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnMouseMotion(x, y);
		ExecuteDeferred_();
	}
}


void Glow::VisibilityFunc_(
	int state)
{
	GLOW_DEBUGSCOPE("Glow::VisibilityFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window and invoke event handler
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
		ExecuteDeferred_();
	}
}


void Glow::EntryFunc_(
	int state)
{
	GLOW_DEBUGSCOPE("Glow::EntryFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window and invoke event handler
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
		ExecuteDeferred_();
	}
}


void Glow::SpecialFunc_(
	int key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::SpecialFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global keyboard filters
		GlowKeyboardData filterData;
		filterData.subwindow = window;
		filterData.key = KeyCode(key+Glow::specialKeyOffset);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		keyboardFilters_.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			filterData.subwindow->OnKeyboard(filterData.key,
				filterData.x, filterData.y, filterData.modifiers);
		}
		ExecuteDeferred_();
	}
}


#ifndef GLOW_OPTION_STRICTGLUT3
void Glow::SpecialUpFunc_(
	int key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::SpecialUpFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global keyboard filters
		GlowKeyboardData filterData;
		filterData.subwindow = window;
		filterData.type = GlowKeyboardData::keyUp;
		filterData.key = KeyCode(key+Glow::specialKeyOffset);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		keyboardFilters_.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			filterData.subwindow->OnKeyboardUp(filterData.key,
				filterData.x, filterData.y, filterData.modifiers);
		}
		ExecuteDeferred_();
	}
}
#endif


void Glow::MenuStatusFunc_(
	int status,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::MenuStatusFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Give GLUT-style function a chance to handle the event
	if (userMenuStatusFunc_ != 0)
	{
		userMenuStatusFunc_(status, x, y);
	}
	if (status == GLUT_MENU_IN_USE)
	{
		// Menu-down event
		menuWindow_ = ResolveWindow(::glutGetWindow());
		menuXClick_ = x;
		menuYClick_ = y;
		menuInUse_ = true;
		if (menuWindow_ != 0)
		{
			menuWindow_->OnMenuDown(x, y);
		}
	}
	else if (status == GLUT_MENU_NOT_IN_USE)
	{
		// Menu-up event
		if (menuWindow_ != 0)
		{
			menuWindow_->OnMenuUp();
		}
		menuInUse_ = false;
	}
	ExecuteDeferred_();
}


void Glow::MenuFunc_(
	int value)
{
	GLOW_DEBUGSCOPE("Glow::MenuFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the menu and invoke event handler
	GlowMenu* menu = ResolveMenu(::glutGetMenu());
	if (menu != 0)
	{
		menu->OnHit(value, menuWindow_, menuXClick_, menuYClick_);
		ExecuteDeferred_();
	}
}


void Glow::IdleFunc_()
{
	GLOW_DEBUGSCOPE("Glow::IdleFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Invoke idle event handlers
	idleSender_.Send(GlowIdleMessage());
	ExecuteDeferred_();
}


#ifndef GLOW_OPTION_STRICTGLUT3
void Glow::JoystickFunc_(
	unsigned int buttonMask,
	int x,
	int y,
	int z)
{
	GLOW_DEBUGSCOPE("Glow::JoystickFunc_");
GLOW_INTERNAL_CHECKEVENTREENTRY();  // Temp
	
	++clock_;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	RaiseDeferredRefresh_();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnJoystick(JoystickButtonMask(buttonMask), x, y, z);
		ExecuteDeferred_();
	}
}
#endif


/*
===============================================================================
	Finalizers (deferred execution)
===============================================================================
*/

// Internal class that prevents re-entry into ExecuteDeferred_ in an
// exception-safe manner.
// We may eventually want to promote something like this into glowDebug,
// if we can design it to handle self-re-entrance (i.e. handle multiple
// pending checks) well enough.
class Glow_DetectReentry_
{
	public:
	
		inline Glow_DetectReentry_()
			throw()
		{
			++entryCounter_;
		}
		inline ~Glow_DetectReentry_()
			throw()
		{
			--entryCounter_;
		}
		inline bool isReentered()
			throw()
		{
			return entryCounter_ > 1;
		}
	
	private:
	
		static int entryCounter_;
};

int Glow_DetectReentry_::entryCounter_ = 0;


void Glow::ExecuteDeferred_()
{
	GLOW_DEBUGSCOPE("Glow::ExecuteDeferred_");
	
	// Prevent re-entrance
	Glow_DetectReentry_ reentryDetector;
	if (reentryDetector.isReentered())
	{
		return;
	}
	
	// Widget notifier
	if (widgetNotifier_ != 0)
	{
		widgetNotifier_();
	}
	
	// Deferred activation and deactivation
	for (GLOW_STD::map<GlowComponent*, bool>::iterator iter = activateNotifyList_.begin(),
		enditer = activateNotifyList_.end(); iter != enditer; ++iter)
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
	activateNotifyList_.clear();
	
	// Deferred deletion of components
	while (!closeList_.empty())
	{
		delete (*(closeList_.begin())).first;
	}
}


#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
void Glow::RaiseDeferredRefresh_()
{
	if (refreshMe_ != 0)
	{
		RefreshGlutWindow(refreshMe_);
		refreshMe_ = 0;
		if (idleSender_.NumReceivers() == 0)
		{
			::glutIdleFunc(0);
		}
	}
}
#endif


/*
===============================================================================
	Modal window management
===============================================================================
*/

void Glow::PushModalWindow(
	GlowWindow* wind)
{
	GLOW_DEBUGSCOPE("Glow::PushModalWindow");
	
	if (modalWindows_.empty())
	{
		// This is the first modal window
		// Go through and deactivate all toplevel windows except the one
		// to be made modal
		for (WindowRegistryIterator_ iter = windowRegistry_.begin(),
			enditer = windowRegistry_.end(); iter != enditer; ++iter)
		{
			GlowSubwindow* subwind = (*iter).second;
			if (subwind->IsTopLevel() && subwind != wind)
			{
				subwind->BroadcastStandby_(false, false);
			}
		}
	}
	else
	{
		// There's a modal window already. Deactivate that one and
		// activate the new modal window
		modalWindows_.back()->BroadcastStandby_(false, false);
		wind->BroadcastStandby_(true, false);
	}
	// push window onto stack, and bring to front
	modalWindows_.push_back(wind);
	wind->Raise();
}


void Glow::PopModalWindow()
{
	GLOW_DEBUGSCOPE("Glow::PopModalWindow");
	
	// Make sure there's a modal window.
	if (!modalWindows_.empty())
	{
		// pop modal window off stack
		GlowWindow* wind = modalWindows_.back();
		modalWindows_.pop_back();
		if (modalWindows_.empty())
		{
			// This was the last modal window. Reactivate windows
			for (WindowRegistryIterator_ iter = windowRegistry_.begin(),
				enditer = windowRegistry_.end(); iter != enditer; ++iter)
			{
				GlowSubwindow* subwind = (*iter).second;
				if (subwind->IsTopLevel() && subwind != wind)
				{
					subwind->BroadcastStandby_(true, false);
				}
			}
		}
		else
		{
			// There are more windows on the stack
			// Deactivate popped window and reactivate next modal window
			wind->BroadcastStandby_(false, false);
			modalWindows_.back()->BroadcastStandby_(true, false);
			modalWindows_.back()->Raise();
		}
	}
}


/*
===============================================================================
	Miscellaneous
===============================================================================
*/

bool Glow::IsExtensionSupported(
	const char* extensionName)
{
	GLOW_DEBUGSCOPE("Glow::IsExtensionSupported");
	
	// If there's no window, there's no OpenGL context.
	// We'll also provide a default behavior: for no windows we return false.
	// If there's just no current window, we'll use the first one registered.
	if (::glutGetWindow() == 0)
	{
		GLOW_DEBUG(windowRegistry_.empty(),
			"Glow::IsExtensionSupported() called when no windows are open");
		if (windowRegistry_.empty()) return false;
		::glutSetWindow((*(windowRegistry_.begin())).first);
	}
	
	// Need to copy the string because glutExtensionSupported wants a
	// non-const pointer.
	char* buf = new char[GLOW_CSTD::strlen(extensionName)+1];
	GLOW_CSTD::strcpy(buf, extensionName);
	bool ret = (::glutExtensionSupported(buf) != 0);
	delete[] buf;
	return ret;
}


bool Glow::IsBufferTypeSupported(
	BufferType mode)
{
	GLOW_DEBUGSCOPE("Glow::IsBufferTypeSupported");
	
	// Test GLUT_DISPLAY_MODE_POSSIBLE. Preserve the old mode.
	int oldMode = ::glutGet(GLenum(GLUT_INIT_DISPLAY_MODE));
	::glutInitDisplayMode(mode);
	bool ret = (::glutGet(GLenum(GLUT_DISPLAY_MODE_POSSIBLE)) != 0);
	::glutInitDisplayMode(oldMode);
	return ret;
}


void Glow::RefreshGlutWindow(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::RefreshGlutWindow");
	
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	if (curDisplayWindow_ == id)
	{
		// Workaround for apparent GLUT bug that affects re-posting refresh events
		// from within a display function. Instead, we defer posting the refresh
		// until the next event handled. (We register an idle event to make sure
		// another event is raised.)
		if (refreshMe_ == 0)
		{
			refreshMe_ = id;
			if (idleSender_.NumReceivers() == 0)
			{
				::glutIdleFunc(IdleFunc_);
			}
		}
	}
	else
	{
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 11)
		// According to the GLUT spec, this version is optimized by
		// not requiring an OpenGL context switch.
		::glutPostWindowRedisplay(id);
#else
		int saveWind = ::glutGetWindow();
		if (saveWind != id)
		{
			::glutSetWindow(id);
		}
		::glutPostRedisplay();
		if (saveWind != 0 && saveWind != id)
		{
			::glutSetWindow(saveWind);
		}
#endif
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	}
#endif
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
	
	activeState_ = 1;
	firstChild_ = lastChild_ = 0;
	prev_ = next_ = 0;
	numChildren_ = 0;
	parent_ = 0;
	if (parent != 0)
	{
		parent->AddChild_(this);
		if (!parent->IsActive())
		{
			activeState_ = 2;
		}
	}
	initializeState_ = 1;
}


GlowComponent::~GlowComponent()
{
	GLOW_DEBUGSCOPE("GlowComponent::~GlowComponent");
	
	KillChildren();
	if (parent_ != 0)
	{
		parent_->RemoveChild_(this);
	}
	GLOW_STD::map<GlowComponent*, bool>::iterator iter =
		Glow::activateNotifyList_.find(this);
	if (iter != Glow::activateNotifyList_.end())
	{
		Glow::activateNotifyList_.erase(iter);
	}
	iter = Glow::closeList_.find(this);
	if (iter != Glow::closeList_.end())
	{
		Glow::closeList_.erase(iter);
	}
}


void GlowComponent::Close()
{
	GLOW_DEBUGSCOPE("GlowComponent::Close");
	
	GLOW_STD::map<GlowComponent*, bool>::iterator iter =
		Glow::closeList_.find(this);
	if (iter == Glow::closeList_.end())
	{
		Glow::closeList_.insert(
			GLOW_STD::pair<GlowComponent* const, bool>(this, true));
	}
	initializeState_ = 0;
}


void GlowComponent::AddChild_(
	GlowComponent* child)
{
	GLOW_DEBUGSCOPE("GlowComponent::AddChild_");
	GLOW_ASSERT(child->parent_ == 0);
	
	child->parent_ = this;
	if (firstChild_ == 0)
	{
		firstChild_ = child;
	}
	else
	{
		lastChild_->next_ = child;
	}
	child->prev_ = lastChild_;
	child->next_ = 0;
	lastChild_ = child;
	++numChildren_;
}


void GlowComponent::RemoveChild_(
	GlowComponent* child)
{
	GLOW_DEBUGSCOPE("GlowComponent::RemoveChild_");
	GLOW_ASSERT(child->parent_ == this);
	
	if (child->next_ == 0)
	{
		lastChild_ = child->prev_;
	}
	else
	{
		child->next_->prev_ = child->prev_;
	}
	if (child->prev_ == 0)
	{
		firstChild_ = child->next_;
	}
	else
	{
		child->prev_->next_ = child->next_;
	}
	child->prev_ = child->next_ = child->parent_ = 0;
	--numChildren_;
}


void GlowComponent::Paint()
{
	GLOW_DEBUGSCOPE("GlowComponent::Paint");
	
	if (OnBeginPaint())
	{
		for (GlowComponent* child = firstChild_; child != 0; child = child->Next())
		{
			if (dynamic_cast<GlowSubwindow*>(child) == 0)
			{
				(child)->Paint();
			}
		}
	}
	OnEndPaint();
}


void GlowComponent::BroadcastStandby_(
	bool activating,
	bool first)
{
	GLOW_DEBUGSCOPE("GlowComponent::BroadcastStandby_");
	
	bool changing = false;
	
	if (activating && (activeState_ == 2 || (first && activeState_ == 0)))
	{
		changing = true;
		activeState_ = 1;
	}
	else if (!activating && activeState_ == 1)
	{
		changing = true;
		activeState_ = (first ? 0 : 2);
	}
	if (changing)
	{
		GlowSubwindow* subwindow = dynamic_cast<GlowSubwindow*>(this);
		if (subwindow != 0)
		{
			subwindow->EventsForActivation_(activating);
		}
		GLOW_STD::map<GlowComponent*, bool>::iterator iter =
			Glow::activateNotifyList_.find(this);
		if (iter != Glow::activateNotifyList_.end())
		{
			if (activating != (*iter).second)
			{
				Glow::activateNotifyList_.erase(iter);
			}
		}
		else
		{
			Glow::activateNotifyList_.insert(
				GLOW_STD::pair<GlowComponent* const, bool>(this, activating));
		}
		for (GlowComponent* child = firstChild_; child != 0; child = child->Next())
		{
			child->BroadcastStandby_(activating, false);
		}
		WhichWindow()->Refresh();
	}
}


void GlowComponent::Activate()
{
	GLOW_DEBUGSCOPE("GlowComponent::Activate");
	
	if (activeState_ == 0)
	{
		if ((IsTopLevel() && (Glow::NumModalWindows() == 0 || Glow::TopModalWindow() == this)) ||
			(!IsTopLevel() && Parent()->IsActive()))
		{
			BroadcastStandby_(true, true);
		}
		else
		{
			activeState_ = 2;
		}
	}
}


void GlowComponent::Deactivate()
{
	GLOW_DEBUGSCOPE("GlowComponent::Deactivate");
	
	if (activeState_ == 1)
	{
		BroadcastStandby_(false, true);
	}
	else if (activeState_ == 2)
	{
		activeState_ = 0;
	}
}


GlowSubwindow* GlowComponent::WhichWindow()
{
	GlowSubwindow* wind = 0;
	GlowComponent* component = this;
	while (wind == 0)
	{
		wind = dynamic_cast<GlowSubwindow*>(component);
		component = component->parent_;
	}
	return wind;
}


GlowWindow* GlowComponent::ToplevelWindow()
{
	GlowComponent* component = this;
	while (component->parent_ != 0)
	{
		component = component->parent_;
	}
	return static_cast<GlowWindow*>(component);
}


void GlowComponent::ReorderChild(
	GlowComponent* child,
	GlowComponent* before)
{
	GLOW_DEBUGSCOPE("GlowComponent::ReorderChild");
	GLOW_ASSERT(child->Parent() == this);
	GLOW_ASSERT(before->Parent() == this);
	
	if (child == before) return;
	
	RemoveChild_(child);
	if (before == 0)
	{
		AddChild_(child);
	}
	else if (before == firstChild_)
	{
		firstChild_->prev_ = child;
		child->next_ = firstChild_;
		child->prev_ = 0;
		firstChild_ = child;
		child->parent_ = this;
		++numChildren_;
	}
	else
	{
		child->next_ = before;
		child->prev_ = before->prev_;
		before->prev_ = child;
		child->prev_->next_ = child;
		child->parent_ = this;
		++numChildren_;
	}
}


void GlowComponent::KillChildren()
{
	GLOW_DEBUGSCOPE("GlowComponent::KillChildren");
	
	while (numChildren_ > 0)
	{
		delete firstChild_;
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
	
	GLOW_DEBUG(params.width != parentWindowSize && params.width <= 0,
		"Subwindow width too small");
	GLOW_DEBUG(params.height != parentWindowSize && params.height <= 0,
		"Subwindow height too small");
	
	GlowComponent::Init(parent);
#ifndef GLOW_OPTION_STRICTGLUT3
	if (params.modeString != 0)
	{
		char* str = new char[GLOW_CSTD::strlen(params.modeString)+1];
		GLOW_CSTD::strcpy(str, params.modeString);
		::glutInitDisplayString(str);
		delete[] str;
	}
	else
	{
#endif
		::glutInitDisplayMode(params.mode);
#ifndef GLOW_OPTION_STRICTGLUT3
	}
#endif
	width_ = params.width;
	if (width_ == parentWindowSize)
	{
		width_ = parent->WhichWindow()->Width() - params.x;
	}
	height_ = params.height;
	if (height_ == parentWindowSize)
	{
		height_ = parent->WhichWindow()->Height() - params.y;
	}
	windowNum_ = ::glutCreateSubWindow(parent->WhichWindow()->windowNum_,
		params.x, params.y, width_, height_);
	eventMask_ = params.eventMask;
	inactiveEventMask_ = params.inactiveEventMask;
	bufferType_ = params.mode;
	leftMenu_ = 0;
	centerMenu_ = 0;
	rightMenu_ = 0;
	RegisterCallbacks_(eventMask_);
	saveCursor_ = Glow::inheritCursor;
	Glow::AddWindow_(this, windowNum_);
	needSwapBuffers_ = ((params.mode & Glow::doubleBuffer) != 0);
	refreshEnabled_ = true;
	autoSwapBuffers_ = true;
	clock_ = Glow::clock_;
	globalXPos_ = ::glutGet((GLenum)GLUT_WINDOW_X);
	globalYPos_ = ::glutGet((GLenum)GLUT_WINDOW_Y);
#ifndef GLOW_OPTION_STRICTGLUT3
	joystickPollInterval_ = 0;
#endif
}


void GlowSubwindow::Init(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Init");
	
	GlowSubwindowParams params;
	params.x = x;
	params.y = y;
	params.width = width;
	params.height = height;
	params.mode = mode;
	params.modeString = 0;
	params.eventMask = eventMask;
	Init(parent, params);
}


#ifndef GLOW_OPTION_STRICTGLUT3
void GlowSubwindow::Init(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	const char* modeString,
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Init");
	
	GLOW_ASSERT(modeString != 0);
	
	GlowSubwindowParams params;
	params.x = x;
	params.y = y;
	params.width = width;
	params.height = height;
	params.mode = Glow::noBuffer;
	params.modeString = modeString;
	params.eventMask = eventMask;
	Init(parent, params);
}
#endif


GlowSubwindow::~GlowSubwindow()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::~GlowSubwindow");
	
	KillChildren();
	Glow::RemoveWindow_(windowNum_);
	::glutDestroyWindow(windowNum_);
}


void GlowSubwindow::RegisterCallbacks_(
	Glow::EventMask eventMask)
{
	::glutDisplayFunc(Glow::DisplayFunc_);
	::glutReshapeFunc(Glow::ReshapeFunc_);
	
	if (eventMask & Glow::keyboardEvents)
	{
		::glutKeyboardFunc(Glow::KeyboardFunc_);
		::glutSpecialFunc(Glow::SpecialFunc_);
	}
	else
	{
		::glutKeyboardFunc(0);
		::glutSpecialFunc(0);
	}
#ifndef GLOW_OPTION_STRICTGLUT3
	if (eventMask & Glow::keyboardUpEvents)
	{
		::glutKeyboardUpFunc(Glow::KeyboardUpFunc_);
		::glutSpecialUpFunc(Glow::SpecialUpFunc_);
	}
	else
	{
		::glutKeyboardUpFunc(0);
		::glutSpecialUpFunc(0);
	}
	if (eventMask & Glow::joystickEvents)
	{
		::glutJoystickFunc(Glow::JoystickFunc_, joystickPollInterval_);
	}
	else
	{
		::glutJoystickFunc(0, 0);
	}
#endif
	if (eventMask & Glow::mouseEvents)
	{
		::glutMouseFunc(Glow::MouseFunc_);
	}
	else
	{
		::glutMouseFunc(0);
	}
	if (eventMask & Glow::dragEvents)
	{
		::glutMotionFunc(Glow::MotionFunc_);
	}
	else
	{
		::glutMotionFunc(0);
	}
	if (eventMask & Glow::motionEvents)
	{
		::glutPassiveMotionFunc(Glow::PassiveMotionFunc_);
	}
	else
	{
		::glutPassiveMotionFunc(0);
	}
	if (eventMask & Glow::visibilityEvents)
	{
		::glutVisibilityFunc(Glow::VisibilityFunc_);
	}
	else
	{
		::glutVisibilityFunc(0);
	}
	if (eventMask & Glow::focusEvents)
	{
		::glutEntryFunc(Glow::EntryFunc_);
	}
	else
	{
		::glutEntryFunc(0);
	}
	if (eventMask & Glow::menuEvents)
	{
		int saveMenu = ::glutGetMenu();
		if (leftMenu_ != 0)
		{
			::glutSetMenu(leftMenu_->menuNum_);
			::glutAttachMenu(Glow::leftButton);
		}
		if (centerMenu_ != 0)
		{
			::glutSetMenu(centerMenu_->menuNum_);
			::glutAttachMenu(Glow::middleButton);
		}
		if (rightMenu_ != 0)
		{
			::glutSetMenu(rightMenu_->menuNum_);
			::glutAttachMenu(Glow::rightButton);
		}
		if (saveMenu != 0)
		{
			::glutSetMenu(saveMenu);
		}
	}
	else
	{
		if (leftMenu_ != 0)
		{
			::glutDetachMenu(Glow::leftButton);
		}
		if (centerMenu_ != 0)
		{
			::glutDetachMenu(Glow::middleButton);
		}
		if (rightMenu_ != 0)
		{
			::glutDetachMenu(Glow::rightButton);
		}
	}
}


#ifndef GLOW_OPTION_STRICTGLUT3
void GlowSubwindow::ReadJoystick()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::ReadJoystick");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutForceJoystickFunc();
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}
#endif


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
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowSubwindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
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
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


#ifndef GLOW_OPTION_STRICTGLUT3
void GlowSubwindow::OnKeyboardUp(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowSubwindow::OnJoystick(
	Glow::JoystickButtonMask button,
	int x,
	int y,
	int z)
{
}
#endif


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
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetEventMask");
	
	eventMask_ = eventMask;
	if (IsActive())
	{
		int saveWind = ::glutGetWindow();
		if (saveWind != windowNum_)
		{
			::glutSetWindow(windowNum_);
		}
		RegisterCallbacks_(eventMask);
		if (saveWind != 0 && saveWind != windowNum_)
		{
			::glutSetWindow(saveWind);
		}
	}
}


void GlowSubwindow::SetInactiveEventMask(
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetInactiveEventMask");
	
	inactiveEventMask_ = eventMask;
	if (!IsActive())
	{
		int saveWind = ::glutGetWindow();
		if (saveWind != windowNum_)
		{
			::glutSetWindow(windowNum_);
		}
		RegisterCallbacks_(eventMask);
		if (saveWind != 0 && saveWind != windowNum_)
		{
			::glutSetWindow(saveWind);
		}
	}
}


#ifndef GLOW_OPTION_STRICTGLUT3

void GlowSubwindow::SetJoystickPollInterval(
	int interval)
{
	joystickPollInterval_ = interval;
	if (joystickPollInterval_ < 0) joystickPollInterval_ = 0;
	
	if ((IsActive() ? eventMask_ : inactiveEventMask_) & Glow::joystickEvents)
	{
		::glutJoystickFunc(Glow::JoystickFunc_, joystickPollInterval_);
	}
	else
	{
		::glutJoystickFunc(0, 0);
	}
}

#endif


void GlowSubwindow::Move(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Move");
	
	clock_ = Glow::clock_;
	globalXPos_ = x;
	globalYPos_ = y;
	GlowSubwindow* parent = ParentWindow();
	if (parent != 0)
	{
		globalXPos_ += parent->GlobalPositionX();
		globalYPos_ += parent->GlobalPositionY();
	}
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutPositionWindow(x, y);
	if (saveWind != 0 && saveWind != windowNum_)
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
	
	width_ = width;
	height_ = height;
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutReshapeWindow(width, height);
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Raise()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Raise");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutPopWindow();
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Lower()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Lower");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutPushWindow();
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Show()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Show");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutShowWindow();
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Hide()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Hide");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutHideWindow();
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::SetCursor(
	Glow::Cursor cursor)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetCursor");
	
	saveCursor_ = cursor;
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutSetCursor(cursor);
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::SetMenu(
	Glow::MouseButton button,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetMenu");
	
	if (button == Glow::leftButton)
	{
		leftMenu_ = menu;
	}
	else if (button == Glow::middleButton)
	{
		centerMenu_ = menu;
	}
	else //if (button == Glow::rightButton)
	{
		rightMenu_ = menu;
	}
	if (menu != 0)
	{
		if ((IsActive() && (eventMask_ & Glow::menuEvents) != 0) ||
			(!IsActive() && (inactiveEventMask_ & Glow::menuEvents) != 0))
		{
			int saveWind = ::glutGetWindow();
			if (saveWind != windowNum_)
			{
				::glutSetWindow(windowNum_);
			}
			int saveMenu = ::glutGetMenu();
			if (saveMenu != menu->menuNum_)
			{
				::glutSetMenu(menu->menuNum_);
			}
			::glutAttachMenu(button);
			if (saveWind != 0 && saveWind != windowNum_)
			{
				::glutSetWindow(saveWind);
			}
			if (saveMenu != 0 && saveMenu != menu->menuNum_)
			{
				::glutSetMenu(saveMenu);
			}
		}
	}
	else
	{
		int saveWind = ::glutGetWindow();
		if (saveWind != windowNum_)
		{
			::glutSetWindow(windowNum_);
		}
		::glutDetachMenu(button);
		if (saveWind != 0 && saveWind != windowNum_)
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
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	ret = ::glutGet(key);
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
	return ret;
}


void GlowSubwindow::EventsForActivation_(
	bool activating)
{
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	if (activating)
	{
		::glutSetCursor(saveCursor_);
		RegisterCallbacks_(eventMask_);
	}
	else
	{
		::glutSetCursor(GLUT_CURSOR_INHERIT);
		RegisterCallbacks_(inactiveEventMask_);
	}
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
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
	eventMask_ = params.eventMask;
	inactiveEventMask_ = params.inactiveEventMask;
	bufferType_ = params.mode;
	leftMenu_ = 0;
	centerMenu_ = 0;
	rightMenu_ = 0;
	saveCursor_ = Glow::inheritCursor;
	width_ = params.width;
	height_ = params.height;
	title_ = new char[GLOW_CSTD::strlen(params.title)+1];
	GLOW_CSTD::strcpy(title_, params.title);
	if (params.iconTitle == 0)
	{
		iconTitle_ = new char[GLOW_CSTD::strlen(params.title)+1];
		GLOW_CSTD::strcpy(iconTitle_, params.title);
	}
	else
	{
		iconTitle_ = new char[GLOW_CSTD::strlen(params.iconTitle)+1];
		GLOW_CSTD::strcpy(iconTitle_, params.iconTitle);
	}
	::glutInitWindowSize(params.width, params.height);
	::glutInitWindowPosition(params.x, params.y);
#ifndef GLOW_OPTION_STRICTGLUT3
	if (params.modeString != 0)
	{
		char* str = new char[GLOW_CSTD::strlen(params.modeString)+1];
		GLOW_CSTD::strcpy(str, params.modeString);
		::glutInitDisplayString(str);
		delete[] str;
	}
	else
	{
#endif
		::glutInitDisplayMode(params.mode);
#ifndef GLOW_OPTION_STRICTGLUT3
	}
#endif
	windowNum_ = ::glutCreateWindow(title_);
	::glutSetIconTitle(iconTitle_);
	RegisterCallbacks_(params.eventMask);
	Glow::AddWindow_(this, windowNum_);
	needSwapBuffers_ = ((params.mode & Glow::doubleBuffer) != 0);
	refreshEnabled_ = true;
	autoSwapBuffers_ = true;
	clock_ = Glow::clock_;
	globalXPos_ = ::glutGet((GLenum)GLUT_WINDOW_X);
	globalYPos_ = ::glutGet((GLenum)GLUT_WINDOW_Y);
	if (Glow::NumModalWindows() != 0)
	{
		activeState_ = 2;
		Glow::TopModalWindow()->Raise();
	}
#ifndef GLOW_OPTION_STRICTGLUT3
	joystickPollInterval_ = 0;
#endif
}


void GlowWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowWindow::Init");
	
	GlowWindowParams params;
	params.title = title;
	params.x = x;
	params.y = y;
	params.width = width;
	params.height = height;
	params.mode = mode;
	params.modeString = 0;
	params.eventMask = eventMask;
	Init(params);
}


#ifndef GLOW_OPTION_STRICTGLUT3
void GlowWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	const char* modeString,
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowWindow::Init");
	
	GLOW_ASSERT(modeString != 0);
	
	GlowWindowParams params;
	params.title = title;
	params.x = x;
	params.y = y;
	params.width = width;
	params.height = height;
	params.mode = Glow::noBuffer;
	params.modeString = modeString;
	params.eventMask = eventMask;
	Init(params);
}
#endif


void GlowWindow::Maximize()
{
	GLOW_DEBUGSCOPE("GlowWindow::Maximize");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutFullScreen();
	if (saveWind != 0 && saveWind != windowNum_)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowWindow::Iconify()
{
	GLOW_DEBUGSCOPE("GlowWindow::Iconify");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	::glutIconifyWindow();
	if (saveWind != 0 && saveWind != windowNum_)
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
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	delete[] title_;
	title_ = new char[GLOW_CSTD::strlen(name)+1];
	GLOW_CSTD::strcpy(title_, name);
	::glutSetWindowTitle(title_);
	if (saveWind != 0 && saveWind != windowNum_)
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
	if (saveWind != windowNum_)
	{
		::glutSetWindow(windowNum_);
	}
	delete[] iconTitle_;
	iconTitle_ = new char[GLOW_CSTD::strlen(name)+1];
	GLOW_CSTD::strcpy(iconTitle_, name);
	::glutSetIconTitle(iconTitle_);
	if (saveWind != 0 && saveWind != windowNum_)
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
	
	menuNum_ = ::glutCreateMenu(Glow::MenuFunc_);
	Glow::AddMenu_(this, menuNum_);
	bindState_ = bindNormal;
}


GlowMenu::~GlowMenu()
{
	GLOW_DEBUGSCOPE("GlowMenu::~GlowMenu");
	
	Glow::RemoveMenu_(menuNum_);
	::glutDestroyMenu(menuNum_);
	for (MenuItemIterator_ iter = itemData_.begin(),
		enditer = itemData_.end(); iter != enditer; ++iter)
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
	
	if (bindState_ != bindNone)
	{
		GlowMenuMessage msg;
		msg.code = code;
		msg.menu = this;
		msg.window = window;
		msg.x = x;
		msg.y = y;
		if (bindState_ == bindSubwindow)
		{
			window->OnDirectMenuHit(msg);
		}
		else
		{
			sender_.Send(msg);
		}
	}
}


void GlowMenu::SetupItem_(
	int itemNum,
	int untilNum) const
{
	if (untilNum == -1) untilNum = itemNum+1;
	char buf[200];
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(menuNum_);
	MenuItemConstIterator_ iter = itemData_.begin()+itemNum;
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
				::glutChangeToSubMenu(i+1, buf, (*iter).subMenu->menuNum_);
			}
			else
			{
				::glutAddSubMenu(buf, (*iter).subMenu->menuNum_);
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
	itemData_.push_back(item);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(menuNum_);
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
	itemData_.push_back(item);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(menuNum_);
	::glutAddSubMenu((char*)label, menu->menuNum_);
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
	GLOW_DEBUG(itemNum<0 || itemNum>int(itemData_.size()),
		"itemNum out of range in GlowMenu::InsertEntry");
	
	Glow_MenuItem item;
	item.label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = code;
	item.subMenu = 0;
	itemData_.insert(itemData_.begin()+itemNum, item);
	
	SetupItem_(itemNum, itemData_.size());
}


void GlowMenu::InsertSubmenu(
	int itemNum,
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::InsertSubmenu");
	GLOW_DEBUG(itemNum<0 || itemNum>int(itemData_.size()),
		"itemNum out of range in GlowMenu::InsertSubmenu");
	
	Glow_MenuItem item;
	item.label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = 0;
	item.subMenu = menu;
	itemData_.insert(itemData_.begin()+itemNum, item);
	
	SetupItem_(itemNum, itemData_.size());
}


void GlowMenu::RemoveItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowMenu::RemoveItem");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
	delete[] (*iter).label;
	delete[] (*iter).mark;
	itemData_.erase(iter);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(menuNum_);
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
	
	int max = itemData_.size();
	for (int i=start+1; i<max; i++)
	{
		if (itemData_[i].mark != 0)
			return i;
	}
	return -1;
}


void GlowMenu::SetItemLabel(
	int itemNum,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemLabel");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	GLOW_ASSERT(label);
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	SetupItem_(itemNum);
}


void GlowMenu::SetItemMark(
	int itemNum,
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemMark");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
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
	SetupItem_(itemNum);
}


void GlowMenu::ToggleItemMark(
	int itemNum,
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowMenu::ToggleItemMark");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
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
	SetupItem_(itemNum);
}


void GlowMenu::UnmarkItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowMenu::UnmarkItem");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
	delete[] (*iter).mark;
	(*iter).mark = 0;
	SetupItem_(itemNum);
}


void GlowMenu::UnmarkAllItems()
{
	GLOW_DEBUGSCOPE("GlowMenu::UnmarkAllItems");
	
	int itemNum = 0;
	for (MenuItemIterator_ iter = itemData_.begin(),
		enditer = itemData_.end(); iter != enditer; ++iter)
	{
		delete[] (*iter).mark;
		(*iter).mark = 0;
		SetupItem_(itemNum);
		itemNum++;
	}
}


void GlowMenu::ChangeToEntry(
	int itemNum,
	const char* label,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::ChangeToEntry");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	GLOW_ASSERT(label);
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	delete[] (*iter).mark;
	(*iter).mark = 0;
	(*iter).code = code;
	(*iter).subMenu = 0;
	SetupItem_(itemNum);
}


void GlowMenu::SetItemCode(
	int itemNum,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemCode");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
	(*iter).code = code;
	(*iter).subMenu = 0;
	SetupItem_(itemNum);
}


void GlowMenu::SetCodesToItemNumbers()
{
	GLOW_DEBUGSCOPE("GlowMenu::SetCodesToItemNumbers");
	
	int mx = itemData_.size();
	for (int i=0; i<mx; ++i)
	{
		if (itemData_[i].subMenu == 0)
		{
			itemData_[i].code = i;
		}
	}
}


void GlowMenu::ChangeToSubmenu(
	int itemNum,
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::ChangeToSubmenu");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	delete[] (*iter).mark;
	(*iter).mark = 0;
	(*iter).code = 0;
	(*iter).subMenu = menu;
	SetupItem_(itemNum);
}


void GlowMenu::SetItemSubmenu(
	int itemNum,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemSubmenu");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(itemData_.size()),
		"itemnum out of range");
	
	MenuItemIterator_ iter = itemData_.begin()+itemNum;
	(*iter).code = 0;
	(*iter).subMenu = menu;
	SetupItem_(itemNum);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

