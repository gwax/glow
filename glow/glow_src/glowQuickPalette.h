/*
===============================================================================

	FILE:  glowQuickPalette.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Quick palette API for GLOW
	
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


#ifndef GLOW_QUICKPALETTE__H
#define GLOW_QUICKPALETTE__H


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
#ifndef GLOW_CHECKBOXWIDGET__H
	#include "glowCheckBoxWidget.h"
#endif
#ifndef GLOW_PANELWIDGET__H
	#include "glowPanelWidget.h"
#endif
#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif
#ifndef GLOW_MENUBUTTONWIDGET__H
	#include "glowMenuButtonWidget.h"
#endif
#ifndef GLOW_PUSHBUTTONWIDGET__H
	#include "glowPushButtonWidget.h"
#endif
#ifndef GLOW_RADIOBUTTONWIDGET__H
	#include "glowRadioButtonWidget.h"
#endif
#ifndef GLOW_SSCROLLBARWIDGET__H
	#include "glowScrollBarWidget.h"
#endif
#ifndef GLOW_SEPARATORWIDGET__H
	#include "glowSeparatorWidget.h"
#endif
#ifndef GLOW_SLIDERWIDGET__H
	#include "glowSliderWidget.h"
#endif
#ifndef GLOW_TEXTFIELDWIDGET__H
	#include "glowTextFieldWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowQuickPaletteBase;
class GlowQuickPanelWidget;
class GlowQuickRadioGroupWidget;


/*
===============================================================================
	CLASS GlowQuickRadioGroupWidget
	
	Quick radio button group
===============================================================================
*/

class GlowQuickRadioGroupWidget :
	public GlowRadioGroupWidget
{
	friend class GlowQuickPanelWidget;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const int vertical = 0;
		static const int horizontal = 1;
	
	public:
	
		GlowRadioButtonWidget* AddRadioButton(
			const char* label);
	
	
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
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowQuickRadioGroupWidget(
			GlowWidget* parent,
			GlowRadioButtonReceiver* receiver,
			int arrangement,
			int spacing);
	
	private:
	
		int _arrangement;
		int _spacing;
};


/*
===============================================================================
	CLASS GlowQuickPanelWidget
	
	Quick panel
===============================================================================
*/

class GlowQuickPanelWidget :
	public GlowPanelWidget
{
	friend class GlowQuickPaletteBase;
	friend class GlowQuickPaletteWindow;
	friend class GlowQuickPaletteSubwindow;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const int vertical = 0;
		static const int horizontal = 1;
		
		static const int alignTop = 0;
		static const int alignLeft = 0;
		static const int alignCenter = 1;
		static const int alignBottom = 2;
		static const int alignRight = 2;
		static const int alignExpand = 4;
		
		static const int parentSetting = -1;
	
	public:
	
		GlowQuickPanelWidget* AddPanel(
			int style = plainStyle,
			const char* label = 0,
			int arrangement = parentSetting,
			int alignment = parentSetting,
			int spacing = parentSetting,
			int hmargin = 10,
			int vmargin = 10);
		inline GlowQuickPanelWidget* AddArrangingPanel(
			int arrangement = parentSetting,
			int alignment = parentSetting,
			int spacing = parentSetting,
			int inset = 0);
		GlowPanelWidget* AddFixedPanel(
			int width,
			int height,
			int style = GlowPanelWidget::plainStyle);
		GlowSeparatorWidget* AddSeparator(
			int style = GlowSeparatorWidget::valleyStyle);
		GlowPushButtonWidget* AddPushButton(
			const char* label,
			GlowPushButtonReceiver* receiver = 0);
		GlowMenuButtonWidget* AddMenuButton(
			const char* label,
			GlowMenu* menu,
			int iconType = GlowMenuButtonWidget::menuIcon);
		GlowLabeledPopupMenuWidget* AddPopupMenu(
			const char* label,
			GlowPopupMenuReceiver* receiver = 0);
		GlowCheckBoxWidget* AddCheckBox(
			const char* label,
			int initialValue = GlowCheckBoxWidget::off,
			GlowCheckBoxReceiver* receiver = 0);
		GlowQuickRadioGroupWidget* AddRadioGroup(
			int arrangement = GlowQuickRadioGroupWidget::vertical,
			int spacing = 3,
			GlowRadioButtonReceiver* receiver = 0);
		GlowLabeledSliderWidget* AddSlider(
			float min,
			float max,
			float initial,
			int options = GlowSliderWidget::defaultOptions,
			int numTicks = 2,
			const char* valueLabel = 0,
			const char* label = 0,
			GlowSliderReceiver* receiver = 0);
		GlowScrollBarWidget* AddScrollBar(
			long min,
			long max,
			long span,
			long initialTop,
			GlowScrollBarReceiver* receiver = 0);
		GlowLabeledTextFieldWidget* AddTextField(
			int width,
			const char* text = "",
			const char* label = 0);
		GlowLabeledHiddenTextFieldWidget* AddHiddenTextField(
			int width,
			const char* text = "",
			char hideCharacter = '#',
			const char* label = 0);
		GlowLabelWidget* AddLabel(
			const char* text);
		
		inline int GetArrangement() const;
		inline int GetAlignment() const;
		inline int GetSpacing() const;
		inline int GetHMargin() const;
		inline int GetVMargin() const;
		inline void SetArrangement(
			int value);
		inline void SetAlignment(
			int value);
		inline void SetSpacing(
			int value);
		inline void SetHMargin(
			int value);
		inline void SetVMargin(
			int value);
	
	
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
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowQuickPanelWidget(
			GlowWidgetRoot* root,
			GlowQuickPanelWidget* parent,
			int style,
			const char* label,
			int arrangement,
			int alignment,
			int spacing,
			int hmargin,
			int vmargin);
	
	private:
	
		GlowLabelWidget* _label;
		int _arrangement;
		int _alignment;
		int _spacing;
		int _hmargin;
		int _vmargin;
};


