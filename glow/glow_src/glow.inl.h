/*
===============================================================================

	FILE:  glow.inl.h
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods for message types
===============================================================================
*/

inline GlowTimerMessage::GlowTimerMessage(
	int i)
{
	id = i;
}


inline GlowIdleMessage::GlowIdleMessage()
{
}


/*
===============================================================================
	Inline methods for event filter types
===============================================================================
*/

inline GlowKeyboardData::GlowKeyboardData()
{
	_continue = true;
}


inline GlowMouseData::GlowMouseData()
{
	_continue = true;
}


/*
===============================================================================
	Inline methods for Glow
===============================================================================
*/

/*
-------------------------------------------------------------------------------
	General methods
-------------------------------------------------------------------------------
*/

inline bool Glow::IsMenuInUse()
{
	return menuInUse_;
}


inline int Glow::NumMouseButtons()
{
	return ::glutDeviceGet(GLenum(GLUT_NUM_MOUSE_BUTTONS));
}


#ifndef GLOW_OPTION_STRICTGLUT3
inline bool Glow::HasJoystick()
{
	return ::glutDeviceGet(GLenum(GLUT_HAS_JOYSTICK)) != 0;
}


inline int Glow::NumJoystickButtons()
{
	return ::glutDeviceGet(GLenum(GLUT_JOYSTICK_BUTTONS));
}


inline int Glow::NumJoystickAxes()
{
	return ::glutDeviceGet(GLenum(GLUT_JOYSTICK_AXES));
}
#endif


/*
-------------------------------------------------------------------------------
	Modal window methods
-------------------------------------------------------------------------------
*/

inline int Glow::NumModalWindows()
{
	return modalWindows_.size();
}


inline GlowWindow* Glow::TopModalWindow()
{
	return modalWindows_.empty() ? 0 : modalWindows_.back();
}


/*
-------------------------------------------------------------------------------
	Idle function methods
-------------------------------------------------------------------------------
*/

inline void Glow::UnregisterAllIdle()
{
	idleSender_.UnbindAll();
	::glutIdleFunc(0);
}


inline unsigned int Glow::NumRegisteredIdle()
{
	return idleSender_.NumReceivers();
}


inline bool Glow::IsIdleRegistered(
	GlowIdleReceiver* idle)
{
	return idleSender_.IsBoundTo(idle);
}


/*
-------------------------------------------------------------------------------
	Timer function methods
-------------------------------------------------------------------------------
*/

inline bool Glow::IsTimerPending(
	int id)
{
	return timerSenders_.find(id) != timerSenders_.end();
}


/*
-------------------------------------------------------------------------------
	Filter methods
-------------------------------------------------------------------------------
*/

inline void Glow::UnregisterAllMouseFilters()
{
	mouseFilters_.UnbindAll();
}


inline void Glow::UnregisterAllKeyboardFilters()
{
	keyboardFilters_.UnbindAll();
}


inline unsigned int Glow::NumRegisteredMouseFilters()
{
	return mouseFilters_.NumReceivers();
}


inline unsigned int Glow::NumRegisteredKeyboardFilters()
{
	return keyboardFilters_.NumReceivers();
}


inline bool Glow::IsFilterRegistered(
	GlowMouseFilter* filter)
{
	return mouseFilters_.IsBoundTo(filter);
}


inline bool Glow::IsFilterRegistered(
	GlowKeyboardFilter* filter)
{
	return keyboardFilters_.IsBoundTo(filter);
}


inline void Glow::RegisterFilter(
	GlowMouseFilter* filter)
{
	if (!mouseFilters_.IsBoundTo(filter))
	{
		mouseFilters_.Bind(filter);
	}
}


inline void Glow::RegisterFilter(
	GlowKeyboardFilter* filter)
{
	if (!keyboardFilters_.IsBoundTo(filter))
	{
		keyboardFilters_.Bind(filter);
	}
}


inline void Glow::UnregisterFilter(
	GlowMouseFilter* filter)
{
	if (mouseFilters_.IsBoundTo(filter))
	{
		mouseFilters_.Unbind(filter);
	}
}


