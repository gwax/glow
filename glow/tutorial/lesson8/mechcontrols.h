/*
===============================================================================

	FILE:  mechcontrols.h
	
	PROJECT:
	
		GLOW widgets for glutmech demo
	
	CONTENTS:
	
		Controls window
	
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

#ifndef MECHCONTROLS__H
#define MECHCONTROLS__H


/*
===============================================================================
	Headers
===============================================================================
*/

#include "glow.h"
#include "glowQuickPalette.h"

GLOW_NAMESPACE_USING

#include "ballwidget.h"


/*
===============================================================================
	Controls class
===============================================================================
*/

// MechControls is an object that receives a number of events.
// The idle event receiver is used for background calculating.
// The TextFieldWindow event receiver is used for the save image dialog.
// The other event receivers are widgets in the control palette.

class MechControls :
	public GlowPushButtonReceiver,
	public GlowSliderReceiver,
	public GlowCheckBoxReceiver,
	public BallReceiver
{
	public:
	
		MechControls(
			int mainWindowID);
		virtual ~MechControls();
	
	protected:
	
		// Widget events
		
		// Implement this method to receive pushbutton events
		virtual void OnMessage(
			const GlowPushButtonMessage& message);
		
		// Implement this method to receive slider events
		virtual void OnMessage(
			const GlowSliderMessage& message);
		
		// Implement this method to receive check box events
		virtual void OnMessage(
			const GlowCheckBoxMessage& message);
		
		// Implement this method to receive ball events (new with lesson 8)
		virtual void OnMessage(
			const BallMessage& message);
	
	private:
	
		// Main drawing window id
		int mainWindowID_;
		
		// Pointer to the control window itself
		GlowQuickPaletteWindow* controlWindow_;
		
		// Pointers to selected widgets. We store these so that we can tell
		// what widget was hit when we receive events.
		GlowCheckBoxWidget* animationCheckbox_;
		GlowCheckBoxWidget* wireframeCheckbox_;
		GlowPushButtonWidget* quitButton_;
		GlowPushButtonWidget* fireButton_;
		GlowSliderWidget* rotateHipSlider_;
		GlowSliderWidget* tiltTorsoSlider_;
		GlowSliderWidget* lShoulderFwdSlider_;
		GlowSliderWidget* lShoulderOutSlider_;
		GlowSliderWidget* lElbowFwdSlider_;
		GlowSliderWidget* lElbowOutSlider_;
		GlowSliderWidget* lHipFwdSlider_;
		GlowSliderWidget* lHipOutSlider_;
		GlowSliderWidget* lKneeSlider_;
		GlowSliderWidget* lAnkleSlider_;
		GlowSliderWidget* rShoulderFwdSlider_;
		GlowSliderWidget* rShoulderOutSlider_;
		GlowSliderWidget* rElbowFwdSlider_;
		GlowSliderWidget* rElbowOutSlider_;
		GlowSliderWidget* rHipFwdSlider_;
		GlowSliderWidget* rHipOutSlider_;
		GlowSliderWidget* rKneeSlider_;
		GlowSliderWidget* rAnkleSlider_;
		BallWidget* viewBall_;
		BallWidget* lightBall_;
};


#endif
