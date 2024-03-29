/*
===============================================================================

	FILE:  glowTextFieldWindow.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Text field window for GLOW
	
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
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 1.1.2dev  (7 August 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


#ifndef GLOW_TEXTFIELDWINDOW__H
#define GLOW_TEXTFIELDWINDOW__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <string>

#ifndef GLOW_WIDGET__H
	#include "glowWidget.h"
#endif
#ifndef GLOW_UTILITIES__H
	#include "glowUtilities.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowLabelWidget;
class GlowTextFieldWidget;
class GlowPushButtonWidget;
class GlowTextFieldWindow;
class GlowWidgetMapToPushButtonFilter;


/*
===============================================================================
	CLASS GlowTextFieldWindowMessage
	
	Action for text field window
===============================================================================
*/

class GlowTextFieldWindowMessage
{
	public:
	
		GlowTextFieldWindow* window;
		GLOW_STD::string text;
		int response;
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};


typedef TReceiver<const GlowTextFieldWindowMessage&> GlowTextFieldWindowReceiver;


/*
===============================================================================
	CLASS GlowTextFieldWindowParams
	
	Text field window params
===============================================================================
*/

class GlowTextFieldWindowParams
{
	public:
	
		const char* windowTitle;
		int x;
		int y;
		const char* labelText;
		GlowFont labelFont;
		const char* fieldText;
		GlowFont fieldFont;
		int fieldWidth;
		char fieldHideCharacter;
		GlowFont buttonFont;
		const char* buttonLabels;
		int windowSpacing;
		int enterButton;
		int escapeButton;
		GlowTextFieldWindowReceiver* receiver;
		GlowColor backColor;
		GlowColor labelColor;
		GlowColor fieldBackColor;
		GlowColor fieldHiliteBackColor;
		GlowColor fieldTextColor;
		GlowColor fieldHiliteTextColor;
		GlowColor fieldCaretColor;
	
		static GlowTextFieldWindowParams defaults;
	
		GlowTextFieldWindowParams();
	
	protected:
	
		GlowTextFieldWindowParams(bool);
};


/*
===============================================================================
	CLASS GlowTextFieldWindow
	
	Text field window with buttons
===============================================================================
*/

class GlowTextFieldWindow :
	public GlowFixedSizeWidgetWindow
{
	friend class Glow_TextFieldWindowButton;
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowTextFieldWindow();
		inline GlowTextFieldWindow(
			const GlowTextFieldWindowParams& params);
		void Init(
			const GlowTextFieldWindowParams& params);
		inline GlowTextFieldWindow(
			const char* windowTitle,
			int x,
			int y,
			const char* labelText,
			int fieldWidth,
			const char* fieldText,
			const char* buttonLabels,
			GlowTextFieldWindowReceiver* receiver);
		void Init(
			const char* windowTitle,
			int x,
			int y,
			const char* labelText,
			int fieldWidth,
			const char* fieldText,
			const char* buttonLabels,
			GlowTextFieldWindowReceiver* receiver);
		
		virtual ~GlowTextFieldWindow();
		
		inline void SetEnterButton(
			int num);
		inline void SetEscapeButton(
			int num);
	
	public:
	
		inline TSender<const GlowTextFieldWindowMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnButtonPressed(
			int response,
			const char* text,
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		TSender<const GlowTextFieldWindowMessage&> sender_;
		GlowTextFieldWidget* field_;
		GLOW_STD::vector<GlowPushButtonWidget*> buttons_;
		GlowWidgetMapToPushButtonFilter* enterFilter_;
		GlowWidgetMapToPushButtonFilter* escapeFilter_;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowTextFieldWindow.inl.h"


#endif

