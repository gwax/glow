/*
===============================================================================

	FILE:  glowWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Widget for GLOW
	
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
	
		The GLOW Toolkit -- version 1.0.0  (29 June 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowWidgetParams
===============================================================================
*/

inline GlowWidgetParams::GlowWidgetParams()
{
}


/*
===============================================================================
	Inline methods of Glow_OldWidgetState
===============================================================================
*/

inline Glow_OldWidgetState::Glow_OldWidgetState(
	bool vis,
	bool keyfoc,
	int wid,
	int hei,
	int xp,
	int yp)
{
	visible = vis;
	keyboardFocus = keyfoc;
	width = wid;
	height = hei;
	x = xp;
	y = yp;
}


/*
===============================================================================
	Inline methods of GlowWidget
===============================================================================
*/

inline GlowWidget::GlowWidget()
{
}


inline GlowWidget::GlowWidget(
	GlowWidgetRoot* root,
	const GlowWidgetParams& params)
{
	Init(root, 0, params);
}


inline GlowWidget::GlowWidget(
	GlowWidget* parent,
	const GlowWidgetParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowWidget::Init(
	GlowWidgetRoot* root,
	const GlowWidgetParams& params)
{
	Init(root, 0, params);
}


inline void GlowWidget::Init(
	GlowWidget* parent,
	const GlowWidgetParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowWidget::Move(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowWidget::Move");
	
	if (xpos_ != x || ypos_ != y)
	{
		BroadcastNotifyList_();
		xpos_ = x;
		ypos_ = y;
		Refresh();
	}
}


inline void GlowWidget::Reshape(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("GlowWidget::Reshape");
	
	if (width_ != width || height_ != height)
	{
		BroadcastNotifyList_();
		width_ = width;
		height_ = height;
		Refresh();
	}
}


inline GlowWidget::AutoPackError GlowWidget::AutoPack(
	int leftLimit,
	int rightLimit,
	int topLimit,
	int bottomLimit,
	AutoPackOptions hOption,
	AutoPackOptions vOption)
{
	int leftMargin = 0;
	int rightMargin = 0;
	int topMargin = 0;
	int bottomMargin = 0;
	return AutoPack(leftLimit, rightLimit, topLimit, bottomLimit,
		hOption, vOption, leftMargin, rightMargin, topMargin, bottomMargin);
}


inline GlowWidget::AutoPackError GlowWidget::AutoReshape()
{
	int leftMargin = 0;
	int rightMargin = 0;
	int topMargin = 0;
	int bottomMargin = 0;
	return AutoPack(unspecifiedPos, unspecifiedPos, unspecifiedPos,
		unspecifiedPos, preferredSize, preferredSize, leftMargin,
		rightMargin, topMargin, bottomMargin);
}


inline GlowWidget::AutoPackError GlowWidget::AutoReshape(
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	return AutoPack(unspecifiedPos, unspecifiedPos, unspecifiedPos,
		unspecifiedPos, preferredSize, preferredSize, leftMargin,
		rightMargin, topMargin, bottomMargin);
}


inline int GlowWidget::Width() const
{
	return width_;
}


inline int GlowWidget::Height() const
{
	return height_;
}


inline int GlowWidget::PositionX() const
{
	return xpos_;
}


inline int GlowWidget::PositionY() const
{
	return ypos_;
}


inline int GlowWidget::GlobalPositionX() const
{
	return RootPositionX() + root_->Subwindow()->GlobalPositionX();
}


inline int GlowWidget::GlobalPositionY() const
{
	return RootPositionY() + root_->Subwindow()->GlobalPositionY();
}


inline GlowWidgetRoot* GlowWidget::Root() const
{
	return root_;
}


inline bool GlowWidget::IsClipping() const
{
	return clipping_;
}


inline void GlowWidget::SetClipping(
	bool clipping)
{
	clipping_ = clipping;
	Refresh();
}


inline long GlowWidget::GetRefCon() const
{
	return refcon_;
}


inline void GlowWidget::SetRefCon(
	long refcon)
{
	refcon_ = refcon;
}


inline bool GlowWidget::IsVisible() const
{
	return visibility_ == 1;
}


inline bool GlowWidget::IsVisibleMasked() const
{
	return visibility_ == 2;
}


inline bool GlowWidget::IsInvisible() const
{
	return visibility_ == 0;
}


inline void GlowWidget::RegisterMouseEvents()
{
	GLOW_DEBUGSCOPE("GlowWidget::RegisterMouseEvents");
	
	if (!receivingMouse_ && IsActive() && visibility_ == 1)
	{
		root_->RegisterMouseWidget_(this);
	}
	receivingMouse_ = true;
}


inline void GlowWidget::RegisterKeyboardEvents()
{
	GLOW_DEBUGSCOPE("GlowWidget::RegisterKeyboardEvents");
	
	if (!receivingKeyboard_ && IsActive() && visibility_ == 1)
	{
		root_->RegisterKeyboardWidget_(this);
	}
	receivingKeyboard_ = true;
}


inline void GlowWidget::UnregisterMouseEvents()
{
	GLOW_DEBUGSCOPE("GlowWidget::UnregisterMouseEvents");
	
	if (receivingMouse_ && IsActive() && visibility_ == 1)
	{
		root_->UnregisterMouseWidget_(this);
	}
	receivingMouse_ = false;
}


inline void GlowWidget::UnregisterKeyboardEvents()
{
	GLOW_DEBUGSCOPE("GlowWidget::UnregisterKeyboardEvents");
	
	if (receivingKeyboard_ && IsActive() && visibility_ == 1)
	{
		root_->UnregisterKeyboardWidget_(this);
	}
	receivingKeyboard_ = false;
}


inline void GlowWidget::GrabKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowWidget::GrabKeyboardFocus");
	GLOW_DEBUG(!IsActive(), "Grab focus on deactivated widget");
	GLOW_DEBUG(visibility_!=1, "Grab focus on invisible widget");
	GLOW_DEBUG(!receivingKeyboard_, "Grab focus on non-keyboard widget");
	if (!hasFocus_)
	{
		root_->SetKeyboardFocus(this);
	}
}


inline void GlowWidget::RelinquishKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowWidget::RelinquishKeyboardFocus");
	GLOW_DEBUG(!IsActive(), "Relinquish focus on deactivated widget");
	GLOW_DEBUG(visibility_!=1, "Relinquish focus on invisible widget");
	GLOW_DEBUG(!receivingKeyboard_, "Relinquish focus on non-keyboard widget");
	if (hasFocus_)
	{
		root_->AdvanceKeyboardFocus();
	}
}


