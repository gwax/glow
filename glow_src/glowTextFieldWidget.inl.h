/*
===============================================================================

	FILE:  glowTextFieldWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Editable text field for GLOW
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowTextFieldWidget
===============================================================================
*/

inline GlowTextFieldWidget::GlowTextFieldWidget()
{
}


inline GlowTextFieldWidget::GlowTextFieldWidget(
	GlowWidget* parent,
	const GlowTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowTextFieldWidget::GlowTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params)
{
	Init(root, 0, params);
}


inline void GlowTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params)
{
	Init(root, 0, params);
}


inline int GlowTextFieldWidget::GetPreferredWidth() const
{
	return preferredWidth_;
}


inline void GlowTextFieldWidget::SetPreferredWidth(
	int width)
{
	preferredWidth_ = width;
}


inline GlowTextFieldWidget::Style GlowTextFieldWidget::GetStyle() const
{
	return style_;
}


inline void GlowTextFieldWidget::SetStyle(
	Style style)
{
	style_ = style;
	Refresh();
}


inline GlowFont GlowTextFieldWidget::GetFont() const
{
	return font_;
}


inline void GlowTextFieldWidget::SetFont(
	GlowFont font)
{
	font_ = font;
	Refresh();
}


inline int GlowTextFieldWidget::GetBlinkInterval() const
{
	return blinkInterval_;
}


inline void GlowTextFieldWidget::SetBlinkInterval(
	int interval)
{
	blinkInterval_ = interval;
}


inline int GlowTextFieldWidget::GetAutoScrollInterval() const
{
	return autoScrollInterval_;
}


inline void GlowTextFieldWidget::SetAutoScrollInterval(
	int interval)
{
	autoScrollInterval_ = interval;
}


inline const char* GlowTextFieldWidget::GetTextChars() const
{
	return data_.String().c_str();
}


inline int GlowTextFieldWidget::GetTextLength() const
{
	return data_.String().length();
}


inline const GLOW_STD::string& GlowTextFieldWidget::GetTextString() const
{
	return data_.String();
}


inline void GlowTextFieldWidget::SetText(
	const char* str)
{
	data_.String().assign(str);
	data_.RevalidateSelection();
	Refresh();
}


inline void GlowTextFieldWidget::SetText(
	const GLOW_STD::string& str)
{
	data_.String().assign(str);
	data_.RevalidateSelection();
	Refresh();
}


inline GLOW_STD::string GlowTextFieldWidget::GetSelectionString() const
{
	return data_.SelectedText();
}


inline int GlowTextFieldWidget::GetSelectionLength() const
{
	return data_.SelectionLength();
}


inline int GlowTextFieldWidget::GetSelectionStart() const
{
	return data_.SelectionStart();
}


inline int GlowTextFieldWidget::GetSelectionEnd() const
{
	return data_.SelectionEnd();
}


inline void GlowTextFieldWidget::SetSelection(
	int start,
	int end)
{
	data_.SetSelection(start, end);
	Refresh();
}


inline void GlowTextFieldWidget::SetSelection(
	int pos)
{
	data_.SetSelection(pos);
	Refresh();
}


inline void GlowTextFieldWidget::ReplaceSelectionWith(
	const char* str)
{
	data_.ReplaceSelectionWith(str);
	Refresh();
}


inline void GlowTextFieldWidget::ReplaceSelectionWith(
	const GLOW_STD::string& str)
{
	data_.ReplaceSelectionWith(str.c_str());
	Refresh();
}


inline GlowColor GlowTextFieldWidget::GetBackColor() const
{
	return backColor_;
}


inline GlowColor GlowTextFieldWidget::GetCaretColor() const
{
	return caretColor_;
}


inline GlowColor GlowTextFieldWidget::GetTextColor() const
{
	return textColor_;
}


inline GlowColor GlowTextFieldWidget::GetFocusBackColor() const
{
	return focusBackColor_;
}


inline GlowColor GlowTextFieldWidget::GetFocusTextColor() const
{
	return focusTextColor_;
}


inline GlowColor GlowTextFieldWidget::GetFocusCaretColor() const
{
	return focusCaretColor_;
}


