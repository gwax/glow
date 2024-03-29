/*
===============================================================================

	FILE:  mechcontrols.cpp
	
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


/*
===============================================================================
	Headers
===============================================================================
*/

#include "mechcontrols.h"


/*
===============================================================================
	External things defined in glowmech.cpp
	that we need access to
===============================================================================
*/

extern int shoulder1, shoulder2, shoulder3, shoulder4, lat1, lat2,
  elbow1, elbow2, pivot, tilt, ankle1, ankle2, heel1,
  heel2, hip11, hip12, hip21, hip22, solid_part;
extern void FireCannon(void);
extern void animation(void);


/*
===============================================================================
	Methods of MandelWind
===============================================================================
*/

// Constructor

MechControls::MechControls(
	int mainWindowID)
{
	GLOW_DEBUGSCOPE("MechControls::MechControls");
	
	mainWindowID_ = mainWindowID;
	
	// Create a control panel window
	controlWindow_ = new GlowQuickPaletteWindow("Controls");
	
	// Add controls
	// First, we'll put a little blurb at the top of the window
	controlWindow_->AddLabel(
		"Glowmech (lesson 6)\n"
		"Original glutmech code by Simon Parkinson-Bates\n"
		"Glow controls added by Daniel Azuma");
	
	// The rest of the window appears in three panels. The panels are arranged
	// horizontally, but widgets are arranged vertically within each panel
	GlowQuickPanelWidget* hpanel = controlWindow_->AddArrangingPanel(
		GlowQuickPanelWidget::horizontal);
	
	// First we have a few general controls
	GlowQuickPanelWidget* panel = hpanel->AddPanel(
		GlowQuickPanelWidget::loweredStyle, "Main",
		GlowQuickPanelWidget::vertical);
	
	// Rotate hip slider
	rotateHipSlider_ = panel->AddSlider(-180, 180, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Rotation:\n%.0f", this);
	// Tilt torso slider
	tiltTorsoSlider_ = panel->AddSlider(-90, 90, 10,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Tilt:\n%.0f", this);
	// A separator
	panel->AddSeparator();
	// Toggle animation checkbox
	animationCheckbox_ = panel->AddCheckBox(
		"Animation", GlowCheckBoxWidget::off, this);
	// Toggle wireframe mode checkbox
	wireframeCheckbox_ = panel->AddCheckBox(
		"Wireframe", GlowCheckBoxWidget::on, this);
	// Fire guns button
	fireButton_ = panel->AddPushButton("Fire cannon", this);
	// Quit button
	quitButton_ = panel->AddPushButton("Quit", this);
	
	// Now make a bunch of sliders for each side of the robot. First the right side.
	panel = hpanel->AddPanel(
		GlowQuickPanelWidget::loweredStyle, "Right",
		GlowQuickPanelWidget::vertical);
	
	// Shoulder back/forward slider
	rShoulderFwdSlider_ = panel->AddSlider(-180, 180, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Shoulder fwd:\n%.0f", this);
	// Shoulder in/out slider
	rShoulderOutSlider_ = panel->AddSlider(0, 180, 20,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Shoulder out:\n%.0f", this);
	// Elbow back/forward slider
	rElbowFwdSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Elbow fwd:\n%.0f", this);
	// Elbow in/out slider
	rElbowOutSlider_ = panel->AddSlider(-180, 180, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Arm out:\n%.0f", this);
	// Hip back/forward slider
	rHipFwdSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Hip fwd:\n%.0f", this);
	// Hip in/out slider
	rHipOutSlider_ = panel->AddSlider(-30, 90, 10,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Hip out:\n%.0f", this);
	// Knee slider
	rKneeSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Knee:\n%.0f", this);
	// Ankle slider
	rAnkleSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Ankle:\n%.0f", this);

	// Now the left side
	panel = hpanel->AddPanel(
		GlowQuickPanelWidget::loweredStyle, "Left",
		GlowQuickPanelWidget::vertical);
	
	// Shoulder back/forward slider
	lShoulderFwdSlider_ = panel->AddSlider(-180, 180, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Shoulder fwd:\n%.0f", this);
	// Shoulder in/out slider
	lShoulderOutSlider_ = panel->AddSlider(0, 180, 20,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Shoulder out:\n%.0f", this);
	// Elbow back/forward slider
	lElbowFwdSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Elbow fwd:\n%.0f", this);
	// Elbow in/out slider
	lElbowOutSlider_ = panel->AddSlider(-180, 180, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Arm out:\n%.0f", this);
	// Hip back/forward slider
	lHipFwdSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Hip fwd:\n%.0f", this);
	// Hip in/out slider
	lHipOutSlider_ = panel->AddSlider(-30, 90, 10,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Hip out:\n%.0f", this);
	// Knee slider
	lKneeSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Knee:\n%.0f", this);
	// Ankle slider
	lAnkleSlider_ = panel->AddSlider(-90, 90, 0,
		GlowSliderWidget::defaultOptions, 3, "%.0f", "Ankle:\n%.0f", this);
	
	// Arrange controls and show the control panel window
	controlWindow_->Pack();
}


// Destructor

MechControls::~MechControls()
{
	delete controlWindow_;
}


// Receive pushbutton events
void MechControls::OnMessage(
	const GlowPushButtonMessage& message)
{
	GLOW_DEBUGSCOPE("MechControls::OnMessage(pushbutton)");
	
	// Was it the quit button?
	if (message.widget == quitButton_)
	{
		exit(0);
	}
	// Fire cannons button?
	else if (message.widget == fireButton_)
	{
		FireCannon();
		Glow::RefreshGlutWindow(mainWindowID_);
	}
}


// Receive checkbox events
void MechControls::OnMessage(
	const GlowCheckBoxMessage& message)
{
	GLOW_DEBUGSCOPE("MechControls::OnMessage(checkbox)");
	
	// Was it the toggle animation checkbox?
	if (message.widget == animationCheckbox_)
	{
		if (message.state == GlowCheckBoxWidget::on)
		{
			// We need to use this special method instead of ::glutIdleFunc()
			Glow::SetIdleFunc(animation);
		}
		else
		{
			// We need to use this special method instead of ::glutIdleFunc()
			Glow::SetIdleFunc(NULL);
		}
	}
	// Was it the toggle wireframe checkbox?
	else if (message.widget == wireframeCheckbox_)
	{
		solid_part = 1-solid_part;
		Glow::RefreshGlutWindow(mainWindowID_);
	}
}


// Receive slider events
void MechControls::OnMessage(
	const GlowSliderMessage& message)
{
	GLOW_DEBUGSCOPE("MechControls::OnMessage(slider)");
	
	// Check central sliders
	
	// Hip rotate
	if (message.widget == rotateHipSlider_)
	{
		pivot = int(message.value);
	}
	else if (message.widget == tiltTorsoSlider_)
	{
		tilt = int(message.value);
	}
	
	// Check left side sliders
	
	// Shoulder forward/back
	else if (message.widget == lShoulderFwdSlider_)
	{
		shoulder2 = int(message.value);
	}
	// Shoulder in/out
	else if (message.widget == lShoulderOutSlider_)
	{
		lat2 = int(message.value);
	}
	// Elbow forward/back
	else if (message.widget == lElbowFwdSlider_)
	{
		elbow2 = int(message.value);
	}
	// Elbow in/out
	else if (message.widget == lElbowOutSlider_)
	{
		shoulder4 = int(message.value);
	}
	// Hip forward/back
	else if (message.widget == lHipFwdSlider_)
	{
		hip21 = int(message.value);
	}
	// Hip in/out
	else if (message.widget == lHipOutSlider_)
	{
		hip22 = int(message.value);
	}
	// Knee
	else if (message.widget == lKneeSlider_)
	{
		heel2 = int(message.value);
	}
	// Ankle
	else if (message.widget == lAnkleSlider_)
	{
		ankle2 = int(message.value);
	}
	
	// Check right side sliders
	
	// Shoulder forward/back
	else if (message.widget == rShoulderFwdSlider_)
	{
		shoulder1 = int(message.value);
	}
	// Shoulder in/out
	else if (message.widget == rShoulderOutSlider_)
	{
		lat1 = int(message.value);
	}
	// Elbow forward/back
	else if (message.widget == rElbowFwdSlider_)
	{
		elbow1 = int(message.value);
	}
	// Elbow in/out
	else if (message.widget == rElbowOutSlider_)
	{
		shoulder3 = int(message.value);
	}
	// Hip forward/back
	else if (message.widget == rHipFwdSlider_)
	{
		hip11 = int(message.value);
	}
	// Hip in/out
	else if (message.widget == rHipOutSlider_)
	{
		hip12 = int(message.value);
	}
	// Knee
	else if (message.widget == rKneeSlider_)
	{
		heel1 = int(message.value);
	}
	// Ankle
	else if (message.widget == rAnkleSlider_)
	{
		ankle1 = int(message.value);
	}
	
	// Redraw scene
	Glow::RefreshGlutWindow(mainWindowID_);
}

