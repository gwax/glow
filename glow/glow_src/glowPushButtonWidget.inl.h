/*
===============================================================================

	FILE:  glowPushButtonWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Pushbutton widget for GLOW
	
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

#include <cstring>


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowPushButtonWidget
===============================================================================
*/

inline GlowPushButtonWidget::GlowPushButtonWidget()
{
}


inline GlowPushButtonWidget::GlowPushButtonWidget(
	GlowWidget* parent,
	const GlowPushButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowPushButtonWidget::GlowPushButtonWidget(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params)
{
	Init(root, 0, params);
}


inline void GlowPushButtonWidget::Init(
	GlowWidget* parent,
	const GlowPushButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowPushButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params)
{
	Init(root, 0, params);
}


inline const char* GlowPushButtonWidget::GetText() const
{
	return label_;
}


inline GlowFont GlowPushButtonWidget::GetFont() const
{
	return font_;
}


inline void GlowPushButtonWidget::SetFont(
	GlowFont font)
{
	font_ = font;
	Refresh();
}


inline GlowPushButtonWidget::Behavior GlowPushButtonWidget::GetBehavior() const
{
	return behavior_;
}


inline void GlowPushButtonWidget::SetBehavior(
	Behavior behavior)
{
	behavior_ = behavior;
	if (behavior_ == normalBehavior && state_)
	{
		state_ = false;
		Refresh();
	}
}


inline bool GlowPushButtonWidget::IsDown() const
{
	return state_;
}


inline void GlowPushButtonWidget::SetDown(
	bool down)
{
	state_ = down;
	Refresh();
}


inline GlowColor GlowPushButtonWidget::GetUpBoxColor() const
{
	return upBoxColor_;
}


inline GlowColor GlowPushButtonWidget::GetUpTextColor() const
{
	return upTextColor_;
}


inline GlowColor GlowPushButtonWidget::GetDownBoxColor() const
{
	return downBoxColor_;
}


inline GlowColor GlowPushButtonWidget::GetDownTextColor() const
{
	return downTextColor_;
}


inline GlowColor GlowPushButtonWidget::GetHiliteBoxColor() const
{
	return hiliteBoxColor_;
}


inline GlowColor GlowPushButtonWidget::GetHiliteTextColor() const
{
	return hiliteTextColor_;
}


inline GlowColor GlowPushButtonWidget::GetDisableUpBoxColor() const
{
	return disableUpBoxColor_;
}


inline GlowColor GlowPushButtonWidget::GetDisableDownBoxColor() const
{
	return disableDownBoxColor_;
}


inline GlowColor GlowPushButtonWidget::GetDisableTextColor() const
{
	return disableTextColor_;
}


inline GlowColor GlowPushButtonWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowPushButtonWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowPushButtonWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowPushButtonWidget::SetUpBoxColor(
	GlowColor c)
{
	upBoxColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetUpTextColor(
	GlowColor c)
{
	upTextColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDownBoxColor(
	GlowColor c)
{
	downBoxColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDownTextColor(
	GlowColor c)
{
	downTextColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	hiliteBoxColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	hiliteTextColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableUpBoxColor(
	GlowColor c)
{
	disableUpBoxColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableDownBoxColor(
	GlowColor c)
{
	disableDownBoxColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	disableTextColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


inline TSender<const GlowPushButtonMessage&>& GlowPushButtonWidget::Notifier()
{
	return sender_;
}


inline void GlowPushButtonWidget::Hit(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	OnHit(button, modifiers);
}


/*
===============================================================================
	Inline methods of GlowDismissPushButtonWidget
===============================================================================
*/

inline GlowDismissPushButtonWidget::GlowDismissPushButtonWidget()
{
}


inline GlowDismissPushButtonWidget::GlowDismissPushButtonWidget(
	GlowWidget* parent,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(parent->Root(), parent, params);
	todismiss_ = todismiss;
}


inline GlowDismissPushButtonWidget::GlowDismissPushButtonWidget(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(root, 0, params);
	todismiss_ = todismiss;
}


inline void GlowDismissPushButtonWidget::Init(
	GlowWidget* parent,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(parent->Root(), parent, params);
	todismiss_ = todismiss;
}


inline void GlowDismissPushButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(root, 0, params);
	todismiss_ = todismiss;
}


inline void GlowDismissPushButtonWidget::SetToDismiss(
	GlowComponent* todismiss)
{
	todismiss_ = todismiss;
}


inline GlowComponent* GlowDismissPushButtonWidget::GetToDismiss() const
{
	return todismiss_;
}


/*
===============================================================================
	Inline methods of GlowWidgetMapToPushButtonFilter
===============================================================================
*/

inline GlowWidgetMapToPushButtonFilter::GlowWidgetMapToPushButtonFilter(
	GlowPushButtonWidget* widget,
	Glow::KeyCode key,
	Glow::Modifiers modifiers)
{
	widget_ = widget;
	keyCode_ = key;
	modifiers_ = modifiers;
}


inline void GlowWidgetMapToPushButtonFilter::SetPushButton(
	GlowPushButtonWidget* widget)
{
	widget_ = widget;
}


inline GlowPushButtonWidget* GlowWidgetMapToPushButtonFilter::GetPushButton() const
{
	return widget_;
}


inline void GlowWidgetMapToPushButtonFilter::SetKeyCode(
	Glow::KeyCode key)
{
	keyCode_ = key;
}


inline Glow::KeyCode GlowWidgetMapToPushButtonFilter::GetKeyCode() const
{
	return keyCode_;
}


inline void GlowWidgetMapToPushButtonFilter::SetModifiers(
	Glow::Modifiers modifiers)
{
	modifiers_ = modifiers;
}


inline Glow::Modifiers GlowWidgetMapToPushButtonFilter::GetModifiers() const
{
	return modifiers_;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

