/*
===============================================================================

	FILE:  glowScrollBarWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Scroll bar widget for GLOW
	
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

#include <cmath>

#ifndef GLOW_SCROLLBARWIDGET__H
	#include "glowScrollBarWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowScrollBarParams GlowScrollBarParams::defaults(true);

GlowScrollBarParams::GlowScrollBarParams()
{
	*this = defaults;
}

GlowScrollBarParams::GlowScrollBarParams(bool) :
GlowWidgetParams(true),
stripColor(0.5f, 0.5f, 0.5f),
indicatorColor(0.7f, 0.7f, 0.7f),
shadowColor(0.5f, 0.5f, 0.5f),
buttonColor(0.7f, 0.7f, 0.7f),
buttonIconColor(0.0f, 0.0f, 1.0f),
hiliteStripColor(0.6f, 0.5f, 0.5f),
hiliteIndicatorColor(0.8f, 0.6f, 0.6f),
hiliteButtonColor(0.8f, 0.6f, 0.6f),
hiliteButtonIconColor(1.0f, 0.0f, 0.0f),
disableStripColor(0.8f, 0.8f, 0.8f),
disableIndicatorColor(0.7f, 0.7f, 0.7f),
disableButtonColor(0.7f, 0.7f, 0.7f),
disableButtonIconColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.3f, 0.3f, 0.3f)
{
	width = 120;
	height = 20;
	min = 0;
	max = 1;
	span = 1;
	arrowStep = 1;
	pageStep = GlowScrollBarWidget::spanPageStep;
	initialTop = 0;
	firstDelay = 400;
	secondDelay = 50;
	receiver = 0;
}


/*
===============================================================================
	Timer function
===============================================================================
*/

class Glow_ScrollBar_TimerFunc :
	public GlowTimerReceiver
{
	public:
	
		inline Glow_ScrollBar_TimerFunc();
		
		inline void Register(
			int delay,
			GlowScrollBarWidget* bar);
		inline void ForceRemove(
			GlowScrollBarWidget* bar);
		inline void Unregister();
	
	private:
	
		static int _timerID;
	
	protected:
	
		virtual void OnMessage(
			const GlowTimerMessage& message);
	
	private:
	
		GlowScrollBarWidget* _scrollBar;
};


inline Glow_ScrollBar_TimerFunc::Glow_ScrollBar_TimerFunc()
{
	_scrollBar = 0;
}


inline void Glow_ScrollBar_TimerFunc::Register(
	int delay,
	GlowScrollBarWidget* bar)
{
	_timerID = Glow::RegisterTimer(delay, this);
	_scrollBar = bar;
}


inline void Glow_ScrollBar_TimerFunc::ForceRemove(
	GlowScrollBarWidget* bar)
{
	if (_scrollBar == bar)
	{
		Glow::UnregisterTimer(_timerID);
	}
}


inline void Glow_ScrollBar_TimerFunc::Unregister()
{
	Glow::UnregisterTimer(_timerID);
}


void Glow_ScrollBar_TimerFunc::OnMessage(
	const GlowTimerMessage& message)
{
	_scrollBar->HandlePartPeriodical_(false);
}


/*
===============================================================================
	Static globals
===============================================================================
*/

static Glow_ScrollBar_TimerFunc* _timer = new Glow_ScrollBar_TimerFunc;

int Glow_ScrollBar_TimerFunc::_timerID = 0;


/*
===============================================================================
	Methods of GlowScrollBar
===============================================================================
*/

void GlowScrollBarWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowScrollBarParams& params)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::Init");
	
	GLOW_DEBUG(params.max < params.min + params.span,
		"max <= min while constructing GlowScrollBar");
	GlowWidget::Init(root, parent, params);
	min_ = params.min;
	max_ = params.max;
	topValue_ = params.initialTop;
	span_ = params.span;
	if (topValue_ > max_-span_) topValue_ = max_-span_;
	if (topValue_ < min_) topValue_ = min_;
	firstDelay_ = params.firstDelay;
	secondDelay_ = params.secondDelay;
	toggleTimer_ = 0;
	arrowStep_ = params.arrowStep;
	pageStep_ = params.pageStep;
	if (params.receiver != 0)
	{
		sender_.Bind(params.receiver);
	}
	stripColor_ = params.stripColor;
	indicatorColor_ = params.indicatorColor;
	shadowColor_ = params.shadowColor;
	buttonColor_ = params.buttonColor;
	buttonIconColor_ = params.buttonIconColor;
	hiliteStripColor_ = params.hiliteStripColor;
	hiliteIndicatorColor_ = params.hiliteIndicatorColor;
	hiliteButtonColor_ = params.hiliteButtonColor;
	hiliteButtonIconColor_ = params.hiliteButtonIconColor;
	disableStripColor_ = params.disableStripColor;
	disableIndicatorColor_ = params.disableIndicatorColor;
	disableButtonColor_ = params.disableButtonColor;
	disableButtonIconColor_ = params.disableButtonIconColor;
	disableOutlineColor_ = params.disableOutlineColor;
	lightBevelColor_ = params.lightBevelColor;
	darkBevelColor_ = params.darkBevelColor;
	curPart_ = noPart;
	
	RegisterMouseEvents();
}


GlowScrollBarWidget::~GlowScrollBarWidget()
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::~GlowScrollBarWidget");
	
	_timer->ForceRemove(this);
}


