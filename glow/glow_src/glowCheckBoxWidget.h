/*
===============================================================================

	FILE:  glowCheckBoxWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Check box widget for GLOW
	
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


#ifndef GLOW_CHECKBOXWIDGET__H
#define GLOW_CHECKBOXWIDGET__H


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


class GlowCheckBoxWidget;


/*
===============================================================================
	CLASS GlowCheckBoxMessage
	
	Action for checkbox toggle
===============================================================================
*/

class GlowCheckBoxMessage
{
	public:
	
		GlowCheckBoxWidget* widget;
		int state;
		int mouseButton;
		int modifiers;
};


typedef TReceiver<const GlowCheckBoxMessage&> GlowCheckBoxReceiver;


/*
===============================================================================
	STRUCT GlowCheckBoxParams
	
	Check box params
===============================================================================
*/

class GlowCheckBoxParams :
	public GlowWidgetParams
{
	public:
	
		const char* label;
		GlowFont font;
		int state;
		int behavior;
		int spacing;
		GlowCheckBoxReceiver* receiver;
		GlowColor boxColor;
		GlowColor textColor;
		GlowColor checkColor;
		GlowColor hiliteBoxColor;
		GlowColor hiliteTextColor;
		GlowColor hiliteCheckColor;
		GlowColor disableBoxColor;
		GlowColor disableTextColor;
		GlowColor disableCheckColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowCheckBoxParams defaults;
		
		GlowCheckBoxParams();
	
	protected:
	
		GlowCheckBoxParams(bool);
};


/*
===============================================================================
	CLASS GlowCheckBoxWidget
	
	Check box widget
===============================================================================
*/

class GlowCheckBoxWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const int off = 0;
		static const int on = 1;
		static const int half = 2;
		
		static const int defaultBehavior = 0;
		static const int onFollowsOff = 0;
		static const int halfFollowsOff = 1;
		static const int offFollowsOn = 0;
		static const int halfFollowsOn = 2;
		static const int onFollowsHalf = 0;
		static const int offFollowsHalf = 4;
	
	public:
	
		inline GlowCheckBoxWidget();
		inline GlowCheckBoxWidget(
			GlowWidget* parent,
			const GlowCheckBoxParams& params);
		inline GlowCheckBoxWidget(
			GlowWidgetRoot* root,
			const GlowCheckBoxParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowCheckBoxParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowCheckBoxParams& params);
		
		virtual ~GlowCheckBoxWidget();
	
	public:
	
		inline const char* GetLabel() const;
		void SetLabel(
			const char* label);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline int GetSpacing() const;
		inline void SetSpacing(
			int spacing);
		
		inline int GetBehavior() const;
		inline void SetBehavior(
			int behavior = defaultBehavior);
		
		inline int GetState() const;
		inline void SetState(
			int state);
		
		void CropWidth();
		
		inline GlowColor GetBoxColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetCheckColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetHiliteCheckColor() const;
		inline GlowColor GetDisableBoxColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableCheckColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBoxColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetCheckColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetHiliteCheckColor(
			GlowColor c);
		inline void SetDisableBoxColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableCheckColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
		
		inline TSender<const GlowCheckBoxMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Subclsss-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowCheckBoxParams& params);
	
	
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
		
		virtual void OnToggled(
			int mouseButton,
			int modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _state;
		char* _label;
		GlowFont _font;
		int _behavior;
		bool _down;
		bool _inside;
		int _button;
		int _modifiers;
		int _labelWidth;
		int _spacing;
		TSender<const GlowCheckBoxMessage&> _sender;
		
		GlowColor _boxColor;
		GlowColor _textColor;
		GlowColor _checkColor;
		GlowColor _hiliteBoxColor;
		GlowColor _hiliteTextColor;
		GlowColor _hiliteCheckColor;
		GlowColor _disableBoxColor;
		GlowColor _disableTextColor;
		GlowColor _disableCheckColor;
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
*/

GLOW_NAMESPACE_END


#include "glowCheckBoxWidget.inl.h"


#endif

