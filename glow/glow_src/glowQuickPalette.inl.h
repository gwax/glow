/*
===============================================================================

	FILE:  glowQuickPalette.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Quick palette API for GLOW
	
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
	Inline methods of GlowQuickPalette
===============================================================================
*/

inline GlowQuickPalette::GlowQuickPalette()
{
	panel_ = 0;
}


/*
===============================================================================
	Inline methods of GlowQuickPanelWidget
===============================================================================
*/

inline GlowQuickPanelWidget* GlowQuickPalette::AddArrangingPanel(
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int inset)
{
	return AddPanel(GlowPanelWidget::plainStyle, 0, arrangement, alignment, spacing,
		(panel_->arrangement_ == vertical) ? inset : 0,
		(panel_->arrangement_ == horizontal) ? inset : 0);
}


inline GlowQuickPalette::Arrangement GlowQuickPalette::GetArrangement() const
{
	return panel_->arrangement_;
}


inline GlowQuickPalette::Alignment GlowQuickPalette::GetAlignment() const
{
	return panel_->alignment_;
}


inline int GlowQuickPalette::GetSpacing() const
{
	return panel_->spacing_;
}


inline int GlowQuickPalette::GetHMargin() const
{
	return panel_->hmargin_;
}


inline int GlowQuickPalette::GetVMargin() const
{
	return panel_->vmargin_;
}


inline void GlowQuickPalette::SetArrangement(
	Arrangement value)
{
	panel_->arrangement_ = value;
}


inline void GlowQuickPalette::SetAlignment(
	Alignment value)
{
	panel_->alignment_ = value;
}


inline void GlowQuickPalette::SetSpacing(
	int value)
{
	panel_->spacing_ = value;
}


inline void GlowQuickPalette::SetHMargin(
	int value)
{
	panel_->hmargin_ = value;
}


inline void GlowQuickPalette::SetVMargin(
	int value)
{
	panel_->vmargin_ = value;
}


inline GlowQuickPanelWidget* GlowQuickPalette::QuickPaletteRoot() const
{
	return panel_;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

