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
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit tutorial -- version 1.1.2dev  (7 August 2000)
	
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
#include <fstream>

using namespace std;

#include "mandelwind.h"
#include "mandeldata.h"
#include "mandelgizmo.h"   // Lesson 5 support


/*
===============================================================================
	Constants
===============================================================================
*/

// Drag status
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
	Glow::mouseEvents | Glow::dragEvents | Glow::menuEvents)
{
	GLOW_DEBUGSCOPE("MandelWind::MandelWind");
	
	// Get data
	data->SetThreshhold(1000);
	data_ = data;
	image_ = 0;
	dragType_ = NO_DRAG;
	r_ = 255;
	g_ = 0;
	b_ = 0;
	imageValid_ = false;
	updateInBackground_ = true;
	multiCycleRate_ = 1.0f;
	multiCycleOffset_ = 0.0f;
	
	// Create a control panel window to go along with this window
	controlWindow_ = new GlowQuickPaletteWindow("Controls");
	
	// Add controls
	// First, we'll put a little blurb at the top of the window
	controlWindow_->AddLabel(
		"Mandelglow (lesson 5)\nby Daniel Azuma");
	
	// The next set of controls will be within a panel called "calculation"
	GlowQuickPanelWidget* panel = controlWindow_->AddPanel(
		GlowQuickPanelWidget::etchedStyle, "Calculation");
	
	// Checkbox for background calculation.
	// Put it within the calculation panel.
	// Initial value is on (checked), and bind notifier to this MandelWind.
	panel->AddCheckBox("Background calcuation", GlowCheckBoxWidget::on, this);
	
	// Create a slider for the number of iterations
	// Min iterations = 100, max = 2000, initial value = 1000.
	// Specify the syntax for the value labels and the main label.
	// Bind notifier to this MandelWind.
	iterationSlider_ = panel->AddSlider(100, 2000, 1000,
		GlowSliderWidget::defaultOptions, 2, "%.0f", "Iterations:\n%.0f", this);
	
	// Create another panel called "view"
	panel = controlWindow_->AddPanel(
		GlowQuickPanelWidget::etchedStyle, "View");
	
	// Color scheme popup menu put it within the view panel
	GlowLabeledPopupMenuWidget* popup = panel->AddPopupMenu(
		"Color scheme:", this);
	popup->AddItem("Red");
	popup->AddItem("Green");
	popup->AddItem("Blue");
	popup->AddItem("Multi");
	
	// Create a nested panel.
	multiColorControlsPanel_ = panel->AddPanel(
		GlowQuickPanelWidget::etchedStyle, "Multicolor settings");
	
	// Slider giving the rate of cycling for "multi" coloring style
	multiCycleRateSlider_ = multiColorControlsPanel_->AddSlider(0.2, 20.0, 1.0,
		GlowSliderWidget::logarithmic, 2, "%.2f", "Cycle rate:\n%.2f", this);
	multiCycleOffsetSlider_ = multiColorControlsPanel_->AddSlider(0.0, 3.0, 0.0,
		GlowSliderWidget::defaultOptions, 2, "%.2f", "Cycle offset:\n%.2f", this);
	
	// The panel starts off inactive because the initial color scheme is
	// plain red (not multicolor). Note that this also deactivates both
	// sliders that are within the panel
	multiColorControlsPanel_->Deactivate();
	
	// Reset view button.
	resetButton_ = panel->AddPushButton("Reset View", this);
	
	// Quit button and save button. Use an arranging panel to cause
	// the buttons to be laid out horizontally
	panel = controlWindow_->AddArrangingPanel(
		GlowQuickPanelWidget::horizontal);
	quitButton_ = panel->AddPushButton("Quit", this);
	saveButton_ = panel->AddPushButton("Save Image", this);
	
	// New button for lesson 5
	gizmoButton_ = panel->AddPushButton("Open Gizmo", this);
	
	// Arrange controls and show the control panel window
	controlWindow_->Pack();
}


// Destructor

MandelWind::~MandelWind()
{
	delete data_;
	delete image_;
	
	// We don't need to delete all the individual widgets because the
	// window contains them and will delete them automatically. Just
	// delete the window.
	delete controlWindow_;
}


// Idle receiver method

void MandelWind::OnMessage(
	const GlowIdleMessage& message)
{
	// Post a refresh event. This causes the window to be redrawn, which
	// will, in turn, cause the next few lines to be computed.
	Refresh();
}


// Reset zoom factor

void MandelWind::ResetZoom()
{
	data_->SetCenter(-0.5, 0);
	data_->SetPixelWidth(0.015);
	Refresh();
}


