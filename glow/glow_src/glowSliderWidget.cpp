/*
===============================================================================

	FILE:  glowSliderWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Slider widget for GLOW
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <cmath>
#include <cstdio>

#ifndef GLOW_SLIDERWIDGET__H
	#include "glowSliderWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowSliderParams GlowSliderParams::defaults(true);

GlowSliderParams::GlowSliderParams()
{
	*this = defaults;
}

GlowSliderParams::GlowSliderParams(bool) :
GlowWidgetParams(true),
stripColor(0.5f, 0.5f, 0.5f),
indicatorColor(0.7f, 0.7f, 0.7f),
tickMarkColor(0.0f, 0.0f, 0.0f),
hiliteIndicatorColor(0.8f, 0.6f, 0.6f),
disableStripColor(0.7f, 0.7f, 0.7f),
disableIndicatorColor(0.7f, 0.7f, 0.7f),
disableTickMarkColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.3f, 0.3f, 0.3f)
{
	width = 130;
	height = 30;
	options = GlowSliderWidget::defaultOptions;
	min = 0.0f;
	max = 1.0f;
	initial = 0.0f;
	numTicks = 0;
	receiver = 0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabeledSliderParams GlowLabeledSliderParams::defaults(true);

GlowLabeledSliderParams::GlowLabeledSliderParams()
{
	*this = defaults;
}

GlowLabeledSliderParams::GlowLabeledSliderParams(bool) :
GlowSliderParams(true),
labelColor(0.0f, 0.0f, 0.0f),
disableLabelColor(0.3f, 0.3f, 0.3f),
minmaxColor(0.0f, 0.0f, 0.0f),
disableMinmaxColor(0.3f, 0.3f, 0.3f)
{
	labelTemplate = 0;
	labelFont = GLUT_BITMAP_HELVETICA_12;
	labelPosition = GlowLabeledSliderWidget::leftLabelPosition;
	labelWidth = 100;
	labelHeight = 15;
	labelSpacing = 5;
	minmaxTemplate = 0;
	minmaxFont = GLUT_BITMAP_HELVETICA_12;
	minmaxSize = 15;
}


/*
===============================================================================
	Methods of GlowSliderWidget
===============================================================================
*/

void GlowSliderWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowSliderParams& params)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::Init");
	
	GLOW_DEBUG(params.max < params.min, "max < min while constructing GlowSlider");
	GlowWidget::Init(root, parent, params);
	dragging_ = false;
	type_ = params.options;
	min_ = params.min;
	max_ = params.max;
	value_ = params.initial;
	if (value_ > max_) value_ = max_;
	if (value_ < min_) value_ = min_;
	numTicks_ = params.numTicks;
	if (params.receiver != 0)
	{
		sender_.Bind(params.receiver);
	}
	
	stripColor_ = params.stripColor;
	indicatorColor_ = params.indicatorColor;
	tickMarkColor_ = params.tickMarkColor;
	hiliteIndicatorColor_ = params.hiliteIndicatorColor;
	disableStripColor_ = params.disableStripColor;
	disableIndicatorColor_ = params.disableIndicatorColor;
	disableTickMarkColor_ = params.disableTickMarkColor;
	disableOutlineColor_ = params.disableOutlineColor;
	lightBevelColor_ = params.lightBevelColor;
	darkBevelColor_ = params.darkBevelColor;
	
	RegisterMouseEvents();
}


GlowWidget::AutoPackError GlowSliderWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnAutoPack");
	
	int pwidth = 30, pheight = 30;
	if (Width() > Height())
	{
		pwidth = 130;
	}
	else
	{
		pheight = 130;
	}
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == expandPreferredSize || hOption == forcedSize)
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
	if (vOption == expandPreferredSize || vOption == forcedSize)
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


void GlowSliderWidget::CalcValue_(
	int x,
	int y)
{
	if (Width() > Height())
	{
		value_ = float(x-Height()/2)/float(Width()-Height());
	}
	else
	{
		value_ = float(y-Width()/2)/float(Height()-Width());
	}
	if (value_ > 1.0f) value_ = 1.0f;
	if (value_ < 0.0f) value_ = 0.0f;
	if ((type_ & logarithmic) != 0)
	{
		if ((type_ & decreasing) != 0)
		{
			value_ = GLOW_CSTD::pow(max_, 1.0f-value_)*GLOW_CSTD::pow(min_, value_);
		}
		else
		{
			value_ = GLOW_CSTD::pow(min_, 1.0f-value_)*GLOW_CSTD::pow(max_, value_);
		}
	}
	else // linear
	{
		if ((type_ & decreasing) != 0)
		{
			value_ = max_*(1.0f-value_)+min_*value_;
		}
		else
		{
			value_ = min_*(1.0f-value_)+max_*value_;
		}
	}
}


void GlowSliderWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetPaint");
	
	float pseudoWidth;
	float pseudoHeight;
	if (Width() < Height())
	{
		if ((type_ & ticksOnTop) != 0)
		{
			::glScalef(-1.0f, 1.0f, 1.0f);
		}
		if ((type_ & decreasing) == 0)
		{
			::glScalef(1.0f, -1.0f, 1.0f);
		}
		::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		pseudoWidth = Height();
		pseudoHeight = Width();
	}
	else
	{
		if ((type_ & ticksOnTop) != 0)
		{
			::glScalef(1.0f, -1.0f, 1.0f);
		}
		if ((type_ & decreasing) != 0)
		{
			::glScalef(-1.0f, 1.0f, 1.0f);
		}
		pseudoWidth = Width();
		pseudoHeight = Height();
	}
	
	GLfloat bevelHeight = float(4)/pseudoHeight;
	GLfloat bevelWidth = float(4)/pseudoWidth;
	GLfloat pixelHeight = float(2)/pseudoHeight;
	GLfloat pixelWidth = float(2)/pseudoWidth;
	
	// Draw strip
	float top, bottom;
	if (numTicks_ == 0)
	{
		top = 0.25f;
		bottom = -0.25f;
	}
	else
	{
		top = 0.4f;
		bottom = -0.1f;
	}
	
	// Strip back
	if (!IsActive())
	{
		disableStripColor_.Apply();
	}
	else
	{
		stripColor_.Apply();
	}
	::glRectf(-1.0f, bottom, 1.0f, top);
	
	if (IsActive())
	{
		// Left bevel
		RightBevelColor_();
		::glBegin(GL_POLYGON);
		::glVertex2f(-1.0f, bottom);
		::glVertex2f(-1.0f+bevelWidth, bottom+bevelHeight);
		::glVertex2f(-1.0f+bevelWidth, top-bevelHeight);
		::glVertex2f(-1.0f, top);
		::glEnd();
		
		// Top bevel
		BottomBevelColor_();
		::glBegin(GL_POLYGON);
		::glVertex2f(-1.0f, top);
		::glVertex2f(-1.0f+bevelWidth, top-bevelHeight);
		::glVertex2f(1.0f-bevelWidth, top-bevelHeight);
		::glVertex2f(1.0f, top);
		::glEnd();
		
		// Right bevel
		LeftBevelColor_();
		::glBegin(GL_POLYGON);
		::glVertex2f(1.0f, bottom);
		::glVertex2f(1.0f-bevelWidth, bottom+bevelHeight);
		::glVertex2f(1.0f-bevelWidth, top-bevelHeight);
		::glVertex2f(1.0f, top);
		::glEnd();
		
		// Bottom bevel
		TopBevelColor_();
		::glBegin(GL_POLYGON);
		::glVertex2f(-1.0f, bottom);
		::glVertex2f(-1.0f+bevelWidth, bottom+bevelHeight);
		::glVertex2f(1.0f-bevelWidth, bottom+bevelHeight);
		::glVertex2f(1.0f, bottom);
		::glEnd();
	}
	else
	{
		disableOutlineColor_.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, bottom);
		::glVertex2f(-1.0f, top);
		::glVertex2f(1.0f, top);
		::glVertex2f(1.0f, bottom);
		::glEnd();
	}
	
	// Draw tickmarks if necessary
	float indicRadius = pseudoHeight/pseudoWidth;
	if (!IsActive())
	{
		disableTickMarkColor_.Apply();
	}
	else
	{
		tickMarkColor_.Apply();
	}
	::glLineWidth(2);
	if (numTicks_ == 1)
	{
		::glBegin(GL_LINES);
		::glVertex2f(0.0f, -0.5f);
		::glVertex2f(0.0f, -1.0f);
		::glEnd();
	}
	else if (numTicks_ > 1)
	{
		for (int i=0; i<numTicks_; i++)
		{
			float pos = float(i)/float(numTicks_-1)*2.0f-1.0f;
			pos *= (1.0-indicRadius);
			::glBegin(GL_LINES);
			::glVertex2f(pos, -0.5f);
			::glVertex2f(pos, -1.0f);
			::glEnd();
		}
	}
	::glLineWidth(1);
	
	// Draw indicator
	if (max_ > min_)
	{
		float indicPos;
		if ((type_ & logarithmic) != 0)
		{
			indicPos = GLOW_CSTD::log(value_/min_)/GLOW_CSTD::log(max_/min_);
		}
		else
		{
			indicPos = (value_-min_)/(max_-min_);
		}
		indicPos = indicRadius-1.0f+indicPos*
			(pseudoWidth-pseudoHeight)/pseudoWidth*2.0f;
		if (numTicks_ > 0)
		{
			// With ticks (pentagons)
			if (!IsActive())
			{
				disableIndicatorColor_.Apply();
			}
			else if (dragging_)
			{
				hiliteIndicatorColor_.Apply();
			}
			else
			{
				indicatorColor_.Apply();
			}
			::glBegin(GL_POLYGON);
			::glVertex2f(indicPos, -0.5f);
			::glVertex2f(indicPos-indicRadius, 0.0f);
			::glVertex2f(indicPos-indicRadius, 1.0f);
			::glVertex2f(indicPos+indicRadius, 1.0f);
			::glVertex2f(indicPos+indicRadius, 0.0f);
			::glEnd();
			
			if (IsActive())
			{
				// Bottom (crooked) bevel
				BottomBevelColor_();
				::glBegin(GL_QUAD_STRIP);
				::glVertex2f(indicPos-indicRadius, 0.0f);
				::glVertex2f(indicPos-indicRadius+bevelWidth, bevelHeight);
				::glVertex2f(indicPos, -0.5f);
				::glVertex2f(indicPos, -0.5f+6.0f/pseudoHeight);
				::glVertex2f(indicPos+indicRadius, 0.0f);
				::glVertex2f(indicPos+indicRadius-bevelWidth, bevelHeight);
				::glEnd();
				
				// Left bevel
				LeftBevelColor_();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, 0.0f);
				::glVertex2f(indicPos-indicRadius+bevelWidth, bevelHeight);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos-indicRadius, 1.0f);
				::glEnd();
				
				// Top bevel
				TopBevelColor_();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, 1.0f);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0f);
				::glEnd();
				
				// Right bevel
				RightBevelColor_();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos+indicRadius, 0.0f);
				::glVertex2f(indicPos+indicRadius-bevelWidth, bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0f);
				::glEnd();
				
				// Ridges
				float ridgeTop = 1.0f-bevelHeight*2.0f;
				float ridgeBottom = -0.5f+bevelHeight*3.0f;
				LeftBevelColor_();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeTop);
				::glEnd();
				RightBevelColor_();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeTop);
				::glEnd();
			}
			else
			{
				disableOutlineColor_.Apply();
				::glBegin(GL_LINE_LOOP);
				::glVertex2f(indicPos, -0.5f);
				::glVertex2f(indicPos-indicRadius+pixelWidth, 0.0f);
				::glVertex2f(indicPos-indicRadius+pixelWidth, 1.0f-pixelHeight);
				::glVertex2f(indicPos+indicRadius-pixelWidth, 1.0f-pixelHeight);
				::glVertex2f(indicPos+indicRadius-pixelWidth, 0.0f);
				::glEnd();
			}
		}
		else
		{
			// No ticks (rects)
			if (!IsActive())
			{
				disableIndicatorColor_.Apply();
			}
			else if (dragging_)
			{
				hiliteIndicatorColor_.Apply();
			}
			else
			{
				indicatorColor_.Apply();
			}
			::glRectf(indicPos-indicRadius, -1.0f, indicPos+indicRadius, 1.0f);
			
			if (IsActive())
			{
				// Bottom bevel
				BottomBevelColor_();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, -1.0);
				::glVertex2f(indicPos-indicRadius+bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos+indicRadius, -1.0);
				::glEnd();
				
				// Left bevel
				LeftBevelColor_();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, -1.0);
				::glVertex2f(indicPos-indicRadius+bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos-indicRadius, 1.0);
				::glEnd();
				
				// Top bevel
				TopBevelColor_();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, 1.0);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0);
				::glEnd();
				
				// Right bevel
				RightBevelColor_();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos+indicRadius, -1.0);
				::glVertex2f(indicPos+indicRadius-bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0);
				::glEnd();
				
				// Ridges
				float ridgeTop = 1.0-bevelHeight*2.0;
				float ridgeBottom = -1.0+bevelHeight*2.0;
				LeftBevelColor_();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeTop);
				::glEnd();
				RightBevelColor_();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeTop);
				::glEnd();
			}
			else
			{
				disableOutlineColor_.Apply();
				::glBegin(GL_LINE_LOOP);
				::glVertex2f(indicPos-indicRadius, -1.0f);
				::glVertex2f(indicPos-indicRadius, 1.0f);
				::glVertex2f(indicPos+indicRadius, 1.0f);
				::glVertex2f(indicPos+indicRadius, -1.0f);
				::glEnd();
			}
		}
	}
}


void GlowSliderWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetMouseDown");
	
	if (max_ > min_)
	{
		int indicRadius, indicPos;
		float valRatio;
		if ((type_ & logarithmic) != 0)
		{
			valRatio = GLOW_CSTD::log(value_/min_)/GLOW_CSTD::log(max_/min_);
		}
		else
		{
			valRatio = (value_-min_)/(max_-min_);
		}
		if ((type_ & decreasing) != 0)
		{
			valRatio = 1.0f-valRatio;
		}
		if (Width() > Height())
		{
			indicRadius = Height()/2;
			indicPos = indicRadius+static_cast<int>(float(Width()-2*indicRadius)*valRatio);
			if (x > indicPos-indicRadius && x < indicPos+indicRadius)
			{
				xoffset_ = indicPos-x;
				yoffset_ = 0;
				dragging_ = true;
				modifiers_ = modifiers;
				Refresh();
			}
		}
		else
		{
			indicRadius = Width()/2;
			indicPos = indicRadius+static_cast<int>(float(Height()-2*indicRadius)*valRatio);
			if (y > indicPos-indicRadius && y < indicPos+indicRadius)
			{
				xoffset_ = 0;
				yoffset_ = indicPos-y;
				dragging_ = true;
				modifiers_ = modifiers;
				Refresh();
			}
		}
	}
}


void GlowSliderWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetMouseUp");
	
	if (dragging_)
	{
		dragging_ = false;
		CalcValue_(x+xoffset_, y+yoffset_);
		OnReleased(button_, modifiers_);
		Refresh();
	}
}


void GlowSliderWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetMouseDrag");
	
	if (dragging_)
	{
		CalcValue_(x+xoffset_, y+yoffset_);
		OnDragged(button_, modifiers_);
		Refresh();
	}
}


void GlowSliderWidget::OnDragged(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnDragged");
	
	GlowSliderMessage msg;
	msg.widget = this;
	msg.released = false;
	msg.value = value_;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	sender_.Send(msg);
}


void GlowSliderWidget::OnReleased(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnReleased");
	
	GlowSliderMessage msg;
	msg.widget = this;
	msg.released = true;
	msg.value = value_;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	sender_.Send(msg);
}


/*
===============================================================================
	Methods of GlowLabeledSliderWidget
===============================================================================
*/

void GlowLabeledSliderWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledSliderParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabeledSliderWidget::Init");
	
	GlowSliderWidget::Init(root, parent, params);
	
	// Main label
	labelSpacing_ = params.labelSpacing;
	labelPosition_ = params.labelPosition;
	labelWidth_ = params.labelWidth;
	labelHeight_ = params.labelHeight;
	labelValue_ = GetValue();
	
	GlowLabelParams lparams;
	char buffer[1000];
	if (params.labelTemplate == 0)
	{
		labelTemplate_ = 0;
		lparams.text = 0;
	}
	else
	{
		labelTemplate_ = new char[GLOW_CSTD::strlen(params.labelTemplate)+1];
		GLOW_CSTD::strcpy(labelTemplate_, params.labelTemplate);
		GLOW_CSTD::sprintf(buffer, labelTemplate_, labelValue_);
		lparams.text = buffer;
	}
	lparams.font = params.labelFont;
	lparams.textColor = params.labelColor;
	lparams.disableTextColor = params.disableLabelColor;
	label_ = new GlowWidgetLabelWidget(this, lparams);
	
	// Min and max labels
	minLabelValue_ = GetMinimum();
	maxLabelValue_ = GetMaximum();
	minmaxSize_ = params.minmaxSize;
	
	lparams.font = params.minmaxFont;
	lparams.textColor = params.minmaxColor;
	lparams.disableTextColor = params.disableMinmaxColor;
	if (params.minmaxTemplate == 0)
	{
		minmaxTemplate_ = 0;
		lparams.text = 0;
	}
	else
	{
		minmaxTemplate_ = new char[GLOW_CSTD::strlen(params.minmaxTemplate)+1];
		GLOW_CSTD::strcpy(minmaxTemplate_, params.minmaxTemplate);
		GLOW_CSTD::sprintf(buffer, minmaxTemplate_, minLabelValue_);
		lparams.text = buffer;
	}
	minLabel_ = new GlowWidgetLabelWidget(this, lparams);
	if (minmaxTemplate_ != 0)
	{
		GLOW_CSTD::sprintf(buffer, minmaxTemplate_, maxLabelValue_);
		lparams.text = buffer;
	}
	maxLabel_ = new GlowWidgetLabelWidget(this, lparams);
	
	ResizeMinmax();
	RepositionMinmax();
	RepositionLabel();
}


