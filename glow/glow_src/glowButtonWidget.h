/*
===============================================================================

	FILE:  glowButtonWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Generalized button widget for GLOW
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
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
	
		The GLOW Toolkit -- version 1.1.1dev  (24 July 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


#ifndef GLOW_BUTTONWIDGET__H
#define GLOW_BUTTONWIDGET__H


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


class GlowButtonWidget;
class GlowButtonParams;


/*
===============================================================================
	CLASS GlowButtonWidget
	
	Generalized button widget
===============================================================================
*/

class GlowButtonWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Event
		{
			noEvent = 0,
			pressEvent = 1,
			releaseInsideEvent = 2,
			releaseOutsideEvent = 3,
			enterEvent = 4,
			exitEvent = 5
		};
	
	public:
	
		inline GlowButtonWidget();
		inline GlowButtonWidget(
			GlowWidget* parent,
			const GlowButtonParams& params);
		inline GlowButtonWidget(
			GlowWidgetRoot* root,
			const GlowButtonParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowButtonParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowButtonParams& params);
	
	public:
	
		inline bool IsDown() const;
		inline void SetDown(
			bool down);
		inline bool IsHilited() const;
		
		inline GlowColor GetUpBoxColor() const;
		inline GlowColor GetDownBoxColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetDisableUpBoxColor() const;
		inline GlowColor GetDisableDownBoxColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetUpBoxColor(
			GlowColor c);
		inline void SetDownBoxColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetDisableUpBoxColor(
			GlowColor c);
		inline void SetDisableDownBoxColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnEvent(
			GlowButtonWidget::Event event,
			Glow::MouseButton button,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Subclsss-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowButtonParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		bool state_;
		bool down_;
		bool inside_;
		Glow::MouseButton button_;
		Glow::Modifiers modifiers_;
		
		GlowColor upBoxColor_;
		GlowColor downBoxColor_;
		GlowColor hiliteBoxColor_;
		GlowColor disableUpBoxColor_;
		GlowColor disableDownBoxColor_;
		GlowColor disableOutlineColor_;
		GlowColor lightBevelColor_;
		GlowColor darkBevelColor_;
	
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
	CLASS GlowButtonParams
	
	Generalized button params
===============================================================================
*/

class GlowButtonParams :
	public GlowWidgetParams
{
	public:
	
		bool down;
		GlowColor upBoxColor;
		GlowColor downBoxColor;
		GlowColor hiliteBoxColor;
		GlowColor disableUpBoxColor;
		GlowColor disableDownBoxColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowButtonParams defaults;
		
		GlowButtonParams();
	
	protected:
	
		GlowButtonParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowButtonWidget.inl.h"


#endif

