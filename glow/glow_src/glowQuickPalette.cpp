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
	int arrangement,
	int alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteWindow::GlowQuickPaletteWindow");
	
	GLOW_ASSERT(arrangement!=parentSetting);
	GLOW_ASSERT(alignment!=parentSetting);
	GLOW_ASSERT(spacing!=parentSetting);
	GLOW_ASSERT(hmargin!=parentSetting);
	GLOW_ASSERT(vmargin!=parentSetting);
	GlowFixedSizeWidgetWindow::Init(name, x, y, 100, 100);
	_rootPanel = new GlowQuickPanelWidget(this, 0,
		GlowQuickPanelWidget::plainStyle, 0, arrangement, alignment,
		spacing, hmargin, vmargin);
	Hide();
}


void GlowQuickPaletteWindow::Pack()
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteWindow::Pack");
	
	_rootPanel->AutoReshape();
	ForceReshape(_rootPanel->Width(), _rootPanel->Height());
	Show();
}


/*
===============================================================================
	Methods of GlowQuickPaletteWindow
===============================================================================
*/

GlowQuickPaletteSubwindow::GlowQuickPaletteSubwindow(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	int arrangement,
	int alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteSubwindow::GlowQuickPaletteSubwindow");
	
	GLOW_ASSERT(arrangement!=parentSetting);
	GLOW_ASSERT(alignment!=parentSetting);
	GLOW_ASSERT(spacing!=parentSetting);
	GLOW_ASSERT(hmargin!=parentSetting);
	GLOW_ASSERT(vmargin!=parentSetting);
	GlowWidgetSubwindow::Init(parent, x, y, width, height);
	_rootPanel = new GlowQuickPanelWidget(this, 0,
		GlowQuickPanelWidget::plainStyle, 0, arrangement, alignment,
		spacing, hmargin, vmargin);
	Hide();
}


void GlowQuickPaletteSubwindow::Pack(
	bool resizeOnPack)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteSubwindow::Pack");
	
	_rootPanel->AutoReshape();
	if (resizeOnPack)
	{
		Reshape(_rootPanel->Width(), _rootPanel->Height());
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
	int arrangement,
	int spacing)
{
	GlowRadioGroupParams params;
	params.x = params.y = 0;
	params.receiver = receiver;
	GlowRadioGroupWidget::Init(parent, params);
	_arrangement = arrangement;
	_spacing = spacing;
}


GlowRadioButtonWidget* GlowQuickRadioGroupWidget::AddRadioButton(
	const char* label)
{
	GlowRadioButtonParams params;
	params.text = label;
	params.x = params.y = 0;
	return new GlowRadioButtonWidget(this, params);
}


int GlowQuickRadioGroupWidget::OnAutoPack(
	int hSize,
	int vSize,
	int hOption,
	int vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowQuickRadioGroupWidget::OnAutoPack");
	
	// Pack buttons
	int position = 0;
	int size = 0;
	for (GlowComponent::ChildIterator iter = BeginChildren();
		iter != EndChildren(); iter++)
	{
		GlowRadioButtonWidget* button = dynamic_cast<GlowRadioButtonWidget*>(*iter);
		if (button != 0)
		{
			if (_arrangement == horizontal)
			{
				button->AutoPack(position, unspecifiedPos, 0, unspecifiedPos,
					leftPos | preferredSize, topPos | preferredSize);
				if (size < button->Height())
				{
					size = button->Height();
				}
				position += button->Width()+_spacing;
			}
			else
			{
				button->AutoPack(0, unspecifiedPos, position, unspecifiedPos,
					leftPos | preferredSize, topPos | preferredSize);
				if (size < button->Width())
				{
					size = button->Width();
				}
				position += button->Height()+_spacing;
			}
		}
	}
	
	// Preferred width and height
	int pwidth = 0, pheight = 0;
	if (_arrangement == horizontal)
	{
		pwidth = position-_spacing;
		pheight = size;
	}
	else
	{
		pwidth = size;
		pheight = position-_spacing;
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

GlowQuickPanelWidget* GlowQuickPanelWidget::AddPanel(
	int style,
	const char* label,
	int arrangement,
	int alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPanel");
	
	return new GlowQuickPanelWidget(Root(), this,
		style, label, arrangement, alignment, spacing, hmargin, vmargin);
}


GlowPanelWidget* GlowQuickPanelWidget::AddFixedPanel(
	int width,
	int height,
	int style)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddFixedPanel");
	
	GlowPanelParams params;
	params.width = width;
	params.height = height;
	params.style = style;
	return new GlowPanelWidget(this, params);
}


GlowSeparatorWidget* GlowQuickPanelWidget::AddSeparator(
	int style)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddSeparator");
	
	GlowSeparatorParams params;
	params.x = params.y = 0;
	params.style = style;
	if (_arrangement == vertical)
	{
		params.width = 10;
		params.height = 4;
	}
	else
	{
		params.height = 10;
		params.width = 4;
	}
	return new GlowSeparatorWidget(this, params);
}


