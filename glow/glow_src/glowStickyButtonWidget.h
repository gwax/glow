/*
===============================================================================

	FILE:  glowStickyButtonWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Stickybutton widget for GLOW
	
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
	
		The GLOW Toolkit -- version 0.9.6  (10 April 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
		10 April 2000 -- DA -- Version 0.9.6 update
	
===============================================================================
*/


#ifndef GLOW_STICKYBUTTONWIDGET__H
#define GLOW_STICKYBUTTONWIDGET__H


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


class GlowStickyButtonWidget;
class GlowStickyButtonMessage;
class GlowStickyButtonParams;
typedef TReceiver<const GlowStickyButtonMessage&> GlowStickyButtonReceiver;


/*
===============================================================================
	CLASS GlowStickyButtonWidget
	
	Stickybutton widget
===============================================================================
*/

class GlowStickyButtonWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Behavior
		{
			noBehavior = 0,
			toggleBehavior = 1,
			stickDownBehavior = 2
		};
	
	public:
	
		inline GlowStickyButtonWidget();
		inline GlowStickyButtonWidget(
			GlowWidget* parent,
			const GlowStickyButtonParams& params);
		inline GlowStickyButtonWidget(
			GlowWidgetRoot* root,
			const GlowStickyButtonParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowStickyButtonParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowStickyButtonParams& params);
		
		virtual ~GlowStickyButtonWidget();
	
	public:
	
		inline const char* GetText() const;
		void SetText(
			const char* text);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline Behavior GetBehavior() const;
		inline void SetBehavior(
			Behavior behavior);
		
		inline bool GetState() const;
		inline void SetState(
			bool state);
		void ToggleState();
		
		inline void Hit(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
		
		inline GlowColor GetUpBoxColor() const;
		inline GlowColor GetUpTextColor() const;
		inline GlowColor GetDownBoxColor() const;
		inline GlowColor GetDownTextColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetDisableUpBoxColor() const;
		inline GlowColor GetDisableDownBoxColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetUpBoxColor(
			GlowColor c);
		inline void SetUpTextColor(
			GlowColor c);
		inline void SetDownBoxColor(
			GlowColor c);
		inline void SetDownTextColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetDisableUpBoxColor(
			GlowColor c);
		inline void SetDisableDownBoxColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
		
		inline TSender<const GlowStickyButtonMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual AutoPackError OnAutoPack(
			int hSize,
			int vSize,
			AutoPackOptions hOption,
			AutoPackOptions vOption,
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
		
		virtual void OnHit(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Subclsss-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowStickyButtonParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		bool _state;
		Behavior _behavior;
		char* _label;
		GlowFont _font;
		int _labelWidth;
		bool _down;
		bool _inside;
		Glow::MouseButton _button;
		Glow::Modifiers _modifiers;
		TSender<const GlowStickyButtonMessage&> _sender;
		
		GlowColor _upBoxColor;
		GlowColor _upTextColor;
		GlowColor _downBoxColor;
		GlowColor _downTextColor;
		GlowColor _hiliteBoxColor;
		GlowColor _hiliteTextColor;
		GlowColor _disableUpBoxColor;
		GlowColor _disableDownBoxColor;
		GlowColor _disableTextColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	protected:
	
		virtual void OnWidgetPaint();
		
		virtual void OnWidgetMouseDown(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseUp(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseDrag(
			int x,
			int y);
};


/*
===============================================================================
	CLASS GlowStickyButtonMessage
	
	Action for stickybutton
===============================================================================
*/

class GlowStickyButtonMessage
{
	public:
	
		GlowStickyButtonWidget* widget;
		bool state;
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};


/*
===============================================================================
	STRUCT GlowStickyButtonParams
	
	Stickybutton params
===============================================================================
*/

class GlowStickyButtonParams :
	public GlowWidgetParams
{
	public:
	
		const char* text;
		GlowFont font;
		bool state;
		GlowStickyButtonWidget::Behavior behavior;
		GlowStickyButtonReceiver* receiver;
		GlowColor upBoxColor;
		GlowColor upTextColor;
		GlowColor downBoxColor;
		GlowColor downTextColor;
		GlowColor hiliteBoxColor;
		GlowColor hiliteTextColor;
		GlowColor disableUpBoxColor;
		GlowColor disableDownBoxColor;
		GlowColor disableTextColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowStickyButtonParams defaults;
		
		GlowStickyButtonParams();
	
	protected:
	
		GlowStickyButtonParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowStickyButtonWidget.inl.h"


#endif

