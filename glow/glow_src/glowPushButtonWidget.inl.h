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
	return _label;
}


inline GlowFont GlowPushButtonWidget::GetFont() const
{
	return _font;
}


inline void GlowPushButtonWidget::SetFont(
	GlowFont font)
{
	_font = font;
	Refresh();
}


inline GlowColor GlowPushButtonWidget::GetBoxColor() const
{
	return _boxColor;
}


inline GlowColor GlowPushButtonWidget::GetTextColor() const
{
	return _textColor;
}


inline GlowColor GlowPushButtonWidget::GetHiliteBoxColor() const
{
	return _hiliteBoxColor;
}


inline GlowColor GlowPushButtonWidget::GetHiliteTextColor() const
{
	return _hiliteTextColor;
}


inline GlowColor GlowPushButtonWidget::GetDisableBoxColor() const
{
	return _disableBoxColor;
}


inline GlowColor GlowPushButtonWidget::GetDisableTextColor() const
{
	return _disableTextColor;
}


inline GlowColor GlowPushButtonWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowPushButtonWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowPushButtonWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowPushButtonWidget::SetBoxColor(
	GlowColor c)
{
	_boxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetTextColor(
	GlowColor c)
{
	_textColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	_hiliteBoxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	_hiliteTextColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableBoxColor(
	GlowColor c)
{
	_disableBoxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	_disableTextColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


inline TSender<const GlowPushButtonMessage&>& GlowPushButtonWidget::Notifier()
{
	return _sender;
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
	_todismiss = todismiss;
}


inline GlowDismissPushButtonWidget::GlowDismissPushButtonWidget(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(root, 0, params);
	_todismiss = todismiss;
}


inline void GlowDismissPushButtonWidget::Init(
	GlowWidget* parent,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(parent->Root(), parent, params);
	_todismiss = todismiss;
}


inline void GlowDismissPushButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(root, 0, params);
	_todismiss = todismiss;
}


inline void GlowDismissPushButtonWidget::SetToDismiss(
	GlowComponent* todismiss)
{
	_todismiss = todismiss;
}


inline GlowComponent* GlowDismissPushButtonWidget::GetToDismiss() const
{
	return _todismiss;
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
	_widget = widget;
	_keyCode = key;
	_modifiers = modifiers;
}


inline void GlowWidgetMapToPushButtonFilter::SetPushButton(
	GlowPushButtonWidget* widget)
{
	_widget = widget;
}


inline GlowPushButtonWidget* GlowWidgetMapToPushButtonFilter::GetPushButton() const
{
	return _widget;
}


inline void GlowWidgetMapToPushButtonFilter::SetKeyCode(
	Glow::KeyCode key)
{
	_keyCode = key;
}


inline Glow::KeyCode GlowWidgetMapToPushButtonFilter::GetKeyCode() const
{
	return _keyCode;
}


inline void GlowWidgetMapToPushButtonFilter::SetModifiers(
	Glow::Modifiers modifiers)
{
	_modifiers = modifiers;
}


inline Glow::Modifiers GlowWidgetMapToPushButtonFilter::GetModifiers() const
{
	return _modifiers;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END
