/*
===============================================================================

	FILE:  glow.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Basic classes for GLOW
	
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

===============================================================================
*/


#ifndef GLOW__H
#define GLOW__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <GL/glut.h>
#include <map>
#include <list>
#include <vector>

#ifndef GLOW_SENDERRECEIVER__H
	#include "glowSenderReceiver.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Declarations
===============================================================================
*/

class Glow;
class GlowComponent;
class GlowSubwindow;
class GlowWindow;
class GlowMenu;


/*
===============================================================================
	Params
===============================================================================
*/

class GlowSubwindowParams
{
	public:
	
		int width;
		int height;
		int x;
		int y;
		int eventMask;
		int inactiveEventMask;
		int mode;
		
		static GlowSubwindowParams defaults;
		
		GlowSubwindowParams();
	
	protected:
	
		GlowSubwindowParams(bool);
};


class GlowWindowParams :
	public GlowSubwindowParams
{
	public:
	
		const char* title;
		const char* iconTitle;
		
		static GlowWindowParams defaults;
		
		GlowWindowParams();
	
	protected:
	
		GlowWindowParams(bool);
};


/*
===============================================================================
	Message types
===============================================================================
*/

class GlowIdleMessage
{
	friend class Glow;
	
	private:
	
		inline GlowIdleMessage();
};


class GlowTimerMessage
{
	friend class Glow;
	
	private:
	
		inline GlowTimerMessage(
			int i);
	
	public:
	
		int id;
};


typedef TReceiver<const GlowIdleMessage&> GlowIdleReceiver;
typedef TSender<const GlowIdleMessage&> Glow_IdleSender;

typedef TReceiver<const GlowTimerMessage&> GlowTimerReceiver;
typedef TSender<const GlowTimerMessage&> Glow_TimerSender;


/*
===============================================================================
	CLASS Glow

	General utility routines
===============================================================================
*/

class Glow_IdleFuncReceiver;

class Glow
{
	friend class GlowWindow;
	friend class GlowComponent;
	friend class GlowSubwindow;
	friend class GlowMenu;
	friend class GlowWidget;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// Event masks
		static const int noEvents = 0x0000;
		static const int keyboardEvents = 0x0001;
		static const int mouseEvents = 0x0002;
		static const int dragEvents = 0x0004;
		static const int motionEvents = 0x0008;
		static const int visibilityEvents = 0x0010;
		static const int focusEvents = 0x0020;
		static const int menuEvents = 0x0040;
		static const int allEvents = 0xffff;
		
		// Mouse button specification
		static const int leftButton = GLUT_LEFT_BUTTON;
		static const int middleButton = GLUT_MIDDLE_BUTTON;
		static const int rightButton = GLUT_RIGHT_BUTTON;
		
		// Frame buffer type
		static const int rgbBuffer = GLUT_RGB;
		static const int alphaBuffer = GLUT_ALPHA;
		static const int rgbaBuffer = GLUT_RGBA | GLUT_ALPHA;
		static const int stencilBuffer = GLUT_STENCIL;
		static const int depthBuffer = GLUT_DEPTH;
		static const int accumBuffer = GLUT_ACCUM;
		static const int doubleBuffer = GLUT_DOUBLE;
		
		// Keyboard modifiers
		static const int shiftModifier = GLUT_ACTIVE_SHIFT;
		static const int ctrlModifier = GLUT_ACTIVE_CTRL;
		static const int altModifier = GLUT_ACTIVE_ALT;
		