inline bool GlowWidget::HasKeyboardFocus() const
{
	return hasFocus_;
}


inline void GlowWidget::Refresh()
{
	GLOW_DEBUGSCOPE("GlowWidget::Refresh");
	if (refreshEnabled_)
	{
		root_->Subwindow()->Refresh();
	}
}


inline bool GlowWidget::IsRefreshEnabled() const
{
	return refreshEnabled_;
}


inline void GlowWidget::SetRefreshEnabled(
	bool enabled)
{
	refreshEnabled_ = enabled;
}


inline void GlowWidget::SetHasKeyboardFocus_(
	bool val)
{
	GLOW_DEBUGSCOPE("GlowWidget::SetHasKeyboardFocus_");
	if (hasFocus_ != val)
	{
		AddToNotifyList_();
		Refresh();
		hasFocus_ = val;
	}
}


inline void GlowWidget::NormalizeCoordinates(
	int x,
	int y,
	GLfloat& xn,
	GLfloat& yn) const
{
	xn = GLfloat(x)*2.0f/GLfloat(width_)-1.0f;
	yn = 1.0f-GLfloat(y)*2.0f/GLfloat(height_);
}


/*
===============================================================================
	Inline methods of GlowWidgetRoot
===============================================================================
*/

inline GlowWidgetRoot::GlowWidgetRoot()
{
}


inline GlowWidget* GlowWidgetRoot::GetKeyboardFocus()
{
	return (curKeyboardFocus_ == keyboardWidgets_.end()) ?
		0 : (*curKeyboardFocus_);
}


inline GlowColor GlowWidgetRoot::GetBackColor() const
{
	return backColor_;
}


inline void GlowWidgetRoot::SetBackColor(
	GlowColor c)
{
	backColor_ = c;
	subwindow_->Refresh();
}


inline void GlowWidgetRoot::RegisterMouseWidget_(
	GlowWidget* widget)
{
	mouseWidgets_.push_front(widget);
}


inline void GlowWidgetRoot::RegisterKeyboardWidget_(
	GlowWidget* widget)
{
	keyboardWidgets_.push_front(widget);
}


inline GlowSubwindow* GlowWidgetRoot::Subwindow() const
{
	return subwindow_;
}


/*
-------------------------------------------------------------------------------
	Filter methods
-------------------------------------------------------------------------------
*/

inline void GlowWidgetRoot::UnregisterAllFilters()
{
	keyboardFilters_.UnbindAll();
}


inline unsigned int GlowWidgetRoot::NumRegisteredFilters() const
{
	return keyboardFilters_.NumReceivers();
}


inline bool GlowWidgetRoot::IsFilterRegistered(
	GlowWidgetKeyboardFilter* filter) const
{
	return keyboardFilters_.IsBoundTo(filter);
}


inline void GlowWidgetRoot::RegisterFilter(
	GlowWidgetKeyboardFilter* filter)
{
	if (!keyboardFilters_.IsBoundTo(filter))
	{
		keyboardFilters_.Bind(filter);
	}
}


inline void GlowWidgetRoot::UnregisterFilter(
	GlowWidgetKeyboardFilter* filter)
{
	if (keyboardFilters_.IsBoundTo(filter))
	{
		keyboardFilters_.Unbind(filter);
	}
}


/*
===============================================================================
	Inline methods for event filter types
===============================================================================
*/

inline GlowWidgetKeyboardData::GlowWidgetKeyboardData()
{
	continue_ = true;
}


