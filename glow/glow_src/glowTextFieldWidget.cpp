/*
===============================================================================

	FILE:  glowTextFieldWidget.cpp
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_TEXTFIELDWIDGET__H
	#include "glowTextFieldWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowTextFieldParams GlowTextFieldParams::defaults(true);

GlowTextFieldParams::GlowTextFieldParams()
{
	*this = defaults;
}

GlowTextFieldParams::GlowTextFieldParams(bool) :
GlowWidgetParams(true),
backColor(1.0f, 1.0f, 1.0f),
textColor(0.0f, 0.0f, 0.0f),
caretColor(0.4f, 0.4f, 0.4f),
focusBackColor(1.0f, 1.0f, 1.0f),
focusTextColor(0.0f, 0.0f, 0.0f),
focusCaretColor(0.0f, 0.0f, 1.0f),
hiliteBackColor(1.0f, 0.8f, 0.8f),
hiliteTextColor(0.0f, 0.0f, 0.0f),
disableBackColor(0.8f, 0.8f, 0.8f),
disableTextColor(0.0f, 0.0f, 0.0f),
disableCaretColor(0.4f, 0.4f, 0.4f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.3f, 0.3f, 0.3f)
{
	width = 100;
	height = 20;
	style = GlowTextFieldWidget::loweredStyle;
	initialText = "";
	font = GLUT_BITMAP_HELVETICA_12;
	height = 24;
	selectionStart = 0;
	selectionEnd = 0;
	blinkInterval = 500;
	autoScrollInterval = 50;
	inset = 5;
	caretInset = 2;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabeledTextFieldParams GlowLabeledTextFieldParams::defaults(true);

GlowLabeledTextFieldParams::GlowLabeledTextFieldParams()
{
	*this = defaults;
}

GlowLabeledTextFieldParams::GlowLabeledTextFieldParams(bool) :
GlowTextFieldParams(true),
labelColor(0.0f, 0.0f, 0.0f),
disableLabelColor(0.3f, 0.3f, 0.3f)
{
	labelText = "";
	labelFont = GLUT_BITMAP_HELVETICA_12;
	labelPosition = GlowWidgetLabelHelper::defaultLabelPosition;
	labelWidth = 100;
	labelHeight = 20;
	labelSpacing = 5;
}


/*
===============================================================================
	Timer function
===============================================================================
*/

void Glow_TextField_BlinkTask::Task()
{
	field_->blink_ = !field_->blink_;
	field_->Refresh();
	Schedule(field_->blinkInterval_);
}


/*
===============================================================================
	Autoscroll timer function
===============================================================================
*/

static int _autoScrollTimerID = 0;


class Glow_TextField_AutoScrollTimer :
	public GlowTimerReceiver
{
	public:
	
		inline Glow_TextField_AutoScrollTimer();
		inline void SetTextField(
			GlowTextFieldWidget* field);
		inline void ForceRemove(
			GlowTextFieldWidget* field);
	
	protected:
	
		virtual void OnMessage(
			const GlowTimerMessage& message);
	
	private:
	
		GlowTextFieldWidget* field_;
};


inline Glow_TextField_AutoScrollTimer::Glow_TextField_AutoScrollTimer()
{
	field_ = 0;
}


inline void Glow_TextField_AutoScrollTimer::SetTextField(
	GlowTextFieldWidget* field)
{
	field_ = field;
}


inline void Glow_TextField_AutoScrollTimer::ForceRemove(
	GlowTextFieldWidget* field)
{
	if (field_ == field)
	{
		Glow::UnregisterTimer(_autoScrollTimerID);
	}
}


void Glow_TextField_AutoScrollTimer::OnMessage(
	const GlowTimerMessage& message)
{
	field_->HandleAutoScrollTimer_();
}


static Glow_TextField_AutoScrollTimer* _autoScrollTimer =
	new Glow_TextField_AutoScrollTimer;


/*
===============================================================================
	Methods of GlowTextFieldWidget
===============================================================================
*/

void GlowTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowTextFieldParams& params)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	style_ = params.style;
	if (params.initialText != 0)
	{
		data_.String().assign(params.initialText);
	}
	font_ = params.font;
	hpos_ = 0;
	dragStart_ = params.selectionStart;
	dragEnd_ = params.selectionEnd;
	data_.SetSelection(dragStart_, dragEnd_);
	blinkInterval_ = params.blinkInterval;
	autoScrollInterval_ = params.autoScrollInterval;
	inset_ = params.inset;
	caretInset_ = params.caretInset;
	backColor_ = params.backColor;
	caretColor_ = params.caretColor;
	textColor_ = params.textColor;
	focusBackColor_ = params.focusBackColor;
	focusTextColor_ = params.focusTextColor;
	focusCaretColor_ = params.focusCaretColor;
	hiliteBackColor_ = params.hiliteBackColor;
	hiliteTextColor_ = params.hiliteTextColor;
	disableBackColor_ = params.disableBackColor;
	disableTextColor_ = params.disableTextColor;
	disableCaretColor_ = params.disableCaretColor;
	disableOutlineColor_ = params.disableOutlineColor;
	lightBevelColor_ = params.lightBevelColor;
	darkBevelColor_ = params.darkBevelColor;
	blink_ = true;
	toggleAutoScroll_ = false;
	blinkTask_.Init(this);
	
	RegisterMouseEvents();
	RegisterKeyboardEvents();
}


GlowTextFieldWidget::~GlowTextFieldWidget()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::~GlowTextFieldWidget");
	
	_autoScrollTimer->ForceRemove(this);
}


void GlowTextFieldWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetPaint");
	
	// Backing
	if (!IsActive())
	{
		disableBackColor_.Apply();
	}
	else if (HasKeyboardFocus())
	{
		focusBackColor_.Apply();
	}
	else
	{
		backColor_.Apply();
	}
	::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	
	// Bevels for backing
	float bevelHeight = float(4)/float(Height());
	float bevelWidth = float(4)/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	if (IsActive())
	{
		if (style_ == etchedStyle)
		{
			lightBevelColor_.Apply();
			::glBegin(GL_LINE_LOOP);
			::glVertex2f(-1.0f+etchWidth, -1.0f);
			::glVertex2f(-1.0f+etchWidth, 1.0f-etchHeight-etchHeight);
			::glVertex2f(1.0f-etchWidth, 1.0f-etchHeight-etchHeight);
			::glVertex2f(1.0f-etchWidth, -1.0f);
			::glEnd();
			darkBevelColor_.Apply();
			::glBegin(GL_LINE_LOOP);
			::glVertex2f(-1.0f, -1.0f+etchHeight);
			::glVertex2f(-1.0f, 1.0f-etchHeight);
			::glVertex2f(1.0f-etchWidth-etchWidth, 1.0f-etchHeight);
			::glVertex2f(1.0f-etchWidth-etchWidth, -1.0f+etchHeight);
			::glEnd();
		}
		else if (style_ == raisedStyle || style_ == loweredStyle)
		{
			if (style_ == raisedStyle)
			{
				lightBevelColor_.Apply();
			}
			else
			{
				darkBevelColor_.Apply();
			}
			::glBegin(GL_QUAD_STRIP);
			::glVertex2f(-1.0f, -1.0f);
			::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
			::glVertex2f(-1.0f, 1.0f);
			::glVertex2f(-1.0f+bevelWidth, 1.0f-bevelHeight);
			::glVertex2f(1.0f, 1.0f);
			::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
			::glEnd();

			if (style_ == raisedStyle)
			{
				darkBevelColor_.Apply();
			}
			else
			{
				lightBevelColor_.Apply();
			}
			::glBegin(GL_QUAD_STRIP);
			::glVertex2f(-1.0f, -1.0f);
			::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
			::glVertex2f(1.0f, -1.0f);
			::glVertex2f(1.0f-bevelWidth, -1.0f+bevelHeight);
			::glVertex2f(1.0f, 1.0f);
			::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
			::glEnd();
		}
	}
	else
	{
		disableOutlineColor_.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, -1.0f);
		::glEnd();
	}
	
	// Everything else gets scissored
	GLint oldScissor[4];
	::glGetIntegerv(GL_SCISSOR_BOX, oldScissor);
	::glScissor(RootPositionX()+inset_,
		Root()->Subwindow()->Height()-Height()-RootPositionY()+inset_,
		Width()-inset_-inset_, Height()-inset_-inset_);
	
	// Highlight backing
	if (data_.SelectionLength() > 0 && HasKeyboardFocus())
	{
		hiliteBackColor_.Apply();
		float left = 0;
		float top = 0;
		float right = 0;
		float bottom = 0;
		NormalizeCoordinates(inset_-hpos_+
			data_.PixelPosOf(font_, data_.SelectionStart(), 0),
			inset_, left, top);
		NormalizeCoordinates(inset_-hpos_+
			data_.PixelPosOf(font_, data_.SelectionEnd(), 0),
			inset_+font_.Leading(), right, bottom);
		::glRectf(left, bottom, right, top);
	}
	
	// Text
	if (!IsActive())
	{
		disableTextColor_.Apply();
	}
	else if (HasKeyboardFocus())
	{
		focusTextColor_.Apply();
	}
	else
	{
		textColor_.Apply();
	}
	int startpos = 0;
	int endpos = 0;
	int pixoffset = 0;
	data_.CalcLineDrawInfo(font_, 0, hpos_, Width()-inset_-inset_,
		startpos, endpos, pixoffset);
	const char* dat = data_.String().data();
	while (pixoffset > inset_)
	{
		pixoffset -= ::glutBitmapWidth(font_, dat[startpos]);
		++startpos;
	}
	float x = 0;
	float y = 0;
	NormalizeCoordinates(inset_-pixoffset,
		inset_+font_.BaselinePos(), x, y);
	::glRasterPos2f(x, y);
	for (int i=startpos; i<endpos; i++)
	{
		::glutBitmapCharacter(font_, dat[i]);
	}
	
	// Insertion caret
	// Special scissoring
	if (blink_ || !HasKeyboardFocus() || !IsActive())
	{
		::glScissor(RootPositionX()+caretInset_,
			Root()->Subwindow()->Height()-Height()-RootPositionY()+caretInset_,
			Width()-caretInset_-caretInset_, Height()-caretInset_-caretInset_);
		if (!IsActive())
		{
			disableCaretColor_.Apply();
		}
		else if (HasKeyboardFocus())
		{
			focusCaretColor_.Apply();
		}
		else
		{
			caretColor_.Apply();
		}
		float horiz = 0;
		float top = 0;
		float bottom = 0;
		NormalizeCoordinates(inset_-hpos_+data_.PixelPosOf(font_, dragEnd_, 0),
			inset_, horiz, top);
		NormalizeCoordinates(inset_-hpos_+data_.PixelPosOf(font_, dragEnd_, 0),
			inset_+font_.Leading(), horiz, bottom);
		float hBump = float(2)/float(Width());
		float vBump = float(2)/float(Height());
		::glBegin(GL_LINES);
		::glVertex2f(horiz, bottom);
		::glVertex2f(horiz, top-vBump);
		::glVertex2f(horiz-bevelWidth, bottom);
		::glVertex2f(horiz+bevelWidth+hBump, bottom);
		::glVertex2f(horiz-bevelWidth, top-vBump);
		::glVertex2f(horiz+bevelWidth+hBump, top-vBump);
		::glEnd();
	}
	
	// Unscissor
	::glScissor(oldScissor[0], oldScissor[1], oldScissor[2], oldScissor[3]);
	
	// Activate autoscroll timer
	if (toggleAutoScroll_)
	{
		toggleAutoScroll_ = false;
		_autoScrollTimer->SetTextField(this);
		_autoScrollTimerID = Glow::RegisterTimer(autoScrollInterval_, _autoScrollTimer);
	}
}