inline void Glow::UnregisterFilter(
	GlowKeyboardFilter* filter)
{
	if (keyboardFilters_.IsBoundTo(filter))
	{
		keyboardFilters_.Unbind(filter);
	}
}


/*
-------------------------------------------------------------------------------
	Event simulation methods
-------------------------------------------------------------------------------
*/

inline void Glow::DeliverKeyboardEvt(
	GlowSubwindow* receiver,
	Glow::KeyCode key,
	int x,
	int y,
	Modifiers modifiers)
{
	receiver->OnKeyboard(key, x, y, modifiers);
}


#ifndef GLOW_OPTION_STRICTGLUT3
inline void Glow::DeliverKeyboardUpEvt(
	GlowSubwindow* receiver,
	Glow::KeyCode key,
	int x,
	int y,
	Modifiers modifiers)
{
	receiver->OnKeyboardUp(key, x, y, modifiers);
}
#endif


inline void Glow::DeliverMouseDownEvt(
	GlowSubwindow* receiver,
	Glow::MouseButton button,
	int x,
	int y,
	Modifiers modifiers)
{
	receiver->OnMouseDown(button, x, y, modifiers);
}


inline void Glow::DeliverMouseUpEvt(
	GlowSubwindow* receiver,
	Glow::MouseButton button,
	int x,
	int y,
	Modifiers modifiers)
{
	receiver->OnMouseUp(button, x, y, modifiers);
}


inline void Glow::DeliverMouseDragEvt(
	GlowSubwindow* receiver,
	int x,
	int y)
{
	receiver->OnMouseDrag(x, y);
}


inline void Glow::DeliverMouseMotionEvt(
	GlowSubwindow* receiver,
	int x,
	int y)
{
	receiver->OnMouseMotion(x, y);
}


inline void Glow::DeliverMouseEnterEvt(
	GlowSubwindow* receiver)
{
	receiver->OnMouseEnter();
}


inline void Glow::DeliverMouseExitEvt(
	GlowSubwindow* receiver)
{
	receiver->OnMouseExit();
}


/*
-------------------------------------------------------------------------------
	Random drawing method
-------------------------------------------------------------------------------
*/

inline void Glow::SwapBuffers()
{
	::glutSwapBuffers();
}


/*
-------------------------------------------------------------------------------
	Miscellaneous methods
-------------------------------------------------------------------------------
*/

inline int Glow::GetMilliseconds()
{
	return ::glutGet((GLenum)GLUT_ELAPSED_TIME);
}


inline void Glow::SetMenuStatusFunc(
	void (*func)(int status, int x, int y))
{
	userMenuStatusFunc_ = func;
}


/*
-------------------------------------------------------------------------------
	Toplevel window methods
-------------------------------------------------------------------------------
*/

inline int Glow::NumToplevelWindows()
{
	return numToplevelWindows_;
}


inline bool Glow::IsAutoQuitting()
{
	return autoQuitting_;
}


inline void Glow::SetAutoQuitting(
	bool autoQuit)
{
	autoQuitting_ = autoQuit;
}


/*
===============================================================================
	Inline methods for GlowComponent
===============================================================================
*/

inline GlowComponent::GlowComponent()
{
	initializeState_ = -1;
	parent_ = 0;
}


inline GlowComponent::GlowComponent(
	GlowComponent* parent)
{
	initializeState_ = -1;
	Init(parent);
}


inline GlowComponent* GlowComponent::Next() const
{
	return next_;
}


inline GlowComponent* GlowComponent::Prev() const
{
	return prev_;
}


inline GlowComponent* GlowComponent::Parent() const
{
	return parent_;
}


inline GlowSubwindow* GlowComponent::ParentWindow() const
{
	if (parent_ == 0)
	{
		return 0;
	}
	else
	{
		return parent_->WhichWindow();
	}
}


inline bool GlowComponent::IsTopLevel() const
{
	return parent_ == 0;
}


inline int GlowComponent::NumChildren() const
{
	return numChildren_;
}


inline GlowComponent* GlowComponent::FirstChild() const
{
	return firstChild_;
}


