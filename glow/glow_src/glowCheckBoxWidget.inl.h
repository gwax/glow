/*
===============================================================================

	FILE:  glowCheckBoxWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Check box widget for GLOW
	
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

#include <cstring>


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowCheckBoxWidget
===============================================================================
*/

inline GlowCheckBoxWidget::GlowCheckBoxWidget()
{
}


inline GlowCheckBoxWidget::GlowCheckBoxWidget(
	GlowWidget* parent,
	const GlowCheckBoxParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowCheckBoxWidget::GlowCheckBoxWidget(
	GlowWidgetRoot* root,
	const GlowCheckBoxParams& params)
{
	Init(root, 0, params);
}


inline void GlowCheckBoxWidget::Init(
	GlowWidget* parent,
	const GlowCheckBoxParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowCheckBoxWidget::Init(
	GlowWidgetRoot* root,
	const GlowCheckBoxParams& params)
{
	Init(root, 0, params);
}


inline const char* GlowCheckBoxWidget::GetText() const
{
	return label_;
}


inline GlowFont GlowCheckBoxWidget::GetFont() const
{
	return font_;
}


inline void GlowCheckBoxWidget::SetFont(
	GlowFont font)
{
	font_ = font;
	Refresh();
}


inline int GlowCheckBoxWidget::GetSpacing() const
{
	return spacing_;
}


inline void GlowCheckBoxWidget::SetSpacing(
	int spacing)
{
	spacing_ = spacing;
	Refresh();
}


inline GlowCheckBoxWidget::Behavior GlowCheckBoxWidget::GetBehavior() const
{
	return behavior_;
}


inline void GlowCheckBoxWidget::SetBehavior(
	Behavior behavior)
{
	behavior_ = behavior;
}


inline GlowCheckBoxWidget::State GlowCheckBoxWidget::GetState() const
{
	return state_;
}


inline void GlowCheckBoxWidget::SetState(
	GlowCheckBoxWidget::State state)
{
	state_ = state;
	Refresh();
}


inline void GlowCheckBoxWidget::Hit(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	OnHit(button, modifiers);
}


inline GlowColor GlowCheckBoxWidget::GetBoxColor() const
{
	return boxColor_;
}


inline GlowColor GlowCheckBoxWidget::GetTextColor() const
{
	return textColor_;
}


inline GlowColor GlowCheckBoxWidget::GetCheckColor() const
{
	return checkColor_;
}


inline GlowColor GlowCheckBoxWidget::GetHiliteBoxColor() const
{
	return hiliteBoxColor_;
}


inline GlowColor GlowCheckBoxWidget::GetHiliteTextColor() const
{
	return hiliteTextColor_;
}


inline GlowColor GlowCheckBoxWidget::GetHiliteCheckColor() const
{
	return hiliteCheckColor_;
}


inline GlowColor GlowCheckBoxWidget::GetDisableBoxColor() const
{
	return disableBoxColor_;
}


inline GlowColor GlowCheckBoxWidget::GetDisableTextColor() const
{
	return disableTextColor_;
}


inline GlowColor GlowCheckBoxWidget::GetDisableCheckColor() const
{
	return disableCheckColor_;
}


inline GlowColor GlowCheckBoxWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowCheckBoxWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowCheckBoxWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowCheckBoxWidget::SetBoxColor(
	GlowColor c)
{
	boxColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetTextColor(
	GlowColor c)
{
	textColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetCheckColor(
	GlowColor c)
{
	checkColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetHiliteBoxColor(
	GlowColor c)
{
	hiliteBoxColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetHiliteTextColor(
	GlowColor c)
{
	hiliteTextColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetHiliteCheckColor(
	GlowColor c)
{
	hiliteCheckColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableBoxColor(
	GlowColor c)
{
	disableBoxColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableTextColor(
	GlowColor c)
{
	disableTextColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableCheckColor(
	GlowColor c)
{
	disableCheckColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


inline TSender<const GlowCheckBoxMessage&>& GlowCheckBoxWidget::Notifier()
{
	return sender_;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

