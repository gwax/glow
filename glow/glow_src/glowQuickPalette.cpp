/*
===============================================================================

	FILE:  glowQuickPalette.cpp
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_QUICKPALETTE__H
	#include "glowQuickPalette.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of GlowQuickPaletteWindow
===============================================================================
*/

GlowQuickPaletteWindow::GlowQuickPaletteWindow(
	const char* name,
	int x,
	int y,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteWindow::GlowQuickPaletteWindow");
	
	GLOW_ASSERT(arrangement!=parentArrangement);
	GLOW_ASSERT(alignment!=parentAlignment);
	GLOW_ASSERT(spacing!=parentSetting);
	GLOW_ASSERT(hmargin!=parentSetting);
	GLOW_ASSERT(vmargin!=parentSetting);
	GlowFixedSizeWidgetWindow::Init(name, x, y, 100, 100);
	panel_ = new GlowQuickPanelWidget(this, 0,
		GlowQuickPanelWidget::plainStyle, 0, arrangement, alignment,
		spacing, hmargin, vmargin);
	Hide();
}


void GlowQuickPaletteWindow::Pack()
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteWindow::Pack");
	
	panel_->AutoReshape();
	ForceReshape(panel_->Width(), panel_->Height());
	Show();
}


/*
===============================================================================
	Methods of GlowQuickPaletteSubwindow
===============================================================================
*/

GlowQuickPaletteSubwindow::GlowQuickPaletteSubwindow(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteSubwindow::GlowQuickPaletteSubwindow");
	
	GLOW_ASSERT(arrangement!=parentArrangement);
	GLOW_ASSERT(alignment!=parentAlignment);
	GLOW_ASSERT(spacing!=parentSetting);
	GLOW_ASSERT(hmargin!=parentSetting);
	GLOW_ASSERT(vmargin!=parentSetting);
	GlowWidgetSubwindow::Init(parent, x, y, width, height);
	panel_ = new GlowQuickPanelWidget(this, 0,
		GlowQuickPanelWidget::plainStyle, 0, arrangement, alignment,
		spacing, hmargin, vmargin);
	Hide();
}


void GlowQuickPaletteSubwindow::Pack(
	bool resizeOnPack)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteSubwindow::Pack");
	
	panel_->AutoReshape();
	if (resizeOnPack)
	{
		Reshape(panel_->Width(), panel_->Height());
	}
	Show();
}


/*
===============================================================================
	Methods of GlowQuickRadioGroupWidget
===============================================================================
*/

GlowQuickRadioGroupWidget::GlowQuickRadioGroupWidget(
	GlowWidget* parent,
	GlowRadioButtonReceiver* receiver,
	GlowQuickPalette::Arrangement arrangement,
	int spacing)
{
	GlowRadioGroupParams params;
	params.x = params.y = 0;
	params.receiver = receiver;
	GlowRadioGroupWidget::Init(parent, params);
	arrangement_ = arrangement;
	spacing_ = spacing;
}


GlowRadioButtonWidget* GlowQuickRadioGroupWidget::AddRadioButton(
	const char* label)
{
	GlowRadioButtonParams params;
	params.text = label;
	params.x = params.y = 0;
	return new GlowRadioButtonWidget(this, params);
}