inline GlowComponent* GlowComponent::LastChild() const
{
	return lastChild_;
}


inline bool GlowComponent::IsActive() const
{
	return activeState_ == 1;
}


inline bool GlowComponent::IsActiveStandby() const
{
	return activeState_ == 2;
}


inline bool GlowComponent::IsInactive() const
{
	return activeState_ == 0;
}


inline bool GlowComponent::IsInitialized() const
{
	return initializeState_ != -1;
}


inline bool GlowComponent::IsClosing() const
{
	return initializeState_ == 0;
}


/*
===============================================================================
	Inline methods for GlowSubwindow
===============================================================================
*/

inline GlowSubwindow::GlowSubwindow()
{
}


inline GlowSubwindow::GlowSubwindow(
	GlowComponent* parent,
	const GlowSubwindowParams& params)
{
	Init(parent, params);
}


inline GlowSubwindow::GlowSubwindow(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	Init(parent, x, y, width, height, mode, eventMask);
}


#ifndef GLOW_OPTION_STRICTGLUT3
inline GlowSubwindow::GlowSubwindow(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	const char* modeString,
	Glow::EventMask eventMask)
{
	Init(parent, x, y, width, height, modeString, eventMask);
}
#endif


inline bool GlowSubwindow::IsRefreshEnabled() const
{
	return refreshEnabled_;
}


inline void GlowSubwindow::SetRefreshEnabled(
	bool enable)
{
	refreshEnabled_ = enable;
}


inline void GlowSubwindow::Refresh()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Refresh");
	
	if (refreshEnabled_)
	{
		Glow::RefreshGlutWindow(windowNum_);
	}
}


inline bool GlowSubwindow::IsAutoSwapBuffersEnabled() const
{
	return autoSwapBuffers_;
}


inline void GlowSubwindow::SetAutoSwapBuffersEnabled(
	bool enable)
{
	autoSwapBuffers_ = enable;
}


#ifndef GLOW_OPTION_STRICTGLUT3
inline bool GlowSubwindow::IsKeyRepeatEnabled() const
{
	return ::glutDeviceGet(GLenum(GLUT_DEVICE_IGNORE_KEY_REPEAT)) == 0;
}


inline void GlowSubwindow::SetKeyRepeatEnabled(
	bool enable)
{
	::glutIgnoreKeyRepeat(enable ? 0 : 1);
}


inline int GlowSubwindow::GetJoystickPollInterval() const
{
	return joystickPollInterval_;
}
#endif


inline void GlowSubwindow::MakeCurGlutWindow()
{
	::glutSetWindow(windowNum_);
}


inline int GlowSubwindow::GlutWindowNum() const
{
	return windowNum_;
}


inline int GlowSubwindow::PositionX() const
{
	if (parent_ == 0)
	{
		return GlobalPositionX();
	}
	else
	{
		return GlobalPositionX() -
			ParentWindow()->GlobalPositionX();
	}
}


inline int GlowSubwindow::PositionY() const
{
	if (parent_ == 0)
	{
		return GlobalPositionY();
	}
	else
	{
		return GlobalPositionY() -
			ParentWindow()->GlobalPositionY();
	}
}


inline int GlowSubwindow::GlobalPositionX() const
{
	if (clock_ != Glow::clock_)
	{
		clock_ = Glow::clock_;
		globalXPos_ = GlutInfo((GLenum)GLUT_WINDOW_X);
		globalYPos_ = GlutInfo((GLenum)GLUT_WINDOW_Y);
	}
	return globalXPos_;
}


inline int GlowSubwindow::GlobalPositionY() const
{
	if (clock_ != Glow::clock_)
	{
		clock_ = Glow::clock_;
		globalXPos_ = GlutInfo((GLenum)GLUT_WINDOW_X);
		globalYPos_ = GlutInfo((GLenum)GLUT_WINDOW_Y);
	}
	return globalYPos_;
}


inline int GlowSubwindow::Width() const
{
	return width_;
}


inline int GlowSubwindow::Height() const
{
	return height_;
}


