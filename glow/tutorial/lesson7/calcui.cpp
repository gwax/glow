/*
===============================================================================

	FILE:  calcui.cpp
	
	PROJECT:
	
		GLOW-based RPN calculator
	
	CONTENTS:
	
		User interface
	
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

#include <cstdlib>

namespace std {} // For compilers that don't put C libraries in namespace std

using namespace std;

// If we were using QuickPalette, the header "glowQuickPalette.h"
// automatically includes all the other widget headers. But since we
// aren't, we include the various widget headers explicitly.
#include "glowPushButtonWidget.h"
#include "glowRadioButtonWidget.h"
#include "glowPanelWidget.h"
#include "glowLabelWidget.h"
#include "glowSeparatorWidget.h"
#include "glowCheckBoxWidget.h"
#include "glowMessageWindow.h"

GLOW_NAMESPACE_USING

#include "calcengine.h"
#include "calcui.h"


/*
===============================================================================
	CalcUIReceiver
	
	This class acts as a general receiver, receiving messages from the
	various widgets in the calculator. Note that we inherit from two
	widget receiver abstract base classes, and the keyboard event filter
	abstract base class.
===============================================================================
*/

class CalcUIReceiver :
	public GlowPushButtonReceiver,
	public GlowRadioButtonReceiver,
	public GlowCheckBoxReceiver,
	public GlowWidgetKeyboardFilter
{
	friend class CalcUI;
	
	private:
	
		CalcUIReceiver(
			CalcEngine* engine,
			GlowFixedSizeWidgetWindow* window);
	
	protected:
	
		// We override these methods to receive widget events
		virtual void OnMessage(
			const GlowPushButtonMessage& message);
		virtual void OnMessage(
			const GlowCheckBoxMessage& message);
		virtual void OnMessage(
			const GlowRadioButtonMessage& message);
		
		// We override this method to filter keypresses
		virtual bool OnFilter(
			GlowWidgetKeyboardData& data);
	
	private:
	
		// A utility method that updates the stack display
		void UpdateDisplay();
	
	private:
	
		// Some pointers we need to keep track of
		
		// The calculator engine and the window pointer
		CalcEngine* engine_;
		GlowFixedSizeWidgetWindow* window_;
		
		// Group most of the widgets in this panel so we can move them
		// en masse
		GlowPanelWidget* mainPanel_;
		
		// The display is contained in this panel
		GlowPanelWidget* displayPanel_;
		
		// The scientific/transcendental function panels. These are shown
		// and hidden in response to pressing the inverse key.
		GlowPanelWidget* panel1_;
		GlowPanelWidget* panel2_;
		
		// Label widgets for displaying the stack
		GlowLabelWidget* displayLabel0_;
		GlowLabelWidget* displayLabel1_;
		GlowLabelWidget* displayLabel2_;
		GlowLabelWidget* displayLabel3_;
		
		// The inverse button. Need this so we can change its color.
		GlowPushButtonWidget* inverseButton_;
		
		// Radio buttons, so we can detect presses and respond to them.
		GlowRadioButtonWidget* decimalRadioButton_;
		GlowRadioButtonWidget* scientificRadioButton_;
		GlowRadioButtonWidget* degreesRadioButton_;
		GlowRadioButtonWidget* radiansRadioButton_;
};


/*
===============================================================================
	Methods of CalcUIReceiver
===============================================================================
*/

// Construct the receiver.
CalcUIReceiver::CalcUIReceiver(
	CalcEngine* engine,
	GlowFixedSizeWidgetWindow* window)
{
	engine_ = engine;
	window_ = window;
}


// Update the stack display.
void CalcUIReceiver::UpdateDisplay()
{
	displayLabel0_->SetText(engine_->Display(0).c_str());
	displayLabel1_->SetText(engine_->Display(1).c_str());
	displayLabel2_->SetText(engine_->Display(2).c_str());
	displayLabel3_->SetText(engine_->Display(3).c_str());
}


