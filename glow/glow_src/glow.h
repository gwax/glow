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
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 1.1.1dev  (24 July 2000)
	
	CHANGES:
	
		Original release (DA)

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
class GlowSubwindowParams;
class GlowWindowParams;
class GlowKeyboardData;
class GlowMouseData;
class GlowKeyboardFilter;
class GlowMouseFilter;


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
		enum EventMask
		{
			noEvents = 0x0000,
			keyboardEvents = 0x0001,
			mouseEvents = 0x0002,
			dragEvents = 0x0004,
			motionEvents = 0x0008,
			visibilityEvents = 0x0010,
			focusEvents = 0x0020,
			menuEvents = 0x0040,
#ifndef GLOW_OPTION_STRICTGLUT3
			keyboardUpEvents = 0x0080,
			joystickEvents = 0x0100,
#endif
			allEvents = 0xffff
		};
		
		// Mouse button specification
		enum MouseButton
		{
			leftButton = GLUT_LEFT_BUTTON,
			middleButton = GLUT_MIDDLE_BUTTON,
			rightButton = GLUT_RIGHT_BUTTON
		};
		
		// Joystick button specification
		enum JoystickButtonMask
		{
			joystickButtonA = GLUT_JOYSTICK_BUTTON_A,
			joystickButtonB = GLUT_JOYSTICK_BUTTON_B,
			joystickButtonC = GLUT_JOYSTICK_BUTTON_C,
			joystickButtonD = GLUT_JOYSTICK_BUTTON_D,
			allJoystickButtons_ = 0x7fffffff
		};
		
		// Frame buffer type
		enum BufferType
		{
			noBuffer = 0,
			rgbBuffer = GLUT_RGB,
			alphaBuffer = GLUT_ALPHA,
			rgbaBuffer = GLUT_RGBA | GLUT_ALPHA,
			stencilBuffer = GLUT_STENCIL,
			depthBuffer = GLUT_DEPTH,
			accumBuffer = GLUT_ACCUM,
			doubleBuffer = GLUT_DOUBLE,
			stereoBuffer = GLUT_STEREO,
			multisampleBuffer = GLUT_MULTISAMPLE
		};
		
		// Keyboard modifiers
		enum Modifiers
		{
			noModifier = 0,
			shiftModifier = GLUT_ACTIVE_SHIFT,
			ctrlModifier = GLUT_ACTIVE_CTRL,
			altModifier = GLUT_ACTIVE_ALT
		};
		
		// Special key constants
		enum KeyCode
		{
			backspaceKey = 8,
			tabKey = 9,
			enterKey = 13,
			escapeKey = 27,
			deleteKey = 127,
			specialKeyOffset = 256,
			f1Key = specialKeyOffset+GLUT_KEY_F1,
			f2Key = specialKeyOffset+GLUT_KEY_F2,
			f3Key = specialKeyOffset+GLUT_KEY_F3,
			f4Key = specialKeyOffset+GLUT_KEY_F4,
			f5Key = specialKeyOffset+GLUT_KEY_F5,
			f6Key = specialKeyOffset+GLUT_KEY_F6,
			f7Key = specialKeyOffset+GLUT_KEY_F7,
			f8Key = specialKeyOffset+GLUT_KEY_F8,
			f9Key = specialKeyOffset+GLUT_KEY_F9,
			f10Key = specialKeyOffset+GLUT_KEY_F10,
			f11Key = specialKeyOffset+GLUT_KEY_F11,
			f12Key = specialKeyOffset+GLUT_KEY_F12,
			leftArrowKey = specialKeyOffset+GLUT_KEY_LEFT,
			upArrowKey = specialKeyOffset+GLUT_KEY_UP,
			rightArrowKey = specialKeyOffset+GLUT_KEY_RIGHT,
			downArrowKey = specialKeyOffset+GLUT_KEY_DOWN,
			pageUpKey = specialKeyOffset+GLUT_KEY_PAGE_UP,
			pageDownKey = specialKeyOffset+GLUT_KEY_PAGE_DOWN,
			homeKey = specialKeyOffset+GLUT_KEY_HOME,
			endKey = specialKeyOffset+GLUT_KEY_END,
			insertKey = specialKeyOffset+GLUT_KEY_INSERT
		};
		
		// Cursors
		enum Cursor
		{
			noCursor = GLUT_CURSOR_NONE,
			inheritCursor = GLUT_CURSOR_INHERIT,
			rightArrowCursor = GLUT_CURSOR_RIGHT_ARROW,
			leftArrowCursor = GLUT_CURSOR_LEFT_ARROW,
			infoCursor = GLUT_CURSOR_INFO,
			destroyCursor = GLUT_CURSOR_DESTROY,
			helpCursor = GLUT_CURSOR_HELP,
			cycleCursor = GLUT_CURSOR_CYCLE,
			sprayCursor = GLUT_CURSOR_SPRAY,
			waitCursor = GLUT_CURSOR_WAIT,
			textCursor = GLUT_CURSOR_TEXT,
			crosshairCursor = GLUT_CURSOR_CROSSHAIR,
			upDownCursor = GLUT_CURSOR_UP_DOWN,
			leftRightCursor = GLUT_CURSOR_LEFT_RIGHT,
			topSideCursor = GLUT_CURSOR_TOP_SIDE,
			bottomSideCursor = GLUT_CURSOR_BOTTOM_SIDE,
			leftSideCursor = GLUT_CURSOR_LEFT_SIDE,
			rightSideCursor = GLUT_CURSOR_RIGHT_SIDE,
			topLeftCornerCursor = GLUT_CURSOR_TOP_LEFT_CORNER,
			topRightCornerCursor = GLUT_CURSOR_TOP_RIGHT_CORNER,
			bottomLeftCornerCursor = GLUT_CURSOR_BOTTOM_LEFT_CORNER,
			bottomRightCornerCursor = GLUT_CURSOR_BOTTOM_RIGHT_CORNER,
			fullCrosshairCursor = GLUT_CURSOR_FULL_CROSSHAIR
		};
	
	public:
	
		// General
		static void Init(
			int& argc,
			char** argv);
		static void MainLoop();
		static double Version();
		static int APIVersion();
		
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
		
		// Filters
		inline static void RegisterFilter(
			GlowMouseFilter* filter);
		inline static void RegisterFilter(
			GlowKeyboardFilter* filter);
		inline static void UnregisterFilter(
			GlowMouseFilter* filter);
		inline static void UnregisterFilter(
			GlowKeyboardFilter* filter);
		inline static void UnregisterAllMouseFilters();
		inline static void UnregisterAllKeyboardFilters();
		inline static unsigned int NumRegisteredMouseFilters();
		inline static unsigned int NumRegisteredKeyboardFilters();
		inline static bool IsFilterRegistered(
			GlowMouseFilter* filter);
		inline static bool IsFilterRegistered(
			GlowKeyboardFilter* filter);
		
		// Resolution of window and menu ids
		static GlowSubwindow* ResolveWindow(
			int windowNum);
		static GlowMenu* ResolveMenu(
			int menuNum);
		
		// Modal windows
		static void PushModalWindow(
			GlowWindow* wind);
		static void PopModalWindow();
		inline static int NumModalWindows();
		inline static GlowWindow* TopModalWindow();
		
		// Event simulation
		inline static void DeliverKeyboardEvt(
			GlowSubwindow* receiver,
			Glow::KeyCode key,
			int x,
			int y,
			Modifiers modifiers);
