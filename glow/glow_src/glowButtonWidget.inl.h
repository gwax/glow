/*
===============================================================================

	FILE:  glowButtonWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Generalized button widget for GLOW
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
	COPYRIGHT:
	
		Copyright (C) 2000  Daniel Azuma  (dazuma@kagi.com)
		
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

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowButtonWidget
===============================================================================
*/

inline GlowButtonWidget::GlowButtonWidget()
{
}


inline GlowButtonWidget::GlowButtonWidget(
	GlowWidget* parent,
	const GlowButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowButtonWidget::GlowButtonWidget(
	GlowWidgetRoot* root,
	const GlowButtonParams& params)
{
	Init(root, 0, params);
}


inline void GlowButtonWidget::Init(
	GlowWidget* parent,
	const GlowButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowButtonParams& params)
{
	Init(root, 0, params);
}


inline bool GlowButtonWidget::IsDown() const
{
	return state_;
}


inline void GlowButtonWidget::SetDown(
	bool down)
{
	state_ = down;
	Refresh();
}


inline bool GlowButtonWidget::IsHilited() const
{
	return down_ && inside_;
}


inline GlowColor GlowButtonWidget::GetUpBoxColor() const
{
	return upBoxColor_;
}


inline GlowColor GlowButtonWidget::GetDownBoxColor() const
{
	return downBoxColor_;
}


inline GlowColor GlowButtonWidget::GetHiliteBoxColor() const
{
	return hiliteBoxColor_;
}


inline GlowColor GlowButtonWidget::GetDisableUpBoxColor() const
{
	return disableUpBoxColor_;
}


inline GlowColor GlowButtonWidget::GetDisableDownBoxColor() const
{
	return disableDownBoxColor_;
}


inline GlowColor GlowButtonWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowButtonWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowButtonWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowButtonWidget::SetUpBoxColor(
	GlowColor c)
{
	upBoxColor_ = c;
	Refresh();
}


inline void GlowButtonWidget::SetDownBoxColor(
	GlowColor c)
{
	downBoxColor_ = c;
	Refresh();
}


inline void GlowButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	hiliteBoxColor_ = c;
	Refresh();
}


inline void GlowButtonWidget::SetDisableUpBoxColor(
	GlowColor c)
{
	disableUpBoxColor_ = c;
	Refresh();
}


inline void GlowButtonWidget::SetDisableDownBoxColor(
	GlowColor c)
{
	disableDownBoxColor_ = c;
	Refresh();
}


inline void GlowButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