// Receive push button events
void CalcUIReceiver::OnMessage(
	const GlowPushButtonMessage& message)
{
	GLOW_DEBUGSCOPE("CalcUIReceiver::OnMessage(GlowPushButtonMessage)");
	
	if (message.widget->GetRefCon() >= 0)
	{
		// Calculator buttons: pass the reference constant to the engine
		engine_->Button(message.widget->GetRefCon());
		UpdateDisplay();
	}
	else if (message.widget->GetRefCon() == -1)
	{
		// Quit button
		exit(0);
	}
	else if (message.widget->GetRefCon() == -2)
	{
		// Inverse button
		if (panel1_->IsVisible())
		{
			// Going to inverse: Hide normal keys and show inverse keys
			panel1_->Hide();
			panel2_->Show();
			inverseButton_->SetUpBoxColor(GlowColor(0.9, 0.7, 0.7));
		}
		else
		{
			// Going to normal: Hide inverse keys and show normal keys
			panel2_->Hide();
			panel1_->Show();
			inverseButton_->SetUpBoxColor(GlowPushButtonParams::defaults.upBoxColor);
		}
	}
	else if (message.widget->GetRefCon() == -3)
	{
		// About button
		GlowMessageWindowParams mparams;
		mparams.windowTitle = "About GlowCalc";
		// Position the window above the calculator
		mparams.x = window_->GlobalPositionX()+30;
		mparams.y = window_->GlobalPositionY()+30;
		// Let's put a little easter egg in here!
		if (message.modifiers & Glow::ctrlModifier)
		{
			mparams.text = "GlowCalc\nA GLOW-based RPN calculator\nby Daniel Azuma\n\n"
				"Down with monopolistic software corporations!";
		}
		else
		{
			mparams.text = "GlowCalc\nA GLOW-based RPN calculator\nby Daniel Azuma";
		}
		GlowMessageWindow* messageWind = new GlowMessageWindow(mparams);
		
		// Now let's make the window modal
		Glow::PushModalWindow(messageWind);
		// When the window is destroyed, the state will automatically go back
		// to non-modal.
	}
}


// Receive check box events
void CalcUIReceiver::OnMessage(
	const GlowCheckBoxMessage& message)
{
	GLOW_DEBUGSCOPE("CalcUIReceiver::OnMessage(GlowCheckBoxMessage)");
	
	// There's only one checkbox.
	
	if (message.state == GlowCheckBoxWidget::off)
	{
		// Going to single-line display
		displayPanel_->Reshape(235, 25);
		displayLabel0_->Move(5, 5);
		displayLabel1_->Hide();
		displayLabel2_->Hide();
		displayLabel3_->Hide();
		mainPanel_->Move(10, 45);
		window_->ForceReshape(255, 330);
	}
	else
	{
		// Going to multi-line display
		displayPanel_->Reshape(235, 80);
		displayLabel0_->Move(5, 60);
		displayLabel1_->Show();
		displayLabel2_->Show();
		displayLabel3_->Show();
		mainPanel_->Move(10, 100);
		window_->ForceReshape(255, 385);
	}
}


// Receive radio button press events
void CalcUIReceiver::OnMessage(
	const GlowRadioButtonMessage& message)
{
	GLOW_DEBUGSCOPE("CalcUIReceiver::OnMessage(GlowRadioButtonMessage)");
	
	if (message.buttonWidget == decimalRadioButton_)
	{
		// Set the engine to decimal display mode
		engine_->SetFormat(CalcEngine::decimalFormat);
		UpdateDisplay();
	}
	else if (message.buttonWidget == scientificRadioButton_)
	{
		// Set the engine to scientific notation display mode
		engine_->SetFormat(CalcEngine::scientificFormat);
		UpdateDisplay();
	}
	else if (message.buttonWidget == degreesRadioButton_)
	{
		// Set the engine to degrees mode
		engine_->SetAngleUnits(CalcEngine::degrees);
	}
	else if (message.buttonWidget == radiansRadioButton_)
	{
		// Set the engine to radians mode
		engine_->SetAngleUnits(CalcEngine::radians);
	}
}


