/*
===============================================================================

	FILE:  glowTextAreaWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Scrollable text area widget for GLOW
	
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

#ifndef GLOW_TEXTAREAWIDGET__H
	#include "glowTextAreaWidget.h"
#endif
#ifndef GLOW_SCROLLBARWIDGET__H
	#include "glowScrollBarWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowTextAreaParams GlowTextAreaParams::defaults(true);

GlowTextAreaParams::GlowTextAreaParams()
{
	*this = defaults;
}

GlowTextAreaParams::GlowTextAreaParams(bool) :
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
	style = GlowTextAreaWidget::loweredStyle;
	initialText = "";
	font = GLUT_BITMAP_HELVETICA_12;
	height = 24;
	selectionStart = 0;
	selectionEnd = 0;
	blinkInterval = 500;
	autoScrollInterval = 50;
	inset = 5;
	caretInset = 2;
	tabLength = 0;
	wrapping = true;
	scrollBarWidth = 0;
	interaction = GlowTextAreaWidget::editableInteraction;
}


/*
===============================================================================
	CLASS Glow_TextArea_MiscReceiver
===============================================================================
*/

class Glow_TextArea_MiscReceiver :
	public GlowDeferredTask,
	public GlowScrollBarReceiver
{
	public:
	
		inline Glow_TextArea_MiscReceiver(
			GlowTextAreaWidget* widget);
	
	protected:
	
		virtual void Task();
		virtual void OnMessage(
			const GlowScrollBarMessage& message);
	
	private:
	
		GlowTextAreaWidget* widget_;
};


inline Glow_TextArea_MiscReceiver::Glow_TextArea_MiscReceiver(
	GlowTextAreaWidget* widget)
{
	widget_ = widget;
}


void Glow_TextArea_MiscReceiver::Task()
{
	widget_->blink_ = !widget_->blink_;
	widget_->Refresh();
	Schedule(widget_->blinkInterval_);
}


void Glow_TextArea_MiscReceiver::OnMessage(
	const GlowScrollBarMessage& message)
{
	if (message.widget == widget_->hScrollBar_)
	{
		widget_->hpos_ = message.topValue;
	}
	else if (message.widget == widget_->vScrollBar_)
	{
		widget_->vpos_ = message.topValue;
	}
	widget_->Refresh();
}


/*
===============================================================================
	Autoscroll timer function
===============================================================================
*/

static int autoScrollTimerID_ = 0;


class Glow_TextArea_AutoScrollTimer :
	public GlowTimerReceiver
{
	public:
	
		inline Glow_TextArea_AutoScrollTimer();
		inline void SetTextField(
			GlowTextAreaWidget* widget);
		inline void ForceRemove(
			GlowTextAreaWidget* widget);
	
	protected:
	
		virtual void OnMessage(
			const GlowTimerMessage& message);
	
	private:
	
		GlowTextAreaWidget* widget_;
};


inline Glow_TextArea_AutoScrollTimer::Glow_TextArea_AutoScrollTimer()
{
	widget_ = 0;
}


inline void Glow_TextArea_AutoScrollTimer::SetTextField(
	GlowTextAreaWidget* widget)
{
	widget_ = widget;
}


inline void Glow_TextArea_AutoScrollTimer::ForceRemove(
	GlowTextAreaWidget* widget)
{
	if (widget_ == widget)
	{
		Glow::UnregisterTimer(autoScrollTimerID_);
	}
}


void Glow_TextArea_AutoScrollTimer::OnMessage(
	const GlowTimerMessage& message)
{
	widget_->HandleAutoScrollTimer_();
}


static Glow_TextArea_AutoScrollTimer* autoScrollTimer_ =
	new Glow_TextArea_AutoScrollTimer;


/*
===============================================================================
	Methods of GlowTextAreaWidget
===============================================================================
*/

void GlowTextAreaWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowTextAreaParams& params)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	style_ = params.style;
	if (params.initialText != 0)
	{
		data_.String().assign(params.initialText);
	}
	font_ = params.font;
	hpos_ = 0;
	vpos_ = 0;
	tabLen_ = params.tabLength;
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
	wrapping_ = params.wrapping;
	data_.RecalcLineBreaks(font_, wrapping_ ? params.width-inset_-inset_ : 0);
	interaction_ = params.interaction;
	miscReceiver_ = new Glow_TextArea_MiscReceiver(this);
	scrollBarWidth_ = GLOW_STD::max(0, params.scrollBarWidth);
	if (scrollBarWidth_ > 0)
	{
		GlowScrollBarParams params;
		params.x = Width();
		params.y = 0;
		params.width = scrollBarWidth_;
		params.height = Height();
		params.min = 0;
		params.span = Height()-inset_-inset_;
		params.max = GLOW_STD::max(params.span,
			long(data_.NumLines()*font_.Leading()));
		params.initialTop = 0;
		params.arrowStep = font_.Leading();
		params.pageStep = Height()-inset_-inset_;
		params.receiver = miscReceiver_;
		vScrollBar_ = new GlowScrollBarWidget(this, params);
		if (wrapping_)
		{
			hScrollBar_ = 0;
		}
		else
		{
			params.x = 0;
			params.y = Height();
			params.width = Width();
			params.height = scrollBarWidth_;
			params.min = 0;
			params.span = Width()-inset_-inset_;
			params.max = GLOW_STD::max(params.span,
				long(data_.MaxPixelWidth(font_)));
			params.arrowStep = 10;
			params.pageStep = Width()-inset_-inset_;
			hScrollBar_ = new GlowScrollBarWidget(this, params);
		}
	}
	else
	{
		hScrollBar_ = 0;
		vScrollBar_ = 0;
	}
	
	if (interaction_ != noInteraction)
	{
		RegisterMouseEvents();
	}
	if (interaction_ == editableInteraction)
	{
		RegisterKeyboardEvents();
	}
}


GlowTextAreaWidget::~GlowTextAreaWidget()
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::~GlowTextAreaWidget");
	
	autoScrollTimer_->ForceRemove(this);
	delete miscReceiver_;
}


void GlowTextAreaWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnWidgetPaint");
	
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
	
	int firstLine = vpos_/font_.Leading();
	int lastLine = (vpos_+Height()-inset_-inset_-1)/font_.Leading();
	
	// Highlight backing
	if (interaction_ != noInteraction && data_.SelectionLength() > 0 &&
		(interaction_ == selectableInteraction || HasKeyboardFocus()))
	{
		hiliteBackColor_.Apply();
		int startSelLine = data_.LineNumOf(data_.SelectionStart());
		int endSelLine = data_.LineNumOf(data_.SelectionEnd());
		int startLine = GLOW_STD::max(firstLine, startSelLine);
		int endLine = GLOW_STD::min(lastLine, endSelLine);
		float left = 0;
		float top = 0;
		float right = 0;
		float bottom = 0;
		for (int line=startLine; line<=endLine; ++line)
		{
			NormalizeCoordinates((line > startLine) ? inset_ : (inset_ - hpos_ +
				data_.PixelPosOf(font_, data_.SelectionStart(), startSelLine)),
				inset_-vpos_+(line*font_.Leading()), left, top);
			NormalizeCoordinates((line < endLine) ? (Width()-inset_) : (inset_-hpos_+
				data_.PixelPosOf(font_, data_.SelectionEnd(), endSelLine)),
				inset_-vpos_+((line+1)*font_.Leading()), right, bottom);
			::glRectf(left, bottom, right, top);
		}
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
	int endLine = GLOW_STD::min(lastLine, data_.NumLines()-1);
	float x = 0;
	float y = 0;
	for (int line=firstLine; line<=endLine; ++line)
	{
		data_.CalcLineDrawInfo(font_, line, hpos_, Width()-inset_-inset_,
			startpos, endpos, pixoffset);
		const char* dat = data_.String().data();
		NormalizeCoordinates(inset_+1, inset_+1, x, y);
		::glRasterPos2f(x, y);
		::glBitmap(0, 0, 0, 0, -pixoffset,
			vpos_-(line*font_.Leading())-font_.BaselinePos()+1, 0);
		for (int i=startpos; i<endpos; ++i)
		{
			if (dat[i] == '\t')
			{
				for (int j=0; j<tabLen_; ++j)
				{
					::glutBitmapCharacter(font_, ' ');
				}
			}
			else
			{
				::glutBitmapCharacter(font_, dat[i]);
			}
		}
	}
	
	// Insertion caret
	// Special scissoring
	if (interaction_ == editableInteraction &&
		(blink_ || !HasKeyboardFocus() || !IsActive()))
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
		int caretLine = data_.LineNumOf(dragEnd_);
		NormalizeCoordinates(0, inset_-vpos_+(caretLine*font_.Leading()), horiz, top);
		NormalizeCoordinates(inset_-hpos_+data_.PixelPosOf(font_, dragEnd_, caretLine),
			inset_-vpos_+((caretLine+1)*font_.Leading()), horiz, bottom);
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
		autoScrollTimer_->SetTextField(this);
		autoScrollTimerID_ = Glow::RegisterTimer(autoScrollInterval_, autoScrollTimer_);
	}
}