#ifndef GLOW_OPTION_STRICTGLUT3
		inline static void DeliverKeyboardUpEvt(
			GlowSubwindow* receiver,
			Glow::KeyCode key,
			int x,
			int y,
			Modifiers modifiers);
#endif
		inline static void DeliverMouseDownEvt(
			GlowSubwindow* receiver,
			Glow::MouseButton button,
			int x,
			int y,
			Modifiers modifiers);
		inline static void DeliverMouseUpEvt(
			GlowSubwindow* receiver,
			Glow::MouseButton button,
			int x,
			int y,
			Modifiers modifiers);
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
		static void RefreshGlutWindow(
			int id);
		
		// GLUT compatibility
		static void SetIdleFunc(
			void (*func)());
		inline static void SetMenuStatusFunc(
			void (*func)(int status, int x, int y));
		
		// Miscellaneous state
		inline static bool IsMenuInUse();
		inline static int GetMilliseconds();
		static bool IsExtensionSupported(
			const char* extensionName);
		static bool IsBufferTypeSupported(
			BufferType mode);
		inline static int NumMouseButtons();
#ifndef GLOW_OPTION_STRICTGLUT3
		inline static bool HasJoystick();
		inline static int NumJoystickButtons();
		inline static int NumJoystickAxes();
#endif
		
		// Toplevel window counter
		inline static int NumToplevelWindows();
		inline static bool IsAutoQuitting();
		inline static void SetAutoQuitting(
			bool autoQuit = true);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		// Prevent instantiation
		inline Glow() {}
	
	private:
	
		// Map glut ids to window and menu pointers
		static GLOW_STD::map<int, GlowSubwindow*> windowRegistry_;
		static GLOW_STD::map<int, GlowMenu*> menuRegistry_;
		
		// List of deferred activate notifications
		static GLOW_STD::map<GlowComponent*, bool> activateNotifyList_;
		// List of deferred component deletions
		static GLOW_STD::map<GlowComponent*, bool> closeList_;
		// Function to notify for widgets
		static void (*widgetNotifier_)();
		// Used to sync up global position to glut's
		static unsigned long clock_;
		
		// Menu state
		static GlowSubwindow* menuWindow_;
		static int menuXClick_;
		static int menuYClick_;
		static bool menuInUse_;
		
		// Modal window stack
		static GLOW_STD::vector<GlowWindow*> modalWindows_;
		
		// Idle, timer and finalizer senders
		static TSender<const GlowIdleMessage&> idleSender_;
		static GLOW_STD::map<int, TSender<const GlowTimerMessage&>*> timerSenders_;
		
		// Next timer id to assign
		static int nextTimerID_;
		
		// GLUT compatibility callbacks
		static Glow_IdleFuncReceiver* idleFuncReceiver_;
		static void (*userMenuStatusFunc_)(int status, int x, int y);
		
		// Event filter senders
		static TSender<GlowMouseData&> mouseFilters_;
		static TSender<GlowKeyboardData&> keyboardFilters_;
		
		// Toplevel window counter
		static int numToplevelWindows_;
		static bool autoQuitting_;
		
		// Workaround for GLUT redisplay bug
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		static int curDisplayWindow_;
		static int refreshMe_;
