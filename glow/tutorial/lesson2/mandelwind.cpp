/*
===============================================================================

	FILE:  mandelwind.cpp
	
	PROJECT:
	
		GLOW-based mandelbrot viewer
	
	CONTENTS:
	
		Viewer window
	
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
	
		The GLOW Toolkit tutorial -- version 1.1.1dev  (24 July 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*
===============================================================================
	Headers
===============================================================================
*/

#include <iostream>
#include <cmath>

using namespace std;

#include "mandelwind.h"
#include "mandeldata.h"


/*
===============================================================================
	Constants (the current state of user dragging)
===============================================================================
*/

const int NO_DRAG = 0;
const int ZOOM_IN_DRAG = 1;
const int ZOOM_OUT_DRAG = 2;


/*
===============================================================================
	Methods of MandelWind
===============================================================================
*/

// Constructor

MandelWind::MandelWind(
	MandelData* data) :
GlowWindow("Mandelglow", GlowWindow::autoPosition, GlowWindow::autoPosition,
	data->Width(), data->Height(), Glow::rgbBuffer | Glow::doubleBuffer,
	Glow::mouseEvents | Glow::dragEvents)
{
	// Get data
	data_ = data;
	image_ = 0;
	dragType_ = NO_DRAG;
	
	// We've moved the image recomputation into OnEndPaint()
	// because it will need to be recomputed again whenever the user
	// resizes the window or zooms in or out.
}


// Destructor

MandelWind::~MandelWind()
{
	delete data_;
	delete image_;
}


// Redraw method

void MandelWind::OnEndPaint()
{
	// Recompute if necessary
	if (!data_->IsDataValid())
	{
		data_->Recalc();
		
		// Make image
		delete[] image_;
		image_ = new unsigned char[data_->Width()*data_->Height()*4];
		const int* rawimage = data_->Data();
		int mx = data_->Width()*data_->Height();
		for (int i=0; i<mx; ++i)
		{
			if (rawimage[i] == 0)
			{
				// Black color
				image_[i*4] = 0;
				image_[i*4+1] = 0;
				image_[i*4+2] = 0;
				image_[i*4+3] = 0;
			}
			else
			{
				// Red color
				image_[i*4] = 255;
				image_[i*4+1] = 0;
				image_[i*4+2] = 0;
				image_[i*4+3] = 0;
			}
		}
	}
	
	// Draw image
	::glDisable(GL_LIGHTING);
	::glDisable(GL_DEPTH_TEST);
	::glRasterPos2f(-1.0f, -1.0f);
	::glDrawPixels(data_->Width(), data_->Height(), GL_RGBA, GL_UNSIGNED_BYTE, image_);
	
	// Draw drag rectangle (lesson 2)
	if ((dragType_ == ZOOM_IN_DRAG || dragType_ == ZOOM_OUT_DRAG) &&
		factor_ != 0)
	{
		GLfloat xcenter, ycenter;
		NormalizeCoordinates(xdown_, ydown_, xcenter, ycenter);
		::glColor3f(1.0f, 1.0f, 1.0f);
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(xcenter-factor_, ycenter-factor_);
		::glVertex2f(xcenter+factor_, ycenter-factor_);
		::glVertex2f(xcenter+factor_, ycenter+factor_);
		::glVertex2f(xcenter-factor_, ycenter+factor_);
		::glEnd();
	}
}


// Respond to reshape events
// (New with lesson 2)

void MandelWind::OnReshape(
	int width,
	int height)
{
	// Update the viewport to specify the entire window
	::glViewport(0, 0, width, height);
	halfdiagonal_ = sqrt(double(width*width+height*height))*0.5;
	
	// Update the mandel data
	data_->SetSize(width, height);
	
	// We don't need to explicitly refresh here, because a refresh
	// event will be generated anyway whenever the window is resized.
}


// Respond to mouse down events
// (New with lesson 2)

void MandelWind::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	// Ignore mousedowns if we're already dragging
	if (dragType_ == NO_DRAG)
	{
		// Zoom with left mouse button
		if (button == Glow::leftButton)
		{
			// Zoom out if shift key is down
			// Set up variables and move into zoom-out state
			if (modifiers & Glow::shiftModifier)
			{
				xdown_ = Width()/2;
				ydown_ = Height()/2;
				dragType_ = ZOOM_OUT_DRAG;
			}
			else
			// Zoom in if shift key isn't down
			// Set up variables and move into zoom-in state
			{
				xdown_ = x;
				ydown_ = y;
				dragType_ = ZOOM_IN_DRAG;
			}
			ComputeZoomFactor_(x, y);
			// Explicitly refresh so the zoom rectangle can be drawn
			Refresh();
		}
	}
}


// Respond to mouse up events
// (New with lesson 2)

void MandelWind::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	if (dragType_ == ZOOM_IN_DRAG)
	{
		// For zooming "in"
		ComputeZoomFactor_(x, y);
		if (factor_ != 0)
		{
			// Alter the data object for new position and scale
			data_->MoveCenter(
				(xdown_-Width()/2)*data_->GetPixelWidth(),
				(Height()/2-ydown_)*data_->GetPixelWidth());
			data_->ScalePixelWidth(factor_);
		}
		// Now in "no-dragging" state
		dragType_ = NO_DRAG;
		// Explicitly refresh so the image can be updated
		Refresh();
	}
	else if (dragType_ == ZOOM_OUT_DRAG)
	{
		// For zooming "out"
		ComputeZoomFactor_(x, y);
		if (factor_ != 0)
		{
			// Alter the data object for new scale
			data_->ScalePixelWidth(1.0/factor_);
		}
		// Now in "no-dragging" state
		dragType_ = NO_DRAG;
		// Explicitly refresh so the image can be updated
		Refresh();
	}
}


// Respond to mouse drag events
// (New with lesson 2)

void MandelWind::OnMouseDrag(
	int x,
	int y)
{
	if (dragType_ == ZOOM_IN_DRAG || dragType_ == ZOOM_OUT_DRAG)
	{
		// Determine new zoom factor from mouse position
		ComputeZoomFactor_(x, y);
		// Explicitly refresh so the zoom rectangle can be redrawn
		// in its new position
		Refresh();
	}
}


// Compute current zooming factor
// (New with lesson 2)

void MandelWind::ComputeZoomFactor_(
	int x,
	int y)
{
	// If the mouse is outside the window, zoom is being cancelled.
	if (x<0 || x>=Width() || y<0 || y>=Height())
	{
		factor_ = 0;
	}
	else
	{
		// Figure out zoom factor
		factor_ = sqrt(double((xdown_-x)*(xdown_-x)+
			(ydown_-y)*(ydown_-y)))/halfdiagonal_;
	}
}