inline GlowWidgetTabFilter::GlowWidgetTabFilter()
{
}


/*
===============================================================================
	Inline methods of GlowWidgetSubwindow
===============================================================================
*/

inline GlowWidgetSubwindow::GlowWidgetSubwindow()
{
}


inline GlowWidgetSubwindow::GlowWidgetSubwindow(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask,
	GlowColor backColor)
{
	GlowSubwindow::Init(parent, x, y, width, height, mode, eventMask);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), backColor);
}


inline GlowWidgetSubwindow::GlowWidgetSubwindow(
	GlowComponent* parent,
	const GlowWidgetSubwindowParams& params)
{
	GlowSubwindow::Init(parent, params);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), params.backColor);
}


inline void GlowWidgetSubwindow::Init(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask,
	GlowColor backColor)
{
	GlowSubwindow::Init(parent, x, y, width, height, mode, eventMask);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), backColor);
}


inline void GlowWidgetSubwindow::Init(
	GlowComponent* parent,
	const GlowWidgetSubwindowParams& params)
{
	GlowSubwindow::Init(parent, params);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), params.backColor);
}


/*
===============================================================================
	Inline methods of GlowWidgetWindow
===============================================================================
*/

inline GlowWidgetWindow::GlowWidgetWindow()
{
}


inline GlowWidgetWindow::GlowWidgetWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask,
	GlowColor backColor)
{
	GlowWindow::Init(title, x, y, width, height, mode, eventMask);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), backColor);
}


inline GlowWidgetWindow::GlowWidgetWindow(
	const GlowWidgetWindowParams& params)
{
	GlowWindow::Init(params);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), params.backColor);
}


inline void GlowWidgetWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask,
	GlowColor backColor)
{
	GlowWindow::Init(title, x, y, width, height, mode, eventMask);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), backColor);
}


inline void GlowWidgetWindow::Init(
	const GlowWidgetWindowParams& params)
{
	GlowWindow::Init(params);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), params.backColor);
}


/*
===============================================================================
	Inline methods of GlowFixedSizeWidgetWindow
===============================================================================
*/

inline GlowFixedSizeWidgetWindow::GlowFixedSizeWidgetWindow()
{
}


inline GlowFixedSizeWidgetWindow::GlowFixedSizeWidgetWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask,
	GlowColor backColor)
{
	GlowFixedSizeWindow::Init(title, x, y, width, height, mode, eventMask);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), backColor);
}


inline GlowFixedSizeWidgetWindow::GlowFixedSizeWidgetWindow(
	const GlowWidgetWindowParams& params)
{
	GlowFixedSizeWindow::Init(params);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), params.backColor);
}


inline void GlowFixedSizeWidgetWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask,
	GlowColor backColor)
{
	GlowFixedSizeWindow::Init(title, x, y, width, height, mode, eventMask);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), backColor);
}


inline void GlowFixedSizeWidgetWindow::Init(
	const GlowWidgetWindowParams& params)
{
	GlowFixedSizeWindow::Init(params);
	GlowWidgetRoot::Init(static_cast<GlowSubwindow*>(this), params.backColor);
}


/*
===============================================================================
	Inline methods of GlowSubwindowInWidget
===============================================================================
*/

inline GlowSubwindowInWidget::GlowSubwindowInWidget()
{
}


inline GlowSubwindowInWidget::GlowSubwindowInWidget(
	GlowWidget* parent,
	const GlowSubwindowParams& params)
{
	GlowSubwindowParams params2 = params;
	params2.x += parent->RootPositionX();
	params2.y += parent->RootPositionY();
	GlowSubwindow::Init(parent, params2);
}


inline GlowSubwindowInWidget::GlowSubwindowInWidget(
	GlowWidget* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	GlowSubwindow::Init(parent,
		x+parent->RootPositionX(), y+parent->RootPositionY(),
		width, height, mode, eventMask);
}


inline void GlowSubwindowInWidget::Init(
	GlowWidget* parent,
	const GlowSubwindowParams& params)
{
	GlowSubwindowParams params2 = params;
	params2.x += parent->RootPositionX();
	params2.y += parent->RootPositionY();
	GlowSubwindow::Init(parent, params2);
}


inline void GlowSubwindowInWidget::Init(
	GlowWidget* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	GlowSubwindow::Init(parent,
		x+parent->RootPositionX(), y+parent->RootPositionY(),
		width, height, mode, eventMask);
}


inline GlowWidget* GlowSubwindowInWidget::ParentWidget() const
{
	return static_cast<GlowWidget*>(Parent());
}


inline GlowWidgetRoot* GlowSubwindowInWidget::Root() const
{
	return static_cast<GlowWidget*>(Parent())->Root();
}


inline int GlowSubwindowInWidget::WidgetPositionX() const
{
	return PositionX()-static_cast<GlowWidget*>(Parent())->RootPositionX();
}


inline int GlowSubwindowInWidget::WidgetPositionY() const
{
	return PositionY()-static_cast<GlowWidget*>(Parent())->RootPositionY();
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

