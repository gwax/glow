/*
===============================================================================

	FILE:  glowPushButtonWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Pushbutton widget for VGLUI
	
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
	
		The GLOW Toolkit -- version 0.95  (27 March 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
	
===============================================================================
*/


#ifndef GLOW_PUSHBUTTONWIDGET__H
#define GLOW_PUSHBUTTONWIDGET__H


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


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowPushButtonWidget;


/*
===============================================================================
	CLASS GlowPushButtonMessage
	
	Action for pushbutton
===============================================================================
*/

class GlowPushButtonMessage
{
	public:
	
		GlowPushButtonWidget* widget;
		int mouseButton;
		int modifiers;
};


typedef TReceiver<const GlowPushButtonMessage&> GlowPushButtonReceiver;


/*
===============================================================================
	STRUCT GlowPushButtonParams
	
	Pushbutton params
===============================================================================
*/

class GlowPushButtonParams :
	public GlowWidgetParams
{
	public:
	
		const char* label;
		GlowFont font;
		GlowPushButtonReceiver* receiver;
		GlowColor boxColor;
		GlowColor textColor;
		GlowColor hiliteBoxColor;
		GlowColor hiliteTextColor;
		GlowColor disableBoxColor;
		GlowColor disableTextColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowPushButtonParams defaults;
		
		GlowPushButtonParams();
	
	protected:
	
		GlowPushButtonParams(bool);
};


/*
===============================================================================
	CLASS GlowPushButtonWidget
	
	Pushbutton widget
===============================================================================
*/

class GlowPushButtonWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowPushButtonWidget();
		inline GlowPushButtonWidget(
			GlowWidget* parent,
			const GlowPushButtonParams& params);
		inline GlowPushButtonWidget(
			GlowWidgetRoot* root,
			const GlowPushButtonParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowPushButtonParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowPushButtonParams& params);
		
		virtual ~GlowPushButtonWidget();
	
	public:
	
		inline const char* GetLabel() const;
		void SetLabel(
			const char* label);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline GlowColor GetBoxColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetDisableBoxColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBoxColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetDisableBoxColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
		
		inline TSender<const GlowPushButtonMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual int OnAutoPack(
			int hSize,
			int vSize,
			int hOption,
			int vOption,
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
		
		virtual void OnPressed(
			int button,
			int modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Subclsss-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowPushButtonParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		char* _label;
		GlowFont _font;
		int _labelWidth;
		bool _down;
		bool _inside;
		int _button;
		int _modifiers;
		TSender<const GlowPushButtonMessage&> _sender;
		
		GlowColor _boxColor;
		GlowColor _textColor;
		GlowColor _hiliteBoxColor;
		GlowColor _hiliteTextColor;
		GlowColor _disableBoxColor;
		GlowColor _disableTextColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	protected:
	
		virtual void OnWidgetPaint();
		
		virtual void OnWidgetMouseDown(
			int button,
			int x,
			int y,
			int modifiers);
		virtual void OnWidgetMouseUp(
			int button,
			int x,
			int y,
			int modifiers);
		virtual void OnWidgetMouseDrag(
			int x,
			int y);
};


/*
===============================================================================
	CLASS GlowDismissPushButtonWidget
	
	Pushbutton that dismisses a window
	Useful if the pushbutton is embedded in the window to be dismissed,
	because doing a delete <window> from one of the pushbutton's receivers
	is unsafe.
===============================================================================
*/

class GlowDismissPushButtonWidget :
	public GlowPushButtonWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowDismissPushButtonWidget();
		inline GlowDismissPushButtonWidget(
			GlowWidget* parent,
			const GlowPushButtonParams& params,
			GlowComponent* todismiss);
		inline GlowDismissPushButtonWidget(
			GlowWidgetRoot* root,
			const GlowPushButtonParams& params,
			GlowComponent* todismiss);
		inline void Init(
			GlowWidget* parent,
			const GlowPushButtonParams& params,
			GlowComponent* todismiss);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowPushButtonParams& params,
			GlowComponent* todismiss);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnPressed(
			int mouseButton,
			int modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowComponent* _todismiss;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowPushButtonWidget.inl.h"


#endif

