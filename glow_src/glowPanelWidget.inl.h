/*
===============================================================================

	FILE:  glowPanelWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Panel widget for GLOW
	
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
	
		The GLOW Toolkit -- version 1.1.2dev  (7 August 2000)
	
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
	Inline methods of GlowPanelWidget
===============================================================================
*/

inline GlowPanelWidget::GlowPanelWidget()
{
}


inline GlowPanelWidget::GlowPanelWidget(
	GlowWidget* parent,
	const GlowPanelParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowPanelWidget::GlowPanelWidget(
	GlowWidgetRoot* root,
	const GlowPanelParams& params)
{
	Init(root, 0, params);
}


inline void GlowPanelWidget::Init(
	GlowWidget* parent,
	const GlowPanelParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowPanelWidget::Init(
	GlowWidgetRoot* root,
	const GlowPanelParams& params)
{
	Init(root, 0, params);
}


inline GlowPanelWidget::Style GlowPanelWidget::GetStyle() const
{
	return style_;
}


inline void GlowPanelWidget::SetStyle(
	Style style)
{
	style_ = style;
	Refresh();
}


inline GlowColor GlowPanelWidget::GetBackColor() const
{
	return backColor_;
}


inline GlowColor GlowPanelWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowPanelWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowPanelWidget::SetBackColor(
	GlowColor c)
{
	backColor_ = c;
	Refresh();
}


inline void GlowPanelWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowPanelWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