#endif
	
	private:
	
		// Manage glut id mappings
		static void AddWindow_(
			GlowSubwindow* window,
			int windowNum);
		static void RemoveWindow_(
			int windowNum);
		
		static void AddMenu_(
			GlowMenu* menu,
			int menuNum);
		static void RemoveMenu_(
			int menuNum);
		
		// Call backs
		static void TimerFunc_(
			int id);
		static void DisplayFunc_();
		static void ReshapeFunc_(
			int width,
			int height);
		static void KeyboardFunc_(
			unsigned char key,
			int x,
			int y);
#ifndef GLOW_OPTION_STRICTGLUT3
		static void KeyboardUpFunc_(
			unsigned char key,
			int x,
			int y);
#endif
		static void MouseFunc_(
			int button,
			int state,
			int x,
			int y);
		static void MotionFunc_(
			int x,
			int y);
		static void PassiveMotionFunc_(
			int x,
			int y);
		static void VisibilityFunc_(
			int state);
		static void EntryFunc_(
			int state);
		static void SpecialFunc_(
			int key,
			int x,
			int y);
#ifndef GLOW_OPTION_STRICTGLUT3
		static void SpecialUpFunc_(
			int key,
			int x,
			int y);
#endif
		static void MenuStatusFunc_(
			int status,
			int x,
			int y);
		static void MenuFunc_(
			int value);
		static void IdleFunc_();
#ifndef GLOW_OPTION_STRICTGLUT3
		static void JoystickFunc_(
			unsigned int buttonMask,
			int x,
			int y,
			int z);
#endif
		
		// Deferred execution
		static void ExecuteDeferred_();
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		static void RaiseDeferredRefresh_();
#endif
};