		// Special key constants
		static const int specialKeyOffset = 256;
		static const int f1Key = specialKeyOffset+GLUT_KEY_F1;
		static const int f2Key = specialKeyOffset+GLUT_KEY_F2;
		static const int f3Key = specialKeyOffset+GLUT_KEY_F3;
		static const int f4Key = specialKeyOffset+GLUT_KEY_F4;
		static const int f5Key = specialKeyOffset+GLUT_KEY_F5;
		static const int f6Key = specialKeyOffset+GLUT_KEY_F6;
		static const int f7Key = specialKeyOffset+GLUT_KEY_F7;
		static const int f8Key = specialKeyOffset+GLUT_KEY_F8;
		static const int f9Key = specialKeyOffset+GLUT_KEY_F9;
		static const int f10Key = specialKeyOffset+GLUT_KEY_F10;
		static const int f11Key = specialKeyOffset+GLUT_KEY_F11;
		static const int f12Key = specialKeyOffset+GLUT_KEY_F12;
		static const int leftArrowKey = specialKeyOffset+GLUT_KEY_LEFT;
		static const int upArrowKey = specialKeyOffset+GLUT_KEY_UP;
		static const int rightArrowKey = specialKeyOffset+GLUT_KEY_RIGHT;
		static const int downArrowKey = specialKeyOffset+GLUT_KEY_DOWN;
		static const int pageUpKey = specialKeyOffset+GLUT_KEY_PAGE_UP;
		static const int pageDownKey = specialKeyOffset+GLUT_KEY_PAGE_DOWN;
		static const int homeKey = specialKeyOffset+GLUT_KEY_HOME;
		static const int endKey = specialKeyOffset+GLUT_KEY_END;
		static const int insertKey = specialKeyOffset+GLUT_KEY_INSERT;
		static const int backspaceKey = 8;
		static const int tabKey = 9;
		static const int enterKey = 13;
		static const int escapeKey = 27;
		static const int deleteKey = 127;
	
	public:
	
		// General
		inline static void Init(
			int& argc,
			char** argv);
		inline static void MainLoop();
		static double Version();
		
		// Idle tasks
		static void RegisterIdle(
			GlowIdleReceiver* idle);
		static void UnregisterIdle(
			GlowIdleReceiver* idle);
		inline static void UnregisterAllIdle();
		inline static unsigned int NumRegisteredIdle();
		inline static bool IsIdleRegistered(
			GlowIdleReceiver* idle);
		
		// Timer tasks
		static int RegisterTimer(
			unsigned int msecs,
			GlowTimerReceiver* timer);
		static void UnregisterTimer(
			int id);
		inline static bool IsTimerPending(
			int id);
		
		// Resolution of window and menu ids
		inline static GlowSubwindow* ResolveWindow(
			int windowNum);
		inline static GlowMenu* ResolveMenu(
			int menuNum);
		
		// Menu state
		inline static bool IsMenuInUse();
		
		// Modal windows
		static void PushModalWindow(
			GlowWindow* wind);
		static void PopModalWindow();
		inline static int NumModalWindows();
		inline static GlowWindow* TopModalWindow();
		
		// Event simulation
		inline static void DeliverKeyboardEvt(
			GlowSubwindow* receiver,
			int key,
			int modifiers,
			int x,
			int y);
		inline static void DeliverMouseDownEvt(
			GlowSubwindow* receiver,
			int button,
			int x,
			int y,
			int modifiers);
		inline static void DeliverMouseUpEvt(
			GlowSubwindow* receiver,
			int button,
			int x,
			int y,
			int modifiers);
		inline static void DeliverMouseDragEvt(
			GlowSubwindow* receiver,
			int x,
			int y);
		inline static void DeliverMouseMotionEvt(
			GlowSubwindow* receiver,
			int x,
			int y);
		inline static void DeliverMouseEnterEvt(
			GlowSubwindow* receiver);
		inline static void DeliverMouseExitEvt(
			GlowSubwindow* receiver);
		
		// Drawing
		inline static void SwapBuffers();
		
		// Miscellaneous
		inline static int GetMilliseconds();
		static void SetIdleFunc(
			void (*func)());
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		typedef GLOW_STD::map<int, GlowSubwindow*>::value_type _WindowRegistryEntry;
		typedef GLOW_STD::map<int, GlowMenu*>::value_type _MenuRegistryEntry;
		typedef GLOW_STD::map<int, GlowSubwindow*>::iterator _WindowRegistryIterator;
		typedef GLOW_STD::map<int, GlowMenu*>::iterator _MenuRegistryIterator;
	