void GlowTextFieldWidget::HandleAutoScrollTimer_()
{
	dragEnd_ = data_.AtPixelPos(font_, 0, dragX_-inset_+hpos_);
	data_.SetSelection(dragStart_, dragEnd_);
	CheckAutoScroll();
	toggleAutoScroll_ = true;
	Refresh();
}


void GlowTextFieldWidget::OnGotKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnGotKeyboardFocus");
	
	blinkTask_.Schedule(blinkInterval_);
}


void GlowTextFieldWidget::OnLostKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnLostKeyboardFocus");
	
	blinkTask_.Unschedule();
}


void GlowTextFieldWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetMouseDown");
	
	if (!HasKeyboardFocus())
	{
		GrabKeyboardFocus();
	}
	dragX_ = x;
	if (modifiers & Glow::shiftModifier)
	{
		dragEnd_ = data_.AtPixelPos(font_, 0, x-inset_+hpos_);
		data_.SetSelection(dragStart_, dragEnd_);
	}
	else
	{
		dragStart_ = dragEnd_ = data_.AtPixelPos(font_, 0, x-inset_+hpos_);
		data_.SetSelection(dragStart_);
	}
	CheckAutoScroll();
	toggleAutoScroll_ = true;
	Refresh();
}


void GlowTextFieldWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetMouseUp");
	
	dragEnd_ = data_.AtPixelPos(font_, 0, x-inset_+hpos_);
	data_.SetSelection(dragStart_, dragEnd_);
	CheckAutoScroll();
	Glow::UnregisterTimer(_autoScrollTimerID);
	toggleAutoScroll_ = false;
	Refresh();
}


void GlowTextFieldWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetMouseDrag");
	
	dragX_ = x;
	dragEnd_ = data_.AtPixelPos(font_, 0, x-inset_+hpos_);
	data_.SetSelection(dragStart_, dragEnd_);
	Refresh();
}


void GlowTextFieldWidget::OnWidgetKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetKeyboard");
	
	if (key >= Glow::specialKeyOffset)
	{
		if (key == Glow::leftArrowKey && dragEnd_ > 0)
		{
			if (modifiers & Glow::shiftModifier)
			{
				--dragEnd_;
				data_.SetSelection(dragStart_, dragEnd_);
			}
			else
			{
				dragStart_ = dragEnd_ = dragEnd_-1;
				data_.SetSelection(dragStart_);
			}
			CheckAutoScroll();
			Refresh();
		}
		else if (key == Glow::rightArrowKey && dragEnd_ < int(data_.String().size()))
		{
			if (modifiers & Glow::shiftModifier)
			{
				++dragEnd_;
				data_.SetSelection(dragStart_, dragEnd_);
			}
			else
			{
				dragStart_ = dragEnd_ = dragEnd_+1;
				data_.SetSelection(dragStart_);
			}
			CheckAutoScroll();
			Refresh();
		}
	}
	else if (key == Glow::backspaceKey || key == Glow::deleteKey)
	{
		if (data_.SelectionLength() > 0)
		{
			data_.DeleteSelection();
			dragStart_ = dragEnd_ = data_.SelectionStart();
			CheckAutoScroll();
			Refresh();
		}
		else if (data_.SelectionStart() > 0)
		{
			dragStart_ = dragEnd_ = data_.SelectionStart()-1;
			data_.SetSelection(dragStart_);
			data_.String().erase(dragStart_, 1);
			CheckAutoScroll();
			Refresh();
		}
	}
	else if (key == Glow::tabKey)
	{
		RelinquishKeyboardFocus();
		Refresh();
	}
	else if (key >= 32)
	{
		data_.ReplaceSelectionWith((unsigned char)key);
		data_.SetSelection(data_.SelectionEnd(), data_.SelectionEnd());
		dragStart_ = dragEnd_ = data_.SelectionEnd();
		CheckAutoScroll();
		Refresh();
	}
}


