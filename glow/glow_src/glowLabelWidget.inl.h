/*
===============================================================================

	FILE:  glowLabelWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Static label widget for GLOW
	
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

#include <cstring>


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowLabel
===============================================================================
*/

inline GlowLabelWidget::GlowLabelWidget()
{
}


inline GlowLabelWidget::GlowLabelWidget(
	GlowWidget* parent,
	const GlowLabelParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabelWidget::GlowLabelWidget(
	GlowWidgetRoot* root,
	const GlowLabelParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabelWidget::Init(
	GlowWidget* parent,
	const GlowLabelParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabelWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabelParams& params)
{
	Init(root, 0, params);
}


inline const char* GlowLabelWidget::GetText() const
{
	return text_;
}


inline GlowFont GlowLabelWidget::GetFont() const
{
	return font_;
}


inline void GlowLabelWidget::SetFont(
	GlowFont font)
{
	font_ = font;
	RecalcText_();
	Refresh();
}


inline bool GlowLabelWidget::IsOpaque() const
{
	return opaque_;
}


inline void GlowLabelWidget::SetOpaque(
	bool opaque)
{
	opaque_ = opaque;
	Refresh();
}


inline int GlowLabelWidget::GetHIndent() const
{
	return hIndent_;
}


inline int GlowLabelWidget::GetVIndent() const
{
	return vIndent_;
}


inline void GlowLabelWidget::SetHIndent(
	int indent)
{
	hIndent_ = indent;
	Refresh();
}


inline void GlowLabelWidget::SetVIndent(
	int indent)
{
	vIndent_ = indent;
	Refresh();
}


inline Alignment GlowLabelWidget::GetAlignment() const
{
	return alignment_;
}


inline void GlowLabelWidget::SetAlignment(
	Alignment alignment)
{
	alignment_ = alignment;
	Refresh();
}


inline GlowColor GlowLabelWidget::GetBackColor() const
{
	return backColor_;
}


inline GlowColor GlowLabelWidget::GetTextColor() const
{
	return textColor_;
}


inline GlowColor GlowLabelWidget::GetDisableTextColor() const
{
	return disableTextColor_;
}


inline void GlowLabelWidget::SetBackColor(
	GlowColor c)
{
	backColor_ = c;
	Refresh();
}


inline void GlowLabelWidget::SetTextColor(
	GlowColor c)
{
	textColor_ = c;
	Refresh();
}


inline void GlowLabelWidget::SetDisableTextColor(
	GlowColor c)
{
	disableTextColor_ = c;
	Refresh();
}


inline void GlowLabelWidget::FindPreferredSize(
	int& preferredWidth,
	int& preferredHeight)
{
	preferredWidth = maxLineWidth_+GetHIndent()*2;
	preferredHeight = textHeight_+GetVIndent()*2;
}


/*
===============================================================================
	Inline methods of GlowWidgetLabelWidget
===============================================================================
*/

inline GlowWidgetLabelWidget::GlowWidgetLabelWidget(
	GlowWidget* parent,
	const GlowLabelParams& params) :
GlowLabelWidget(parent, params)
{
}


/*
===============================================================================
	Inline methods of GlowWidgetLabelHelper
===============================================================================
*/

inline GlowColor GlowWidgetLabelHelper::GetLabelTextColor() const
{
	return label_->GetTextColor();
}


inline void GlowWidgetLabelHelper::SetLabelTextColor(
	GlowColor c)
{
	label_->SetTextColor(c);
}


inline GlowColor GlowWidgetLabelHelper::GetLabelDisableTextColor() const
{
	return label_->GetDisableTextColor();
}


inline void GlowWidgetLabelHelper::SetLabelDisableTextColor(
	GlowColor c)
{
	label_->SetDisableTextColor(c);
}


inline const char* GlowWidgetLabelHelper::GetLabelText() const
{
	return label_->GetText();
}


inline void GlowWidgetLabelHelper::SetLabelText(
	const char* text)
{
	label_->SetText(text);
	RepositionLabel();
}


inline GlowFont GlowWidgetLabelHelper::GetLabelFont() const
{
	return label_->GetFont();
}


inline void GlowWidgetLabelHelper::SetLabelFont(
	GlowFont font)
{
	label_->SetFont(font);
	RepositionLabel();
}


inline int GlowWidgetLabelHelper::GetLabelSpacing() const
{
	return labelSpacing_;
}


inline void GlowWidgetLabelHelper::SetLabelSpacing(
	int spacing)
{
	if (spacing != labelSpacing_)
	{
		labelSpacing_ = spacing;
		RepositionLabel();
	}
}


inline GlowWidgetLabelHelper::LabelPosition GlowWidgetLabelHelper::GetLabelPosition() const
{
	return labelPosition_;
}


inline void GlowWidgetLabelHelper::SetLabelPosition(
	LabelPosition position)
{
	if (position != labelPosition_)
	{
		labelPosition_ = position;
		RepositionLabel();
	}
}


inline int GlowWidgetLabelHelper::GetLabelWidth() const
{
	return labelWidth_;
}


inline void GlowWidgetLabelHelper::SetLabelWidth(
	int width)
{
	if (width != labelWidth_)
	{
		labelWidth_ = width;
		RepositionLabel();
	}
}


inline int GlowWidgetLabelHelper::GetLabelHeight() const
{
	return labelHeight_;
}


inline void GlowWidgetLabelHelper::SetLabelHeight(
	int height)
{
	if (height != labelHeight_)
	{
		labelHeight_ = height;
		RepositionLabel();
	}
}


inline GlowWidgetLabelWidget* GlowWidgetLabelHelper::Label() const
{
	return label_;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