int GlowTextAreaWidget::CalcPos(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::CalcPos");
	
	int mouseLine = (y-inset_+vpos_)/font_.Leading();
	if (mouseLine < 0) return 0;
	if (mouseLine >= data_.NumLines()) return data_.String().size();
	int pos = data_.AtPixelPos(font_, mouseLine, x-inset_+hpos_);
	if (mouseLine < data_.NumLines()-1 &&
		pos >= data_.LineStartPos(mouseLine+1))
	{
		pos = data_.LineStartPos(mouseLine+1)-1;
	}
	return pos;
}


void GlowTextAreaWidget::HandleAutoScrollTimer_()
{
	dragEnd_ = CalcPos(dragX_, dragY_);
	data_.SetSelection(dragStart_, dragEnd_);
	CheckAutoScroll();
	Refresh();
	toggleAutoScroll_ = true;
}


void GlowTextAreaWidget::OnGotKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnGotKeyboardFocus");
	
	miscReceiver_->Schedule(blinkInterval_);
}


void GlowTextAreaWidget::OnLostKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnLostKeyboardFocus");
	
	miscReceiver_->Unschedule();
}


void GlowTextAreaWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnWidgetMouseDown");
	
	if (!HasKeyboardFocus() && interaction_ == editableInteraction)
	{
		GrabKeyboardFocus();
	}
	dragX_ = x;
	dragY_ = y;
	dragEnd_ = CalcPos(dragX_, dragY_);
	if (modifiers & Glow::shiftModifier)
	{
		data_.SetSelection(dragStart_, dragEnd_);
	}
	else
	{
		dragStart_ = dragEnd_;
		data_.SetSelection(dragStart_);
	}
	CheckAutoScroll();
	toggleAutoScroll_ = true;
	Refresh();
}


void GlowTextAreaWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnWidgetMouseUp");
	
	dragEnd_ = CalcPos(x, y);
	data_.SetSelection(dragStart_, dragEnd_);
	CheckAutoScroll();
	Glow::UnregisterTimer(autoScrollTimerID_);
	toggleAutoScroll_ = false;
	Refresh();
}


void GlowTextAreaWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnWidgetMouseDrag");
	
	dragY_ = y;
	dragX_ = x;
	dragEnd_ = CalcPos(x, y);
	data_.SetSelection(dragStart_, dragEnd_);
	Refresh();
}