/*
===============================================================================
	CLASS GlowQuickPaletteBase
	
	Quick palette base class
===============================================================================
*/

class GlowQuickPaletteBase
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const int vertical = 0;
		static const int horizontal = 1;
		
		static const int alignTop = 0;
		static const int alignLeft = 0;
		static const int alignCenter = 1;
		static const int alignBottom = 2;
		static const int alignRight = 2;
		static const int alignExpand = 4;
		
		static const int parentSetting = -1;
	
	public:
	
		inline GlowQuickPanelWidget* AddPanel(
			int style = GlowQuickPanelWidget::plainStyle,
			const char* label = 0,
			int arrangement = parentSetting,
			int alignment = parentSetting,
			int spacing = parentSetting,
			int hmargin = 10,
			int vmargin = 10);
		inline GlowQuickPanelWidget* AddArrangingPanel(
			int arrangement = parentSetting,
			int alignment = parentSetting,
			int spacing = parentSetting,
			int inset = 0);
		inline GlowPanelWidget* AddFixedPanel(
			int width,
			int height,
			int style = GlowPanelWidget::plainStyle);
		inline GlowSeparatorWidget* AddSeparator(
			int style = GlowSeparatorWidget::valleyStyle);
		inline GlowPushButtonWidget* AddPushButton(
			const char* label,
			GlowPushButtonReceiver* receiver = 0);
		inline GlowMenuButtonWidget* AddMenuButton(
			const char* label,
			GlowMenu* menu,
			int iconType = GlowMenuButtonWidget::menuIcon);
		inline GlowLabeledPopupMenuWidget* AddPopupMenu(
			const char* label,
			GlowPopupMenuReceiver* receiver = 0);
		inline GlowCheckBoxWidget* AddCheckBox(
			const char* label,
			int initialValue = GlowCheckBoxWidget::off,
			GlowCheckBoxReceiver* receiver = 0);
		inline GlowQuickRadioGroupWidget* AddRadioGroup(
			int arrangement = GlowQuickRadioGroupWidget::vertical,
			int spacing = 3,
			GlowRadioButtonReceiver* receiver = 0);
		inline GlowLabeledSliderWidget* AddSlider(
			float min,
			float max,
			float initial,
			int options = GlowSliderWidget::defaultOptions,
			int numTicks = 2,
			const char* valueLabel = 0,
			const char* label = 0,
			GlowSliderReceiver* receiver = 0);
		inline GlowScrollBarWidget* AddScrollBar(
			long min,
			long max,
			long span,
			long initialTop,
			GlowScrollBarReceiver* receiver = 0);
		inline GlowLabeledTextFieldWidget* AddTextField(
			int width,
			const char* text = "",
			const char* label = 0);
		inline GlowLabeledHiddenTextFieldWidget* AddHiddenTextField(
			int width,
			const char* text = "",
			char hideCharacter = '#',
			const char* label = 0);
		inline GlowLabelWidget* AddLabel(
			const char* text);
		
		inline int GetArrangement() const;
		inline int GetAlignment() const;
		inline int GetSpacing() const;
		inline int GetHMargin() const;
		inline int GetVMargin() const;
		inline void SetArrangement(
			int value);
		inline void SetAlignment(
			int value);
		inline void SetSpacing(
			int value);
		inline void SetHMargin(
			int value);
		inline void SetVMargin(
			int value);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		GlowQuickPanelWidget* _rootPanel;
};


/*
===============================================================================
	CLASS GlowQuickPaletteWindow
	
	Quick palette window
===============================================================================
*/

class GlowQuickPaletteWindow :
	public GlowQuickPaletteBase,
	public GlowFixedSizeWidgetWindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		GlowQuickPaletteWindow(
			const char* name,
			int x = GlowWindow::autoPosition,
			int y = GlowWindow::autoPosition,
			int arrangement = vertical,
			int alignment = alignLeft,
			int spacing = 8,
			int hmargin = 10,
			int vmargin = 10);
	
	public:
	
		void Pack();
};


/*
===============================================================================
	CLASS GlowQuickPaletteSubwindow
	
	Quick palette window
===============================================================================
*/

class GlowQuickPaletteSubwindow :
	public GlowQuickPaletteBase,
	public GlowWidgetSubwindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		GlowQuickPaletteSubwindow(
			GlowComponent* parent,
			int x = 0,
			int y = 0,
			int width = GlowSubwindow::parentWindowSize,
			int height = GlowSubwindow::parentWindowSize,
			int arrangement = vertical,
			int alignment = alignLeft,
			int spacing = 8,
			int hmargin = 10,
			int vmargin = 10);
	
	public:
	
		void Pack(
			bool resizeOnPack = true);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowQuickPalette.inl.h"


#endif
