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
		License along with this library; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 0.95  (27 March 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
	
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
	Inline methods of GlowQuickPanelWidget
===============================================================================
*/

inline GlowQuickPanelWidget* GlowQuickPanelWidget::AddArrangingPanel(
	int arrangement,
	int alignment,
	int spacing,
	int inset)
{
	return AddPanel(plainStyle, 0, arrangement, alignment, spacing,
		(_arrangement == vertical) ? inset : 0,
		(_arrangement == horizontal) ? inset : 0);
}


inline int GlowQuickPanelWidget::GetArrangement() const
{
	return _arrangement;
}


inline int GlowQuickPanelWidget::GetAlignment() const
{
	return _alignment;
}


inline int GlowQuickPanelWidget::GetSpacing() const
{
	return _spacing;
}


inline int GlowQuickPanelWidget::GetHMargin() const
{
	return _hmargin;
}


inline int GlowQuickPanelWidget::GetVMargin() const
{
	return _vmargin;
}


inline void GlowQuickPanelWidget::SetArrangement(
	int value)
{
	_arrangement = value;
}


inline void GlowQuickPanelWidget::SetAlignment(
	int value)
{
	_alignment = value;
}


inline void GlowQuickPanelWidget::SetSpacing(
	int value)
{
	_spacing = value;
}


inline void GlowQuickPanelWidget::SetHMargin(
	int value)
{
	_hmargin = value;
}


inline void GlowQuickPanelWidget::SetVMargin(
	int value)
{
	_vmargin = value;
}


/*
===============================================================================
	Inline methods of GlowQuickPaletteBase
===============================================================================
*/

inline GlowQuickPanelWidget* GlowQuickPaletteBase::AddPanel(
	int style,
	const char* label,
	int arrangement,
	int alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	return _rootPanel->AddPanel(style, label, arrangement, alignment,
		spacing, hmargin, vmargin);
}


inline GlowQuickPanelWidget* GlowQuickPaletteBase::AddArrangingPanel(
	int arrangement,
	int alignment,
	int spacing,
	int inset)
{
	return _rootPanel->AddArrangingPanel(arrangement, alignment,
		spacing, inset);
}


inline GlowPanelWidget* GlowQuickPaletteBase::AddFixedPanel(
	int width,
	int height,
	int style)
{
	return _rootPanel->AddFixedPanel(width, height, style);
}


inline GlowSeparatorWidget* GlowQuickPaletteBase::AddSeparator(
	int style)
{
	return _rootPanel->AddSeparator(style);
}


inline GlowPushButtonWidget* GlowQuickPaletteBase::AddPushButton(
	const char* label,
	GlowPushButtonReceiver* receiver)
{
	return _rootPanel->AddPushButton(label, receiver);
}


inline GlowMenuButtonWidget* GlowQuickPaletteBase::AddMenuButton(
	const char* label,
	GlowMenu* menu,
	int iconType)
{
	return _rootPanel->AddMenuButton(label, menu, iconType);
}


inline GlowLabeledPopupMenuWidget* GlowQuickPaletteBase::AddPopupMenu(
	const char* label,
	GlowPopupMenuReceiver* receiver)
{
	return _rootPanel->AddPopupMenu(label, receiver);
}


inline GlowCheckBoxWidget* GlowQuickPaletteBase::AddCheckBox(
	const char* label,
	int initialValue,
	GlowCheckBoxReceiver* receiver)
{
	return _rootPanel->AddCheckBox(label, initialValue, receiver);
}


inline GlowQuickRadioGroupWidget* GlowQuickPaletteBase::AddRadioGroup(
	int arrangement,
	int spacing,
	GlowRadioButtonReceiver* receiver)
{
	return _rootPanel->AddRadioGroup(arrangement, spacing, receiver);
}


inline GlowLabeledSliderWidget* GlowQuickPaletteBase::AddSlider(
	float min,
	float max,
	float initial,
	int options,
	int numTicks,
	const char* valueLabel,
	const char* label,
	GlowSliderReceiver* receiver)
{
	return _rootPanel->AddSlider(min, max, initial, options, numTicks,
		valueLabel, label, receiver);
}


inline GlowScrollBarWidget* GlowQuickPaletteBase::AddScrollBar(
	long min,
	long max,
	long span,
	long initialTop,
	GlowScrollBarReceiver* receiver)
{
	return _rootPanel->AddScrollBar(min, max, span, initialTop, receiver);
}


inline GlowLabeledTextFieldWidget* GlowQuickPaletteBase::AddTextField(
	int width,
	const char* text,
	const char* label)
{
	return _rootPanel->AddTextField(width, text, label);
}


inline GlowLabeledHiddenTextFieldWidget* GlowQuickPaletteBase::AddHiddenTextField(
	int width,
	const char* text,
	char hideCharacter,
	const char* label)
{
	return _rootPanel->AddHiddenTextField(width, text, hideCharacter, label);
}


inline GlowLabelWidget* GlowQuickPaletteBase::AddLabel(
	const char* text)
{
	return _rootPanel->AddLabel(text);
}


inline int GlowQuickPaletteBase::GetArrangement() const
{
	return _rootPanel->GetArrangement();
}


inline int GlowQuickPaletteBase::GetAlignment() const
{
	return _rootPanel->GetAlignment();
}


inline int GlowQuickPaletteBase::GetSpacing() const
{
	return _rootPanel->GetSpacing();
}


inline int GlowQuickPaletteBase::GetHMargin() const
{
	return _rootPanel->GetHMargin();
}


inline int GlowQuickPaletteBase::GetVMargin() const
{
	return _rootPanel->GetVMargin();
}


inline void GlowQuickPaletteBase::SetArrangement(
	int value)
{
	_rootPanel->SetArrangement(value);
}


inline void GlowQuickPaletteBase::SetAlignment(
	int value)
{
	_rootPanel->SetAlignment(value);
}


inline void GlowQuickPaletteBase::SetSpacing(
	int value)
{
	_rootPanel->SetSpacing(value);
}


inline void GlowQuickPaletteBase::SetHMargin(
	int value)
{
	_rootPanel->SetHMargin(value);
}


inline void GlowQuickPaletteBase::SetVMargin(
	int value)
{
	_rootPanel->SetVMargin(value);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