void GlowTextAreaWidget::OnWidgetKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnWidgetKeyboard");
	
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
		else if (key == Glow::upArrowKey && dragEnd_ > 0)
		{
			int line = data_.LineNumOf(dragEnd_);
			dragEnd_ = (line == 0) ? 0 : data_.AtPixelPos(font_, line-1,
				data_.PixelPosOf(font_, dragEnd_, line));
			if (line > 0 && dragEnd_ >= data_.LineStartPos(line))
			{
				dragEnd_ = data_.LineStartPos(line)-1;
			}
			if (modifiers & Glow::shiftModifier)
			{
				data_.SetSelection(dragStart_, dragEnd_);
			}
			else
			{
				dragStart_ = dragEnd_;
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
		else if (key == Glow::downArrowKey && dragEnd_ < int(data_.String().size()))
		{
			int line = data_.LineNumOf(dragEnd_);
			dragEnd_ = (line == data_.NumLines()-1) ? int(data_.String().size()) :
				data_.AtPixelPos(font_, line+1, data_.PixelPosOf(font_, dragEnd_, line));
			if (line+2 < data_.NumLines() && dragEnd_ >= data_.LineStartPos(line+2))
			{
				dragEnd_ = data_.LineStartPos(line+2)-1;
			}
			if (modifiers & Glow::shiftModifier)
			{
				data_.SetSelection(dragStart_, dragEnd_);
			}
			else
			{
				dragStart_ = dragEnd_;
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
			RecalcLineBreaks(dragStart_);
			UpdateScrollBars(false);
			CheckAutoScroll();
			Refresh();
		}
		else if (data_.SelectionStart() > 0)
		{
			dragStart_ = dragEnd_ = data_.SelectionStart()-1;
			data_.SetSelection(dragStart_);
			data_.String().erase(dragStart_, 1);
			RecalcLineBreaks(dragStart_);
			UpdateScrollBars(false);
			CheckAutoScroll();
			Refresh();
		}
	}
	else if (key == Glow::enterKey || key == Glow::tabKey || key >= 32)
	{
		data_.ReplaceSelectionWith((unsigned char)key);
		data_.SetSelection(data_.SelectionEnd(), data_.SelectionEnd());
		dragStart_ = dragEnd_ = data_.SelectionEnd();
		RecalcLineBreaks(dragStart_);
		UpdateScrollBars(false);
		CheckAutoScroll();
		Refresh();
	}
}


void GlowTextAreaWidget::OnWidgetReshape(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnWidgetReshape");
	
	RecalcLineBreaks();
	CheckAutoScroll();
	UpdateScrollBars(true);
	Refresh();
}


GlowWidget::AutoPackError GlowTextAreaWidget::OnAutoPack( // TODO
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::OnAutoPack");
	
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


void GlowTextAreaWidget::SetText(
	const char* str)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::SetText");
	
	data_.String().assign(str);
	data_.RevalidateSelection();
	RecalcLineBreaks();
	UpdateScrollBars(false);
	Refresh();
}


void GlowTextAreaWidget::SetText(
	const GLOW_STD::string& str)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::SetText");
	
	data_.String().assign(str);
	data_.RevalidateSelection();
	RecalcLineBreaks();
	UpdateScrollBars(false);
	Refresh();
}


void GlowTextAreaWidget::SetWrapping(
	bool wrapping)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::SetWrapping");
	
	if (wrapping_ != wrapping)
	{
		wrapping_ = wrapping;
		if (wrapping_)
		{
			delete hScrollBar_;
			hScrollBar_ = 0;
		}
		else
		{
			GlowScrollBarParams params;
			params.x = 0;
			params.y = Height();
			params.width = Width();
			params.height = scrollBarWidth_;
			params.min = 0;
			params.span = Width()-inset_-inset_;
			params.max = GLOW_STD::max(params.span,
				long(data_.MaxPixelWidth(font_)));
			params.initialTop = vpos_;
			params.arrowStep = 10;
			params.pageStep = Width()-inset_-inset_;
			params.receiver = miscReceiver_;
			hScrollBar_ = new GlowScrollBarWidget(this, params);
		}
		RecalcLineBreaks();
		UpdateScrollBars(false);
		CheckAutoScroll();
		Refresh();
	}
}


void GlowTextAreaWidget::SetFont(
	GlowFont font)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::SetFont");
	
	font_ = font;
	if (wrapping_)
	{
		RecalcLineBreaks();
	}
	UpdateScrollBars(false);
	Refresh();
}


void GlowTextAreaWidget::SetTabLength(
	int tabLen)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::SetTabLength");
	
	tabLen_ = tabLen;
	data_.SetTabSize(tabLen);
	if (wrapping_)
	{
		RecalcLineBreaks();
	}
	UpdateScrollBars(false);
	Refresh();
}


void GlowTextAreaWidget::SetInteractionType(
	GlowTextAreaWidget::Interaction interaction)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::SetInteractionType");
	
	if (interaction_ == editableInteraction &&
		interaction != editableInteraction)
	{
		UnregisterKeyboardEvents();
	}
	else if (interaction == editableInteraction &&
		interaction_ != editableInteraction)
	{
		RegisterKeyboardEvents();
	}
	if (interaction_ == noInteraction &&
		interaction != noInteraction)
	{
		RegisterMouseEvents();
	}
	else if (interaction == noInteraction &&
		interaction_ != noInteraction)
	{
		UnregisterMouseEvents();
	}
	interaction_ = interaction;
}