	private:
	
		// Map glut ids to window and menu pointers
		static GLOW_STD::map<int, GlowSubwindow*> _windowRegistry;
		static GLOW_STD::map<int, GlowMenu*> _menuRegistry;
		
		// List of deferred activate notifications
		static GLOW_STD::map<GlowComponent*, bool> _activateNotifyList;
		// List of deferred component deletions
		static GLOW_STD::map<GlowComponent*, bool> _closeList;
		// Function to notify for widgets
		static void (*_widgetNotifier)();
		// Used to sync up global position to glut's
		static unsigned long _clock;
		
		// Menu state
		static GlowSubwindow* _menuWindow;
		static int _menuXClick;
		static int _menuYClick;
		static bool _menuInUse;
		
		// Modal window stack
		static GLOW_STD::vector<GlowWindow*> _modalWindows;
		
		// Idle, timer and finalizer senders
		static TSender<const GlowIdleMessage&> _idleSender;
		static GLOW_STD::map<int, TSender<const GlowTimerMessage&>*> _timerSenders;
		
		// Next timer id to assign
		static int _nextTimerID;
		
		// Special idle receiver for callback-based idle
		static Glow_IdleFuncReceiver* _idleFuncReceiver;
	
	private:
	
		// Manage glut id mappings
		inline static void _AddWindow(
			GlowSubwindow* window,
			int windowNum);
		static void _RemoveWindow(
			int windowNum);
		
		inline static void _AddMenu(
			GlowMenu* menu,
			int menuNum);
		inline static void _RemoveMenu(
			int menuNum);
		
		// Call backs
		static void _TimerFunc(
			int id);
		static void _DisplayFunc();
		static void _ReshapeFunc(
			int width,
			int height);
		static void _KeyboardFunc(
			unsigned char key,
			int x,
			int y);
		static void _MouseFunc(
			int button,
			int state,
			int x,
			int y);
		static void _MotionFunc(
			int x,
			int y);
		static void _PassiveMotionFunc(
			int x,
			int y);
		static void _VisibilityFunc(
			int state);
		static void _EntryFunc(
			int state);
		static void _SpecialFunc(
			int key,
			int x,
			int y);
		static void _MenuStatusFunc(
			int status,
			int x,
			int y);
		static void _MenuFunc(
			int value);
		static void _IdleFunc();
		
		// Deferred execution
		static void _ExecuteDeferred();
};


/*
===============================================================================
	CLASS GlowComponent

	User interface component class
===============================================================================
*/

class GlowComponent
{
	friend class GlowSubwindow;
	friend class GlowWindow;
	friend class Glow;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef GLOW_STD::list<GlowComponent*>::iterator ChildIterator;
		typedef GLOW_STD::list<GlowComponent*>::const_iterator ConstChildIterator;
	
	public:
	
		inline GlowComponent(
			GlowComponent* parent);
		inline GlowComponent();
		void Init(
			GlowComponent* parent);
		
		virtual ~GlowComponent();
		void Close();
		
		inline GlowComponent* Parent() const;
		virtual GlowSubwindow* WhichWindow();
		inline GlowSubwindow* ParentWindow() const;
		GlowWindow* ToplevelWindow();
		inline bool IsTopLevel() const;
		
		inline int NumChildren() const;
		inline ChildIterator BeginChildren();
		inline ChildIterator EndChildren();
		inline ConstChildIterator BeginChildren() const;
		inline ConstChildIterator EndChildren() const;
		void KillChildren();
		
		void Activate();
		void Deactivate();
		inline bool IsActive() const;
		inline bool IsActiveStandby() const;
		inline bool IsInactive() const;
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnActivate();
		virtual void OnDeactivate();
	
	protected:
	
		virtual bool OnBeginPaint();
		virtual void OnEndPaint();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowComponent* _parent;
		GLOW_STD::list<GlowComponent*> _children;
		short _activeState;
	
	private:
	
