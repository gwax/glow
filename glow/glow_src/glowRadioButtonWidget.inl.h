/*
===============================================================================

	FILE:  glowRadioButtonWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Radio button widget for GLOW
	
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
	Inline methods of GlowRadioButtonWidget
===============================================================================
*/

inline GlowRadioButtonWidget::GlowRadioButtonWidget()
{
}


inline GlowRadioButtonWidget::GlowRadioButtonWidget(
	GlowRadioGroupWidget* parent,
	const GlowRadioButtonParams& params)
{
	Init(parent, params);
}


inline const char* GlowRadioButtonWidget::GetText() const
{
	return label_;
}


inline GlowFont GlowRadioButtonWidget::GetFont() const
{
	return font_;
}


inline void GlowRadioButtonWidget::SetFont(
	GlowFont font)
{
	font_ = font;
	Refresh();
}


inline int GlowRadioButtonWidget::GetSpacing() const
{
	return spacing_;
}


inline void GlowRadioButtonWidget::SetSpacing(
	int spacing)
{
	spacing_ = spacing;
	Refresh();
}


inline bool GlowRadioButtonWidget::GetState() const
{
	return state_;
}


inline GlowColor GlowRadioButtonWidget::GetBoxColor() const
{
	return boxColor_;
}


inline GlowColor GlowRadioButtonWidget::GetTextColor() const
{
	return textColor_;
}


inline GlowColor GlowRadioButtonWidget::GetSpotColor() const
{
	return spotColor_;
}


inline GlowColor GlowRadioButtonWidget::GetHiliteBoxColor() const
{
	return hiliteBoxColor_;
}


inline GlowColor GlowRadioButtonWidget::GetHiliteTextColor() const
{
	return hiliteTextColor_;
}


inline GlowColor GlowRadioButtonWidget::GetHiliteSpotColor() const
{
	return hiliteSpotColor_;
}


inline GlowColor GlowRadioButtonWidget::GetDisableBoxColor() const
{
	return disableBoxColor_;
}


inline GlowColor GlowRadioButtonWidget::GetDisableTextColor() const
{
	return disableTextColor_;
}


inline GlowColor GlowRadioButtonWidget::GetDisableSpotColor() const
{
	return disableSpotColor_;
}


inline GlowColor GlowRadioButtonWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowRadioButtonWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowRadioButtonWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowRadioButtonWidget::SetBoxColor(
	GlowColor c)
{
	boxColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetTextColor(
	GlowColor c)
{
	textColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetSpotColor(
	GlowColor c)
{
	spotColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	hiliteBoxColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	hiliteTextColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetHiliteSpotColor(
	GlowColor c)
{
	hiliteSpotColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableBoxColor(
	GlowColor c)
{
	disableBoxColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	disableTextColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableSpotColor(
	GlowColor c)
{
	disableSpotColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetState(
	bool state)
{
	if (state != state_)
	{
		state_ = state;
		Refresh();
	}
}


/*
===============================================================================
	Inline methods of GlowRadioGroupWidget
===============================================================================
*/

inline GlowRadioGroupWidget::GlowRadioGroupWidget()
{
}


inline GlowRadioGroupWidget::GlowRadioGroupWidget(
	GlowWidgetRoot* root,
	const GlowRadioGroupParams& params)
{
	Init(root, 0, params);
}


inline GlowRadioGroupWidget::GlowRadioGroupWidget(
	GlowWidget* parent,
	const GlowRadioGroupParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowRadioGroupWidget::Init(
	GlowWidgetRoot* root,
	const GlowRadioGroupParams& params)
{
	Init(root, 0, params);
}


inline void GlowRadioGroupWidget::Init(
	GlowWidget* parent,
	const GlowRadioGroupParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowRadioGroupWidget::SetState(
	GlowRadioButtonWidget* button)
{
	GLOW_DEBUGSCOPE("GlowRadioGroupWidget::SetState");
	
	if (button != state_)
	{
		button->SetState(true);
		state_->SetState(false);
		state_ = button;
	}
}


inline GlowRadioButtonWidget* GlowRadioGroupWidget::GetState() const
{
	return state_;
}


inline TSender<const GlowRadioButtonMessage&>& GlowRadioGroupWidget::Notifier()
{
	return sender_;
}


inline bool GlowRadioGroupWidget::Register_(
	GlowRadioButtonWidget* button)
{
	if (state_ == 0)
	{
		state_ = button;
		return true;
	}
	return false;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

