/*
===============================================================================

	FILE:  glowTextAreaWidget.inl.h
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowTextAreaWidget
===============================================================================
*/

inline GlowTextAreaWidget::GlowTextAreaWidget()
{
}


inline GlowTextAreaWidget::GlowTextAreaWidget(
	GlowWidget* parent,
	const GlowTextAreaParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowTextAreaWidget::GlowTextAreaWidget(
	GlowWidgetRoot* root,
	const GlowTextAreaParams& params)
{
	Init(root, 0, params);
}


inline void GlowTextAreaWidget::Init(
	GlowWidget* parent,
	const GlowTextAreaParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowTextAreaWidget::Init(
	GlowWidgetRoot* root,
	const GlowTextAreaParams& params)
{
	Init(root, 0, params);
}


inline GlowTextAreaWidget::Style GlowTextAreaWidget::GetStyle() const
{
	return style_;
}


inline void GlowTextAreaWidget::SetStyle(
	Style style)
{
	style_ = style;
	Refresh();
}


inline GlowFont GlowTextAreaWidget::GetFont() const
{
	return font_;
}


inline void GlowTextAreaWidget::SetFont(
	GlowFont font)
{
	font_ = font;
	Refresh();
}


inline int GlowTextAreaWidget::GetBlinkInterval() const
{
	return blinkInterval_;
}


inline void GlowTextAreaWidget::SetBlinkInterval(
	int interval)
{
	blinkInterval_ = interval;
}


inline int GlowTextAreaWidget::GetAutoScrollInterval() const
{
	return autoScrollInterval_;
}


inline void GlowTextAreaWidget::SetAutoScrollInterval(
	int interval)
{
	autoScrollInterval_ = interval;
}


inline bool GlowTextAreaWidget::IsWrapping() const
{
	return wrapping_;
}


inline bool GlowTextAreaWidget::HasScrollBars() const
{
	return hasScrollBars_;
}


inline const char* GlowTextAreaWidget::GetTextChars() const
{
	return data_.String().c_str();
}


inline int GlowTextAreaWidget::GetTextLength() const
{
	return data_.String().length();
}


inline const GLOW_STD::string& GlowTextAreaWidget::GetTextString() const
{
	return data_.String();
}


inline void GlowTextAreaWidget::SetText(
	const char* str)
{
	data_.String().assign(str);
	data_.RevalidateSelection();
	Refresh();
}


inline void GlowTextAreaWidget::SetText(
	const GLOW_STD::string& str)
{
	data_.String().assign(str);
	data_.RevalidateSelection();
	Refresh();
}


inline GLOW_STD::string GlowTextAreaWidget::GetSelectionString() const
{
	return data_.SelectedText();
}


inline int GlowTextAreaWidget::GetSelectionLength() const
{
	return data_.SelectionLength();
}


inline int GlowTextAreaWidget::GetSelectionStart() const
{
	return data_.SelectionStart();
}


inline int GlowTextAreaWidget::GetSelectionEnd() const
{
	return data_.SelectionEnd();
}


inline void GlowTextAreaWidget::SetSelection(
	int start,
	int end)
{
	data_.SetSelection(start, end);
	Refresh();
}


inline void GlowTextAreaWidget::SetSelection(
	int pos)
{
	data_.SetSelection(pos);
	Refresh();
}


inline void GlowTextAreaWidget::ReplaceSelectionWith(
	const char* str)
{
	data_.ReplaceSelectionWith(str);
	Refresh();
}


inline void GlowTextAreaWidget::ReplaceSelectionWith(
	const GLOW_STD::string& str)
{
	data_.ReplaceSelectionWith(str.c_str());
	Refresh();
}


inline GlowColor GlowTextAreaWidget::GetBackColor() const
{
	return backColor_;
}


inline GlowColor GlowTextAreaWidget::GetCaretColor() const
{
	return caretColor_;
}


inline GlowColor GlowTextAreaWidget::GetTextColor() const
{
	return textColor_;
}


inline GlowColor GlowTextAreaWidget::GetFocusBackColor() const
{
	return focusBackColor_;
}


inline GlowColor GlowTextAreaWidget::GetFocusTextColor() const
{
	return focusTextColor_;
}


inline GlowColor GlowTextAreaWidget::GetFocusCaretColor() const
{
	return focusCaretColor_;
}


inline GlowColor GlowTextAreaWidget::GetHiliteBackColor() const
{
	return hiliteBackColor_;
}


inline GlowColor GlowTextAreaWidget::GetHiliteTextColor() const
{
	return hiliteTextColor_;
}


inline GlowColor GlowTextAreaWidget::GetDisableBackColor() const
{
	return disableBackColor_;
}


inline GlowColor GlowTextAreaWidget::GetDisableTextColor() const
{
	return disableTextColor_;
}


inline GlowColor GlowTextAreaWidget::GetDisableCaretColor() const
{
	return disableCaretColor_;
}


inline GlowColor GlowTextAreaWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowTextAreaWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowTextAreaWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowTextAreaWidget::SetBackColor(
	GlowColor c)
{
	backColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetCaretColor(
	GlowColor c)
{
	caretColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetTextColor(
	GlowColor c)
{
	textColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetFocusBackColor(
	GlowColor c)
{
	focusBackColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetFocusTextColor(
	GlowColor c)
{
	focusTextColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetFocusCaretColor(
	GlowColor c)
{
	focusCaretColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetHiliteBackColor(
	GlowColor c)
{
	hiliteBackColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetHiliteTextColor(
	GlowColor c)
{
	hiliteTextColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetDisableBackColor(
	GlowColor c)
{
	disableBackColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetDisableTextColor(
	GlowColor c)
{
	disableTextColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetDisableCaretColor(
	GlowColor c)
{
	disableCaretColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowTextAreaWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


/*
===============================================================================
	Timer function
===============================================================================
*/

inline void Glow_TextArea_BlinkTask::Init(
	GlowTextAreaWidget* widget)
{
	widget_ = widget;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

