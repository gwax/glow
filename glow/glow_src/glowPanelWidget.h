/*
===============================================================================

	FILE:  glowPanelWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Panel widget for GLOW
	
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


#ifndef GLOW_PANELWIDGET__H
#define GLOW_PANELWIDGET__H


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


/*
===============================================================================
	STRUCT GlowPanelParams
	
	Panel widget params
===============================================================================
*/

class GlowPanelParams :
	public GlowWidgetParams
{
	public:
	
		int style;
		GlowColor backColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowPanelParams defaults;
		
		GlowPanelParams();
	
	protected:
	
		GlowPanelParams(bool);
};


/*
===============================================================================
	CLASS GlowPanelWidget
	
	Panel widget
===============================================================================
*/

class GlowPanelWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum {
			transparentStyle = 0,
			plainStyle = 1,
			etchedStyle = 2,
			raisedStyle = 3,
			loweredStyle = 4
		};
	
	public:
	
		inline GlowPanelWidget();
		inline GlowPanelWidget(
			GlowWidgetRoot* root,
			const GlowPanelParams& params);
		inline GlowPanelWidget(
			GlowWidget* parent,
			const GlowPanelParams& params);
		
		inline void Init(
			GlowWidgetRoot* root,
			const GlowPanelParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowPanelParams& params);
	
	public:
	
		inline int GetStyle() const;
		inline void SetStyle(
			int style);
		
		inline GlowColor GetBackColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBackColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
	
	
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
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowPanelParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _style;
		
		GlowColor _backColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	protected:
	
		virtual void OnWidgetPaint();
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowPanelWidget.inl.h"


#endif