GlowWidget::AutoPackError GlowTextFieldWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnAutoPack");
	
	int hnew = Width();
	if (hnew < 20 || (hSize != unspecifiedSize && hSize < hnew))
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	
	int vnew = Height();
	int preferred = font_.Leading()+inset_+inset_;
	if (vSize != unspecifiedSize && vSize < preferred)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == expandPreferredSize || vOption == preferredSize)
	{
		vnew = preferred;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowTextFieldWidget::CheckAutoScroll()
{
	int pos = data_.PixelPosOf(font_, dragEnd_, 0);
	if (pos < hpos_)
	{
		hpos_ = pos;
	}
	else if (pos > hpos_+Width()-inset_-inset_)
	{
		hpos_ = pos-Width()+inset_+inset_;
	}
}


/*
===============================================================================
	Methods of GlowHiddenTextFieldWidget
===============================================================================
*/

void GlowHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	GLOW_DEBUGSCOPE("GlowHiddenTextFieldWidget::Init");
	
	GlowTextFieldWidget::Init(root, parent, params);
	
	if (params.initialText != 0)
	{
		hiddenData_.assign(params.initialText);
		data_.String().assign(hiddenData_.size(), hideCharacter);
	}
	
	hideCharacter_ = hideCharacter;
}


void GlowHiddenTextFieldWidget::SetHideCharacter(
	char hc)
{
	if (hc != hideCharacter_)
	{
		hideCharacter_ = hc;
		data_.String().assign(data_.String().size(), hc);
		CheckAutoScroll();
		Refresh();
	}
}


void GlowHiddenTextFieldWidget::OnWidgetKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowHiddenTextFieldWidget::OnWidgetKeyboard");
	
	if (key >= 32 && key < Glow::specialKeyOffset)
	{
		hiddenData_.replace(data_.SelectionStart(),
			data_.SelectionEnd()-data_.SelectionStart(), 1, (unsigned char)key);
		GlowTextFieldWidget::OnWidgetKeyboard(Glow::KeyCode(hideCharacter_), x, y, modifiers);
	}
	else
	{
		GlowTextFieldWidget::OnWidgetKeyboard(key, x, y, modifiers);
	}
}


/*
===============================================================================
	Methods of GlowLabeledTextFieldWidget
===============================================================================
*/

void GlowLabeledTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabeledTextFieldWidget::Init");
	
	GlowTextFieldWidget::Init(root, parent, params);
	InitLabel(this, params.labelPosition, params.labelWidth,
		params.labelHeight, params.labelSpacing, params.labelText,
		params.labelFont, params.labelColor, params.disableLabelColor);
}


GlowWidget::AutoPackError GlowLabeledTextFieldWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledTextFieldWidget::OnAutoPack");
	
	// Use helper
	AutoPackError result = HelpAutoPack(hSize, vSize, leftMargin, rightMargin,
		topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	result = GlowTextFieldWidget::OnAutoPack(hSize, vSize, hOption,
		vOption, leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	RepositionLabel();
	return noAutoPackError;
}


/*
===============================================================================
	Methods of GlowLabeledHiddenTextFieldWidget
===============================================================================
*/

void GlowLabeledHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	GLOW_DEBUGSCOPE("GlowLabeledHiddenTextFieldWidget::Init");
	
	GlowHiddenTextFieldWidget::Init(root, parent, params, hideCharacter);
	InitLabel(this, params.labelPosition, params.labelWidth,
		params.labelHeight, params.labelSpacing, params.labelText,
		params.labelFont, params.labelColor, params.disableLabelColor);
}


GlowWidget::AutoPackError GlowLabeledHiddenTextFieldWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledHiddenTextFieldWidget::OnAutoPack");
	
	// Use helper
	AutoPackError result = HelpAutoPack(hSize, vSize, leftMargin, rightMargin,
		topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	result = GlowTextFieldWidget::OnAutoPack(hSize, vSize, hOption,
		vOption, leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	RepositionLabel();
	return noAutoPackError;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

