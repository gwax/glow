/*
===============================================================================

	FILE:  glowCheckBoxWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Check box widget for GLOW
	
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

#ifndef GLOW_CHECKBOXWIDGET__H
	#include "glowCheckBoxWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowCheckBoxParams GlowCheckBoxParams::defaults(true);

GlowCheckBoxParams::GlowCheckBoxParams()
{
	*this = defaults;
}

GlowCheckBoxParams::GlowCheckBoxParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
checkColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.4f, 0.4f, 0.4f),
hiliteTextColor(0.8f, 0.0f, 0.0f),
hiliteCheckColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableCheckColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	width = 100;
	height = 16;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	spacing = 5;
	state = GlowCheckBoxWidget::off;
	behavior = GlowCheckBoxWidget::defaultBehavior;
	receiver = 0;
}


/*
===============================================================================
	Methods of GlowCheckBoxWidget
===============================================================================
*/

void GlowCheckBoxWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowCheckBoxParams& params)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	label_ = new char[GLOW_CSTD::strlen(params.text)+1];
	GLOW_CSTD::strcpy(label_, params.text);
	font_ = params.font;
	labelWidth_ = font_.StringWidth(label_);
	spacing_ = params.spacing;
	state_ = params.state;
	behavior_ = params.behavior;
	down_ = false;
	inside_ = false;
	boxColor_ = params.boxColor;
	textColor_ = params.textColor;
	checkColor_ = params.checkColor;
	hiliteBoxColor_ = params.hiliteBoxColor;
	hiliteTextColor_ = params.hiliteTextColor;
	hiliteCheckColor_ = params.hiliteCheckColor;
	disableBoxColor_ = params.disableBoxColor;
	disableTextColor_ = params.disableTextColor;
	disableCheckColor_ = params.disableCheckColor;
	disableOutlineColor_ = params.disableOutlineColor;
	lightBevelColor_ = params.lightBevelColor;
	darkBevelColor_ = params.darkBevelColor;
	if (params.receiver != 0)
	{
		sender_.Bind(params.receiver);
	}
	RegisterMouseEvents();
}


GlowCheckBoxWidget::~GlowCheckBoxWidget()
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::~GlowCheckBoxWidget");
	
	delete[] label_;
}


void GlowCheckBoxWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::SetText");
	
	delete[] label_;
	label_ = new char[GLOW_CSTD::strlen(text)+1];
	GLOW_CSTD::strcpy(label_, text);
	labelWidth_ = font_.StringWidth(label_);
	Refresh();
}


GlowWidget::AutoPackError GlowCheckBoxWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnAutoPack");
	
	int vnew = Height();
	int preferred = font_.Leading() + 1;
	if (vSize != unspecifiedSize && vSize < preferred)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == expandPreferredSize || vOption == preferredSize)
	{
		vnew = preferred;
	}
	
	int hnew = Width();
	preferred = labelWidth_+spacing_+vnew+1;
	if (hSize != unspecifiedSize && hSize < preferred)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize || hOption == expandPreferredSize)
	{
		hnew = preferred;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowCheckBoxWidget::CropWidth()
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::CropWidth");
	
	int width = labelWidth_ + spacing_ + Height() + 1;
	if (width < Width())
	{
		Reshape(width, Height());
	}
}


void GlowCheckBoxWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetPaint");
	
	float boxRight = float(Height())/float(Width())*2.0f-1.0f;
	float bevelHeight = 4.0f/float(Height());
	float bevelWidth = 4.0f/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	
	// Draw box
	if (IsActive())
	{
		if (down_ && inside_)
		{
			hiliteBoxColor_.Apply();
		}
		else
		{
			boxColor_.Apply();
		}
	}
	else
	{
		disableBoxColor_.Apply();
	}
	::glRectf(-1.0f, -1.0f, boxRight, 1.0f);
	
	// Draw bevels
	if (IsActive())
	{
		if (down_ && inside_)
		{
			darkBevelColor_.Apply();
		}
		else
		{
			lightBevelColor_.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(-1.0f, 1.0f);
		::glVertex2f(-1.0f+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(boxRight, 1.0f);
		::glVertex2f(boxRight-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		if (down_ && inside_)
		{
			lightBevelColor_.Apply();
		}
		else
		{
			darkBevelColor_.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(boxRight, -1.0f);
		::glVertex2f(boxRight-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(boxRight, 1.0f);
		::glVertex2f(boxRight-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
	else
	{
		disableOutlineColor_.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f, 1.0f-etchHeight);
		::glVertex2f(boxRight-etchWidth, 1.0f-etchHeight);
		::glVertex2f(boxRight-etchWidth, -1.0f);
		::glEnd();
	}
	
	// Draw mark
	if (state_ != off)
	{
		if (IsActive())
		{
			if (down_ && inside_)
			{
				hiliteCheckColor_.Apply();
			}
			else
			{
				checkColor_.Apply();
			}
		}
		else
		{
			disableCheckColor_.Apply();
		}
		
		::glBegin(GL_QUADS);
		float hscale = (boxRight+1.0f)/2.0f;
		if (state_ == on)
		{
			const float markOffset = 0.1f;
			
			::glVertex2f((0.5f-markOffset)*hscale-1.0f, -0.5f+markOffset);
			::glVertex2f((0.5f+markOffset)*hscale-1.0f, -0.5f-markOffset);
			::glVertex2f((1.5f+markOffset)*hscale-1.0f, 0.5f-markOffset);
			::glVertex2f((1.5f-markOffset)*hscale-1.0f, 0.5f+markOffset);
			
			::glVertex2f((0.5f+markOffset)*hscale-1.0f, 0.5f+markOffset);
			::glVertex2f((0.5f-markOffset)*hscale-1.0f, 0.5f-markOffset);
			::glVertex2f((1.5f-markOffset)*hscale-1.0f, -0.5f-markOffset);
			::glVertex2f((1.5f+markOffset)*hscale-1.0f, -0.5f+markOffset);
		}
		else //if (state_ == half)
		{
			const float markOffset = 0.2;
			
			::glVertex2f(0.5f*hscale-1.0f, markOffset);
			::glVertex2f(0.5f*hscale-1.0f, -markOffset);
			::glVertex2f(1.5f*hscale-1.0f, -markOffset);
			::glVertex2f(1.5f*hscale-1.0f, markOffset);
		}
		::glEnd();
	}
	
	// Text label
	if (IsActive())
	{
		if (down_ && inside_)
		{
			hiliteTextColor_.Apply();
		}
		else
		{
			textColor_.Apply();
		}
	}
	else
	{
		disableTextColor_.Apply();
	}
	::glRasterPos2f(boxRight+float(spacing_*2)/float(Width()),
		float(font_.Leading()-font_.BaselinePos()*2)/float(Height()));
	int labellen = GLOW_CSTD::strlen(label_);
	for (int i=0; i<labellen; i++)
	{
		::glutBitmapCharacter(font_, label_[i]);
	}
}


void GlowCheckBoxWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetMouseDown");
	
	down_ = true;
	inside_ = true;
	button_ = button;
	modifiers_ = modifiers;
	Refresh();
}


void GlowCheckBoxWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetMouseUp");
	
	if (down_)
	{
		down_ = false;
		Refresh();
		if (inside_)
		{
			OnHit(button_, modifiers_);
		}
	}
}


void GlowCheckBoxWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetMouseDrag");
	
	bool inside = (x>=0 && y>=0 && x<=Width() && y<=Height());
	if (inside == !inside_)
	{
		Refresh();
	}
	inside_ = inside;
}


void GlowCheckBoxWidget::ToggleState()
{
	if (state_ == off)
	{
		state_ = ((behavior_ & halfFollowsOff) != 0) ? half : on;
	}
	else if (state_ == on)
	{
		state_ = ((behavior_ & halfFollowsOn) != 0) ? half : off;
	}
	else //if (state_ == half)
	{
		state_ = ((behavior_ & offFollowsHalf) != 0) ? off : on;
	}
}


void GlowCheckBoxWidget::OnHit(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnHit");
	
	ToggleState();
	GlowCheckBoxMessage msg;
	msg.widget = this;
	msg.state = state_;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	sender_.Send(msg);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

