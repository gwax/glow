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
	Constants
===============================================================================
*/

// Drag status
const int NO_DRAG = 0;
const int ZOOM_IN_DRAG = 1;
const int ZOOM_OUT_DRAG = 2;

// Identifiers for menu items. These constants are reported to the
// menu receiver so it can identify which item was selected
// (new for lesson 3)
const int RESETZOOM_ITEM = 0;
const int QUIT_ITEM = 1;
const int RED_ITEM = 10;
const int GREEN_ITEM = 11;
const int BLUE_ITEM = 12;
const int MULTI_ITEM = 13;


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
	Glow::mouseEvents | Glow::dragEvents | Glow::menuEvents)
{
	// Get data
	data->SetThreshhold(1500);
	data_ = data;
	image_ = 0;
	dragType_ = NO_DRAG;
	r_ = 255;
	g_ = 0;
	b_ = 0;
	imageValid_ = false;
	
	// The following is new for lesson 3...
	
	// Create new menu
	GlowMenu* menu = new GlowMenu;
	
	// Add entries to the menu. Give each item an identifying constant
	menu->AddEntry("Reset zoom", RESETZOOM_ITEM);
	menu->AddEntry("Quit", QUIT_ITEM);
	
	// Create a submenu
	GlowMenu* colorMenu = new GlowMenu;
	colorMenu->AddEntry("Red", RED_ITEM);
	colorMenu->AddEntry("Green", GREEN_ITEM);
	colorMenu->AddEntry("Blue", BLUE_ITEM);
	colorMenu->AddEntry("Multi", MULTI_ITEM);
	
	// Attach this submenu to the first menu
	menu->AddSubmenu("Color", colorMenu);
	
	// Mark the first item in the submenu
	colorMenu->SetItemMark(0, "=> ");
	
	// Bind the menu to the right mouse button on our window
	SetMenu(Glow::rightButton, menu);
	
	// Set both menus to notify our receiver when it gets a hit
	menu->Notifier().Bind(this);
	colorMenu->Notifier().Bind(this);
}


// Destructor

MandelWind::~MandelWind()
{
	delete data_;
	delete image_;
}


// Menu receiver method. This method is called when the MandelWind
// receives notification that a menu item was selected.
// (New with lesson 3)

void MandelWind::OnMessage(
	const GlowMenuMessage& message)
{
	// Respond based on the identifier given in the message
	switch (message.code)
	{
		case RESETZOOM_ITEM:
			// Reset the zoom to the starting point
			ResetZoom();
			break;
		
		case QUIT_ITEM:
			// Exit the program
			exit(0);
			break;
		
		case RED_ITEM:
			// Set the background color to red
			SetColor(255, 0, 0);
			// Mark the red item, and unmark the other two items
			message.menu->UnmarkAllItems();
			message.menu->SetItemMark(0, "=> ");
			break;
		
		case GREEN_ITEM:
			SetColor(0, 255, 0);
			message.menu->UnmarkAllItems();
			message.menu->SetItemMark(1, "=> ");
			break;
		
		case BLUE_ITEM:
			SetColor(0, 0, 255);
			message.menu->UnmarkAllItems();
			message.menu->SetItemMark(2, "=> ");
			break;
		
		case MULTI_ITEM:
			SetColor(0, 0, 0);
			message.menu->UnmarkAllItems();
			message.menu->SetItemMark(3, "=> ");
			break;
	}
}


// Idle receiver method
// (New with lesson 3)

void MandelWind::OnMessage(
	const GlowIdleMessage& message)
{
	// Post a refresh event. This causes the window to be redrawn, which
	// will, in turn, cause the next few lines to be computed.
	Refresh();
}


// Reset zoom factor
// (New with lesson 3)

void MandelWind::ResetZoom()
{
	data_->SetCenter(-0.5, 0);
	data_->SetPixelWidth(0.015);
	Refresh();
}


// Set color
// (New with lesson 3)

void MandelWind::SetColor(
	unsigned char r,
	unsigned char g,
	unsigned char b)
{
	r_ = r;
	g_ = g;
	b_ = b;
	imageValid_ = false;
	Refresh();
}


// Redraw method

