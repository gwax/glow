/*
===============================================================================

	FILE:  glowLabelWidget.cpp
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabelParams GlowLabelParams::defaults(true);

GlowLabelParams::GlowLabelParams()
{
	*this = defaults;
}

GlowLabelParams::GlowLabelParams(bool) :
GlowWidgetParams(true),
backColor(0.8f, 0.8f, 0.8f),
textColor(0.0f, 0.0f, 0.0f),
disableTextColor(0.0f, 0.0f, 0.0f)
{
	clipping = true;
	width = 100;
	height = 100;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	opaque = false;
	alignment = GlowLabelWidget::alignLeft;
	hIndent = 1;
	vIndent = 0;
}


/*
===============================================================================
	Methods of GlowLabelWidget
===============================================================================
*/

void GlowLabelWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabelParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	if (params.text == 0)
	{
		text_ = 0;
	}
	else
	{
		text_ = new char[GLOW_CSTD::strlen(params.text)+1];
		GLOW_CSTD::strcpy(text_, params.text);
	}
	font_ = params.font;
	RecalcText_();
	opaque_ = params.opaque;
	hIndent_ = params.hIndent;
	vIndent_ = params.vIndent;
	backColor_ = params.backColor;
	textColor_ = params.textColor;
	disableTextColor_ = params.disableTextColor;
	alignment_ = params.alignment;
}


GlowLabelWidget::~GlowLabelWidget()
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::~GlowLabelWidget");
	
	delete[] text_;
}


void GlowLabelWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::SetText");
	
	delete[] text_;
	if (text == 0)
	{
		text_ = 0;
	}
	else
	{
		text_ = new char[GLOW_CSTD::strlen(text)+1];
		GLOW_CSTD::strcpy(text_, text);
	}
	RecalcText_();
	Refresh();
}


void GlowLabelWidget::RecalcText_()
{
	textHeight_ = 1;
	maxLineWidth_ = 0;
	int thisLineWidth = 0;
	int strlength = (text_ == 0) ? 0 : GLOW_CSTD::strlen(text_);
	for (int i=0; i<strlength; i++)
	{
		if (text_[i] == '\r')
		{
			textHeight_++;
			thisLineWidth = 0;
			if (i+1<strlength && text_[i+1] == '\n')
			{
				i++;
			}
		}
		else if (text_[i] == '\n')
		{
			textHeight_++;
			thisLineWidth = 0;
		}
		else
		{
			thisLineWidth += font_.CharWidth(text_[i]);
			if (thisLineWidth > maxLineWidth_)
			{
				maxLineWidth_ = thisLineWidth;
			}
		}
	}
	textHeight_ *= font_.Leading();
}


GlowWidget::AutoPackError GlowLabelWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::OnAutoPack");

	int hnew = Width();
	int preferred = maxLineWidth_+hIndent_+hIndent_;
	if (hSize != unspecifiedSize && hSize < preferred)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = preferred;
	}
	
	int vnew = Height();
	preferred = textHeight_+vIndent_+vIndent_;
	if (vSize != unspecifiedSize && vSize < preferred)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize || vOption == expandPreferredSize)
	{
		vnew = vSize;
	}
	else if (vOption == preferredSize)
	{
		vnew = preferred;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowLabelWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::OnWidgetPaint");
	
	if (opaque_)
	{
		backColor_.Apply();
		::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	}
	
	if (IsActive())
	{
		textColor_.Apply();
	}
	else
	{
		disableTextColor_.Apply();
	}
	
	int vPos = font_.BaselinePos()+vIndent_;
	int textlen = (text_ == 0) ? 0 : GLOW_CSTD::strlen(text_);
	for (int i=-1; i<textlen; ++i)
	{
		if (i == -1 || text_[i] == '\n' ||
			(text_[i] == '\r' && i+1 < textlen && text_[i+1] != '\n'))
		{
			GLfloat x, y;
			
			// Next v position
			if (i >= 0)
			{
				vPos += font_.Leading();
			}
			
			// Set alignment raster position
			int hPos = (alignment_ == alignLeft) ? hIndent_ :
				((alignment_ == alignRight) ? Width()-hIndent_ : Width()/2);
			NormalizeCoordinates(hPos, vPos, x, y);
			::glRasterPos2f(x, y);
			
			// Now we need to align the text (if center or right).
			// This means we need to subtract the length of the line if
			// aligned right, or half the length if aligned center
			if (alignment_ != alignLeft)
			{
				// First, find the length of the line.
				int thisLineWidth = 0;
				for (int j=i+1; j<textlen && text_[j] != '\r' &&
					text_[j] != '\n'; ++j)
				{
					thisLineWidth += font_.CharWidth(text_[j]);
				}
				
				// Offset
				::glBitmap(0, 0, 0, 0, (alignment_ == alignRight) ?
					-thisLineWidth : -thisLineWidth/2, 0, 0);
			}
		}
		else
		{
			::glutBitmapCharacter(font_, text_[i]);
		}
	}
}


/*
===============================================================================
	Methods of GlowWidgetLabelWidget
===============================================================================
*/

