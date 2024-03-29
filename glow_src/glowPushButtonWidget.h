/*
===============================================================================

	FILE:  glowPushButtonWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Pushbutton widget for GLOW
	
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

#ifndef GLOW_BUTTONWIDGET__H
	#include "glowButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowPushButtonWidget;
class GlowPushButtonMessage;
class GlowPushButtonParams;
typedef TReceiver<const GlowPushButtonMessage&> GlowPushButtonReceiver;


/*
===============================================================================
	CLASS GlowPushButtonWidget
	
	Pushbutton widget
===============================================================================
*/

class GlowPushButtonWidget :
	public GlowButtonWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Behavior
		{
			normalBehavior = 0,
			toggleBehavior = 1,
			stickDownBehavior = 2
		};
	
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
	
		inline const char* GetText() const;
		void SetText(
			const char* text);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline Behavior GetBehavior() const;
		inline void SetBehavior(
			Behavior behavior);
		
		inline GlowColor GetUpTextColor() const;
		inline GlowColor GetDownTextColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetDisableTextColor() const;
		
		inline void SetUpTextColor(
			GlowColor c);
		inline void SetDownTextColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		
		inline TSender<const GlowPushButtonMessage&>& Notifier();
		
		void ToggleState();
		inline void Hit(
			Glow::MouseButton button = Glow::leftButton,
			Glow::Modifiers modifiers = Glow::noModifier);
	
	
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
			Glow::MouseButton button,
			Glow::Modifiers modifiers);
	
	
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
	
		Behavior behavior_;
		char* label_;
		GlowFont font_;
		int labelWidth_;
		TSender<const GlowPushButtonMessage&> sender_;
		
		GlowColor upTextColor_;
		GlowColor downTextColor_;
		GlowColor hiliteTextColor_;
		GlowColor disableTextColor_;
	
	protected:
	
		virtual void OnEvent(
			GlowButtonWidget::Event event,
			Glow::MouseButton button,
			Glow::Modifiers modifiers);
		
		virtual void OnWidgetPaint();
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
		
		inline GlowComponent* GetToDismiss() const;
		inline void SetToDismiss(
			GlowComponent* todismiss);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnHit(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowComponent* todismiss_;
};


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
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};


/*
===============================================================================
	CLASS GlowPushButtonParams
	
	Pushbutton params
===============================================================================
*/

class GlowPushButtonParams :
	public GlowButtonParams
{
	public:
	
		const char* text;
		GlowFont font;
		GlowPushButtonWidget::Behavior behavior;
		GlowPushButtonReceiver* receiver;
		GlowColor upTextColor;
		GlowColor downTextColor;
		GlowColor hiliteTextColor;
		GlowColor disableTextColor;
		
		static GlowPushButtonParams defaults;
		
		GlowPushButtonParams();
	
	protected:
	
		GlowPushButtonParams(bool);
};


/*
===============================================================================
	Useful widget keyboard filter
===============================================================================
*/

class GlowWidgetMapToPushButtonFilter :
	public GlowWidgetKeyboardFilter
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowWidgetMapToPushButtonFilter(
			GlowPushButtonWidget* widget,
			Glow::KeyCode key,
			Glow::Modifiers modifiers = Glow::noModifier);
		
		inline void SetPushButton(
			GlowPushButtonWidget* widget);
		inline GlowPushButtonWidget* GetPushButton() const;
		inline void SetKeyCode(
			Glow::KeyCode key);
		inline Glow::KeyCode GetKeyCode() const;
		inline void SetModifiers(
			Glow::Modifiers modifiers);
		inline Glow::Modifiers GetModifiers() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual bool OnFilter(
			GlowWidgetKeyboardData& data);
	
	private:
	
		GlowPushButtonWidget* widget_;
		Glow::KeyCode keyCode_;
		Glow::Modifiers modifiers_;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowPushButtonWidget.inl.h"


#endif