void GlowLabeledSliderWidget::RepositionLabel()
{
	// need to make sure label doesn't hit minmax
	int leftMinmax = 0, rightMinmax = 0, topMinmax = 0, bottomMinmax = 0;
	if (minmaxTemplate_ != 0)
	{
		if (Width() > Height())
		{
			// Horizontal slider
			if (GetOptions() & ticksOnTop)
			{
				topMinmax = minmaxSize_;
			}
			else
			{
				bottomMinmax = minmaxSize_;
			}
		}
		else
		{
			// Vertical slider
			if (GetOptions() & ticksOnLeft)
			{
				leftMinmax = minmaxSize_;
			}
			else
			{
				rightMinmax = minmaxSize_;
			}
		}
	}
	switch (labelPosition_)
	{
		case leftLabelPosition:
			label_->AutoPack(
				-leftMinmax-labelWidth_-labelSpacing_,
				-leftMinmax-labelSpacing_,
				(Height()-labelHeight_)/2,
				(Height()+labelHeight_)/2,
				expandPreferredSize | rightPos, expandPreferredSize | centerPos);
			break;
		case rightLabelPosition:
			label_->AutoPack(
				rightMinmax+labelSpacing_,
				rightMinmax+labelSpacing_+labelWidth_,
				(Height()-labelHeight_)/2,
				(Height()+labelHeight_)/2,
				expandPreferredSize | leftPos, expandPreferredSize | centerPos);
			break;
		case topLabelPosition:
			label_->AutoPack(
				(Width()-labelWidth_)/2,
				(Width()+labelWidth_)/2,
				-topMinmax-labelHeight_-labelSpacing_,
				-topMinmax-labelSpacing_,
				expandPreferredSize | centerPos, expandPreferredSize | bottomPos);
			break;
		case bottomLabelPosition:
			label_->AutoPack(
				(Width()-labelWidth_)/2,
				(Width()+labelWidth_)/2,
				bottomMinmax+labelSpacing_,
				bottomMinmax+labelSpacing_+labelHeight_,
				expandPreferredSize | centerPos, expandPreferredSize | topPos);
			break;
	}
}