GlowWidget::AutoPackError GlowWidgetLabelWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowWidgetLabelWidget::OnAutoPack");
	
	int hnew = Width();
	int pwidth = 0, pheight = 0;
	FindPreferredSize(pwidth, pheight);
	
	if (hSize != unspecifiedSize)
	{
		if (hSize < 2)
		{
			return hAutoPackError;
		}
		if (hSize < pwidth)
		{
			pwidth = hSize;
		}
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
	if (vSize != unspecifiedSize)
	{
		if (vSize < 2)
		{
			return vAutoPackError;
		}
		if (vSize < pheight)
		{
			pheight = vSize;
		}
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
	Methods of GlowWidgetLabelHelper
===============================================================================
*/

void GlowWidgetLabelHelper::InitLabel(
	GlowWidget* main,
	LabelPosition position,
	int width,
	int height,
	int spacing,
	const char* text,
	GlowFont font,
	GlowColor textColor,
	GlowColor disableTextColor)
{
	main_ = main;
	labelSpacing_ = spacing;
	labelPosition_ = position;
	labelWidth_ = width;
	labelHeight_ = height;
	GlowLabelParams params;
	params.text = text;
	params.font = font;
	params.textColor = textColor;
	params.disableTextColor = disableTextColor;
	label_ = new GlowWidgetLabelWidget(main, params);
	RepositionLabel();
}


GlowWidget::AutoPackError GlowWidgetLabelHelper::HelpAutoPack(
	int& hSize,
	int& vSize,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	if (label_->GetText() == 0)
	{
		return GlowWidget::noAutoPackError;
	}
	
	// Determine preferred size for label
	label_->FindPreferredSize(labelWidth_, labelHeight_);
	
	// Adjust size according to label
	if (hSize != GlowWidget::unspecifiedSize)
	{
		if (labelPosition_ == leftLabelPosition ||
			labelPosition_ == rightLabelPosition)
		{
			hSize -= labelWidth_ + labelSpacing_;
		}
		else if (labelWidth_ > main_->Width())
		{
			hSize -= labelWidth_ - main_->Width();
		}
		if (hSize < 2)
		{
			return GlowWidget::hAutoPackError;
		}
	}
	if (vSize != GlowWidget::unspecifiedSize)
	{
		if (labelPosition_ == topLabelPosition ||
			labelPosition_ == bottomLabelPosition)
		{
			vSize -= labelHeight_ + labelSpacing_;
		}
		else if (labelHeight_ > main_->Height())
		{
			vSize -= labelHeight_ - main_->Height();
		}
		if (vSize < 2)
		{
			return GlowWidget::vAutoPackError;
		}
	}
	
	// Adjust margins according to label
	switch (labelPosition_)
	{
		case leftLabelPosition:
			leftMargin = GLOW_STD::max(leftMargin, labelWidth_+labelSpacing_);
			break;
		case rightLabelPosition:
			rightMargin = GLOW_STD::max(rightMargin, labelWidth_+labelSpacing_);
			break;
		case topLabelPosition:
			topMargin = GLOW_STD::max(topMargin, labelHeight_+labelSpacing_);
			break;
		case bottomLabelPosition:
			bottomMargin = GLOW_STD::max(bottomMargin, labelHeight_+labelSpacing_);
			break;
	}
	if ((labelPosition_ == leftLabelPosition || labelPosition_ == rightLabelPosition) &&
		labelHeight_ > main_->Height())
	{
		topMargin = GLOW_STD::max(topMargin, (labelHeight_-main_->Height())/2);
		bottomMargin = GLOW_STD::max(bottomMargin, (labelHeight_-main_->Height())/2);
	}
	if ((labelPosition_ == topLabelPosition || labelPosition_ == bottomLabelPosition) &&
		labelWidth_ > main_->Width())
	{
		rightMargin = GLOW_STD::max(rightMargin, labelWidth_-main_->Width());
	}
	
	return GlowWidget::noAutoPackError;
}


void GlowWidgetLabelHelper::RepositionLabel()
{
	switch (labelPosition_)
	{
		case leftLabelPosition:
			label_->AutoPack(
				-labelWidth_-labelSpacing_,
				-labelSpacing_,
				(main_->Height()-labelHeight_-1)/2,
				(main_->Height()+labelHeight_+1)/2,
				GlowWidget::preferredSize | GlowWidget::rightPos,
				GlowWidget::preferredSize | GlowWidget::centerPos);
			break;
		case rightLabelPosition:
			label_->AutoPack(
				labelSpacing_,
				labelSpacing_+labelWidth_,
				(main_->Height()-labelHeight_-1)/2,
				(main_->Height()+labelHeight_+1)/2,
				GlowWidget::preferredSize | GlowWidget::leftPos,
				GlowWidget::preferredSize | GlowWidget::centerPos);
			break;
		case topLabelPosition:
			label_->AutoPack(
				0, main_->Width(),
				-labelHeight_-labelSpacing_,
				-labelSpacing_,
				GlowWidget::preferredSize | GlowWidget::leftPos,
				GlowWidget::preferredSize | GlowWidget::bottomPos);
			break;
		case bottomLabelPosition:
			label_->AutoPack(
				0, main_->Width(),
				labelSpacing_,
				labelSpacing_+labelHeight_,
				GlowWidget::preferredSize | GlowWidget::leftPos,
				GlowWidget::preferredSize | GlowWidget::topPos);
			break;
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

