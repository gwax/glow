/*
===============================================================================

	FILE:  glowTextAreaWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Scrollable text area widget for GLOW
	
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


#ifndef GLOW_TEXTAREAWIDGET__H
#define GLOW_TEXTAREAWIDGET__H


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
#ifndef GLOW_TEXTDATA__H
	#include "glowTextData.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowTextAreaParams;
class GlowTextAreaWidget;


/*
===============================================================================
	CLASS Glow_TextArea_BlinkTask
===============================================================================
*/


class Glow_TextArea_BlinkTask :
	public GlowDeferredTask
{
	public:
	
		inline void Init(
			GlowTextAreaWidget* widget);
	
	protected:
	
		virtual void Task();
	
	private:
	
		GlowTextAreaWidget* widget_;
};


/*
===============================================================================
	CLASS GlowTextAreaWidget
	
	Scrollable text area widget
===============================================================================
*/

class GlowTextAreaWidget :
	public GlowWidget
{
	friend class Glow_TextArea_BlinkTask;
	friend class Glow_TextArea_AutoScrollTimer;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Style
		{
			plainStyle = 1,
			etchedStyle = 2,
			raisedStyle = 3,
			loweredStyle = 4
		};
	
	public:
	
		inline GlowTextAreaWidget();
		inline GlowTextAreaWidget(
			GlowWidgetRoot* root,
			const GlowTextAreaParams& params);
		inline GlowTextAreaWidget(
			GlowWidget* parent,
			const GlowTextAreaParams& params);
		
		inline void Init(
			GlowWidgetRoot* root,
			const GlowTextAreaParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowTextAreaParams& params);
		
		virtual ~GlowTextAreaWidget();
	
	public:
	
		inline void SetStyle(
			Style style);
		inline Style GetStyle() const;
		
		inline void SetFont(
			GlowFont font);
		inline GlowFont GetFont() const;
		
		inline int GetBlinkInterval() const;
		inline void SetBlinkInterval(
			int interval);
		
		inline int GetAutoScrollInterval() const;
		inline void SetAutoScrollInterval(
			int interval);
		
		inline bool IsWrapping() const;
		void SetWrapping(
			bool wrapping);
		
		inline bool HasScrollBars() const;
		void SetScrollBars(
			bool has);
		
		inline const char* GetTextChars() const;
		inline int GetTextLength() const;
		inline const GLOW_STD::string& GetTextString() const;
		inline void SetText(
			const char* str);
		inline void SetText(
			const GLOW_STD::string& str);
		
		inline GLOW_STD::string GetSelectionString() const;
		inline int GetSelectionLength() const;
		inline int GetSelectionStart() const;
		inline int GetSelectionEnd() const;
		
		inline void SetSelection(
			int start,
			int end);
		inline void SetSelection(
			int pos);
		inline void ReplaceSelectionWith(
			const char* str);
		inline void ReplaceSelectionWith(
			const GLOW_STD::string& str);
		
		inline GlowColor GetBackColor() const;
		inline GlowColor GetCaretColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetFocusBackColor() const;
		inline GlowColor GetFocusTextColor() const;
		inline GlowColor GetFocusCaretColor() const;
		inline GlowColor GetHiliteBackColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetDisableBackColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableCaretColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBackColor(
			GlowColor c);
		inline void SetCaretColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetFocusBackColor(
			GlowColor c);
		inline void SetFocusTextColor(
			GlowColor c);
		inline void SetFocusCaretColor(
			GlowColor c);
		inline void SetHiliteBackColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetDisableBackColor(
			GlowColor c);
		inline void SetDisableCaretColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowTextAreaParams& params);
		
		void CheckAutoScroll();
	
	protected:
	
		GlowTextData data_;
	
	
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
		
		virtual void OnWidgetKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
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
		
		virtual void OnWidgetPaint();
		
		virtual void OnGotKeyboardFocus();
		virtual void OnLostKeyboardFocus();
	
	private:
	
		void HandleAutoScrollTimer_();
	
	private:
	
		GlowFont font_;
		int hpos_;
		Style style_;
		int blinkInterval_;
		int autoScrollInterval_;
		int inset_;
		int caretInset_;
		int dragStart_;
		int dragEnd_;
		int dragX_;
		bool blink_;
		bool toggleAutoScroll_;
		Glow_TextArea_BlinkTask blinkTask_;
		bool wrapping_;
		GlowScrollBarWidget* hScrollBar_;
		GlowScrollBarWidget* vScrollBar_;
		
		GlowColor backColor_;
		GlowColor caretColor_;
		GlowColor textColor_;
		GlowColor focusBackColor_;
		GlowColor focusTextColor_;
		GlowColor focusCaretColor_;
		GlowColor hiliteBackColor_;
		GlowColor hiliteTextColor_;
		GlowColor disableBackColor_;
		GlowColor disableCaretColor_;
		GlowColor disableTextColor_;
		GlowColor disableOutlineColor_;
		GlowColor lightBevelColor_;
		GlowColor darkBevelColor_;
};


/*
===============================================================================
	CLASS GlowTextAreaParams
	
	Scrollable text area params
===============================================================================
*/

class GlowTextAreaParams :
	public GlowWidgetParams
{
	public:
	
		GlowTextAreaWidget::Style style;
		const char* initialText;
		GlowFont font;
		int selectionStart;
		int selectionEnd;
		int blinkInterval;
		int autoScrollInterval;
		int inset;
		int caretInset;
		bool wrapping;
		bool hasScrollBars;
		GlowColor backColor;
		GlowColor textColor;
		GlowColor caretColor;
		GlowColor focusBackColor;
		GlowColor focusTextColor;
		GlowColor focusCaretColor;
		GlowColor hiliteBackColor;
		GlowColor hiliteTextColor;
		GlowColor disableBackColor;
		GlowColor disableTextColor;
		GlowColor disableCaretColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowTextAreaParams defaults;
		
		GlowTextAreaParams();
	
	protected:
	
		GlowTextAreaParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowTextAreaWidget.inl.h"


#endif
