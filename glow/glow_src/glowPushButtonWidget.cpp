/*
===============================================================================

	FILE:  glowPushButtonWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Pushbutton widget for GLOW
	
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

#ifndef GLOW_PUSHBUTTONWIDGET__H
	#include "glowPushButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowPushButtonParams GlowPushButtonParams::defaults(true);

GlowPushButtonParams::GlowPushButtonParams()
{
	*this = defaults;
}

GlowPushButtonParams::GlowPushButtonParams(bool) :
GlowWidgetParams(true),
upBoxColor(0.7f, 0.7f, 0.7f),
upTextColor(0.0f, 0.0f, 0.0f),
downBoxColor(0.5f, 0.5f, 0.5f),
downTextColor(0.8f, 0.8f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(1.0f, 0.7f, 0.7f),
disableUpBoxColor(0.7f, 0.7f, 0.7f),
disableDownBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	width = 100;
	height = 25;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	receiver = 0;
	down = false;
	behavior = GlowPushButtonWidget::normalBehavior;
}


/*
===============================================================================
	Methods of GlowPushButton
===============================================================================
*/

void GlowPushButtonWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowPushButtonParams& params)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	upBoxColor_ = params.upBoxColor;
	upTextColor_ = params.upTextColor;
	downBoxColor_ = params.downBoxColor;
	downTextColor_ = params.downTextColor;
	hiliteBoxColor_ = params.hiliteBoxColor;
	hiliteTextColor_ = params.hiliteTextColor;
	disableUpBoxColor_ = params.disableUpBoxColor;
	disableDownBoxColor_ = params.disableDownBoxColor;
	disableTextColor_ = params.disableTextColor;
	disableOutlineColor_ = params.disableOutlineColor;
	lightBevelColor_ = params.lightBevelColor;
	darkBevelColor_ = params.darkBevelColor;
	label_ = new char[GLOW_CSTD::strlen(params.text)+1];
	GLOW_CSTD::strcpy(label_, params.text);
	font_ = params.font;
	labelWidth_ = font_.StringWidth(label_);
	down_ = false;
	inside_ = false;
	state_ = params.down;
	behavior_ = params.behavior;
	if (params.receiver != 0)
	{
		sender_.Bind(params.receiver);
	}
	RegisterMouseEvents();
}


GlowPushButtonWidget::~GlowPushButtonWidget()
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::~GlowPushButtonWidget");
	
	delete[] label_;
}


void GlowPushButtonWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::SetText");
	
	delete[] label_;
	label_ = new char[GLOW_CSTD::strlen(text)+1];
	GLOW_CSTD::strcpy(label_, text);
	labelWidth_ = font_.StringWidth(label_);
	Refresh();
}


GlowWidget::AutoPackError GlowPushButtonWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnAutoPack");
	
	int hnew = Width();
	int preferred = labelWidth_ + 20;
	if (hSize != unspecifiedSize && hSize < preferred-10)
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
	preferred = font_.Leading() + 12;
	if (vSize != unspecifiedSize && vSize < preferred-4)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == preferredSize || vOption == expandPreferredSize)
	{
		vnew = preferred;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowPushButtonWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetPaint");
	
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
	
	// Text label
	if (IsActive())
	{
		if (down_ && inside_)
		{
			hiliteTextColor_.Apply();
		}
		else if (state_)
		{
			downTextColor_.Apply();
		}
		else
		{
			upTextColor_.Apply();
		}
	}
	else
	{
		disableTextColor_.Apply();
	}
	::glRasterPos2f(-float(labelWidth_)/float(Width()),
		float(font_.Leading()-font_.BaselinePos()*2)/float(Height()));
	int labellen = GLOW_CSTD::strlen(label_);
	for (int i=0; i<labellen; i++)
	{
		::glutBitmapCharacter(font_, label_[i]);
	}
}


void GlowPushButtonWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetMouseDown");
	
	down_ = true;
	inside_ = true;
	button_ = button;
	modifiers_ = modifiers;
	Refresh();
}


void GlowPushButtonWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetMouseUp");
	
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


void GlowPushButtonWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetMouseDrag");
	
	bool inside = (x>=0 && y>=0 && x<=Width() && y<=Height());
	if (inside == !inside_)
	{
		Refresh();
	}
	inside_ = inside;
}


void GlowPushButtonWidget::ToggleState()
{
	if (behavior_ == toggleBehavior)
	{
		state_ = !state_;
	}
	else if (behavior_ == stickDownBehavior)
	{
		state_ = true;
	}
}


void GlowPushButtonWidget::OnHit(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnHit");
	
	ToggleState();
	GlowPushButtonMessage msg;
	msg.widget = this;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	sender_.Send(msg);
}


/*
===============================================================================
	Methods of GlowDismissPushButtonWidget
===============================================================================
*/

void GlowDismissPushButtonWidget::OnHit(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowDismissPushButtonWidget::OnHit");
	
	GlowPushButtonWidget::OnHit(button, modifiers);
	if (todismiss_)
	{
		todismiss_->Close();
	}
}


/*
===============================================================================
	Methods of GlowWidgetMapToPushButtonFilter
===============================================================================
*/

bool GlowWidgetMapToPushButtonFilter::OnFilter(
	GlowWidgetKeyboardData& data)
{
	GLOW_DEBUGSCOPE("GlowWidgetMapToPushButtonFilter::OnFilter");
	
	if (widget_ != 0 && keyCode_ == data.key &&
		(modifiers_ & data.modifiers) == modifiers_)
	{
		widget_->Hit();
		return false;
	}
	return true;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