GlowWidget::AutoPackError GlowLabeledSliderWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledSliderWidget::OnAutoPack");
	
	// Deal with min and max labels
	CheckMinmax();
	ResizeMinmax();
	if (minmaxTemplate_ != 0)
	{
		if (Width() > Height())
		{
			// Horizontal slider
			minmaxSize_ = GLOW_STD::max(minLabel_->Height(), maxLabel_->Height());
			if (GetOptions() & ticksOnTop)
			{
				topMargin = minmaxSize_;
			}
			else
			{
				bottomMargin = minmaxSize_;
			}
		}
		else
		{
			// Vertical slider
			minmaxSize_ = GLOW_STD::max(minLabel_->Width(), maxLabel_->Width());
			if (GetOptions() & ticksOnLeft)
			{
				leftMargin = minmaxSize_;
			}
			else
			{
				rightMargin = minmaxSize_;
			}
		}
	}
	
	// Deal with labels
	if (labelTemplate_ != 0)
	{
		label_->FindPreferredSize(labelWidth_, labelHeight_);
		
		// Adjust size according to label
		if (hSize != unspecifiedSize)
		{
			if (labelPosition_ == leftLabelPosition ||
				labelPosition_ == rightLabelPosition)
			{
				hSize -= labelWidth_ + labelSpacing_;
			}
			else if (labelWidth_ > Width())
			{
				hSize -= labelWidth_ - Width();
			}
			if (hSize < 2)
			{
				return hAutoPackError;
			}
		}
		if (vSize != unspecifiedSize)
		{
			if (labelPosition_ == topLabelPosition ||
				labelPosition_ == bottomLabelPosition)
			{
				vSize -= labelHeight_ + labelSpacing_;
			}
			else if (labelHeight_ > Height())
			{
				vSize -= labelHeight_ - Height();
			}
			if (vSize < 2)
			{
				return vAutoPackError;
			}
		}
		
		// Adjust margins according to label
		switch (labelPosition_)
		{
			case leftLabelPosition:
				leftMargin += labelWidth_+labelSpacing_;
				break;
			case rightLabelPosition:
				rightMargin +=  labelWidth_+labelSpacing_;
				break;
			case topLabelPosition:
				topMargin +=  labelHeight_+labelSpacing_;
				break;
			case bottomLabelPosition:
				bottomMargin +=  labelHeight_+labelSpacing_;
				break;
		}
		if ((labelPosition_ == leftLabelPosition || labelPosition_ == rightLabelPosition) &&
			labelHeight_ > Height())
		{
			topMargin = GLOW_STD::max(topMargin, (labelHeight_-Height())/2);
			bottomMargin = GLOW_STD::max(bottomMargin, (labelHeight_-Height())/2);
		}
		if ((labelPosition_ == topLabelPosition || labelPosition_ == bottomLabelPosition) &&
			labelWidth_ > Width())
		{
			leftMargin = GLOW_STD::max(leftMargin, (labelWidth_-Width())/2);
			rightMargin = GLOW_STD::max(rightMargin, (labelWidth_-Width())/2);
		}
	}
	
	AutoPackError result = GlowSliderWidget::OnAutoPack(hSize, vSize, hOption,
		vOption, leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	// Position labels
	RepositionMinmax();
	RepositionLabel();
	return noAutoPackError;
}


void GlowLabeledSliderWidget::SetLabelTemplate(
	const char* text)
{
	delete labelTemplate_;
	if (text == 0)
	{
		labelTemplate_ = 0;
		label_->SetText(0);
	}
	else
	{
		labelTemplate_ = new char[GLOW_CSTD::strlen(text)+1];
		GLOW_CSTD::strcpy(labelTemplate_, text);
		labelValue_ = GetValue();
		char buffer[1000];
		GLOW_CSTD::sprintf(buffer, labelTemplate_, labelValue_);
		label_->SetText(buffer);
	}
}


void GlowLabeledSliderWidget::SetMinmaxTemplate(
	const char* text)
{
	delete minmaxTemplate_;
	if (text == 0)
	{
		minmaxTemplate_ = 0;
		minLabel_->SetText(0);
		maxLabel_->SetText(0);
	}
	else
	{
		minmaxTemplate_ = new char[GLOW_CSTD::strlen(text)+1];
		GLOW_CSTD::strcpy(minmaxTemplate_, text);
		char buffer[1000];
		minLabelValue_ = GetMinimum();
		GLOW_CSTD::sprintf(buffer, minmaxTemplate_, minLabelValue_);
		minLabel_->SetText(buffer);
		maxLabelValue_ = GetMaximum();
		GLOW_CSTD::sprintf(buffer, minmaxTemplate_, maxLabelValue_);
		maxLabel_->SetText(buffer);
		ResizeMinmax();
		RepositionMinmax();
	}
}


void GlowLabeledSliderWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowLabeledSliderWidget::OnWidgetPaint");
	
	if (labelValue_ != GetValue())
	{
		labelValue_ = GetValue();
		if (labelTemplate_ != 0)
		{
			char buffer[1000];
			GLOW_CSTD::sprintf(buffer, labelTemplate_, labelValue_);
			label_->SetRefreshEnabled(false);
			label_->SetText(buffer);
			label_->SetRefreshEnabled(true);
		}
	}
	if (CheckMinmax())
	{
		ResizeMinmax();
		RepositionMinmax();
	}
	GlowSliderWidget::OnWidgetPaint();
}