GLOW_INTERNAL_SETUPENUMBITFIELD(Glow::EventMask)
GLOW_INTERNAL_SETUPENUMBITFIELD(Glow::BufferType)
GLOW_INTERNAL_SETUPENUMBITFIELD(Glow::Modifiers)
#ifndef GLOW_OPTION_STRICTGLUT3
GLOW_INTERNAL_SETUPENUMBITFIELD(Glow::JoystickButtonMask)
#endif


/*
===============================================================================
	Event filters
===============================================================================
*/

class GlowKeyboardData
{
	friend class Glow;
	friend class GlowKeyboardFilter;
	
	public:
	
#ifndef GLOW_OPTION_STRICTGLUT3
		enum EventType
		{
			keyDown = 1,
			keyUp = 2
		};
#endif
	
	public:
	
		GlowSubwindow* subwindow;
#ifndef GLOW_OPTION_STRICTGLUT3
		EventType type;
#endif
		Glow::KeyCode key;
		int x;
		int y;
		Glow::Modifiers modifiers;
	
	private:
	
		inline GlowKeyboardData();
	
	private:
	
		bool _continue;
};


class GlowMouseData
{
	friend class Glow;
	friend class GlowMouseFilter;
	
	public:
	
		enum EventType
		{
			mouseDown = 1,
			mouseUp = 2
		};
	
	public:
	
		GlowSubwindow* subwindow;
		EventType type;
		Glow::MouseButton button;
		int x;
		int y;
		Glow::Modifiers modifiers;
	
	private:
	
		inline GlowMouseData();
	
	private:
	
		bool _continue;
};


class GlowKeyboardFilter :
	public TReceiver<GlowKeyboardData&>
{
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual bool OnFilter(
			GlowKeyboardData& data) = 0;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage(
			GlowKeyboardData& message);
};


class GlowMouseFilter :
	public TReceiver<GlowMouseData&>
{
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual bool OnFilter(
			GlowMouseData& data) = 0;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage(
			GlowMouseData& message);
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
	
		inline GlowComponent(
			GlowComponent* parent);
		inline GlowComponent();
		void Init(
			GlowComponent* parent);
		
		virtual ~GlowComponent();
		void Close();
		
		inline GlowComponent* Next() const;
		inline GlowComponent* Prev() const;
		
		inline GlowComponent* Parent() const;
		GlowSubwindow* WhichWindow();
		inline GlowSubwindow* ParentWindow() const;
		GlowWindow* ToplevelWindow();
		inline bool IsTopLevel() const;
		
		inline int NumChildren() const;
		inline GlowComponent* FirstChild() const;
		inline GlowComponent* LastChild() const;
		void ReorderChild(
			GlowComponent* child,
			GlowComponent* before);
		void KillChildren();
		
		void Activate();
		void Deactivate();
		inline bool IsActive() const;
		inline bool IsActiveStandby() const;
		inline bool IsInactive() const;
		
		inline bool IsInitialized() const;
		inline bool IsClosing() const;
		
		void Paint();
	
	
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
	
		GlowComponent* parent_;
		GlowComponent* next_;
		GlowComponent* prev_;
		int numChildren_;
		GlowComponent* firstChild_;
		GlowComponent* lastChild_;
		short activeState_;
		short initializeState_;
	
	private:
	
		void AddChild_(
			GlowComponent* child);
		void RemoveChild_(
			GlowComponent* child);
		
		void BroadcastStandby_(
			bool activating,
			bool first);
};