// Filter keypress events that are being sent to the widgets
bool CalcUIReceiver::OnFilter(
	GlowWidgetKeyboardData& data)
{
	GLOW_DEBUGSCOPE("CalcUIReceiver::OnFilter(GlowWidgetKeyboardData)");
	
	if (data.key >= '0' && data.key <= '9')
	{
		// Numeric keys: send the keypress to the engine.
		engine_->Button(data.key - '0');
		UpdateDisplay();
	}
	else switch (int(data.key))
	{
		// Certain function keys: send the keypress to the engine.
		case '+':
			engine_->Button(CalcEngine::plusButton);
			UpdateDisplay();
			break;
		case '-':
			engine_->Button(CalcEngine::minusButton);
			UpdateDisplay();
			break;
		case '*':
			engine_->Button(CalcEngine::multButton);
			UpdateDisplay();
			break;
		case '/':
			engine_->Button(CalcEngine::divButton);
			UpdateDisplay();
			break;
		case '.':
			engine_->Button(CalcEngine::pointButton);
			UpdateDisplay();
			break;
		case 'e':
			engine_->Button(CalcEngine::exponentButton);
			UpdateDisplay();
			break;
		case Glow::backspaceKey:
		case Glow::deleteKey:
			engine_->Button(CalcEngine::deleteButton);
			UpdateDisplay();
			break;
		case Glow::enterKey:
			engine_->Button(CalcEngine::enterButton);
			UpdateDisplay();
			break;
		case 'c':
			engine_->Button(CalcEngine::clearButton);
			UpdateDisplay();
			break;
		case '_':
			engine_->Button(CalcEngine::negateButton);
			UpdateDisplay();
			break;
		case 'q':
		case Glow::escapeKey:
			exit(0);
			break;
	}
	
	// Return true to notify GLOW that we've handled the event. This
	// prevents the event from being reported normally.
	return true;
}


/*
===============================================================================
	Methods of CalcUI
===============================================================================
*/