GlowWidget::AutoPackError GlowQuickRadioGroupWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowQuickRadioGroupWidget::OnAutoPack");
	
	// Pack buttons
	int position = 0;
	int size = 0;
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
	{
		GlowRadioButtonWidget* button = dynamic_cast<GlowRadioButtonWidget*>(child);
		if (button != 0 && button->IsVisible() && !button->IsClosing())
		{
			if (arrangement_ == GlowQuickPalette::horizontal)
			{
				button->AutoPack(position, unspecifiedPos, 0, unspecifiedPos,
					leftPos | preferredSize, topPos | preferredSize);
				if (size < button->Height())
				{
					size = button->Height();
				}
				position += button->Width()+spacing_;
			}
			else
			{
				button->AutoPack(0, unspecifiedPos, position, unspecifiedPos,
					leftPos | preferredSize, topPos | preferredSize);
				if (size < button->Width())
				{
					size = button->Width();
				}
				position += button->Height()+spacing_;
			}
		}
	}
	
	// Preferred width and height
	int pwidth = 0, pheight = 0;
	if (arrangement_ == GlowQuickPalette::horizontal)
	{
		pwidth = position-spacing_;
		pheight = size;
	}
	else
	{
		pwidth = size;
		pheight = position-spacing_;
	}
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	
	int vnew = Height();
	if (vSize != unspecifiedSize && vSize < pheight)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize || vOption == expandPreferredSize)
	{
		vnew = vSize;
	}
	else if (vOption == preferredSize)
	{
		vnew = pheight;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


/*
===============================================================================
	Methods of GlowQuickPanelWidget
===============================================================================
*/

GlowQuickPanelWidget* GlowQuickPalette::AddPanel(
	GlowPanelWidget::Style style,
	const char* label,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPanel");
	
	return new GlowQuickPanelWidget(panel_->Root(), panel_,
		style, label, arrangement, alignment, spacing, hmargin, vmargin);
}


GlowPanelWidget* GlowQuickPalette::AddFixedPanel(
	int width,
	int height,
	GlowPanelWidget::Style style)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddFixedPanel");
	
	GlowPanelParams params;
	params.width = width;
	params.height = height;
	params.style = style;
	return new GlowPanelWidget(panel_, params);
}


GlowSeparatorWidget* GlowQuickPalette::AddSeparator(
	GlowSeparatorWidget::Style style)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddSeparator");
	
	GlowSeparatorParams params;
	params.x = params.y = 0;
	params.style = style;
	if (panel_->arrangement_ == GlowQuickPalette::vertical)
	{
		params.width = 10;
		params.height = 4;
	}
	else
	{
		params.height = 10;
		params.width = 4;
	}
	return new GlowSeparatorWidget(panel_, params);
}


GlowPushButtonWidget* GlowQuickPalette::AddPushButton(
	const char* label,
	GlowPushButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPushButton");
	
	GlowPushButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	return new GlowPushButtonWidget(panel_, params);
}


GlowDismissPushButtonWidget* GlowQuickPalette::AddDismissPushButton(
	const char* label,
	GlowComponent* toDismiss,
	GlowPushButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddDismissPushButton");
	
	GlowPushButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	return new GlowDismissPushButtonWidget(panel_, params, toDismiss);
}


GlowMenuButtonWidget* GlowQuickPalette::AddMenuButton(
	const char* label,
	GlowMenu* menu,
	GlowMenuButtonWidget::IconType iconType)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddMenuButton");
	
	GlowMenuButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.menu = menu;
	params.iconType = iconType;
	return new GlowMenuButtonWidget(panel_, params);
}


GlowLabeledPopupMenuWidget* GlowQuickPalette::AddPopupMenu(
	const char* label,
	GlowPopupMenuReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPopupMenu");
	
	GlowLabeledPopupMenuParams params;
	params.x = params.y = 0;
	params.receiver = receiver;
	params.labelText = label;
	return new GlowLabeledPopupMenuWidget(panel_, params);
}


GlowCheckBoxWidget* GlowQuickPalette::AddCheckBox(
	const char* label,
	GlowCheckBoxWidget::State initialValue,
	GlowCheckBoxReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddCheckBox");
	
	GlowCheckBoxParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	params.state = initialValue;
	return new GlowCheckBoxWidget(panel_, params);
}


GlowQuickRadioGroupWidget* GlowQuickPalette::AddRadioGroup(
	Arrangement arrangement,
	int spacing,
	GlowRadioButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddRadioGroup");
	
	return new GlowQuickRadioGroupWidget(panel_, receiver, arrangement, spacing);
}


GlowLabeledSliderWidget* GlowQuickPalette::AddSlider(
	float min,
	float max,
	float initial,
	GlowSliderWidget::Options options,
	int numTicks,
	const char* valueLabel,
	const char* label,
	GlowSliderReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddSlider");
	
	GlowLabeledSliderParams params;
	params.x = params.y = 0;
	if (panel_->arrangement_ == horizontal)
	{
		params.width = 30;
		params.height = 130;
		params.labelPosition = GlowLabeledSliderWidget::topLabelPosition;
	}
	else
	{
		params.height = 30;
		params.width = 130;
		params.labelPosition = GlowLabeledSliderWidget::leftLabelPosition;
	}
	params.min = min;
	params.max = max;
	params.initial = initial;
	params.numTicks = numTicks;
	params.options = options;
	params.receiver = receiver;
	params.labelTemplate = label;
	params.minmaxTemplate = valueLabel;
	return new GlowLabeledSliderWidget(panel_, params);
}


