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
	
		The GLOW Toolkit tutorial -- version 0.9.7  (27 April 2000)
	
	CHANGE HISTORY:
	
		27 April 2000 -- DA -- Initial CVS checkin

===============================================================================
*/


/*
===============================================================================
	Headers
===============================================================================
*/

#include <cstdlib>

using namespace std;

// If we were using QuickPalette, the header "glowQuickPalette.h"
// automatically includes all the other widget headers. But since we
// aren't, we include the various widget headers explicitly.
#include "glowPushButtonWidget.h"
#include "glowRadioButtonWidget.h"
#include "glowPanelWidget.h"
#include "glowLabelWidget.h"
#include "glowSeparatorWidget.h"
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
		CalcEngine* _engine;
		GlowFixedSizeWidgetWindow* _window;
		
		// The scientific/transcendental function panels. These are shown
		// and hidden in response to pressing the inverse key.
		GlowPanelWidget* _panel1;
		GlowPanelWidget* _panel2;
		
		// Label widgets for displaying the stack
		GlowLabelWidget* _displayLabel0;
		GlowLabelWidget* _displayLabel1;
		GlowLabelWidget* _displayLabel2;
		GlowLabelWidget* _displayLabel3;
		
		// The inverse button. Need this so we can change its color.
		GlowPushButtonWidget* _inverseButton;
		
		// Radio buttons, so we can detect presses and respond to them.
		GlowRadioButtonWidget* _decimalRadioButton;
		GlowRadioButtonWidget* _scientificRadioButton;
		GlowRadioButtonWidget* _degreesRadioButton;
		GlowRadioButtonWidget* _radiansRadioButton;
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
	_engine = engine;
	_window = window;
}


// Update the stack display.
void CalcUIReceiver::UpdateDisplay()
{
	_displayLabel0->SetText(_engine->Display(0).c_str());
	_displayLabel1->SetText(_engine->Display(1).c_str());
	_displayLabel2->SetText(_engine->Display(2).c_str());
	_displayLabel3->SetText(_engine->Display(3).c_str());
}


// Receive push button events
void CalcUIReceiver::OnMessage(
	const GlowPushButtonMessage& message)
{
	GLOW_DEBUGSCOPE("CalcUIReceiver::OnMessage(GlowPushButtonMessage)");
	
	if (message.widget->GetRefCon() == -1)
	{
		// Inverse button
		if (_panel1->IsVisible())
		{
			// Going to inverse: Hide normal keys and show inverse keys
			_panel1->Hide();
			_panel2->Show();
			_inverseButton->SetBoxColor(GlowColor(0.9, 0.7, 0.7));
		}
		else
		{
			// Going to normal: Hide inverse keys and show normal keys
			_panel2->Hide();
			_panel1->Show();
			_inverseButton->SetBoxColor(GlowPushButtonParams::defaults.boxColor);
		}
	}
	else if (message.widget->GetRefCon() == -2)
	{
		// Quit button
		exit(0);
	}
	else if (message.widget->GetRefCon() == -3)
	{
		// About button
		GlowMessageWindowParams mparams;
		mparams.windowTitle = "About GlowCalc";
		// Position the window above the calculator
		mparams.x = _window->GlobalPositionX()+30;
		mparams.y = _window->GlobalPositionY()+30;
		// Let's put a little easter egg in here!
		if (message.modifiers & Glow::ctrlModifier)
		{
			mparams.text = "GlowCalc (lesson 7) version 0.9.7\nby Daniel Azuma\n\n"
				"Down with monopolistic software corporations!";
		}
		else
		{
			mparams.text = "GlowCalc (lesson 7) version 0.9.7\nby Daniel Azuma";
		}
		GlowMessageWindow* messageWind = new GlowMessageWindow(mparams);
		
		// Now let's make the window modal
		Glow::PushModalWindow(messageWind);
		// When the window is destroyed, the state will automatically go back
		// to non-modal.
	}
	else
	{
		// Other buttons: pass the reference constant to the engine
		_engine->Button(message.widget->GetRefCon());
		UpdateDisplay();
	}
}