bool GlowLabeledSliderWidget::CheckMinmax()
{
	bool needReposition = false;
	if (minLabelValue_ != GetMinimum())
	{
		minLabelValue_ = GetMinimum();
		if (minmaxTemplate_ != 0)
		{
			char buffer[1000];
			GLOW_CSTD::sprintf(buffer, minmaxTemplate_, minLabelValue_);
			minLabel_->SetRefreshEnabled(false);
			minLabel_->SetText(buffer);
			needReposition = true;
			minLabel_->SetRefreshEnabled(true);
		}
	}
	if (maxLabelValue_ != GetMaximum())
	{
		maxLabelValue_ = GetMaximum();
		if (minmaxTemplate_ != 0)
		{
			char buffer[1000];
			GLOW_CSTD::sprintf(buffer, minmaxTemplate_, maxLabelValue_);
			maxLabel_->SetRefreshEnabled(false);
			maxLabel_->SetText(buffer);
			needReposition = true;
			maxLabel_->SetRefreshEnabled(true);
		}
	}
	return needReposition;
}


void GlowLabeledSliderWidget::ResizeMinmax()
{
	int pwidth = 0, pheight = 0;
	minLabel_->FindPreferredSize(pwidth, pheight);
	minLabel_->Reshape(pwidth, pheight);
	maxLabel_->FindPreferredSize(pwidth, pheight);
	maxLabel_->Reshape(pwidth, pheight);
}


void GlowLabeledSliderWidget::RepositionMinmax()
{
	if (Width() > Height())
	{
		// Horizontal slider
		int topLimit, bottomLimit;
		AutoPackOptions vOption;
		if (GetOptions() & ticksOnTop)
		{
			topLimit = -minmaxSize_;
			bottomLimit = 0;
			vOption = bottomPos;
		}
		else
		{
			topLimit = Height();
			bottomLimit = Height()+minmaxSize_;
			vOption = topPos;
		}
		int minLeftLimit, minRightLimit, maxLeftLimit, maxRightLimit;
		AutoPackOptions minHOption, maxHOption;
		if (GetOptions() & GlowSliderWidget::decreasing)
		{
			minLeftLimit = Width()-Height();
			minRightLimit = Width();
			maxLeftLimit = 0;
			maxRightLimit = Height();
			minHOption = (minLabel_->Width() <= Height()) ?
				centerPos : rightPos;
			maxHOption = (maxLabel_->Width() <= Height()) ?
				centerPos : leftPos;
		}
		else
		{
			minLeftLimit = 0;
			minRightLimit = Height();
			maxLeftLimit = Width()-Height();
			maxRightLimit = Width();
			minHOption = (minLabel_->Width() <= Height()) ?
				centerPos : leftPos;
			maxHOption = (maxLabel_->Width() <= Height()) ?
				centerPos : rightPos;
		}
		minLabel_->AutoPack(minLeftLimit, minRightLimit, topLimit, bottomLimit,
			minHOption, vOption);
		maxLabel_->AutoPack(maxLeftLimit, maxRightLimit, topLimit, bottomLimit,
			maxHOption, vOption);
	}
	else
	{
		// Vertical slider
		int leftLimit, rightLimit;
		AutoPackOptions hOption;
		if (GetOptions() & ticksOnLeft)
		{
			leftLimit = -minmaxSize_;
			rightLimit = 0;
			hOption = rightPos;
		}
		else
		{
			leftLimit = Width();
			rightLimit = Width()+minmaxSize_;
			hOption = leftPos;
		}
		int minTopLimit, minBottomLimit, maxTopLimit, maxBottomLimit;
		AutoPackOptions minVOption, maxVOption;
		if (GetOptions() & GlowSliderWidget::decreasing)
		{
			minTopLimit = Height()-Width();
			minBottomLimit = Height();
			maxTopLimit = 0;
			maxBottomLimit = Width();
			minVOption = (minLabel_->Height() <= Width()) ?
				centerPos : bottomPos;
			maxVOption = (maxLabel_->Height() <= Width()) ?
				centerPos : topPos;
		}
		else
		{
			minTopLimit = 0;
			minBottomLimit = Width();
			maxTopLimit = Height()-Width();
			maxBottomLimit = Height();
			minVOption = (minLabel_->Height() <= Width()) ?
				centerPos : topPos;
			maxVOption = (maxLabel_->Height() <= Width()) ?
				centerPos : bottomPos;
		}
		minLabel_->AutoPack(leftLimit, rightLimit, minTopLimit, minBottomLimit,
			hOption, minVOption);
		maxLabel_->AutoPack(leftLimit, rightLimit, maxTopLimit, maxBottomLimit,
			hOption, maxVOption);
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