inline GlowColor GlowTextFieldWidget::GetHiliteBackColor() const
{
	return hiliteBackColor_;
}


inline GlowColor GlowTextFieldWidget::GetHiliteTextColor() const
{
	return hiliteTextColor_;
}


inline GlowColor GlowTextFieldWidget::GetDisableBackColor() const
{
	return disableBackColor_;
}


inline GlowColor GlowTextFieldWidget::GetDisableTextColor() const
{
	return disableTextColor_;
}


inline GlowColor GlowTextFieldWidget::GetDisableCaretColor() const
{
	return disableCaretColor_;
}


inline GlowColor GlowTextFieldWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowTextFieldWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowTextFieldWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowTextFieldWidget::SetBackColor(
	GlowColor c)
{
	backColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetCaretColor(
	GlowColor c)
{
	caretColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetTextColor(
	GlowColor c)
{
	textColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetFocusBackColor(
	GlowColor c)
{
	focusBackColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetFocusTextColor(
	GlowColor c)
{
	focusTextColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetFocusCaretColor(
	GlowColor c)
{
	focusCaretColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetHiliteBackColor(
	GlowColor c)
{
	hiliteBackColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetHiliteTextColor(
	GlowColor c)
{
	hiliteTextColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableBackColor(
	GlowColor c)
{
	disableBackColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableTextColor(
	GlowColor c)
{
	disableTextColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableCaretColor(
	GlowColor c)
{
	disableCaretColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDarkBevelColor(
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

inline void Glow_TextField_BlinkTask::Init(
	GlowTextFieldWidget* field)
{
	field_ = field;
}


/*
===============================================================================
	Inline methods of GlowHiddenTextFieldWidget
===============================================================================
*/

inline GlowHiddenTextFieldWidget::GlowHiddenTextFieldWidget()
{
}


inline GlowHiddenTextFieldWidget::GlowHiddenTextFieldWidget(
	GlowWidget* parent,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline GlowHiddenTextFieldWidget::GlowHiddenTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


inline void GlowHiddenTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline void GlowHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


inline const char* GlowHiddenTextFieldWidget::GetTextChars() const
{
	return hiddenData_.c_str();
}


inline const GLOW_STD::string& GlowHiddenTextFieldWidget::GetTextString() const
{
	return hiddenData_;
}


inline void GlowHiddenTextFieldWidget::SetText(
	const char* str)
{
	hiddenData_.assign(str);
	data_.String().assign(hiddenData_.size(), hideCharacter_);
	data_.RevalidateSelection();
	Refresh();
}


inline void GlowHiddenTextFieldWidget::SetText(
	const GLOW_STD::string& str)
{
	hiddenData_.assign(str);
	data_.String().assign(hiddenData_.size(), hideCharacter_);
	data_.RevalidateSelection();
	Refresh();
}


inline GLOW_STD::string GlowHiddenTextFieldWidget::GetSelectionString() const
{
	return hiddenData_.substr(data_.SelectionStart(),
		data_.SelectionEnd()-data_.SelectionStart());
}


inline char GlowHiddenTextFieldWidget::GetHideCharacter() const
{
	return hideCharacter_;
}


/*
===============================================================================
	Inline methods of GlowLabeledTextFieldWidget
===============================================================================
*/

inline GlowLabeledTextFieldWidget::GlowLabeledTextFieldWidget()
{
}


inline GlowLabeledTextFieldWidget::GlowLabeledTextFieldWidget(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabeledTextFieldWidget::GlowLabeledTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabeledTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabeledTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params)
{
	Init(root, 0, params);
}


/*
===============================================================================
	Inline methods of GlowLabeledHiddenTextFieldWidget
===============================================================================
*/

inline GlowLabeledHiddenTextFieldWidget::GlowLabeledHiddenTextFieldWidget()
{
}


inline GlowLabeledHiddenTextFieldWidget::GlowLabeledHiddenTextFieldWidget(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline GlowLabeledHiddenTextFieldWidget::GlowLabeledHiddenTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


inline void GlowLabeledHiddenTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline void GlowLabeledHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

