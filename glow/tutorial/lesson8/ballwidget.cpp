/*
===============================================================================

	FILE:  mechcontrols.h
	
	PROJECT:
	
		GLOW widgets for glutmech demo
	
	CONTENTS:
	
		Ball widget
	
	PROGRAMMERS:
	
		Daniel Azuma <dazuma@kagi.com>
	
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
		License along with this library; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit tutorial -- version 0.9.8  (23 May 2000)
	
	CHANGE HISTORY:
	
		12 June 2000 -- DA -- Initial CVS checkin

===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#include "glowViewTransform.h"

GLOW_NAMESPACE_USING

#include "ballwidget.h"


/*
===============================================================================
	Component that draws a wire sphere
===============================================================================
*/

class DrawWireSphere :
	public GlowComponent
{
	public:
	
		DrawWireSphere(
			GlowComponent* parent) :
		GlowComponent(parent) {}
	
	protected:
	
		virtual void OnEndPaint();
};


void DrawWireSphere::OnEndPaint()
{
//	GLint vals[4];
	::glEnable(GL_CULL_FACE);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	::glGetIntegerv(GL_VIEWPORT, vals);
//	::glViewport(vals[0]+3, vals[1]+3, vals[2]-6, vals[3]-6);
	::glColor3ub(0, 255, 255);
	::glutSolidSphere(1.0, 12, 6);
//	::glViewport(vals[0], vals[1], vals[2], vals[3]);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	::glDisable(GL_CULL_FACE);
}


/*
===============================================================================
	Methods of BallWidget
===============================================================================
*/

BallWidget::BallWidget(
	GlowWidget* parent,
	int x,
	int y,
	int width)
{
	GLOW_DEBUGSCOPE("BallWidget::BallWidget");
	
	// Initialize widget
	GlowWidgetParams params;
	params.x = x;
	params.y = y;
	params.width = params.height = width;
	GlowWidget::Init(parent->Root(), parent, params);
	
	// Make view manipulator and drawing component
	_manip = new GlowViewManipulator(this, GlowViewManipulatorParams::defaults);
	new DrawWireSphere(_manip);
	
	// Register to receive mouse events
	RegisterMouseEvents();
}


void BallWidget::OnWidgetPaint()
{
	::glColor3ub(0, 0, 0);
	::glBegin(GL_QUADS);
	::glVertex2f(-1.0f, -1.0f);
	::glVertex2f(1.0f, -1.0f);
	::glVertex2f(1.0f, 1.0f);
	::glVertex2f(-1.0f, 1.0f);
	::glEnd();
}


void BallWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnWidgetMouseDown");
	
	if (!_manip->IsDragging())
	{
		GLfloat xn;
		GLfloat yn;
		NormalizeCoordinates(x, y, xn, yn);
		_manip->BeginRotationDrag(xn, yn);
		_button = button;
		_modifiers = modifiers;
	}
}


void BallWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnWidgetMouseUp");
	
	if (_manip->IsDragging())
	{
		GLfloat xn;
		GLfloat yn;
		NormalizeCoordinates(x, y, xn, yn);
		_manip->EndDrag(xn, yn);
		OnReleased(_button, _modifiers);
	}
}


void BallWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("BallWidget::OnWidgetMouseDrag");
	
	if (_manip->IsDragging())
	{
		GLfloat xn;
		GLfloat yn;
		NormalizeCoordinates(x, y, xn, yn);
		_manip->InDrag(xn, yn);
		OnDragged(_button, _modifiers);
	}
}


void BallWidget::OnDragged(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnDragged");
	
	BallMessage message;
	message.widget = this;
	message.rotation = GetRotation();
	message.released = false;
	message.mouseButton = button;
	message.modifiers = modifiers;
	_sender.Send(message);
}


void BallWidget::OnReleased(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnReleased");
	
	BallMessage message;
	message.widget = this;
	message.rotation = GetRotation();
	message.released = true;
	message.mouseButton = button;
	message.modifiers = modifiers;
	_sender.Send(message);
}


GlowWidget::AutoPackError BallWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("BallWidget::OnAutoPack");
	
	int hnew = Width();
	if ((hOption == preferredSize || hOption == expandPreferredSize) &&
		hSize != unspecifiedSize && hnew > hSize)
	{
		return hAutoPackError;
	}
	else if (hOption == forcedSize)
	{
		if (hSize < 20)
		{
			return hAutoPackError;
		}
		hnew = hSize;
	}
	
	int vnew = Height();
	if ((vOption == preferredSize || vOption == expandPreferredSize) &&
		vSize != unspecifiedSize && vnew > vSize)
	{
		return vAutoPackError;
	}
	else if (vOption == forcedSize)
	{
		if (vSize < 20)
		{
			return vAutoPackError;
		}
		vnew = vSize;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


/*
===============================================================================
*/

