/*
===============================================================================

	FILE:  mandelwind.h
	
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

#ifndef MANDELWIND__H
#define MANDELWIND__H


/*
===============================================================================
	Headers
===============================================================================
*/

#include "glow.h"

// Lesson 4: QuickPalette interface
#include "glowQuickPalette.h"
#include "glowTextFieldWindow.h"

GLOW_NAMESPACE_USING


/*
===============================================================================
	Window class
===============================================================================
*/

class MandelData;

// New with lesson 4:
// Implement GlowPushButtonReceiver, GlowSliderReceiver,
// GlowCheckBoxReceiver, GlowPopupMenuReceiver and
// GlowTextFieldWindowReceiver interfaces to receive widget events
// We also remove the menu receiver because we're not using the
// menu-based interface anymore

class MandelWind :
	public GlowWindow,
	//public GlowMenuReceiver,	
	public GlowIdleReceiver,
	public GlowPushButtonReceiver,
	public GlowSliderReceiver,
	public GlowCheckBoxReceiver,
	public GlowPopupMenuReceiver,
	public GlowTextFieldWindowReceiver
{
	public:
	
		MandelWind(
			MandelData* data);
		virtual ~MandelWind();
		
		void ResetZoom();
		void SetColor(
			unsigned char r,
			unsigned char g,
			unsigned char b);
	
	protected:
	
		// Refresh event
		virtual void OnEndPaint();
		
		// Basic window and mouse events
		virtual void OnReshape(
			int width,
			int height);
		virtual void OnMouseDown(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnMouseUp(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnMouseDrag(
			int x,
			int y);
		
		// Idle events for background processing
		virtual void OnMessage(
			const GlowIdleMessage& message);
		
		// No more menu events
		//virtual void OnMessage(
		//	const GlowMenuMessage& message);
		
		// Widget events (new with lesson 4)...
		
		// Implement this method to receive pushbutton events
		virtual void OnMessage(
			const GlowPushButtonMessage& message);
		
		// Implement this method to receive slider events
		virtual void OnMessage(
			const GlowSliderMessage& message);
		
		// Implement this method to receive check box events
		virtual void OnMessage(
			const GlowCheckBoxMessage& message);
		
		// Implement this method to receive popup menu events
		virtual void OnMessage(
			const GlowPopupMenuMessage& message);
		
		// Implement this method to receive events from a text field window
		virtual void OnMessage(
			const GlowTextFieldWindowMessage& message);
			
	
	private:
	
		void ComputeZoomFactor_(
			int x,
			int y);
	
	private:
	
		// Image data
		MandelData* data_;
		unsigned char* image_;
		
		// Zooming code
		double halfdiagonal_;
		int dragType_;
		int xdown_;
		int ydown_;
		double factor_;
		
		// Lesson 3 data
		unsigned char r_;
		unsigned char g_;
		unsigned char b_;
		bool imageValid_;
		
		// New with lesson 4:
		
		// Are we doing background updating or not?
		bool updateInBackground_;
		
		// Multi-color options
		// Rate of 1.0 is the rate we used in lesson 3
		// Offset of 0.0 is the offset we used in lesson 3
		float multiCycleRate_;
		float multiCycleOffset_;
		
		// Pointer to the control window itself
		GlowQuickPaletteWindow* controlWindow_;
		
		// Pointers to selected widgets. We store these so that we can tell what
		// widget was hit when we receive events.
		GlowPushButtonWidget* quitButton_;
		GlowPushButtonWidget* saveButton_;
		GlowPushButtonWidget* resetButton_;
		GlowSliderWidget* iterationSlider_;
		GlowSliderWidget* multiCycleRateSlider_;
		GlowSliderWidget* multiCycleOffsetSlider_;
		GlowQuickPanelWidget* multiColorControlsPanel_;
};


#endif
