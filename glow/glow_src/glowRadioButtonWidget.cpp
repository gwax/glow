/*
===============================================================================

	FILE:  glowRadioButtonWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Radio button widget for GLOW
	
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

#ifndef GLOW_RADIOBUTTONWIDGET__H
	#include "glowRadioButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowRadioButtonParams GlowRadioButtonParams::defaults(true);

GlowRadioButtonParams::GlowRadioButtonParams()
{
	*this = defaults;
}

GlowRadioButtonParams::GlowRadioButtonParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
spotColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(0.8f, 0.0f, 0.0f),
hiliteSpotColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableSpotColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	width = 100;
	height = 16;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	spacing = 5;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowRadioGroupParams GlowRadioGroupParams::defaults(true);

GlowRadioGroupParams::GlowRadioGroupParams()
{
	*this = defaults;
}

GlowRadioGroupParams::GlowRadioGroupParams(bool) :
GlowPanelParams(true)
{
	style = GlowRadioGroupWidget::transparentStyle;
	clipping = false;
	receiver = 0;
}


/*
===============================================================================
	Methods of GlowRadioButton
===============================================================================
*/

void GlowRadioButtonWidget::Init(
	GlowRadioGroupWidget* parent,
	const GlowRadioButtonParams& params)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::Init");
	
	GlowWidget::Init(parent->Root(), parent, params);
	group_ = parent;
	label_ = new char[GLOW_CSTD::strlen(params.text)+1];
	GLOW_CSTD::strcpy(label_, params.text);
	font_ = params.font;
	labelWidth_ = font_.StringWidth(label_);
	spacing_ = params.spacing;
	down_ = false;
	inside_ = false;
	boxColor_ = params.boxColor;
	textColor_ = params.textColor;
	spotColor_ = params.spotColor;
	hiliteBoxColor_ = params.hiliteBoxColor;
	hiliteTextColor_ = params.hiliteTextColor;
	hiliteSpotColor_ = params.hiliteSpotColor;
	disableBoxColor_ = params.disableBoxColor;
	disableTextColor_ = params.disableTextColor;
	disableSpotColor_ = params.disableSpotColor;
	disableOutlineColor_ = params.disableOutlineColor;
	lightBevelColor_ = params.lightBevelColor;
	darkBevelColor_ = params.darkBevelColor;
	state_ = group_->Register_(this);
	
	RegisterMouseEvents();
}


GlowRadioButtonWidget::~GlowRadioButtonWidget()
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::~GlowRadioButtonWidget");
	
	group_->UnRegister_(this);
	delete[] label_;
}


void GlowRadioButtonWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::SetText");
	
	delete[] label_;
	label_ = new char[GLOW_CSTD::strlen(text)+1];
	GLOW_CSTD::strcpy(label_, text);
	labelWidth_ = font_.StringWidth(label_);
	Refresh();
}


GlowWidget::AutoPackError GlowRadioButtonWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnAutoPack");
	
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


void GlowRadioButtonWidget::CropWidth()
{
	int width = labelWidth_ + spacing_ + Height() + 1;
	if (width < Width())
	{
		Reshape(width, Height());
	}
}


void GlowRadioButtonWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetPaint");
	
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
	if (state_)
	{
		if (IsActive())
		{
			if (down_ && inside_)
			{
				hiliteSpotColor_.Apply();
			}
			else
			{
				spotColor_.Apply();
			}
		}
		else
		{
			disableSpotColor_.Apply();
		}
		float spotInset = float(Height())/float(Width())*0.5f;
		::glRectf(-1.0f+spotInset, -0.5f, boxRight-spotInset, 0.5f);
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


void GlowRadioButtonWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetMouseDown");
	
	down_ = true;
	inside_ = true;
	button_ = button;
	modifiers_ = modifiers;
	Refresh();
}


void GlowRadioButtonWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetMouseUp");
	
	if (down_)
	{
		down_ = false;
		Refresh();
		if (inside_)
		{
			group_->OnHit(this, button_, modifiers_);
		}
	}
}


void GlowRadioButtonWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetMouseDrag");
	
	bool inside = (x>=0 && y>=0 && x<=Width() && y<=Height());
	if (inside == !inside_)
	{
		Refresh();
	}
	inside_ = inside;
}


/*
===============================================================================
	Methods of GlowRadioGroupWidget
===============================================================================
*/

void GlowRadioGroupWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowRadioGroupParams& params)
{
	GLOW_DEBUGSCOPE("GlowRadioGroupWidget::Init");
	
	GlowPanelWidget::Init(root, parent, params);
	state_ = 0;
	if (params.receiver != 0)
	{
		sender_.Bind(params.receiver);
	}
}


void GlowRadioGroupWidget::OnHit(
	GlowRadioButtonWidget* widget,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowRadioGroupWidget::OnHit");
	
	if (widget != state_)
	{
		GlowRadioButtonMessage message;
		message.groupWidget = this;
		message.buttonWidget = widget;
		message.oldButtonWidget = state_;
		message.mouseButton = mouseButton;
		message.modifiers = modifiers;
		state_ = widget;
		message.oldButtonWidget->SetState(false);
		message.buttonWidget->SetState(true);
		sender_.Send(message);
	}
}


void GlowRadioGroupWidget::UnRegister_(
	GlowRadioButtonWidget* button)
{
	if (state_ == button)
	{
		for (GlowComponent* child = FirstChild(); true; child = child->Next())
		{
			if (child == 0)
			{
				state_ = 0;
				break;
			}
			if (child != button)
			{
				GlowRadioButtonWidget* newbutton =
					dynamic_cast<GlowRadioButtonWidget*>(child);
				if (newbutton != 0)
				{
					state_ = newbutton;
					state_->SetState(true);
					break;
				}
			}
		}
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