// Set color

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
	GLOW_DEBUGSCOPE("MandelWind::OnEndPaint");
	
	// Recompute if necessary
	if (!data_->IsDataValid())
	{
		// Lesson 4: use updateInBackground_ setting
		if (updateInBackground_)
		{
			for (int i=0; i<10; ++i) data_->RecalcOneLine();
		}
		else
		{
			data_->Recalc();
		}
		
		// Make image
		delete[] image_;
		image_ = new unsigned char[data_->Width()*data_->Height()*4];
		imageValid_ = false;
		
		// Make sure we update again at idle time
		if (updateInBackground_)
		{
			Glow::RegisterIdle(this);
		}
	}
	else
	{
		// No longer need to update at idle time
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
				image_[i*4+3] = 255;
			}
			else if (r_ != 0 || g_ != 0 || b_ != 0)
			{
				// Outside color (non-multi)
				image_[i*4] = r_;
				image_[i*4+1] = g_;
				image_[i*4+2] = b_;
				image_[i*4+3] = 255;
			}
			// outside color (multi...)
			else
			{
				// Lesson 4: better code that uses multi cycle rate/offset
				float cycleLength = 256.0f/multiCycleRate_;
				float value = rawimage[i]+multiCycleOffset_*cycleLength;
				value -= floor(value/(cycleLength*3))*(cycleLength*3);
				if (value < cycleLength)
				{
					image_[i*4] = int((cycleLength-value)*255.0f/cycleLength);
					image_[i*4+1] = int(value*255.0f/cycleLength);
					image_[i*4+2] = 0;
					image_[i*4+3] = 255;
				}
				else if (value < cycleLength*2.0f)
				{
					image_[i*4] = 0;
					image_[i*4+1] = int((cycleLength*2.0f-value)*255.0f/cycleLength);
					image_[i*4+2] = int((value-cycleLength)*255.0f/cycleLength);
					image_[i*4+3] = 255;
				}
				else
				{
					image_[i*4] = int((value-cycleLength*2.0f)*255.0f/cycleLength);
					image_[i*4+1] = 0;
					image_[i*4+2] = int((cycleLength*3.0f-value)*255.0f/cycleLength);
					image_[i*4+3] = 255;
				}
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
	GLOW_DEBUGSCOPE("MandelWind::OnReshape");
	
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
	GLOW_DEBUGSCOPE("MandelWind::OnMouseDown");
	
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
	GLOW_DEBUGSCOPE("MandelWind::OnMouseUp");
	
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
	GLOW_DEBUGSCOPE("MandelWind::OnMouseDrag");
	
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


// Receive pushbutton events
void MandelWind::OnMessage(
	const GlowPushButtonMessage& message)
{
	GLOW_DEBUGSCOPE("MandelWind::OnMessage(pushbutton)");
	
	// Was it the quit button?
	if (message.widget == quitButton_)
	{
		exit(0);
	}
	// Reset button?
	else if (message.widget == resetButton_)
	{
		ResetZoom();
	}
	// Save button?
	else if (message.widget == saveButton_)
	{
		// Create window to prompt for save filename
		// Have it notify this MandelWind when the user dismisses it with
		// a button press
		new GlowTextFieldWindow("Save Image", GlowWindow::autoPosition,
			GlowWindow::autoPosition, "Enter file name:", 300, "image.ppm",
			"OK\tCancel", this);
	}
	// Gizmo button? (New with lesson 5)
	else if (message.widget == gizmoButton_)
	{
		// Create a new gizmo window
		new MandelGizmoWindow(image_, Width(), Height());
	}
}


// Receive slider events
void MandelWind::OnMessage(
	const GlowSliderMessage& message)
{
	GLOW_DEBUGSCOPE("MandelWind::OnMessage(slider)");
	
	// Was it the multicolor cycle rate slider?
	if (message.widget == multiCycleRateSlider_)
	{
		multiCycleRate_ = message.value;
		imageValid_ = false;
		Refresh();
	}
	// Was it the multicolor cycle offset slider?
	else if (message.widget == multiCycleOffsetSlider_)
	{
		multiCycleOffset_ = message.value;
		imageValid_ = false;
		Refresh();
	}
	// Was it the number of iterations slider?
	else if (message.widget == iterationSlider_)
	{
		// Respond only on slider release (i.e. non-"live" slider)
		if (message.released)
		{
			data_->SetThreshhold(int(message.value));
			Refresh();
		}
	}
}


// Receive checkbox events
void MandelWind::OnMessage(
	const GlowCheckBoxMessage& message)
{
	GLOW_DEBUGSCOPE("MandelWind::OnMessage(checkbox)");
	
	// Only one checkbox was created-- assume it's the background calculation
	// checkbox
	updateInBackground_ = (message.state == GlowCheckBoxWidget::on);
}


// Receive popup menu events
void MandelWind::OnMessage(
	const GlowPopupMenuMessage& message)
{
	GLOW_DEBUGSCOPE("MandelWind::OnMessage(popup menu)");
	
	// Only one popup menu was created-- assume it's the color scheme menu
	switch (message.item)
	{
		case 0:
			// Set the background color to red
			SetColor(255, 0, 0);
			// The popup menu will automatically mark and unmark items
			// Deactivate the sliders that control multicolor options
			multiColorControlsPanel_->Deactivate();
			break;
		
		case 1:
			// Set the background color to green
			SetColor(0, 255, 0);
			// Deactivate the sliders that control multicolor options
			multiColorControlsPanel_->Deactivate();
			break;
		
		case 2:
			// Set the background color to blue
			SetColor(0, 0, 255);
			// Deactivate the sliders that control multicolor options
			multiColorControlsPanel_->Deactivate();
			break;
		
		case 3:
			// Set the background color to multi
			SetColor(0, 0, 0);
			// Activate the sliders that control multicolor options
			multiColorControlsPanel_->Activate();
			break;
	}
}


// Receive events from the save dialog
void MandelWind::OnMessage(
	const GlowTextFieldWindowMessage& message)
{
	GLOW_DEBUGSCOPE("MandelWind::OnMessage(text field window)");
	
	// Only one text field window was created-- assume it's the save dialog
	
	// Bail out if the user pressed the cancel button
	if (message.response == 1) return;
	
	// Otherwise OK button (response == 0) was pressed...
	// We're just going to save the image as simple P3 ppm. You can implement
	// a better image format if you want to
	ofstream ofs(message.text.c_str());
	if (!ofs) return;
	ofs << "P3 " << data_->Width() << " " << data_->Height() << " 255\n";
	int numpixels = data_->Width()*data_->Height();
	for (int i=0; i<numpixels; ++i)
	{
		ofs << int(image_[i*4]) << " " << int(image_[i*4+1]) << " " <<
			int(image_[i*4+2]) << endl;
	}
}