CalcUI::CalcUI(
	CalcEngine* engine)
{
	GLOW_DEBUGSCOPE("CalcUI::CalcUI");
	
	// Create calculator window
	window_ = new GlowFixedSizeWidgetWindow("glowcalc",
		GlowWindow::autoPosition, GlowWindow::autoPosition, 255, 385);
	
	// Create receiver object
	receiver_ = new CalcUIReceiver(engine, window_);
	
	// Set up display. First, create a panel
	GlowPanelParams pparams;
	pparams.height = 80;
	pparams.width = 235;
	pparams.x = 10;
	pparams.y = 10;
	// The panel will appear lowered and will have a white background
	pparams.backColor = GlowColor::white;
	pparams.style = GlowPanelWidget::loweredStyle;
	receiver_->displayPanel_ = new GlowPanelWidget(window_, pparams);
	
	// Put four labels in it for displaying the stack
	GlowLabelParams lparams;
	lparams.height = 15;
	lparams.width = 225;
	// Note that x and y are given in local coordinates within the enclosing
	// panel. Because of this, you may choose to use panels to help you
	// arrange groups of widgets. We'll see an example of this below.
	lparams.x = 5;
	lparams.y = 5;
	lparams.font = GlowFont::fixed9by15;
	receiver_->displayLabel3_ = new GlowLabelWidget(
		receiver_->displayPanel_, lparams);
	lparams.y = 22;
	receiver_->displayLabel2_ = new GlowLabelWidget(
		receiver_->displayPanel_, lparams);
	lparams.y = 39;
	receiver_->displayLabel1_ = new GlowLabelWidget(
		receiver_->displayPanel_, lparams);
	lparams.y = 60;
	receiver_->displayLabel0_ = new GlowLabelWidget(
		receiver_->displayPanel_, lparams);
	receiver_->UpdateDisplay();
	
	// Now we create the rest of the widgets. We'll want to be able to move
	// them as a group because we're going to allow resizing of the display.
	// So we'll put them all into an invisible panel, and just move the
	// panel.
	pparams.height = 275;
	pparams.width = 235;
	pparams.x = 10;
	pparams.y = 100;
	// Restore original background color by copying from defaults
	pparams.backColor = GlowPanelParams::defaults.backColor;
	// No drawing for this panel
	pparams.style = GlowPanelWidget::plainStyle;
	receiver_->mainPanel_ = new GlowPanelWidget(window_, pparams);
	
	// All pushbuttons send their events to our receiver
	GlowPushButtonParams bparams;
	bparams.receiver = receiver_;
	
	// All the calculator keys have height 20
	bparams.height = 20;
	// Most calculator keys have width 40
	// We'll change this later when we create narrower keys
	bparams.width = 40;
	
	// Vertical position for bottom row of keys
	// Remember, this is local coordinates within mainPanel_.
	// So the window coordinates is 125+100=225.
	bparams.y = 125;
	
	// Set up clear button parameters
	bparams.text = "clr";
	// Horizontal position of key
	bparams.x = 0;
	// Refcon is for your miscellaneous use. Here we store the code that
	// should be sent to the engine when the calculator key is pressed.
	bparams.refcon = CalcEngine::clearButton;
	// Create button
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "0";
	bparams.x = 54;
	bparams.refcon = 0;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = ".";
	bparams.x = 101;
	bparams.refcon = CalcEngine::pointButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "+/-";
	bparams.x = 148;
	bparams.refcon = CalcEngine::negateButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "+";
	bparams.x = 195;
	bparams.refcon = CalcEngine::plusButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	// Next row
	bparams.y = 100;
	
	bparams.text = "del";
	bparams.x = 0;
	bparams.refcon = CalcEngine::deleteButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "1";
	bparams.x = 54;
	bparams.refcon = 1;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "2";
	bparams.x = 101;
	bparams.refcon = 2;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "3";
	bparams.x = 148;
	bparams.refcon = 3;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "-";
	bparams.x = 195;
	bparams.refcon = CalcEngine::minusButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	// Next row
	bparams.y = 75;
	
	bparams.text = "e";
	bparams.x = 0;
	bparams.refcon = CalcEngine::exponentButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "4";
	bparams.x = 54;
	bparams.refcon = 4;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "5";
	bparams.x = 101;
	bparams.refcon = 5;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "6";
	bparams.x = 148;
	bparams.refcon = 6;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "*";
	bparams.x = 195;
	bparams.refcon = CalcEngine::multButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	// Next row
	bparams.y = 50;
	
	bparams.text = "inv";
	bparams.x = 0;
	// We treat this button specially. Note that the receiver detects this
	// refcon explicitly and handles the inverse button
	bparams.refcon = -2;
	receiver_->inverseButton_ = new GlowPushButtonWidget(
		receiver_->mainPanel_, bparams);
	
	bparams.text = "7";
	bparams.x = 54;
	bparams.refcon = 7;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "8";
	bparams.x = 101;
	bparams.refcon = 8;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "9";
	bparams.x = 148;
	bparams.refcon = 9;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "/";
	bparams.x = 195;
	bparams.refcon = CalcEngine::divButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	// Next row
	bparams.y = 25;
	
	bparams.text = "swap";
	bparams.x = 101;
	bparams.refcon = CalcEngine::swapButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "pop";
	bparams.x = 148;
	bparams.refcon = CalcEngine::consumeButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "1/x";
	bparams.x = 195;
	bparams.refcon = CalcEngine::recipButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.text = "enter";
	bparams.width = 94;    // Create a wider button
	bparams.x = 0;
	bparams.refcon = CalcEngine::enterButton;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	// SPECIAL FUNCTIONS
	
	// We have twelve more keys left. We only want six to be visible
	// at a time, however, depending on the state of the "inverse" key.
	// We'll do this by putting the keys into a non-drawing panel widget.
	// One panel will be hidden and the other visible at a time.
	
	// Panel widgets parameter block
	pparams.width = 235;
	pparams.height = 20;
	pparams.x = 0;
	pparams.y = 0;
	// Panel 1 is for the "normal" keys (e.g. exp, sin)
	receiver_->panel1_ = new GlowPanelWidget(receiver_->mainPanel_, pparams);
	// Panel 2 is for the "inverse" keys (e.g. log, arcsin)
	receiver_->panel2_ = new GlowPanelWidget(receiver_->mainPanel_, pparams);
	
	// Buttons inside panel 1
	// Note again, x and y coordinates are local within the panel.
	bparams.y = 0;
	// These buttons are narrower than the others
	bparams.width = 35;
	
	bparams.text = "sqr";
	bparams.x = 0;
	bparams.refcon = CalcEngine::sqrButton;
	// Note that we're putting the widget inside the panel.
	new GlowPushButtonWidget(receiver_->panel1_, bparams);
	
	bparams.text = "exp";
	bparams.x = 40;
	bparams.refcon = CalcEngine::expButton;
	new GlowPushButtonWidget(receiver_->panel1_, bparams);
	
	bparams.text = "pow";
	bparams.x = 80;
	bparams.refcon = CalcEngine::powButton;
	new GlowPushButtonWidget(receiver_->panel1_, bparams);
	
	bparams.text = "sin";
	bparams.x = 120;
	bparams.refcon = CalcEngine::sinButton;
	new GlowPushButtonWidget(receiver_->panel1_, bparams);
	
	bparams.text = "cos";
	bparams.x = 160;
	bparams.refcon = CalcEngine::cosButton;
	new GlowPushButtonWidget(receiver_->panel1_, bparams);
	
	bparams.text = "tan";
	bparams.x = 200;
	bparams.refcon = CalcEngine::tanButton;
	new GlowPushButtonWidget(receiver_->panel1_, bparams);
	
	// Buttons inside panel 2
	bparams.text = "sqrt";
	bparams.x = 0;
	bparams.refcon = CalcEngine::sqrtButton;
	new GlowPushButtonWidget(receiver_->panel2_, bparams);
	
	bparams.text = "log";
	bparams.x = 40;
	bparams.refcon = CalcEngine::logButton;
	new GlowPushButtonWidget(receiver_->panel2_, bparams);
	
	bparams.text = "root";
	bparams.x = 80;
	bparams.refcon = CalcEngine::rootButton;
	new GlowPushButtonWidget(receiver_->panel2_, bparams);
	
	bparams.text = "asin";
	bparams.x = 120;
	bparams.refcon = CalcEngine::arcsinButton;
	new GlowPushButtonWidget(receiver_->panel2_, bparams);
	
	bparams.text = "acos";
	bparams.x = 160;
	bparams.refcon = CalcEngine::arccosButton;
	new GlowPushButtonWidget(receiver_->panel2_, bparams);
	
	bparams.text = "atan";
	bparams.x = 200;
	bparams.refcon = CalcEngine::arctanButton;
	new GlowPushButtonWidget(receiver_->panel2_, bparams);
	
	// Initial inverse state is false, so hide panel 2
	receiver_->panel2_->Hide();
	
	// Separate calculator from auxiliary controls with a separator rule
	GlowSeparatorParams sparams;
	sparams.height = 2;
	sparams.width = 235;
	sparams.x = 0;
	sparams.y = 155;
	new GlowSeparatorWidget(receiver_->mainPanel_, sparams);
	
	// Now we'll make some widgets at the bottom to control prefs.
	// First will be a checkbox controlling the size of the display.
	// Then we'll make some radio buttons.
	// On the left will be the display format (normal vs. scientific)
	// On the right will be angle units (degrees vs. radians)
	
	// Checkbox
	GlowCheckBoxParams cparams;
	cparams.x = 0;
	cparams.y = 165;
	cparams.height = 15;
	cparams.width = 160;
	cparams.text = "Display entire stack";
	cparams.state = GlowCheckBoxWidget::on;
	cparams.receiver = receiver_;
	GlowCheckBoxWidget* checkbox = new GlowCheckBoxWidget(
		receiver_->mainPanel_, cparams);
	// Crop the width of the widget to tightly enclose the box and label
	checkbox->CropWidth();
	
	// Make a label for the first group of radio buttons
	lparams.height = 15;
	lparams.width = 100;
	lparams.x = 0;
	lparams.y = 190;
	// Reset the font to helvetica, because we changed the font to fixed9by15
	// the last time we used the parameter block
	lparams.font = GlowFont::helvetica12;
	lparams.text = "Format:";
	new GlowLabelWidget(receiver_->mainPanel_, lparams);
	
	// Now we make a radio group to put the radio buttons into. A radio group
	// is a panel with some extra wiring that lets it keep track of a bunch
	// of radio buttons
	GlowRadioGroupParams gparams;
	gparams.height = 35;
	gparams.width = 120;
	gparams.y = 205;
	gparams.x = 0;
	// Make sure our receiver receives messages from this radio group
	gparams.receiver = receiver_;
	GlowRadioGroupWidget* radioGroup = new GlowRadioGroupWidget(
		receiver_->mainPanel_, gparams);
	
	// Add radio buttons to the radio group. Remember that x and y are given
	// in local coordinates within the radio group panel.
	GlowRadioButtonParams rparams;
	rparams.height = 15;
	rparams.width = 100;
	rparams.x = 20;
	rparams.y = 1;
	rparams.text = "Decimal";
	receiver_->decimalRadioButton_ =
		new GlowRadioButtonWidget(radioGroup, rparams);
	// Crop the width of the widget to tightly enclose the box and label
	receiver_->decimalRadioButton_->CropWidth();
	rparams.y = 19;
	rparams.text = "Scientific";
	receiver_->scientificRadioButton_ =
		new GlowRadioButtonWidget(radioGroup, rparams);
	receiver_->scientificRadioButton_->CropWidth();
	
	// Angle units radio buttons
	lparams.height = 15;
	lparams.width = 90;
	lparams.x = 120;
	lparams.y = 190;
	lparams.text = "Angles:";
	new GlowLabelWidget(receiver_->mainPanel_, lparams);
	
	// Make the radio group
	gparams.x = 120;
	gparams.y = 205;
	radioGroup = new GlowRadioGroupWidget(receiver_->mainPanel_, gparams);
	
	//  ... and add the buttons
	rparams.y = 1;
	rparams.text = "Degrees";
	receiver_->degreesRadioButton_ =
		new GlowRadioButtonWidget(radioGroup, rparams);
	receiver_->degreesRadioButton_->CropWidth();
	rparams.y = 19;
	rparams.text = "Radians";
	receiver_->radiansRadioButton_ =
		new GlowRadioButtonWidget(radioGroup, rparams);
	receiver_->radiansRadioButton_->CropWidth();
	
	// Finally, let's add a quit button and an about button
	bparams.height = 25;
	bparams.width = 80;
	bparams.x = 0;
	bparams.y = 250;
	bparams.text = "Quit";
	// We'll detect this refcon and handle this button specially
	bparams.refcon = -1;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	bparams.x = 100;
	bparams.text = "About";
	bparams.refcon = -3;
	new GlowPushButtonWidget(receiver_->mainPanel_, bparams);
	
	// Set up a keyboard event filter. This will let us respond to keyboard
	// events even though we don't have any keyboard-listening widgets.
	window_->RegisterFilter(receiver_);
}


// Delete the window and receiver if the UI object is destructed.
CalcUI::~CalcUI()
{
	delete window_;
	delete receiver_;
}