void GlowTextAreaWidget::SetScrollBarWidth(
	int width)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::SetScrollBarWidth");
	
	if (width > 0) width = 0;
	if (width != scrollBarWidth_)
	{
		if (width == 0 && scrollBarWidth_ != 0)
		{
			delete hScrollBar_;
			delete vScrollBar_;
			hScrollBar_ = 0;
			vScrollBar_ = 0;
		}
		else if (width != 0 && scrollBarWidth_ == 0)
		{
		GlowScrollBarParams params;
		params.x = Width();
		params.y = 0;
		params.width = scrollBarWidth_;
		params.height = Height();
		params.min = 0;
		params.span = Height()-inset_-inset_;
		params.max = GLOW_STD::max(params.span,
			long(data_.NumLines()*font_.Leading()));
		params.initialTop = vpos_;
		params.arrowStep = font_.Leading();
		params.pageStep = Height()-inset_-inset_;
		params.receiver = miscReceiver_;
		vScrollBar_ = new GlowScrollBarWidget(this, params);
		if (!wrapping_)
		{
			params.x = 0;
			params.y = Height();
			params.width = Width();
			params.height = scrollBarWidth_;
			params.min = 0;
			params.span = Width()-inset_-inset_;
			params.max = GLOW_STD::max(params.span,
				long(data_.MaxPixelWidth(font_)));
			params.initialTop = hpos_;
			params.arrowStep = 10;
			params.pageStep = Width()-inset_-inset_;
			hScrollBar_ = new GlowScrollBarWidget(this, params);
		}
		}
		else
		{
			vScrollBar_->Reshape(scrollBarWidth_, vScrollBar_->Height());
			if (!wrapping_)
			{
				hScrollBar_->Reshape(hScrollBar_->Width(), scrollBarWidth_);
			}
		}
	}
}


void GlowTextAreaWidget::ReplaceSelectionWith(
	const char* str)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::ReplaceSelectionWith");
	
	data_.ReplaceSelectionWith(str);
	RecalcLineBreaks();
	UpdateScrollBars(false);
	Refresh();
}


void GlowTextAreaWidget::CheckAutoScroll()
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::CheckAutoScroll");
	
	int line = data_.LineNumOf(dragEnd_);
	
	int pos = line*font_.Leading();
	if (pos < vpos_)
	{
		vpos_ = pos;
	}
	else if (pos+font_.Leading() > vpos_+Height()-inset_-inset_)
	{
		vpos_ = pos+font_.Leading()-Height()+inset_+inset_;
	}
	if (scrollBarWidth_ != 0)
	{
		vScrollBar_->SetTopValue(vpos_);
	}
	
	pos = data_.PixelPosOf(font_, dragEnd_, line);
	if (pos < hpos_)
	{
		hpos_ = pos;
	}
	else if (pos > hpos_+Width()-inset_-inset_)
	{
		hpos_ = pos-Width()+inset_+inset_;
	}
	if (scrollBarWidth_ != 0 && !wrapping_)
	{
		hScrollBar_->SetTopValue(hpos_);
	}
}


void GlowTextAreaWidget::UpdateScrollBars(
	bool resizing)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::UpdateScrollBars");
	if (scrollBarWidth_ != 0)
	{
		if (resizing)
		{
			vScrollBar_->Move(Width(), 0);
			vScrollBar_->Reshape(scrollBarWidth_, Height());
			vScrollBar_->SetSpan(Height()-inset_-inset_);
			vScrollBar_->SetPageStep(Height()-inset_-inset_);
		}
		vScrollBar_->SetMaximum(GLOW_STD::max(Height()-inset_-inset_,
			data_.NumLines()*font_.Leading()));
		vScrollBar_->SetArrowStep(font_.Leading());
		vpos_ = vScrollBar_->GetTopValue();
		if (!wrapping_)
		{
			if (resizing)
			{
				hScrollBar_->Move(0, Height());
				hScrollBar_->Reshape(Width(), scrollBarWidth_);
				hScrollBar_->SetSpan(Width()-inset_-inset_);
				hScrollBar_->SetPageStep(Width()-inset_-inset_);
			}
			hScrollBar_->SetMaximum(GLOW_STD::max(Width()-inset_-inset_,
				data_.MaxPixelWidth(font_)));
			hpos_ = hScrollBar_->GetTopValue();
		}
	}
}


void GlowTextAreaWidget::RecalcLineBreaks(
	int pos)
{
	GLOW_DEBUGSCOPE("GlowTextAreaWidget::RecalcLineBreaks");
	
	data_.RecalcLineBreaks(font_, wrapping_ ? Width()-inset_-inset_ : 0,
		GLOW_STD::max(0, data_.LineNumOf(pos)-1));
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