GlowScrollBarWidget* GlowQuickPalette::AddScrollBar(
	long min,
	long max,
	long span,
	long initialTop,
	GlowScrollBarReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddScrollBar");
	
	GlowScrollBarParams params;
	params.x = params.y = 0;
	if (panel_->arrangement_ == horizontal)
	{
		params.width = 20;
		params.height = 120;
	}
	else
	{
		params.height = 20;
		params.width = 120;
	}
	params.min = min;
	params.max = max;
	params.span = span;
	params.initialTop = initialTop;
	params.receiver = receiver;
	return new GlowScrollBarWidget(panel_, params);
}


GlowLabeledTextFieldWidget* GlowQuickPalette::AddTextField(
	int width,
	const char* text,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddTextField");
	
	GlowLabeledTextFieldParams params;
	params.x = params.y = 0;
	params.width = width;
	params.initialText = text;
	params.labelText = label;
	GlowLabeledTextFieldWidget* ret =
		new GlowLabeledTextFieldWidget(panel_, params);
	if (panel_->Root()->GetKeyboardFocus() == 0)
	{
		ret->GrabKeyboardFocus();
	}
	return ret;
}


GlowLabeledHiddenTextFieldWidget* GlowQuickPalette::AddHiddenTextField(
	int width,
	const char* text,
	char hideCharacter,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddHiddenTextField");
	
	GlowLabeledTextFieldParams params;
	params.x = params.y = 0;
	params.width = width;
	params.initialText = text;
	params.labelText = label;
	GlowLabeledHiddenTextFieldWidget* ret =
		new GlowLabeledHiddenTextFieldWidget(panel_, params, hideCharacter);
	if (panel_->Root()->GetKeyboardFocus() == 0)
	{
		ret->GrabKeyboardFocus();
	}
	return ret;
}


GlowLabelWidget* GlowQuickPalette::AddLabel(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddLabel");
	
	GlowLabelParams params;
	params.x = params.y = 0;
	params.text = text;
	return new GlowLabelWidget(panel_, params);
}


GlowQuickPanelWidget::GlowQuickPanelWidget(
	GlowWidgetRoot* root,
	GlowQuickPanelWidget* parent,
	Style style,
	const char* label,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::GlowQuickPanelWidget");
	
	GlowPanelParams params;
	params.style = style;
	params.clipping = false;
	GlowPanelWidget::Init(root, parent, params);
	
	panel_ = this;
	
	arrangement_ = (arrangement == parentArrangement ?
		parent->GetArrangement() : arrangement);
	alignment_ = (alignment == parentAlignment ?
		parent->GetAlignment() : alignment);
	spacing_ = (spacing == parentSetting ?
		parent->GetSpacing() : spacing);
	hmargin_ = (hmargin == parentSetting ?
		parent->GetHMargin() : hmargin);
	vmargin_ = (vmargin == parentSetting ?
		parent->GetVMargin() : vmargin);
	label_ = 0;
	
	if (label != 0 && *label != 0)
	{
		GlowLabelParams lparams;
		lparams.opaque = true;
		lparams.hIndent = 3;
		lparams.x = lparams.y = 0;
		lparams.text = label;
		label_ = new GlowLabelWidget(this, lparams);
	}
}