		inline void _AddChild(
			GlowComponent* child);
		inline void _RemoveChild(
			GlowComponent* child);
		
		void _BroadcastPaint();
		void _BroadcastStandby(
			bool activating,
			bool first);
};


/*
===============================================================================
	STRUCT GlowMenuMessage

	Message sent on menu hit
===============================================================================
*/

class GlowMenuMessage
{
	public:
	
		int code;
		GlowMenu* menu;
		GlowSubwindow* window;
		int x;
		int y;
};


typedef TReceiver<const GlowMenuMessage&> GlowMenuReceiver;


/*
===============================================================================
	CLASS GlowSubwindow

	User interface component that can receive events
===============================================================================
*/

class GlowSubwindow :
	public GlowComponent
{
	friend class GlowWindow;
	friend class Glow;
	friend class GlowMenu;
	friend class GlowComponent;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const int parentWindowSize = -1;
	
	public:
	
		inline GlowSubwindow();
		inline GlowSubwindow(
			GlowComponent* parent,
			const GlowSubwindowParams& params);
		inline GlowSubwindow(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			int mode,
			int eventMask);
		void Init(
			GlowComponent* parent,
			const GlowSubwindowParams& params);
		void Init(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			int mode,
			int eventMask);
		
		virtual ~GlowSubwindow();
	
	public:
	
		inline void MakeCurGlutWindow();
		int GlutInfo(
			GLenum key) const;
		int GlutWindowNum() const;
		
		void Refresh();
		inline bool IsRefreshEnabled() const;
		inline void SetRefreshEnabled(
			bool enable);
		inline bool IsAutoSwapBuffersEnabled() const;
		inline void SetAutoSwapBuffersEnabled(
			bool enable);
		
		void Move(
			int x,
			int y);
		void Reshape(
			int width,
			int height);
		void Raise();
		void Lower();
		void Show();
		void Hide();
		
		void SetMenu(
			int button,
			GlowMenu* menu = 0);
		inline GlowMenu* GetMenu(
			int button) const;
		inline void UnsetMenu(
			int button);
		
		inline int PositionX() const;
		inline int PositionY() const;
		inline int GlobalPositionX() const;
		inline int GlobalPositionY() const;
		inline int Width() const;
		inline int Height() const;
		inline void NormalizeCoordinates(
			int x,
			int y,
			GLfloat& xn,
			GLfloat& yn) const;
		virtual GlowSubwindow* WhichWindow();
		
		inline int GetCursor() const;
		void SetCursor(
			int cursor = GLUT_CURSOR_INHERIT);
		inline int GetEventMask() const;
		void SetEventMask(
			int eventMask);
		inline int GetInactiveEventMask() const;
		void SetInactiveEventMask(
			int eventMask);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnReshape(
			int width,
			int height);
		virtual void OnMouseEnter();
		virtual void OnMouseExit();
		virtual void OnMouseDown(
			int button,
			int x,
			int y,
			int modifiers);
		virtual void OnMouseUp(
			int button,
			int x,
			int y,
			int modifiers);
		virtual void OnMenuDown(
			int x,
			int y);
		virtual void OnMenuUp();
		virtual void OnMouseMotion(
			int x,
			int y);
		virtual void OnMouseDrag(
			int x,
			int y);
		virtual void OnKeyboard(
			int key,
			int modifiers,
			int x,
			int y);
		virtual void OnVisible();
		virtual void OnInvisible();
		virtual void OnDirectMenuHit(
			const GlowMenuMessage& message);
		virtual bool OnBeginPaint();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _width;
		int _height;
		int _windowNum;
		int _eventMask;
		int _inactiveEventMask;
		int _saveCursor;
		mutable unsigned long _clock;
		mutable int _globalXPos;
		mutable int _globalYPos;
		GlowMenu* _leftMenu;
		GlowMenu* _centerMenu;
		GlowMenu* _rightMenu;
		bool _needSwapBuffers;
		bool _refreshEnabled;
		bool _autoSwapBuffers;
	
	private:
	
		void _RegisterCallbacks(
			int eventMask);
		inline void _FinishRender() const;
		void _EventsForActivation(
			bool activating);
};


