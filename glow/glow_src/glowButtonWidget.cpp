/*
===============================================================================

	FILE:  glowButtonWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Generalized button widget for GLOW
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
	COPYRIGHT:
	
		Copyright (C) 2000  Daniel Azuma  (dazuma@kagi.com)
		
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

#ifndef GLOW_BUTTONWIDGET__H
	#include "glowButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowButtonParams GlowButtonParams::defaults(true);

GlowButtonParams::GlowButtonParams()
{
	*this = defaults;
}

GlowButtonParams::GlowButtonParams(bool) :
GlowWidgetParams(true),
upBoxColor(0.7f, 0.7f, 0.7f),
downBoxColor(0.5f, 0.5f, 0.5f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
disableUpBoxColor(0.7f, 0.7f, 0.7f),
disableDownBoxColor(0.7f, 0.7f, 0.7f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	width = 100;
	height = 25;
	down = false;
}


/*
===============================================================================
	Methods of GlowButtonWidget
===============================================================================
*/

void GlowButtonWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowButtonParams& params)
{
	GLOW_DEBUGSCOPE("GlowButtonWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	upBoxColor_ = params.upBoxColor;
	downBoxColor_ = params.downBoxColor;
	hiliteBoxColor_ = params.hiliteBoxColor;
	disableUpBoxColor_ = params.disableUpBoxColor;
	disableDownBoxColor_ = params.disableDownBoxColor;
	disableOutlineColor_ = params.disableOutlineColor;
	lightBevelColor_ = params.lightBevelColor;
	darkBevelColor_ = params.darkBevelColor;
	down_ = false;
	inside_ = false;
	state_ = params.down;
	RegisterMouseEvents();
}


void GlowButtonWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowButtonWidget::OnWidgetPaint");
	
	float bevelHeight = float(4)/float(Height());
	float bevelWidth = float(4)/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	
	// Box
	if (down_ && inside_)
	{
		hiliteBoxColor_.Apply();
	}
	else if (!IsActive())
	{
		if (state_)
		{
			disableDownBoxColor_.Apply();
		}
		else
		{
			disableUpBoxColor_.Apply();
		}
	}
	else
	{
		if (state_)
		{
			downBoxColor_.Apply();
		}
		else
		{
			upBoxColor_.Apply();
		}
	}
	::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	
	// Bevels
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
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
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
		::glVertex2f(1.0f, -1.0f);
		::glVertex2f(1.0f-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
	else
	{
		disableOutlineColor_.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, -1.0f);
		::glEnd();
	}
}


void GlowButtonWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowButtonWidget::OnWidgetMouseDown");
	
	if (!down_)
	{
		down_ = true;
		inside_ = true;
		button_ = button;
		modifiers_ = modifiers;
		OnEvent(pressEvent, button, modifiers);
		Refresh();
	}
}


void GlowButtonWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowButtonWidget::OnWidgetMouseUp");
	
	if (down_)
	{
		down_ = false;
		OnEvent(inside_ ? releaseInsideEvent : releaseOutsideEvent,
			button_, modifiers_);
		Refresh();
	}
}


void GlowButtonWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowButtonWidget::OnWidgetMouseDrag");
	
	if (down_)
	{
		bool inside = (x>=0 && y>=0 && x<=Width() && y<=Height());
		if (inside == !inside_)
		{
			OnEvent(inside ? enterEvent : exitEvent, button_, modifiers_);
			Refresh();
		}
		inside_ = inside;
	}
}


void GlowButtonWidget::OnEvent(
	GlowButtonWidget::Event event,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