/*
===============================================================================
	CLASS GlowMenuMessage

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
	
		enum {
			parentWindowSize = -1
		};
	
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
			Glow::BufferType mode,
			Glow::EventMask eventMask);
#ifndef GLOW_OPTION_STRICTGLUT3
		inline GlowSubwindow(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			const char* modeString,
			Glow::EventMask eventMask);
#endif
		void Init(
			GlowComponent* parent,
			const GlowSubwindowParams& params);
		void Init(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
#ifndef GLOW_OPTION_STRICTGLUT3
		void Init(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			const char* modeString,
			Glow::EventMask eventMask);
#endif
		
		virtual ~GlowSubwindow();
	
	public:
	
		inline void MakeCurGlutWindow();
		int GlutInfo(
			GLenum key) const;
		inline int GlutWindowNum() const;
		
		inline void Refresh();
		inline bool IsRefreshEnabled() const;
		inline void SetRefreshEnabled(
			bool enable);
		inline bool IsAutoSwapBuffersEnabled() const;
		inline void SetAutoSwapBuffersEnabled(
			bool enable);
		
#ifndef GLOW_OPTION_STRICTGLUT3
		inline bool IsKeyRepeatEnabled() const;
		inline void SetKeyRepeatEnabled(
			bool enable);
		void ReadJoystick();
		inline int GetJoystickPollInterval() const;
		void SetJoystickPollInterval(
			int interval);
#endif
		
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
			Glow::MouseButton button,
			GlowMenu* menu = 0);
		inline GlowMenu* GetMenu(
			Glow::MouseButton button) const;
		inline void UnsetMenu(
			Glow::MouseButton button);
		
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
		
		inline Glow::Cursor GetCursor() const;
		void SetCursor(
			Glow::Cursor cursor = Glow::inheritCursor);
#ifndef GLOW_OPTION_STRICTGLUT3
		inline void WarpCursor(
			int x,
			int y) const;
#endif
		
		inline Glow::EventMask GetEventMask() const;
		void SetEventMask(
			Glow::EventMask eventMask);
		inline Glow::EventMask GetInactiveEventMask() const;
		void SetInactiveEventMask(
			Glow::EventMask eventMask);
		
		inline Glow::BufferType GetBufferType() const;
	
	
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
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnMouseUp(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
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
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
#ifndef GLOW_OPTION_STRICTGLUT3
		virtual void OnKeyboardUp(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnJoystick(
			Glow::JoystickButtonMask buttonMask,
			int x,
			int y,
			int z);
#endif
		virtual void OnVisible();
		virtual void OnInvisible();
		virtual void OnDirectMenuHit(
			const GlowMenuMessage& message);
		virtual bool OnBeginPaint();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int width_;
		int height_;
		int windowNum_;
		Glow::EventMask eventMask_;
		Glow::EventMask inactiveEventMask_;
		Glow::BufferType bufferType_;
		Glow::Cursor saveCursor_;
		mutable unsigned long clock_;
		mutable int globalXPos_;
		mutable int globalYPos_;
		GlowMenu* leftMenu_;
		GlowMenu* centerMenu_;
		GlowMenu* rightMenu_;
		bool needSwapBuffers_;
		bool refreshEnabled_;
		bool autoSwapBuffers_;
#ifndef GLOW_OPTION_STRICTGLUT3
		int joystickPollInterval_;
#endif
	
	private:
	
		void RegisterCallbacks_(
			Glow::EventMask eventMask);
		inline void FinishRender_() const;
		void EventsForActivation_(
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
	
		enum {
			autoPosition = -1
		};
	
	public:
	
		inline GlowWindow(
			const GlowWindowParams& params);
		inline GlowWindow(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
#ifndef GLOW_OPTION_STRICTGLUT3
		inline GlowWindow(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			const char* modeString,
			Glow::EventMask eventMask);
#endif
		inline GlowWindow();
		void Init(
			const GlowWindowParams& params);
		void Init(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
#ifndef GLOW_OPTION_STRICTGLUT3
		void Init(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			const char* modeString,
			Glow::EventMask eventMask);
#endif
		
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
	
		char* title_;
		char* iconTitle_;
};


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
		Glow::EventMask eventMask;
		Glow::EventMask inactiveEventMask;
		Glow::BufferType mode;
#ifndef GLOW_OPTION_STRICTGLUT3
		const char* modeString;
#endif
		
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
	
		enum BindState
		{
			bindNone = 0,
			bindNormal = 1,
			bindSubwindow = 2
		};
	
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
			BindState bindState);
		inline BindState GetBindState() const;
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
	
		int menuNum_;
		BindState bindState_;
		GLOW_STD::vector<Glow_MenuItem> itemData_;
		TSender<const GlowMenuMessage&> sender_;
	
	private:
	
		void SetupItem_(
			int itemNum,
			int untilNum = -1) const;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glow.inl.h"


#endif