GlowWidget::AutoPackError GlowScrollBarWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnAutoPack");
	
	if (hSize != unspecifiedSize && hSize < 16)
	{
		return hAutoPackError;
	}
	if (vSize != unspecifiedSize && vSize < 16)
	{
		return vAutoPackError;
	}
	
	int hnew = Width();
	int vnew = Height();
	
	if (Width() > Height())
	{
		if (hOption != noReshape)
		{
			if (hSize != unspecifiedSize)
			{
				hnew = hSize;
			}
		}
		if (vOption == forcedSize)
		{
			vnew = vSize;
		}
		else if (vOption == preferredSize || vOption == expandPreferredSize)
		{
			vnew = 20;
		}
	}
	else
	{
		if (hOption == forcedSize)
		{
			hnew = hSize;
		}
		else if (hOption == preferredSize || hOption == expandPreferredSize)
		{
			hnew = 20;
		}
		if (vOption != noReshape)
		{
			if (vSize != unspecifiedSize)
			{
				vnew = vSize;
			}
		}
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowScrollBarWidget::DrawArrowButton_(
	Part part,
	float left,
	float right,
	float bevelWidth,
	float bevelHeight,
	float buttonWidth)
{
	// Quad
	if (!IsActive())
	{
		disableButtonColor_.Apply();
	}
	else if (curPart_ == part)
	{
		hiliteButtonColor_.Apply();
	}
	else
	{
		buttonColor_.Apply();
	}
	::glRectf(left, -1.0f, right, 1.0f);
	
	if (IsActive())
	{
		// Left/top bevels
		if (curPart_ == part)
		{
			darkBevelColor_.Apply();
		}
		else
		{
			lightBevelColor_.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(left, -1.0f);
		::glVertex2f(left+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(left, 1.0f);
		::glVertex2f(left+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(right, 1.0f);
		::glVertex2f(right-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		
		// Right/bottom bevels
		if (curPart_ == part)
		{
			lightBevelColor_.Apply();
		}
		else
		{
			darkBevelColor_.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(right, 1.0f);
		::glVertex2f(right-bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(right, -1.0f);
		::glVertex2f(right-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(left, -1.0f);
		::glVertex2f(left+bevelWidth, -1.0f+bevelHeight);
		::glEnd();
	}
	else
	{
		disableOutlineColor_.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(right, 1.0f);
		::glVertex2f(right, -1.0f);
		::glVertex2f(left, -1.0f);
		::glVertex2f(left, 1.0f);
		::glEnd();
	}
	
	// Arrow
	if (!IsActive())
	{
		disableButtonIconColor_.Apply();
	}
	else if (curPart_ == part)
	{
		hiliteButtonIconColor_.Apply();
	}
	else
	{
		buttonIconColor_.Apply();
	}
	if (part == upButtonPart)
	{
		::glBegin(GL_TRIANGLES);
		::glVertex2f(left+buttonWidth*0.3f, 0.0f);
		::glVertex2f(left+buttonWidth*0.6f, -0.6f);
		::glVertex2f(left+buttonWidth*0.6f, 0.6f);
		::glEnd();
	}
	else
	{
		::glBegin(GL_TRIANGLES);
		::glVertex2f(left+buttonWidth*0.7f, 0.0f);
		::glVertex2f(left+buttonWidth*0.4f, -0.6f);
		::glVertex2f(left+buttonWidth*0.4f, 0.6f);
		::glEnd();
	}
}


// Compute normalized coordinates for parts.
void GlowScrollBarWidget::ComputePartPositions_(
	float& pseudoWidth,
	float& pseudoHeight,
	int& arrowButtons,
	float& buttonWidth,
	float& stripLeft,
	float& stripRight,
	float& indicLeft,
	float& indicRight)
{
	if (Width() < Height())
	{
		pseudoWidth = Height();
		pseudoHeight = Width();
	}
	else
	{
		pseudoWidth = Width();
		pseudoHeight = Height();
	}
	
	arrowButtons = 0;
	if (pseudoWidth > pseudoHeight*8.0f)
	{
		arrowButtons = 2;
	}
	else if (pseudoWidth > pseudoHeight*4.0f)
	{
		arrowButtons = 1;
	}
	
	buttonWidth = pseudoHeight/pseudoWidth*2.0f;
	stripLeft = -1.0f+buttonWidth*arrowButtons;
	stripRight = 1.0f-buttonWidth*arrowButtons;
	
	indicLeft = stripLeft + (stripRight-stripLeft)/float(max_-min_)*float(topValue_-min_);
	float indicWidth = (stripRight-stripLeft)/float(max_-min_)*float(span_);
	if (indicWidth < buttonWidth)
	{
		indicWidth = buttonWidth;
		indicLeft = stripLeft + (stripRight-stripLeft-indicWidth)/float(max_-min_-span_)*float(topValue_-min_);
	}
	indicRight = indicLeft + indicWidth;
}


// Compute normalized position from a mouse position.
// left=-1, right=1, top=-1, bottom=1
// Returns true iff inside the widget
bool GlowScrollBarWidget::NormalizePosition_(
	int x,
	int y,
	float& pos)
{
	if (Width() > Height())
	{
		pos = float(x)*2.0f/float(Width())-1.0f;
		return (pos <= 1.0f && pos >= -1.0f && y <= Height() && y >= 0);
	}
	else
	{
		pos = float(y)*2.0f/float(Height())-1.0f;
		return (pos <= 1.0f && pos >= -1.0f && x <= Width() && x >= 0);
	}
}


// When dragging indicator, update control value according to mouse position.
// Return true iff value changed
bool GlowScrollBarWidget::UpdateDragValue_(
	int x,
	int y)
{
	// Find values
	float pseudoWidth = 0.0f;
	float pseudoHeight = 0.0f;
	int arrowButtons = 0;
	float buttonWidth = 0.0f;
	float stripLeft = 0.0f;
	float stripRight = 0.0f;
	float indicLeft = 0.0f;
	float indicRight = 0.0f;
	ComputePartPositions_(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);
	
	// Interpret position
	float pos = 0.0f;
	NormalizePosition_(x, y, pos);
	float value = float(pos-posOffset_-stripLeft)*float(max_-span_-min_)/
		float(stripRight-stripLeft-(indicRight-indicLeft));
	int newtop = min_ + static_cast<int>(GLOW_CSTD::floor(value+0.5f));
	if (newtop < min_) newtop = min_;
	if (newtop > max_-span_) newtop = max_-span_;
	if (newtop != topValue_)
	{
		topValue_ = newtop;
		return true;
	}
	return false;
}


// When dragging in button parts or page parts, determine if we're
// still in the part, and update inside_. Return true iff change.
bool GlowScrollBarWidget::UpdateInsideState_(
	int x,
	int y)
{
	// Find values
	float pseudoWidth = 0;
	float pseudoHeight = 0;
	int arrowButtons = 0;
	float buttonWidth = 0;
	float stripLeft = 0;
	float stripRight = 0;
	float indicLeft = 0;
	float indicRight = 0;
	ComputePartPositions_(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);
	
	// Interpret position
	float pos = 0;
	bool ninside = false;
	if (NormalizePosition_(x, y, pos))
	{
		savePosition_ = pos;
		if (curPart_ == upPagePart)
		{
			ninside = (pos >= stripLeft && pos <= indicLeft);
		}
		else if (curPart_ == downPagePart)
		{
			ninside = (pos >= indicRight && pos <= stripRight);
		}
		else if (curPart_ == upButtonPart && !innerButton_)
		{
			ninside = (pos >= -1.0f && pos <= buttonWidth-1.0f);
		}
		else if (curPart_ == downButtonPart && !innerButton_)
		{
			ninside = (pos >= 1.0f-buttonWidth && pos <= 1.0f);
		}
		else if (curPart_ == downButtonPart && innerButton_)
		{
			ninside = (pos >= buttonWidth-1.0f && pos <= buttonWidth*2.0f-1.0f);
		}
		else if (curPart_ == upButtonPart && innerButton_)
		{
			ninside = (pos >= 1.0f-buttonWidth*2.0f && pos <= 1.0f-buttonWidth);
		}
		else
		{
			ninside = false;
		}
	}
	
	// Check for changes
	if (ninside && !inside_)
	{
		inside_ = true;
		toggleTimer_ = 1;
		return true;
	}
	else if (!ninside && inside_)
	{
		inside_ = false;
		_timer->Unregister();
		return true;
	}
	return false;
}


void GlowScrollBarWidget::HandlePartPeriodical_(
	bool first)
{
	// Handle part stuff
	OnPart(curPart_, button_, modifiers_);
	Refresh();
	
	if ((curPart_ == upPagePart || curPart_ == downPagePart) && inside_)
	{
		// Check to make sure we're still in page region
		float pseudoWidth = 0;
		float pseudoHeight = 0;
		int arrowButtons = 0;
		float buttonWidth = 0;
		float stripLeft = 0;
		float stripRight = 0;
		float indicLeft = 0;
		float indicRight = 0;
		ComputePartPositions_(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
			stripLeft, stripRight, indicLeft, indicRight);
		
		if (curPart_ == upPagePart)
		{
			if (savePosition_ < stripLeft || savePosition_ > indicLeft)
			{
				inside_ = false;
				toggleTimer_ = 0;
			}
		}
		else //if (curPart_ == downPagePart)
		{
			if (savePosition_ < indicRight || savePosition_ > stripRight)
			{
				inside_ = false;
				toggleTimer_ = 0;
			}
		}
	}
	
	if (inside_)
	{
		if (first)
		{
			toggleTimer_ = firstDelay_;
		}
		else
		{
			toggleTimer_ = secondDelay_;
		}
	}
}


void GlowScrollBarWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetPaint");
	
	// Find values
	float pseudoWidth = 0;
	float pseudoHeight = 0;
	int arrowButtons = 0;
	float buttonWidth = 0;
	float stripLeft = 0;
	float stripRight = 0;
	float indicLeft = 0;
	float indicRight = 0;
	ComputePartPositions_(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);
	GLfloat bevelHeight = 4.0f/pseudoHeight;
	GLfloat bevelWidth = 4.0f/pseudoWidth;
	
	// Apply transform if upright scroll bar
	if (Width() < Height())
	{
		::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		::glScalef(1.0f, -1.0f, 1.0f);
	}
	
	// Draw strip back
	if (!IsActive())
	{
		disableStripColor_.Apply();
	}
	else if (curPart_ == upPagePart)
	{
		hiliteStripColor_.Apply();
	}
	else
	{
		stripColor_.Apply();
	}
	::glRectf(-1.0f, -1.0f, indicLeft, 1.0f);
	if (IsActive())
	{
		if (curPart_ == downPagePart)
		{
			hiliteStripColor_.Apply();
		}
		else
		{
			stripColor_.Apply();
		}
	}
	::glRectf(indicRight, -1.0f, 1.0f, 1.0f);
	if (!IsActive())
	{
		disableOutlineColor_.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, 1.0f);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(1.0f, -1.0f);
		::glVertex2f(1.0f, 1.0f);
		::glEnd();
	}
	
	// Draw shadow
	if (curPart_ == indicatorPart)
	{
		shadowColor_.Apply();
		::glRectf(savePosition_, -1.0f,
			savePosition_+indicRight-indicLeft, 1.0f);
		
		// Top/left bevels
		darkBevelColor_.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(savePosition_+indicRight-indicLeft, 1.0f);
		::glVertex2f(savePosition_+indicRight-indicLeft-bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(savePosition_, 1.0f);
		::glVertex2f(savePosition_+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(savePosition_, -1.0f);
		::glVertex2f(savePosition_+bevelWidth, -1.0f+bevelHeight);
		::glEnd();
		
		// Bottom/right bevel
		lightBevelColor_.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(savePosition_, -1.0f);
		::glVertex2f(savePosition_+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(savePosition_+indicRight-indicLeft, -1.0f);
		::glVertex2f(savePosition_+indicRight-indicLeft-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(savePosition_+indicRight-indicLeft, 1.0f);
		::glVertex2f(savePosition_+indicRight-indicLeft-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
		
	// Draw strip bevels
/*	if (IsActive())
	{
		// Top bevel
		darkBevelColor_.Apply();
		::glBegin(GL_QUADS);
		::glVertex2f(stripLeft-bevelWidth, 1.0f);
		::glVertex2f(stripLeft, 1.0-bevelHeight);
		::glVertex2f(stripRight, 1.0-bevelHeight);
		::glVertex2f(stripRight+bevelWidth, 1.0f);
		::glEnd();
		
		// Bottom bevel
		lightBevelColor_.Apply();
		::glBegin(GL_QUADS);
		::glVertex2f(stripLeft-bevelWidth, -1.0f);
		::glVertex2f(stripLeft, -1.0+bevelHeight);
		::glVertex2f(stripRight, -1.0+bevelHeight);
		::glVertex2f(stripRight+bevelWidth, -1.0f);
		::glEnd();
	}*/
	
	// Draw buttons
	if (arrowButtons != 0)
	{
		// Left up button
		DrawArrowButton_(upButtonPart, -1.0f, -1.0f+buttonWidth,
			bevelWidth, bevelHeight, buttonWidth);
		// Right down button
		DrawArrowButton_(downButtonPart, 1.0f-buttonWidth, 1.0f,
			bevelWidth, bevelHeight, buttonWidth);
	}
	if (arrowButtons == 2)
	{
		// Left down button
		DrawArrowButton_(downButtonPart, -1.0f+buttonWidth, -1.0f+buttonWidth*2.0f,
			bevelWidth, bevelHeight, buttonWidth);
		// Right up button
		DrawArrowButton_(upButtonPart, 1.0f-buttonWidth*2.0f, 1.0f-buttonWidth,
			bevelWidth, bevelHeight, buttonWidth);
	}
	
	// Draw indicator
	if (!IsActive())
	{
		disableIndicatorColor_.Apply();
	}
	else if (curPart_ == indicatorPart)
	{
		hiliteIndicatorColor_.Apply();
	}
	else
	{
		indicatorColor_.Apply();
	}
	::glRectf(indicLeft, -1.0f, indicRight, 1.0f);
		
	if (IsActive())
	{
		// Top bevel
		lightBevelColor_.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(indicRight, 1.0f);
		::glVertex2f(indicRight-bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(indicLeft, 1.0f);
		::glVertex2f(indicLeft+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(indicLeft, -1.0f);
		::glVertex2f(indicLeft+bevelWidth, -1.0f+bevelHeight);
		::glEnd();
		
		// Bottom bevel
		darkBevelColor_.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(indicLeft, -1.0f);
		::glVertex2f(indicLeft+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(indicRight, -1.0f);
		::glVertex2f(indicRight-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(indicRight, 1.0f);
		::glVertex2f(indicRight-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		
		// Ridges
		float ridgeTop = 1.0f-bevelHeight*2.0f;
		float ridgeBottom = -1.0f+bevelHeight*3.0f;
		float indicCenter = (indicRight+indicLeft)*0.5f;
		lightBevelColor_.Apply();
		::glBegin(GL_LINES);
		::glVertex2f(indicCenter-float(9)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter-float(9)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter-float(1)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter-float(1)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter+float(7)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter+float(7)/pseudoWidth, ridgeTop);
		::glEnd();
		darkBevelColor_.Apply();
		::glBegin(GL_LINES);
		::glVertex2f(indicCenter-float(7)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter-float(7)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter+float(1)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter+float(1)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter+float(9)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter+float(9)/pseudoWidth, ridgeTop);
		::glEnd();
	}
	else
	{
		disableOutlineColor_.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(indicLeft, 1.0f);
		::glVertex2f(indicLeft, -1.0f);
		::glVertex2f(indicRight, -1.0f);
		::glVertex2f(indicRight, 1.0f);
		::glEnd();
	}
	
	// If a timer start is scheduled, then start it
	if (toggleTimer_ > 0)
	{
		_timer->Register(toggleTimer_, this);
		toggleTimer_ = 0;
	}
}


void GlowScrollBarWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetMouseDown");
	
	if (curPart_ != noPart)
	{
		return;
	}
	
	// Find values
	float pseudoWidth = 0;
	float pseudoHeight = 0;
	int arrowButtons = 0;
	float buttonWidth = 0;
	float stripLeft = 0;
	float stripRight = 0;
	float indicLeft = 0;
	float indicRight = 0;
	ComputePartPositions_(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);

	// Interpret position
	float pos = 0;
	NormalizePosition_(x, y, pos);
	savePosition_ = pos;
	button_ = button;
	modifiers_ = modifiers;
	inside_ = true;
	if (arrowButtons > 0)
	{
		if (pos < buttonWidth-1.0f)
		{
			curPart_ = upButtonPart;
			innerButton_ = false;
			HandlePartPeriodical_(true);
		}
		else if (pos > 1.0f-buttonWidth)
		{
			curPart_ = downButtonPart;
			innerButton_ = false;
			HandlePartPeriodical_(true);
		}
		else if (arrowButtons == 2)
		{
			if (pos < buttonWidth*2.0f-1.0f)
			{
				curPart_ = downButtonPart;
				innerButton_ = true;
				HandlePartPeriodical_(true);
			}
			else if (pos > 1.0f-buttonWidth*2.0f)
			{
				curPart_ = upButtonPart;
				innerButton_ = true;
				HandlePartPeriodical_(true);
			}
		}
	}
	if (curPart_ == noPart)
	{
		if (pos < indicLeft)
		{
			curPart_ = upPagePart;
			HandlePartPeriodical_(true);
		}
		else if (pos > indicRight)
		{
			curPart_ = downPagePart;
			HandlePartPeriodical_(true);
		}
		else
		{
			curPart_ = indicatorPart;
			posOffset_ = pos-indicLeft;
			savePosition_ = indicLeft;
			Refresh();
		}
	}
}


void GlowScrollBarWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetMouseUp");
	
	Part savePart = curPart_;
	if (curPart_ != noPart)
	{
		if (curPart_ == indicatorPart)
		{
			UpdateDragValue_(x, y);
		}
		else
		{
			_timer->Unregister();
		}
		curPart_ = noPart;
		inside_ = false;
		OnReleased(savePart, button_, modifiers_);
		Refresh();
	}
}


void GlowScrollBarWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetMouseDrag");
	
	if (curPart_ != noPart)
	{
		if (curPart_ == indicatorPart)
		{
			// Dragging indicator
			if (UpdateDragValue_(x, y))
			{
				OnDragged(button_, modifiers_);
				Refresh();
			}
		}
		else
		{
			// Dragging button or page
			if (UpdateInsideState_(x, y))
			{
				Refresh();
			}
		}
	}
}


void GlowScrollBarWidget::OnDragged(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnDragged");
	
	GlowScrollBarMessage msg;
	msg.widget = this;
	msg.part = indicatorPart;
	msg.released = false;
	msg.topValue = topValue_;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	sender_.Send(msg);
}


void GlowScrollBarWidget::OnPart(
	Part part,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnPart");
	
	switch (part)
	{
		case upButtonPart:
			MoveTopValue(-arrowStep_);
			break;
		
		case downButtonPart:
			MoveTopValue(arrowStep_);
			break;
		
		case upPagePart:
			if (pageStep_ == spanPageStep)
			{
				MoveTopValue(-span_);
			}
			else
			{
				MoveTopValue(-pageStep_);
			}
			break;
		
		case downPagePart:
			if (pageStep_ == spanPageStep)
			{
				MoveTopValue(span_);
			}
			else
			{
				MoveTopValue(pageStep_);
			}
			break;
		
		default:
			return;
	}
	
	GlowScrollBarMessage msg;
	msg.widget = this;
	msg.part = part;
	msg.released = false;
	msg.topValue = topValue_;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	sender_.Send(msg);
}


void GlowScrollBarWidget::OnReleased(
	Part part,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnReleased");
	
	GlowScrollBarMessage msg;
	msg.widget = this;
	msg.part = part;
	msg.released = true;
	msg.topValue = topValue_;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	sender_.Send(msg);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