GlowPushButtonWidget* GlowQuickPanelWidget::AddPushButton(
	const char* label,
	GlowPushButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPushButton");
	
	GlowPushButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	return new GlowPushButtonWidget(this, params);
}


GlowMenuButtonWidget* GlowQuickPanelWidget::AddMenuButton(
	const char* label,
	GlowMenu* menu,
	int iconType)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddMenuButton");
	
	GlowMenuButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.menu = menu;
	params.iconType = iconType;
	return new GlowMenuButtonWidget(this, params);
}


GlowLabeledPopupMenuWidget* GlowQuickPanelWidget::AddPopupMenu(
	const char* label,
	GlowPopupMenuReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPopupMenu");
	
	GlowLabeledPopupMenuParams params;
	params.x = params.y = 0;
	params.receiver = receiver;
	params.labelText = label;
	return new GlowLabeledPopupMenuWidget(this, params);
}


GlowCheckBoxWidget* GlowQuickPanelWidget::AddCheckBox(
	const char* label,
	int initialValue,
	GlowCheckBoxReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddCheckBox");
	
	GlowCheckBoxParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	params.state = initialValue;
	return new GlowCheckBoxWidget(this, params);
}


GlowQuickRadioGroupWidget* GlowQuickPanelWidget::AddRadioGroup(
	int arrangement,
	int spacing,
	GlowRadioButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddRadioGroup");
	
	return new GlowQuickRadioGroupWidget(this, receiver, arrangement, spacing);
}


GlowLabeledSliderWidget* GlowQuickPanelWidget::AddSlider(
	float min,
	float max,
	float initial,
	int options,
	int numTicks,
	const char* valueLabel,
	const char* label,
	GlowSliderReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddSlider");
	
	GlowLabeledSliderParams params;
	params.x = params.y = 0;
	if (_arrangement == horizontal)
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
	return new GlowLabeledSliderWidget(this, params);
}


GlowScrollBarWidget* GlowQuickPanelWidget::AddScrollBar(
	long min,
	long max,
	long span,
	long initialTop,
	GlowScrollBarReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddScrollBar");
	
	GlowScrollBarParams params;
	params.x = params.y = 0;
	if (_arrangement == horizontal)
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
	return new GlowScrollBarWidget(this, params);
}


GlowLabeledTextFieldWidget* GlowQuickPanelWidget::AddTextField(
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
		new GlowLabeledTextFieldWidget(this, params);
	if (Root()->GetKeyboardFocus() == 0)
	{
		ret->GrabKeyboardFocus();
	}
	return ret;
}


GlowLabeledHiddenTextFieldWidget* GlowQuickPanelWidget::AddHiddenTextField(
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
		new GlowLabeledHiddenTextFieldWidget(this, params, hideCharacter);
	if (Root()->GetKeyboardFocus() == 0)
	{
		ret->GrabKeyboardFocus();
	}
	return ret;
}


GlowLabelWidget* GlowQuickPanelWidget::AddLabel(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddLabel");
	
	GlowLabelParams params;
	params.x = params.y = 0;
	params.text = text;
	return new GlowLabelWidget(this, params);
}


GlowQuickPanelWidget::GlowQuickPanelWidget(
	GlowWidgetRoot* root,
	GlowQuickPanelWidget* parent,
	int style,
	const char* label,
	int arrangement,
	int alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::GlowQuickPanelWidget");
	
	GlowPanelParams params;
	params.style = style;
	params.clipping = false;
	GlowPanelWidget::Init(root, parent, params);
	
	_arrangement = (arrangement == parentSetting ?
		parent->GetArrangement() : arrangement);
	_alignment = (alignment == parentSetting ?
		parent->GetAlignment() : alignment);
	_spacing = (spacing == parentSetting ?
		parent->GetSpacing() : spacing);
	_hmargin = (hmargin == parentSetting ?
		parent->GetHMargin() : hmargin);
	_vmargin = (vmargin == parentSetting ?
		parent->GetVMargin() : vmargin);
	_label = 0;
	
	if (label != 0 && *label != 0)
	{
		GlowLabelParams lparams;
		lparams.opaque = true;
		lparams.hIndent = 3;
		lparams.x = lparams.y = 0;
		lparams.text = label;
		_label = new GlowLabelWidget(this, lparams);
	}
}


