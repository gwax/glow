/*
===============================================================================

	FILE:  glowStickyButtonWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Stickybutton widget for GLOW
	
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
	
		The GLOW Toolkit -- version 0.9.6  (10 April 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
		10 April 2000 -- DA -- Version 0.9.6 update
	
===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#include <cstring>


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowStickyButtonWidget
===============================================================================
*/

inline GlowStickyButtonWidget::GlowStickyButtonWidget()
{
}


inline GlowStickyButtonWidget::GlowStickyButtonWidget(
	GlowWidget* parent,
	const GlowStickyButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowStickyButtonWidget::GlowStickyButtonWidget(
	GlowWidgetRoot* root,
	const GlowStickyButtonParams& params)
{
	Init(root, 0, params);
}


inline void GlowStickyButtonWidget::Init(
	GlowWidget* parent,
	const GlowStickyButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowStickyButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowStickyButtonParams& params)
{
	Init(root, 0, params);
}


inline const char* GlowStickyButtonWidget::GetText() const
{
	return _label;
}


inline GlowFont GlowStickyButtonWidget::GetFont() const
{
	return _font;
}


inline void GlowStickyButtonWidget::SetFont(
	GlowFont font)
{
	_font = font;
	Refresh();
}


inline GlowStickyButtonWidget::Behavior GlowStickyButtonWidget::GetBehavior() const
{
	return _behavior;
}


inline void GlowStickyButtonWidget::SetBehavior(
	Behavior behavior)
{
	_behavior = behavior;
}


inline bool GlowStickyButtonWidget::GetState() const
{
	return _state;
}


inline void GlowStickyButtonWidget::SetState(
	bool state)
{
	_state = state;
	Refresh();
}


inline void GlowStickyButtonWidget::Hit(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	OnHit(mouseButton, modifiers);
}


inline GlowColor GlowStickyButtonWidget::GetUpBoxColor() const
{
	return _upBoxColor;
}


inline GlowColor GlowStickyButtonWidget::GetUpTextColor() const
{
	return _upTextColor;
}


inline GlowColor GlowStickyButtonWidget::GetDownBoxColor() const
{
	return _downBoxColor;
}


inline GlowColor GlowStickyButtonWidget::GetDownTextColor() const
{
	return _downTextColor;
}


inline GlowColor GlowStickyButtonWidget::GetHiliteBoxColor() const
{
	return _hiliteBoxColor;
}


inline GlowColor GlowStickyButtonWidget::GetHiliteTextColor() const
{
	return _hiliteTextColor;
}


inline GlowColor GlowStickyButtonWidget::GetDisableUpBoxColor() const
{
	return _disableUpBoxColor;
}


inline GlowColor GlowStickyButtonWidget::GetDisableDownBoxColor() const
{
	return _disableDownBoxColor;
}


inline GlowColor GlowStickyButtonWidget::GetDisableTextColor() const
{
	return _disableTextColor;
}


inline GlowColor GlowStickyButtonWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowStickyButtonWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowStickyButtonWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowStickyButtonWidget::SetUpBoxColor(
	GlowColor c)
{
	_upBoxColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetUpTextColor(
	GlowColor c)
{
	_upTextColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetDownBoxColor(
	GlowColor c)
{
	_downBoxColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetDownTextColor(
	GlowColor c)
{
	_downTextColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	_hiliteBoxColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	_hiliteTextColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetDisableUpBoxColor(
	GlowColor c)
{
	_disableUpBoxColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetDisableDownBoxColor(
	GlowColor c)
{
	_disableDownBoxColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	_disableTextColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowStickyButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


inline TSender<const GlowStickyButtonMessage&>& GlowStickyButtonWidget::Notifier()
{
	return _sender;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