/*
===============================================================================
	CLASS GlowWindow

	Top-level window
===============================================================================
*/

class GlowWindow :
	public GlowSubwindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const int autoPosition = -1;
	
	public:
	
		inline GlowWindow(
			const GlowWindowParams& params);
		inline GlowWindow(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			int mode,
			int eventMask);
		inline GlowWindow();
		void Init(
			const GlowWindowParams& params);
		void Init(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			int mode,
			int eventMask);
		
		void Maximize();
		void Iconify();
		
		inline const char* GetTitle() const;
		void SetTitle(
			const char* name);
		inline const char* GetIconTitle() const;
		void SetIconTitle(
			const char* name);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		char* _title;
		char* _iconTitle;
};


/*
===============================================================================
	CLASS Glow_MenuItem

	Menu entry class
===============================================================================
*/

class Glow_MenuItem
{
	friend class GlowMenu;
	
	private:
	
		char* label;
		char* mark;
		int code;
		GlowMenu* subMenu;
};


/*
===============================================================================
	CLASS GlowMenu

	Menu class
	Note: item numbers are 0-based
===============================================================================
*/

class GlowMenu
{
	friend class Glow;
	friend class GlowSubwindow;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const int bindNone = 0;
		static const int bindNormal = 1;
		static const int bindSubwindow = 2;
	
	public:
	
		GlowMenu();
		virtual ~GlowMenu();
	
	public:
	
		// Add and remove items
		void AddEntry(
			const char* label,
			int code);
		void AddSubmenu(
			const char* label,
			GlowMenu* menu);
		void InsertEntry(
			int itemNum,
			const char* label,
			int code);
		void InsertSubmenu(
			int itemNum,
			const char* label,
			GlowMenu* menu);
		void RemoveItem(
			int itemNum);
		inline int NumItems() const;
		
		// Manipulate label
		inline const char* GetItemLabel(
			int itemNum) const;
		void SetItemLabel(
			int itemNum,
			const char* label);
		
		// Manipulate mark
		inline const char* GetItemMark(
			int itemNum) const;
		void SetItemMark(
			int itemNum,
			const char* mark);
		void ToggleItemMark(
			int itemNum,
			const char* mark);
		void UnmarkItem(
			int itemNum);
		inline bool IsItemMarked(
			int itemNum) const;
		int NextMarkedItem(
			int itemNum) const;
		void UnmarkAllItems();
		
		// Manipulate code and submenu data
		inline int GetItemCode(
			int itemNum) const;
		void SetItemCode(
			int itemNum,
			int code);
		void SetCodesToItemNumbers();
		inline GlowMenu* GetItemSubmenu(
			int itemNum) const;
		void SetItemSubmenu(
			int itemNum,
			GlowMenu* menu);
		
		// Query and change item type
		inline bool IsEntry(
			int itemNum) const;
		inline bool IsSubmenu(
			int itemNum) const;
		void ChangeToEntry(
			int itemNum,
			const char* label,
			int code);
		void ChangeToSubmenu(
			int itemNum,
			const char* label,
			GlowMenu* menu);
		
		// Event reporting
		inline void SetBindState(
			int bindState);
		inline int GetBindState() const;
		inline TSender<const GlowMenuMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnHit(
			int code,
			GlowSubwindow* window,
			int x,
			int y);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _menuNum;
		int _bindState;
		GLOW_STD::vector<Glow_MenuItem> _itemData;
		TSender<const GlowMenuMessage&> _sender;
	
	private:
	
		typedef GLOW_STD::vector<Glow_MenuItem>::iterator ItemIterator;
		typedef GLOW_STD::vector<Glow_MenuItem>::const_iterator ItemConstIterator;
	
	private:
	
		void _SetupItem(
			int itemNum,
			int untilNum = -1) const;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glow.inl.h"


#endif