inline Glow::Cursor GlowSubwindow::GetCursor() const
{
	return Glow::Cursor(GlutInfo((GLenum)GLUT_WINDOW_CURSOR));
}


#ifndef GLOW_OPTION_STRICTGLUT3
inline void GlowSubwindow::WarpCursor(
	int x,
	int y) const
{
	::glutWarpPointer(x, y);
}
#endif


inline GlowMenu* GlowSubwindow::GetMenu(
	Glow::MouseButton button) const
{
	if (button == Glow::leftButton)
	{
		return leftMenu_;
	}
	else if (button == Glow::middleButton)
	{
		return centerMenu_;
	}
	else //if (button == Glow::rightButton)
	{
		return rightMenu_;
	}
}


inline void GlowSubwindow::UnsetMenu(
	Glow::MouseButton button)
{
	SetMenu(button, 0);
}


inline Glow::EventMask GlowSubwindow::GetEventMask() const
{
	return eventMask_;
}


inline Glow::EventMask GlowSubwindow::GetInactiveEventMask() const
{
	return inactiveEventMask_;
}


inline Glow::BufferType GlowSubwindow::GetBufferType() const
{
	return bufferType_;
}


inline void GlowSubwindow::NormalizeCoordinates(
	int x,
	int y,
	GLfloat& xn,
	GLfloat& yn) const
{
	xn = GLfloat(x)*2.0f/GLfloat(width_)-1.0f;
	yn = 1.0f-GLfloat(y)*2.0f/GLfloat(height_);
}


inline void GlowSubwindow::FinishRender_() const
{
	::glFlush();
	if (needSwapBuffers_ && autoSwapBuffers_)
	{
		::glutSwapBuffers();
	}
}


/*
===============================================================================
	Inline methods for GlowWindow
===============================================================================
*/

inline GlowWindow::GlowWindow()
{
}


inline GlowWindow::GlowWindow(
	const GlowWindowParams& params)
{
	Init(params);
}


inline GlowWindow::GlowWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	Init(title, x, y, width, height, mode, eventMask);
}


#ifndef GLOW_OPTION_STRICTGLUT3
inline GlowWindow::GlowWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	const char* modeString,
	Glow::EventMask eventMask)
{
	Init(title, x, y, width, height, modeString, eventMask);
}
#endif


inline const char* GlowWindow::GetTitle() const
{
	return title_;
}


inline const char* GlowWindow::GetIconTitle() const
{
	return iconTitle_;
}


/*
===============================================================================
	Inline methods for GlowMenu
===============================================================================
*/

inline bool GlowMenu::IsEntry(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(itemData_.size()));
	
	return itemData_[itemNum].subMenu == 0;
}


inline bool GlowMenu::IsSubmenu(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(itemData_.size()));
	
	return itemData_[itemNum].subMenu != 0;
}


inline const char* GlowMenu::GetItemLabel(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(itemData_.size()));
	
	return itemData_[itemNum].label;
}


inline const char* GlowMenu::GetItemMark(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(itemData_.size()));
	
	return itemData_[itemNum].mark;
}


inline int GlowMenu::GetItemCode(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(itemData_.size()));
	GLOW_DEBUG(itemData_[itemNum].subMenu != 0,
		"GlowMenu::GetItemCode on submenu item");
	
	return itemData_[itemNum].code;
}


inline GlowMenu* GlowMenu::GetItemSubmenu(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(itemData_.size()));
	GLOW_DEBUG(itemData_[itemNum].subMenu == 0,
		"GlowMenu::GetItemSubmenu on entry item");
	
	return itemData_[itemNum].subMenu;
}


inline bool GlowMenu::IsItemMarked(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(itemData_.size()));
	
	return itemData_[itemNum].mark != 0;
}


inline int GlowMenu::NumItems() const
{
	return itemData_.size();
}


inline void GlowMenu::SetBindState(
	GlowMenu::BindState bindState)
{
	bindState_ = bindState;
}


inline GlowMenu::BindState GlowMenu::GetBindState() const
{
	return bindState_;
}


inline TSender<const GlowMenuMessage&>& GlowMenu::Notifier()
{
	return sender_;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