GlowWidget::AutoPackError GlowQuickPanelWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::OnAutoPack");
	
	int position = (arrangement_ == vertical) ? vmargin_ : hmargin_;
	int size = 0;
	int minWidth = position;
	
	// Arrange widgets with default alignment
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
	{
		// Get next widget
		GlowWidget* widget = dynamic_cast<GlowWidget*>(child);
		if (widget == 0 || !widget->IsVisible() || widget->IsClosing())
		{
			continue;
		}
		
		// Handle the label of the panel (if any) differently.
		if (widget == label_)
		{
			if (GetStyle() == GlowPanelWidget::etchedStyle)
			{
				// Etched style: label is half in and half out of the panel
				widget->AutoPack(hmargin_, unspecifiedPos, 0, unspecifiedPos,
					leftPos | preferredSize, centerPos | preferredSize);
				topMargin = widget->Height()/2;
				if (arrangement_ == vertical)
				{
					position += widget->Height()/2;
				}
			}
			else
			{
				// Other style: label is completely out of the panel
				widget->AutoPack(hmargin_, unspecifiedPos, unspecifiedPos, 0,
					leftPos | preferredSize, bottomPos | preferredSize);
				topMargin = widget->Height();
			}
			minWidth = widget->Width()+hmargin_+hmargin_;
			continue;
		}
		
		int leftMargin = 0, rightMargin = 0, topMargin = 0, bottomMargin = 0;
		if (arrangement_ == horizontal)
		{
			widget->AutoPack(position, unspecifiedPos, vmargin_, unspecifiedPos,
				leftPos | preferredSize, topPos | preferredSize,
				leftMargin, rightMargin, topMargin, bottomMargin);
			size = GLOW_STD::max(size, widget->Height()+topMargin+bottomMargin);
			position += widget->Width()+leftMargin+rightMargin+spacing_;
		}
		else
		{
			widget->AutoPack(hmargin_, unspecifiedPos, position, unspecifiedPos,
				leftPos | preferredSize, topPos | preferredSize,
				leftMargin, rightMargin, topMargin, bottomMargin);
			size = GLOW_STD::max(size, widget->Width()+leftMargin+rightMargin);
			position += widget->Height()+topMargin+bottomMargin+spacing_;
		}
	}
	
	// Preferred width and height
	int pwidth = 0, pheight = 0;
	if (arrangement_ == horizontal)
	{
		pwidth = GLOW_STD::max(minWidth, position-spacing_+hmargin_);
		pheight = size+vmargin_+vmargin_;
	}
	else
	{
		pwidth = GLOW_STD::max(minWidth, size+hmargin_+hmargin_);
		pheight = position-spacing_+vmargin_;
	}
	
	// Panel width
	int hnew = Width();
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	if (hnew < pwidth)
	{
		return hAutoPackError;
	}
	
	// Panel height
	int vnew = Height();
	if (vOption == forcedSize || vOption == expandPreferredSize)
	{
		vnew = vSize-topMargin;  // because of label
	}
	else if (vOption == preferredSize)
	{
		vnew = pheight;
	}
	if (vnew < pheight)
	{
		return vAutoPackError;
	}
	
	// Update size
	if (arrangement_ == horizontal)
	{
		size = vnew-vmargin_-vmargin_;
	}
	else
	{
		size = hnew-hmargin_-hmargin_;
	}
	
	// Now align contents
	int i=0;
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next(), ++i)
	{
		GlowWidget* widget = dynamic_cast<GlowWidget*>(child);
		if (widget == 0 || widget == label_ || !widget->IsVisible())
		{
			continue;
		}
		
		if (arrangement_ == horizontal)
		{
			// Horizontal arrangement
			AutoPackOptions vOption = (alignment_ & alignExpand) ?
				expandPreferredSize : preferredSize;
			
			if ((alignment_ & 3) == alignBottom)
			{
				vOption |= bottomPos;
			}
			else if ((alignment_ & 3) == alignCenter)
			{
				vOption |= centerPos;
			}
			else
			{
				vOption |= topPos;
			}
			widget->AutoPack(unspecifiedPos, unspecifiedPos,
				vmargin_, vmargin_+size, noMove | noReshape, vOption);
		}
		else
		{
			// Vertical arrangement
			AutoPackOptions hOption = (alignment_ & alignExpand) ?
				expandPreferredSize : preferredSize;
			
			if ((alignment_ & 3) == alignLeft)
			{
				hOption |= leftPos;
			}
			else if ((alignment_ & 3) == alignCenter)
			{
				hOption |= centerPos;
			}
			else
			{
				hOption |= rightPos;
			}
			widget->AutoPack(hmargin_, hmargin_+size,
				unspecifiedPos, unspecifiedPos, hOption, noMove | noReshape);
		}
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

