/*
===============================================================================

	FILE:  glowMessageWindow.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Message window for GLOW
	
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
		License along with this library; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 0.9.8  (23 May 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
		10 April 2000 -- DA -- Version 0.9.6 update
		1 May 2000 -- DA -- Version 0.9.7 update
		23 May 2000 -- DA -- Version 0.9.8 update

===============================================================================
*/


#ifndef GLOW_MESSAGEWINDOW__H
#define GLOW_MESSAGEWINDOW__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_WIDGET__H
	#include "glowWidget.h"
#endif
#ifndef GLOW_UTILITIES__H
	#include "glowUtilities.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowLabel;
class GlowPushButtonWidget;
class GlowMessageWindow;
class GlowWidgetMapToPushButtonFilter;


/*
===============================================================================
	CLASS GlowMessageWindowMessage
	
	Action for buttons window
===============================================================================
*/

struct GlowMessageWindowMessage
{
	GlowMessageWindow* window;
	int response;
	Glow::MouseButton mouseButton;
	Glow::Modifiers modifiers;
};


typedef TReceiver<const GlowMessageWindowMessage&> GlowMessageWindowReceiver;


/*
===============================================================================
	STRUCT GlowMessageWindowParams
	
	Message window params
===============================================================================
*/

struct GlowMessageWindowParams
{
	const char* windowTitle;
	int x;
	int y;
	const char* text;
	GlowFont textFont;
	GlowFont buttonFont;
	const char* buttonLabels;
	int windowSpacing;
	int enterButton;
	int escapeButton;
	GlowMessageWindowReceiver* receiver;
	GlowColor backColor;
	GlowColor textColor;
	
	static GlowMessageWindowParams defaults;
	
	GlowMessageWindowParams();
	GlowMessageWindowParams(bool);
};


/*
===============================================================================
	CLASS GlowMessageWindow
	
	Message window with buttons
===============================================================================
*/

class GlowMessageWindow :
	public GlowFixedSizeWidgetWindow
{
	friend class Glow_MessageWindowButton;
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowMessageWindow();
		inline GlowMessageWindow(
			const GlowMessageWindowParams& params);
		void Init(
			const GlowMessageWindowParams& params);
		inline GlowMessageWindow(
			const char* windowTitle,
			int x,
			int y,
			const char* text,
			const char* buttonLabels,
			GlowMessageWindowReceiver* receiver);
		void Init(
			const char* windowTitle,
			int x,
			int y,
			const char* text,
			const char* buttonLabels,
			GlowMessageWindowReceiver* receiver);
		
		virtual ~GlowMessageWindow();
		
		inline void SetEnterButton(
			int num);
		inline void SetEscapeButton(
			int num);
	
	public:
	
		inline TSender<const GlowMessageWindowMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnButtonPressed(
			int response,
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		TSender<const GlowMessageWindowMessage&> _sender;
		GLOW_STD::vector<GlowPushButtonWidget*> _buttons;
		GlowWidgetMapToPushButtonFilter* _enterFilter;
		GlowWidgetMapToPushButtonFilter* _escapeFilter;
};


/*		23 May 2000 -- DA -- Version 0.9.8 update

===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowMessageWindow.inl.h"


#endif