void MandelWind::OnEndPaint()
{
	// Recompute if necessary
	if (!data_->IsDataValid())
	{
		// Lesson 3: changed from Recalc()
		for (int i=0; i<10; ++i) data_->RecalcOneLine();
		
		// Make image
		delete[] image_;
		image_ = new unsigned char[data_->Width()*data_->Height()*4];
		imageValid_ = false;
		
		// Lesson 3 (incremental updating)
		Glow::RegisterIdle(this);
	}
	else
	{
		// Lesson 3 (incremental updating)
		Glow::UnregisterIdle(this);
	}
	
	if (!imageValid_)
	{
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
			else if (r_ != 0 || g_ != 0 || b_ != 0)
			{
				// Outside color (non-multi)
				image_[i*4] = r_;
				image_[i*4+1] = g_;
				image_[i*4+2] = b_;
				image_[i*4+3] = 0;
			}
			// outside color (multi...)
			else if (rawimage[i]<=256)
			{
				image_[i*4] = 256-rawimage[i];
				image_[i*4+1] = rawimage[i]-1;
				image_[i*4+2] = 0;
				image_[i*4+3] = 0;
			}
			else if (rawimage[i]<=512)
			{
				image_[i*4] = 0;
				image_[i*4+1] = 512-rawimage[i];
				image_[i*4+2] = rawimage[i]-257;
				image_[i*4+3] = 0;
			}
			else if (rawimage[i]<=768)
			{
				image_[i*4] = rawimage[i]-513;
				image_[i*4+1] = 0;
				image_[i*4+2] = 768-rawimage[i];
				image_[i*4+3] = 0;
			}
			else if (rawimage[i]<=1024)
			{
				image_[i*4] = 1024-rawimage[i];
				image_[i*4+1] = rawimage[i]-769;
				image_[i*4+2] = 0;
				image_[i*4+3] = 0;
			}
			else if (rawimage[i]<=1280)
			{
				image_[i*4] = 0;
				image_[i*4+1] = 1280-rawimage[i];
				image_[i*4+2] = rawimage[i]-1025;
				image_[i*4+3] = 0;
			}
			else if (rawimage[i]<=1536)
			{
				image_[i*4] = rawimage[i]-1281;
				image_[i*4+1] = 0;
				image_[i*4+2] = 1536-rawimage[i];
				image_[i*4+3] = 0;
			}
			else
			{
				// Default color (red)
				image_[i*4] = 255;
				image_[i*4+1] = 0;
				image_[i*4+2] = 0;
				image_[i*4+3] = 0;
			}
		}
		imageValid_ = true;
	}
	
	// Draw image
	::glDisable(GL_LIGHTING);
	::glDisable(GL_DEPTH_TEST);
	::glRasterPos2f(-1.0f, -1.0f);
	::glDrawPixels(data_->Width(), data_->Height(), GL_RGBA, GL_UNSIGNED_BYTE, image_);
	
	// Draw drag rectangle
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

void MandelWind::OnReshape(
	int width,
	int height)
{
	// Update the viewport to specify the entire window
	::glViewport(0, 0, width, height);
	halfdiagonal_ = sqrt(double(width*width+height*height))*0.5;
	
	// Update the mandel data
	data_->SetSize(width, height);
}


// Respond to mouse down events

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
			if (modifiers & Glow::shiftModifier)
			{
				xdown_ = Width()/2;
				ydown_ = Height()/2;
				dragType_ = ZOOM_OUT_DRAG;
			}
			else
			// Zoom in if shift key isn't down
			{
				xdown_ = x;
				ydown_ = y;
				dragType_ = ZOOM_IN_DRAG;
			}
			ComputeZoomFactor_(x, y);
			Refresh();
		}
	}
}


// Respond to mouse up events

void MandelWind::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	if (dragType_ == ZOOM_IN_DRAG)
	{
		// for zooming "in"
		ComputeZoomFactor_(x, y);
		if (factor_ != 0)
		{
			data_->MoveCenter(
				(xdown_-Width()/2)*data_->GetPixelWidth(),
				(Height()/2-ydown_)*data_->GetPixelWidth());
			data_->ScalePixelWidth(factor_);
		}
		dragType_ = NO_DRAG;
		Refresh();
	}
	else if (dragType_ == ZOOM_OUT_DRAG)
	{
		// for zooming "out"
		ComputeZoomFactor_(x, y);
		if (factor_ != 0)
		{
			data_->ScalePixelWidth(1.0/factor_);
		}
		dragType_ = NO_DRAG;
		Refresh();
	}
}


// Respond to mouse drag events

void MandelWind::OnMouseDrag(
	int x,
	int y)
{
	if (dragType_ == ZOOM_IN_DRAG || dragType_ == ZOOM_OUT_DRAG)
	{
		ComputeZoomFactor_(x, y);
		Refresh();
	}
}


// Compute current zooming factor

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