// Receive radio button press events
void CalcUIReceiver::OnMessage(
	const GlowRadioButtonMessage& message)
{
	GLOW_DEBUGSCOPE("CalcUIReceiver::OnMessage(GlowRadioButtonMessage)");
	
	if (message.buttonWidget == _decimalRadioButton)
	{
		// Set the engine to decimal display mode
		_engine->SetFormat(CalcEngine::decimalFormat);
		UpdateDisplay();
	}
	else if (message.buttonWidget == _scientificRadioButton)
	{
		// Set the engine to scientific notation display mode
		_engine->SetFormat(CalcEngine::scientificFormat);
		UpdateDisplay();
	}
	else if (message.buttonWidget == _degreesRadioButton)
	{
		// Set the engine to degrees mode
		_engine->SetAngleUnits(CalcEngine::degrees);
	}
	else if (message.buttonWidget == _radiansRadioButton)
	{
		// Set the engine to radians mode
		_engine->SetAngleUnits(CalcEngine::radians);
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
		_engine->Button(data.key - '0');
		UpdateDisplay();
	}
	else switch (int(data.key))
	{
		// Certain function keys: send the keypress to the engine.
		case '+':
			_engine->Button(CalcEngine::plusButton);
			UpdateDisplay();
			break;
		case '-':
			_engine->Button(CalcEngine::minusButton);
			UpdateDisplay();
			break;
		case '*':
			_engine->Button(CalcEngine::multButton);
			UpdateDisplay();
			break;
		case '/':
			_engine->Button(CalcEngine::divButton);
			UpdateDisplay();
			break;
		case '.':
			_engine->Button(CalcEngine::pointButton);
			UpdateDisplay();
			break;
		case 'e':
			_engine->Button(CalcEngine::exponentButton);
			UpdateDisplay();
			break;
		case Glow::backspaceKey:
		case Glow::deleteKey:
			_engine->Button(CalcEngine::deleteButton);
			UpdateDisplay();
			break;
		case Glow::enterKey:
			_engine->Button(CalcEngine::enterButton);
			UpdateDisplay();
			break;
		case 'c':
			_engine->Button(CalcEngine::clearButton);
			UpdateDisplay();
			break;
		case '_':
			_engine->Button(CalcEngine::negateButton);
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
	_window = new GlowFixedSizeWidgetWindow("glowcalc",
		GlowWindow::autoPosition, GlowWindow::autoPosition, 255, 365);
	
	// Create receiver object
	_receiver = new CalcUIReceiver(engine, _window);
	
	// Pushbutton parameter block
	GlowPushButtonParams bparams;
	// All pushbuttons send their events to our receiver
	bparams.receiver = _receiver;
	
	// All the calculator keys have height 20
	bparams.height = 20;
	// Most calculator keys have width 40
	// We'll change this later when we create narrower keys
	bparams.width = 40;
	
	// Vertical position for bottom row of keys
	bparams.y = 225;
	
	// Set up clear button parameters
	bparams.text = "clr";
	// Horizontal position of key
	bparams.x = 10;
	// Refcon is for your miscellaneous use. Here we store the code that
	// should be sent to the engine when the calculator key is pressed.
	bparams.refcon = CalcEngine::clearButton;
	// Create button
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "0";
	bparams.x = 64;
	bparams.refcon = 0;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = ".";
	bparams.x = 111;
	bparams.refcon = CalcEngine::pointButton;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "+/-";
	bparams.x = 158;
	bparams.refcon = CalcEngine::negateButton;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "+";
	bparams.x = 205;
	bparams.refcon = CalcEngine::plusButton;
	new GlowPushButtonWidget(_window, bparams);
	
	// Next row
	bparams.y = 200;
	
	bparams.text = "del";
	bparams.x = 10;
	bparams.refcon = CalcEngine::deleteButton;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "1";
	bparams.x = 64;
	bparams.refcon = 1;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "2";
	bparams.x = 111;
	bparams.refcon = 2;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "3";
	bparams.x = 158;
	bparams.refcon = 3;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "-";
	bparams.x = 205;
	bparams.refcon = CalcEngine::minusButton;
	new GlowPushButtonWidget(_window, bparams);
	
	// Next row
	bparams.y = 175;
	
	bparams.text = "e";
	bparams.x = 10;
	bparams.refcon = CalcEngine::exponentButton;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "4";
	bparams.x = 64;
	bparams.refcon = 4;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "5";
	bparams.x = 111;
	bparams.refcon = 5;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "6";
	bparams.x = 158;
	bparams.refcon = 6;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "*";
	bparams.x = 205;
	bparams.refcon = CalcEngine::multButton;
	new GlowPushButtonWidget(_window, bparams);
	
	// Next row
	bparams.y = 150;
	
	bparams.text = "inv";
	bparams.x = 10;
	// We treat this button specially. Note that the receiver detects this
	// refcon explicitly and handles the inverse button
	bparams.refcon = -1;
	_receiver->_inverseButton = new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "7";
	bparams.x = 64;
	bparams.refcon = 7;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "8";
	bparams.x = 111;
	bparams.refcon = 8;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "9";
	bparams.x = 158;
	bparams.refcon = 9;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "/";
	bparams.x = 205;
	bparams.refcon = CalcEngine::divButton;
	new GlowPushButtonWidget(_window, bparams);
	
	// Next row
	bparams.y = 125;
	
	bparams.text = "swap";
	bparams.x = 111;
	bparams.refcon = CalcEngine::swapButton;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "pop";
	bparams.x = 158;
	bparams.refcon = CalcEngine::consumeButton;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "1/x";
	bparams.x = 205;
	bparams.refcon = CalcEngine::recipButton;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.text = "enter";
	bparams.width = 94;    // Create a wider button
	bparams.x = 10;
	bparams.refcon = CalcEngine::enterButton;
	new GlowPushButtonWidget(_window, bparams);
	
	// We have twelve more keys left. We only want six to be visible
	// at a time, however, depending on the state of the "inverse" key.
	// We'll do this by putting the keys into a non-drawing panel widget.
	// One panel will be hidden and the other visible at a time.
	
	// Panel widgets parameter block
	GlowPanelParams pparams;
	pparams.width = 235;
	pparams.height = 20;
	pparams.x = 10;
	pparams.y = 100;
	// Panel 1 is for the "normal" keys (e.g. exp, sin)
	_receiver->_panel1 = new GlowPanelWidget(_window, pparams);
	// Panel 2 is for the "inverse" keys (e.g. log, arcsin)
	_receiver->_panel2 = new GlowPanelWidget(_window, pparams);
	
	// Buttons inside panel 1
	// Note that x and y are given in local coordinates within the enclosing panel.
	// Because of this, you may choose to use panels to help you arrange groups
	// of widgets.
	bparams.y = 0;
	// These buttons are narrower than the others
	bparams.width = 35;
	
	bparams.text = "sqr";
	bparams.x = 0;
	bparams.refcon = CalcEngine::sqrButton;
	// Note that we're putting the widget inside the panel.
	new GlowPushButtonWidget(_receiver->_panel1, bparams);
	
	bparams.text = "exp";
	bparams.x = 40;
	bparams.refcon = CalcEngine::expButton;
	new GlowPushButtonWidget(_receiver->_panel1, bparams);
	
	bparams.text = "pow";
	bparams.x = 80;
	bparams.refcon = CalcEngine::powButton;
	new GlowPushButtonWidget(_receiver->_panel1, bparams);
	
	bparams.text = "sin";
	bparams.x = 120;
	bparams.refcon = CalcEngine::sinButton;
	new GlowPushButtonWidget(_receiver->_panel1, bparams);
	
	bparams.text = "cos";
	bparams.x = 160;
	bparams.refcon = CalcEngine::cosButton;
	new GlowPushButtonWidget(_receiver->_panel1, bparams);
	
	bparams.text = "tan";
	bparams.x = 200;
	bparams.refcon = CalcEngine::tanButton;
	new GlowPushButtonWidget(_receiver->_panel1, bparams);
	
	// Buttons inside panel 2
	bparams.text = "sqrt";
	bparams.x = 0;
	bparams.refcon = CalcEngine::sqrtButton;
	new GlowPushButtonWidget(_receiver->_panel2, bparams);
	
	bparams.text = "log";
	bparams.x = 40;
	bparams.refcon = CalcEngine::logButton;
	new GlowPushButtonWidget(_receiver->_panel2, bparams);
	
	bparams.text = "root";
	bparams.x = 80;
	bparams.refcon = CalcEngine::rootButton;
	new GlowPushButtonWidget(_receiver->_panel2, bparams);
	
	bparams.text = "asin";
	bparams.x = 120;
	bparams.refcon = CalcEngine::arcsinButton;
	new GlowPushButtonWidget(_receiver->_panel2, bparams);
	
	bparams.text = "acos";
	bparams.x = 160;
	bparams.refcon = CalcEngine::arccosButton;
	new GlowPushButtonWidget(_receiver->_panel2, bparams);
	
	bparams.text = "atan";
	bparams.x = 200;
	bparams.refcon = CalcEngine::arctanButton;
	new GlowPushButtonWidget(_receiver->_panel2, bparams);
	
	// Initial inverse state is false, so hide panel 2
	_receiver->_panel2->Hide();
	
	// Set up display. First, create a panel
	pparams.height = 80;
	pparams.width = 235;
	pparams.x = 10;
	pparams.y = 10;
	// The panel will appear lowered and will have a white background
	pparams.backColor = GlowColor::white;
	pparams.style = GlowPanelWidget::loweredStyle;
	GlowPanelWidget* displayPanel = new GlowPanelWidget(_window, pparams);
	
	// Put four labels in it for displaying the stack
	GlowLabelParams lparams;
	lparams.height = 15;
	lparams.width = 225;
	// Note again, x and y coordinates are local within the panel.
	lparams.x = 5;
	lparams.y = 5;
	lparams.font = GlowFont::fixed9by15;
	_receiver->_displayLabel3 = new GlowLabelWidget(displayPanel, lparams);
	lparams.y = 22;
	_receiver->_displayLabel2 = new GlowLabelWidget(displayPanel, lparams);
	lparams.y = 39;
	_receiver->_displayLabel1 = new GlowLabelWidget(displayPanel, lparams);
	lparams.y = 60;
	_receiver->_displayLabel0 = new GlowLabelWidget(displayPanel, lparams);
	_receiver->UpdateDisplay();
	
	// Set up a keyboard event filter. This will let us respond to keyboard
	// events even though we don't have any keyboard-listening widgets.
	_window->RegisterFilter(_receiver);
	
	// Separate calculator from auxiliary controls with a separator rule
	GlowSeparatorParams sparams;
	sparams.height = 2;
	sparams.width = 235;
	sparams.x = 10;
	sparams.y = 255;
	new GlowSeparatorWidget(_window, sparams);
	
	// Now we'll make some radio buttons at the bottom to control prefs.
	// On the left will be the display format (normal vs. scientific)
	// On the right will be angle units (degrees vs. radians)
	
	// First, we'll make a label for the first group of radio buttons
	lparams.height = 15;
	lparams.width = 100;
	lparams.x = 10;
	lparams.y = 265;
	// Reset the font to helvetica, because we changed the font to fixed9by15
	// the last time we used the parameter block
	lparams.font = GlowFont::helvetica12;
	lparams.text = "Format:";
	new GlowLabelWidget(_window, lparams);
	
	// Now we make a radio group to put the radio buttons into. A radio group
	// is a panel with some extra wiring that lets it keep track of a bunch
	// of radio buttons
	GlowRadioGroupParams gparams;
	gparams.height = 40;
	gparams.width = 120;
	gparams.y = 280;
	gparams.x = 10;
	// Make sure our receiver receives messages from this radio group
	gparams.receiver = _receiver;
	GlowRadioGroupWidget* radioGroup = new GlowRadioGroupWidget(_window, gparams);
	
	// Add radio buttons to the radio group. Remember that x and y are given
	// in local coordinates within the radio group panel.
	GlowRadioButtonParams rparams;
	rparams.height = 16;
	rparams.width = 100;
	rparams.x = 20;
	rparams.y = 4;
	rparams.text = "Decimal";
	_receiver->_decimalRadioButton =
		new GlowRadioButtonWidget(radioGroup, rparams);
	// Crop the width of the widget to tightly enclose the box and label
	_receiver->_decimalRadioButton->CropWidth();
	rparams.y = 24;
	rparams.text = "Scientific";
	_receiver->_scientificRadioButton =
		new GlowRadioButtonWidget(radioGroup, rparams);
	_receiver->_scientificRadioButton->CropWidth();
	
	// Angle units radio buttons
	lparams.height = 15;
	lparams.width = 90;
	lparams.x = 130;
	lparams.y = 265;
	lparams.text = "Angles:";
	new GlowLabelWidget(_window, lparams);
	
	// Make the radio group
	gparams.x = 130;
	radioGroup = new GlowRadioGroupWidget(_window, gparams);
	
	//  ... and add the buttons
	rparams.y = 4;
	rparams.text = "Degrees";
	_receiver->_degreesRadioButton =
		new GlowRadioButtonWidget(radioGroup, rparams);
	_receiver->_degreesRadioButton->CropWidth();
	rparams.y = 24;
	rparams.text = "Radians";
	_receiver->_radiansRadioButton =
		new GlowRadioButtonWidget(radioGroup, rparams);
	_receiver->_radiansRadioButton->CropWidth();
	
	// Finally, let's add a quit button and an about button
	bparams.height = 25;
	bparams.width = 80;
	bparams.x = 10;
	bparams.y = 330;
	bparams.text = "Quit";
	// We'll detect this refcon and handle this button specially
	bparams.refcon = -2;
	new GlowPushButtonWidget(_window, bparams);
	
	bparams.x = 100;
	bparams.text = "About";
	bparams.refcon = -3;
	new GlowPushButtonWidget(_window, bparams);
}


// Delete the window and receiver if the UI object is destructed.
CalcUI::~CalcUI()
{
	delete _window;
	delete _receiver;
}