int GlowQuickPanelWidget::OnAutoPack(
	int hSize,
	int vSize,
	int hOption,
	int vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::OnAutoPack");
	
	int position = (_arrangement == vertical) ? _vmargin : _hmargin;
	int size = 0;
	int minWidth = position;
	
	// Arrange widgets with default alignment
	for (GlowComponent::ChildIterator iter = BeginChildren();
		iter != EndChildren(); ++iter)
	{
		GlowWidget* widget = dynamic_cast<GlowWidget*>(*iter);
		if (widget == 0)
		{
			continue;
		}
		
		if (widget == _label)
		{
			if (GetStyle() == GlowPanelWidget::etchedStyle)
			{
				widget->AutoPack(_hmargin, unspecifiedPos, 0, unspecifiedPos,
					leftPos | preferredSize, centerPos | preferredSize);
				topMargin = widget->Height()/2;
				if (_arrangement == vertical)
				{
					position += widget->Height()/2;
				}
			}
			else
			{
				widget->AutoPack(_hmargin, unspecifiedPos, unspecifiedPos, 0,
					leftPos | preferredSize, bottomPos | preferredSize);
				topMargin = widget->Height();
			}
			minWidth = widget->Width()+_hmargin+_hmargin;
			continue;
		}
		
		int leftMargin = 0, rightMargin = 0, topMargin = 0, bottomMargin = 0;
		if (_arrangement == horizontal)
		{
			widget->AutoPack(position, unspecifiedPos, _vmargin, unspecifiedPos,
				leftPos | preferredSize, topPos | preferredSize,
				leftMargin, rightMargin, topMargin, bottomMargin);
			size = GLOW_STD::max(size, widget->Height()+topMargin+bottomMargin);
			position += widget->Width()+leftMargin+rightMargin+_spacing;
		}
		else
		{
			widget->AutoPack(_hmargin, unspecifiedPos, position, unspecifiedPos,
				leftPos | preferredSize, topPos | preferredSize,
				leftMargin, rightMargin, topMargin, bottomMargin);
			size = GLOW_STD::max(size, widget->Width()+leftMargin+rightMargin);
			position += widget->Height()+topMargin+bottomMargin+_spacing;
		}
	}
	
	// Preferred width and height
	int pwidth = 0, pheight = 0;
	if (_arrangement == horizontal)
	{
		pwidth = GLOW_STD::max(minWidth, position-_spacing+_hmargin);
		pheight = size+_vmargin+_vmargin;
	}
	else
	{
		pwidth = GLOW_STD::max(minWidth, size+_hmargin+_hmargin);
		pheight = position-_spacing+_vmargin;
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
		vnew = vSize;
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
	if (_arrangement == horizontal)
	{
		size = vnew-_vmargin-_vmargin;
	}
	else
	{
		size = hnew-_hmargin-_hmargin;
	}
	
	// Now align contents
	int i=0;
	for (GlowComponent::ChildIterator iter = BeginChildren();
		iter != EndChildren(); ++iter, ++i)
	{
		GlowWidget* widget = dynamic_cast<GlowWidget*>(*iter);
		if (widget == 0 || widget == _label)
		{
			continue;
		}
		
		if (_arrangement == horizontal)
		{
			// Horizontal arrangement
			int vOption = (_alignment & alignExpand) ?
				expandPreferredSize : preferredSize;
			
			if ((_alignment & 3) == alignBottom)
			{
				vOption |= bottomPos;
			}
			else if ((_alignment & 3) == alignCenter)
			{
				vOption |= centerPos;
			}
			else
			{
				vOption |= topPos;
			}
			widget->AutoPack(unspecifiedPos, unspecifiedPos,
				_vmargin, _vmargin+size, noMove | noReshape, vOption);
		}
		else
		{
			// Vertical arrangement
			int hOption = (_alignment & alignExpand) ?
				expandPreferredSize : preferredSize;
			
			if ((_alignment & 3) == alignLeft)
			{
				hOption |= leftPos;
			}
			else if ((_alignment & 3) == alignCenter)
			{
				hOption |= centerPos;
			}
			else
			{
				hOption |= rightPos;
			}
			widget->AutoPack(_hmargin, _hmargin+size,
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

