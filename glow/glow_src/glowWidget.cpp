/*
===============================================================================

	FILE:  glowWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Widget for GLOW
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <iostream>

#ifndef GLOW_WIDGET__H
	#include "glowWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Static data
===============================================================================
*/

GLOW_STD::map<GlowWidget*, Glow_OldWidgetState> GlowWidget::notifyList_;


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWidgetParams::GlowWidgetParams(bool)
{
	clipping = false;
	x = 0;
	y = 0;
	width = 100;
	height = 100;
	refcon = 0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWidgetSubwindowParams GlowWidgetSubwindowParams::defaults(true);

GlowWidgetSubwindowParams::GlowWidgetSubwindowParams()
{
	*this = defaults;
}

GlowWidgetSubwindowParams::GlowWidgetSubwindowParams(bool) :
GlowSubwindowParams(true),
backColor(0.8f, 0.8f, 0.8f)
{
//	mode = Glow::rgbaBuffer | Glow::depthBuffer | 
//		Glow::stencilBuffer | Glow::doubleBuffer;
	eventMask = Glow::mouseEvents | Glow::dragEvents | Glow::keyboardEvents;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWidgetWindowParams GlowWidgetWindowParams::defaults(true);

GlowWidgetWindowParams::GlowWidgetWindowParams()
{
	*this = defaults;
}

GlowWidgetWindowParams::GlowWidgetWindowParams(bool) :
GlowWindowParams(true),
backColor(0.8f, 0.8f, 0.8f)
{
//	mode = Glow::rgbaBuffer | Glow::depthBuffer | 
//		Glow::stencilBuffer | Glow::doubleBuffer;
	eventMask = Glow::mouseEvents | Glow::dragEvents | Glow::keyboardEvents;
}


/*
===============================================================================
	Methods of GlowWidget
===============================================================================
*/

void GlowWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowWidgetParams& params)
{
	GLOW_DEBUGSCOPE("GlowWidget::Init");
	
	if (Glow::widgetNotifier_ == 0)
	{
		Glow::widgetNotifier_ = GlowWidget::ExecuteNotify_;
	}
	
	if (parent == 0)
	{
		GlowComponent::Init(root->Subwindow());
	}
	else
	{
		GlowComponent::Init(parent);
	}
	refcon_ = params.refcon;
	xpos_ = params.x;
	ypos_ = params.y;
	width_ = params.width;
	height_ = params.height;
	clipping_ = params.clipping;
	parentWidget_ = parent;
	root_ = root;
	receivingMouse_ = false;
	receivingKeyboard_ = false;
	hasFocus_ = false;
	refreshEnabled_ = true;
	if (parent == 0 || parent->IsVisible())
	{
		visibility_ = 1;
	}
	else
	{
		visibility_ = 2;
	}
	Refresh();
}


GlowWidget::~GlowWidget()
{
	GLOW_DEBUGSCOPE("GlowWidget::~GlowWidget");
	
	UnregisterMouseEvents();
	UnregisterKeyboardEvents();
	Refresh();
	
	GLOW_STD::map<GlowWidget*, Glow_OldWidgetState>::iterator iter =
		notifyList_.find(this);
	if (iter != notifyList_.end())
	{
		notifyList_.erase(iter);
	}
}


int GlowWidget::RootPositionX() const
{
	return parentWidget_ == 0 ?
		xpos_ : parentWidget_->RootPositionX() + xpos_;
}


int GlowWidget::RootPositionY() const
{
	return parentWidget_ == 0 ?
		ypos_ : parentWidget_->RootPositionY() + ypos_;
}


GlowWidget::AutoPackError GlowWidget::AutoPack(
	int leftLimit,
	int rightLimit,
	int topLimit,
	int bottomLimit,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowWidget::AutoPack");
	
	// Make sure positions are specified for justified moves
	GLOW_DEBUG(
		(hOption & posOptionMask) == leftPos && leftLimit == unspecifiedPos,
		"unspecified left position");
	GLOW_DEBUG(
		(hOption & posOptionMask) == rightPos && rightLimit == unspecifiedPos,
		"unspecified right position");
	GLOW_DEBUG(
		(hOption & posOptionMask) == centerPos && leftLimit == unspecifiedPos,
		"unspecified horizontal center position");
	GLOW_DEBUG(
		(vOption & posOptionMask) == topPos && topLimit == unspecifiedPos,
		"unspecified top position");
	GLOW_DEBUG(
		(vOption & posOptionMask) == bottomPos && bottomLimit == unspecifiedPos,
		"unspecified bottom position");
	GLOW_DEBUG(
		(vOption & posOptionMask) == centerPos && topLimit == unspecifiedPos,
		"unspecified vertical center position");
	
	// Determine sizes
	int hSize = (leftLimit == unspecifiedPos || rightLimit == unspecifiedPos) ?
		unspecifiedSize : rightLimit - leftLimit;
	int vSize = (topLimit == unspecifiedPos || bottomLimit == unspecifiedPos) ?
		unspecifiedSize : bottomLimit - topLimit;
	
	// Make sure size is specified for requestedSize or forcedSize
	GLOW_DEBUG(hSize == unspecifiedSize &&
		((hOption & sizeOptionMask) == expandPreferredSize ||
		(hOption & sizeOptionMask) == forcedSize),
		"Trying to fill unspecified horizontal size");
	GLOW_DEBUG(vSize == unspecifiedSize &&
		((vOption & sizeOptionMask) == expandPreferredSize ||
		(vOption & sizeOptionMask) == forcedSize),
		"Trying to fill unspecified vertical size");
	
	// Reshape
	leftMargin = 0;
	rightMargin = 0;
	topMargin = 0;
	bottomMargin = 0;
	AutoPackError result = OnAutoPack(hSize, vSize,
		hOption & sizeOptionMask, vOption & sizeOptionMask,
		leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	// Adjust limits according to margins
	if (leftLimit != unspecifiedPos)
	{
		leftLimit += leftMargin;
	}
	if (rightLimit != unspecifiedPos)
	{
		rightLimit -= rightMargin;
	}
	if (topLimit != unspecifiedPos)
	{
		topLimit += topMargin;
	}
	if (bottomLimit != unspecifiedPos)
	{
		bottomLimit -= bottomMargin;
	}
	
	// Move and justify
	int hpos = PositionX();
	if ((hOption & posOptionMask) == leftPos)
	{
		hpos = leftLimit;
	}
	else if ((hOption & posOptionMask) == rightPos)
	{
		hpos = rightLimit-Width();
	}
	else if ((hOption & posOptionMask) == centerPos)
	{
		if (rightLimit == unspecifiedPos)
		{
			rightLimit = leftLimit;
		}
		hpos = (rightLimit+leftLimit-Width())/2;
	}
	
	int vpos = PositionY();
	if ((vOption & posOptionMask) == topPos)
	{
		vpos = topLimit;
	}
	else if ((vOption & posOptionMask) == bottomPos)
	{
		vpos = bottomLimit-Height();
	}
	else if ((vOption & posOptionMask) == centerPos)
	{
		if (bottomLimit == unspecifiedPos)
		{
			bottomLimit = topLimit;
		}
		vpos = (bottomLimit+topLimit-Height())/2;
	}
	
	Move(hpos, vpos);
	
	return noAutoPackError;
}


GlowWidget::AutoPackError GlowWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowWidget::OnAutoPack");
	
	if (hOption != noReshape &&
		((hSize != unspecifiedSize && hSize < Width()) ||
		(hOption == forcedSize && hSize != Width())))
	{
		return hAutoPackError;
	}
	if (vOption != noReshape &&
		((vSize != unspecifiedSize && vSize < Height()) ||
		(vOption == forcedSize && vSize != Height())))
	{
		return vAutoPackError;
	}
	
	return noAutoPackError;
}


void GlowWidget::OnActivate()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnActivate");
	
	if (visibility_ == 1)
	{
		if (receivingMouse_)
		{
			root_->RegisterMouseWidget_(this);
		}
		if (receivingKeyboard_)
		{
			root_->RegisterKeyboardWidget_(this);
		}
		Refresh();
	}
	OnWidgetActivate();
}


void GlowWidget::OnDeactivate()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnDeactivate");
	
	if (visibility_ == 1)
	{
		if (receivingMouse_)
		{
			root_->UnregisterMouseWidget_(this);
		}
		if (receivingKeyboard_)
		{
			root_->UnregisterKeyboardWidget_(this);
		}
		Refresh();
	}
	OnWidgetDeactivate();
}


bool GlowWidget::OnBeginPaint()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnBeginPaint");
	
	if (visibility_ != 1)
	{
		return false;
	}
	::glViewport(
		RootPositionX(), root_->Subwindow()->Height()-RootPositionY()-height_,
		width_, height_);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	if (clipping_)
	{
		::glGetIntegerv(GL_SCISSOR_BOX, oldScissor_);
		int x = RootPositionX();
		if (x < oldScissor_[0]) x = oldScissor_[0];
		int y = root_->Subwindow()->Height()-Height()-RootPositionY();
		if (y < oldScissor_[1]) y = oldScissor_[1];
		int w = Width();
		if (x+w>oldScissor_[0]+oldScissor_[2]) w = oldScissor_[0]+oldScissor_[2]-x;
		int h = Height();
		if (y+h>oldScissor_[1]+oldScissor_[3]) h = oldScissor_[1]+oldScissor_[3]-y;
		::glScissor(x, y, w, h);
	}
	OnWidgetPaint();
	return true;
}


void GlowWidget::OnEndPaint()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnEndPaint");
	
	if (clipping_ && visibility_ == 1)
	{
		::glScissor(oldScissor_[0], oldScissor_[1], oldScissor_[2], oldScissor_[3]);
	}
}


void GlowWidget::ExecuteNotify_()
{
	GLOW_DEBUGSCOPE("GlowWidget::ExecuteNotify_");
	
	for (GLOW_STD::map<GlowWidget*, Glow_OldWidgetState>::iterator iter =
		notifyList_.begin(); iter != notifyList_.end(); ++iter)
	{
		(*iter).first->NotifyOne_((*iter).second);
	}
	notifyList_.clear();
}


void GlowWidget::BroadcastNotifyList_()
{
	GLOW_DEBUGSCOPE("GlowWidget::BroadcastNotifyList_");
	
	AddToNotifyList_();
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
	{
		GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
		if (childWidget != 0)
		{
			childWidget->BroadcastNotifyList_();
		}
	}
}


void GlowWidget::AddToNotifyList_()
{
	GLOW_DEBUGSCOPE("GlowWidget::AddToNotifyList_");
	
	GLOW_STD::map<GlowWidget*, Glow_OldWidgetState>::iterator iter =
		notifyList_.find(this);
	if (iter == notifyList_.end())
	{
		notifyList_.insert(
			GLOW_STD::pair<GlowWidget* const, Glow_OldWidgetState>(this,
				Glow_OldWidgetState(visibility_==1, hasFocus_, width_, height_,
					RootPositionX(), RootPositionY())));
	}
}


void GlowWidget::NotifyOne_(
	const Glow_OldWidgetState& oldState)
{
	GLOW_DEBUGSCOPE("GlowWidget::NotifyOne_");
	
	if (!oldState.visible && IsVisible())
	{
		OnWidgetVisible();
	}
	if (oldState.width != width_ ||
		oldState.height != height_)
	{
		OnWidgetReshape();
	}
	if (oldState.x != RootPositionX() ||
		oldState.y != RootPositionY())
	{
		OnWidgetMove();
	}
	if (oldState.keyboardFocus && !HasKeyboardFocus())
	{
		OnLostKeyboardFocus();
	}
	else if (!oldState.keyboardFocus && HasKeyboardFocus())
	{
		OnGotKeyboardFocus();
	}
	if (oldState.visible && !IsVisible())
	{
		OnWidgetInvisible();
	}
}


void GlowWidget::BroadcastMask_(
	bool unmasking)
{
	if (unmasking && visibility_ == 2)
	{
		AddToNotifyList_();
		visibility_ = 1;
		if (IsActive())
		{
			if (receivingMouse_)
			{
				root_->RegisterMouseWidget_(this);
			}
			if (receivingKeyboard_)
			{
				root_->RegisterKeyboardWidget_(this);
			}
		}
		for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
		{
			GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
			if (childWidget != 0)
			{
				childWidget->BroadcastMask_(true);
			}
		}
	}
	else if (!unmasking && visibility_ == 1)
	{
		AddToNotifyList_();
		visibility_ = 2;
		if (IsActive())
		{
			if (receivingMouse_)
			{
				root_->UnregisterMouseWidget_(this);
			}
			if (receivingKeyboard_)
			{
				root_->UnregisterKeyboardWidget_(this);
			}
		}
		for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
		{
			GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
			if (childWidget != 0)
			{
				childWidget->BroadcastMask_(false);
			}
		}
	}
}


void GlowWidget::Show()
{
	GLOW_DEBUGSCOPE("GlowWidget::Show");
	
	if (visibility_ == 0)
	{
		if (parentWidget_==0 || parentWidget_->IsVisible())
		{
			AddToNotifyList_();
			visibility_ = 1;
			if (IsActive())
			{
				if (receivingMouse_)
				{
					root_->RegisterMouseWidget_(this);
				}
				if (receivingKeyboard_)
				{
					root_->RegisterKeyboardWidget_(this);
				}
			}
			for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
			{
				GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
				if (childWidget != 0)
				{
					childWidget->BroadcastMask_(true);
				}
			}
			Refresh();
		}
		else
		{
			visibility_ = 2;
		}
	}
}


void GlowWidget::Hide()
{
	GLOW_DEBUGSCOPE("GlowWidget::Hide");
	
	if (visibility_ == 1)
	{
		AddToNotifyList_();
		visibility_ = 0;
		if (IsActive())
		{
			if (receivingMouse_)
			{
				root_->UnregisterMouseWidget_(this);
			}
			if (receivingKeyboard_)
			{
				root_->UnregisterKeyboardWidget_(this);
			}
		}
		for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
		{
			GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
			if (childWidget != 0)
			{
				childWidget->BroadcastMask_(false);
			}
		}
		Refresh();
	}
	else if (visibility_ == 2)
	{
		visibility_ = 0;
	}
}


void GlowWidget::OnWidgetPaint()
{
}


void GlowWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
}


void GlowWidget::OnWidgetKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowWidget::OnGotKeyboardFocus()
{
}


void GlowWidget::OnLostKeyboardFocus()
{
}


void GlowWidget::OnWidgetMove()
{
}


void GlowWidget::OnWidgetReshape()
{
}


void GlowWidget::OnWidgetVisible()
{
}


void GlowWidget::OnWidgetInvisible()
{
}


void GlowWidget::OnWidgetActivate()
{
}


void GlowWidget::OnWidgetDeactivate()
{
}


/*
===============================================================================
	Methods of GlowWidgetRoot
===============================================================================
*/

void GlowWidgetRoot::Init(
	GlowSubwindow* subwindow,
	GlowColor backColor)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::Init");
	
	subwindow_ = subwindow;
	backColor_ = backColor;
	curKeyboardFocus_ = keyboardWidgets_.end();
	leftButton_ = 0;
	middleButton_ = 0;
	rightButton_ = 0;
}


GlowWidgetRoot::~GlowWidgetRoot()
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::~GlowWidgetRoot");
	
	GLOW_STD::list<GlowWidget*>::iterator iter;
	while ((iter = mouseWidgets_.begin()) != mouseWidgets_.end())
	{
		(*iter)->UnregisterMouseEvents();
	}
	while ((iter = keyboardWidgets_.begin()) != keyboardWidgets_.end())
	{
		(*iter)->UnregisterKeyboardEvents();
	}
}


void GlowWidgetRoot::SetKeyboardFocus(
	GlowWidget* widget)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::SetKeyboardFocus");
	
	if (widget == 0)
	{
		if (curKeyboardFocus_ != keyboardWidgets_.end())
		{
			(*curKeyboardFocus_)->SetHasKeyboardFocus_(false);
			curKeyboardFocus_ = keyboardWidgets_.end();
		}
	}
	else
	{
		GLOW_STD::list<GlowWidget*>::iterator iter = GLOW_STD::find(
			keyboardWidgets_.begin(), keyboardWidgets_.end(), widget);
		GLOW_DEBUG(iter == keyboardWidgets_.end(), "Illegal keyboard focus");
		if (iter != curKeyboardFocus_)
		{
			if (curKeyboardFocus_ != keyboardWidgets_.end())
			{
				(*curKeyboardFocus_)->SetHasKeyboardFocus_(false);
			}
			widget->SetHasKeyboardFocus_(true);
			curKeyboardFocus_ = iter;
		}
	}
}


void GlowWidgetRoot::AdvanceKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::AdvanceKeyboardFocus");
	
	if (curKeyboardFocus_ != keyboardWidgets_.end())
	{
		(*curKeyboardFocus_)->SetHasKeyboardFocus_(false);
		++curKeyboardFocus_;
	}
	if (curKeyboardFocus_ == keyboardWidgets_.end())
	{
		curKeyboardFocus_ = keyboardWidgets_.begin();
	}
	if (curKeyboardFocus_ != keyboardWidgets_.end())
	{
		(*curKeyboardFocus_)->SetHasKeyboardFocus_(true);
	}
}


GlowWidget* GlowWidgetRoot::FindWidget(
	int& x,
	int& y)
{
	for (GLOW_STD::list<GlowWidget*>::iterator iter = mouseWidgets_.begin();
		iter != mouseWidgets_.end(); ++iter)
	{
		GlowWidget* widget = (*iter);
		int xmin = widget->RootPositionX();
		int ymin = widget->RootPositionY();
		if (x>=xmin && y>=ymin &&
			x<=xmin+widget->Width() && y<=ymin+widget->Height())
		{
			x -= xmin;
			y -= ymin;
			return widget;
		}
	}
	return 0;
}


void GlowWidgetRoot::WRMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRMouseDown");
	
	GlowWidget* widget = FindWidget(x, y);
	if (widget != 0)
	{
		if (button == Glow::leftButton)
		{
			leftButton_ = widget;
		}
		else if (button == Glow::middleButton)
		{
			middleButton_ = widget;
		}
		else //if (button == Glow::rightButton)
		{
			rightButton_ = widget;
		}
		widget->OnWidgetMouseDown(button, x, y, modifiers);
	}
}


void GlowWidgetRoot::WRMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRMouseUp");
	
	GlowWidget* widget = 0;
	if (button == Glow::leftButton)
	{
		widget = leftButton_;
		leftButton_ = 0;
	}
	else if (button == Glow::middleButton)
	{
		widget = middleButton_;
		middleButton_ = 0;
	}
	else if (button == Glow::rightButton)
	{
		widget = rightButton_;
		rightButton_ = 0;
	}
	if (widget != 0)
	{
		widget->OnWidgetMouseUp(button, x-widget->RootPositionX(),
			y-widget->RootPositionY(), modifiers);
	}
}


void GlowWidgetRoot::WRMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRMouseDrag");
	
	if (leftButton_ != 0)
	{
		int xmin = leftButton_->RootPositionX();
		int ymin = leftButton_->RootPositionY();
		leftButton_->OnWidgetMouseDrag(x-xmin, y-ymin);
	}
	if (middleButton_ != 0 && middleButton_ != leftButton_)
	{
		int xmin = middleButton_->RootPositionX();
		int ymin = middleButton_->RootPositionY();
		middleButton_->OnWidgetMouseDrag(x-xmin, y-ymin);
	}
	if (rightButton_ != 0 && rightButton_ != leftButton_ && rightButton_ != middleButton_)
	{
		int xmin = rightButton_->RootPositionX();
		int ymin = rightButton_->RootPositionY();
		rightButton_->OnWidgetMouseDrag(x-xmin, y-ymin);
	}
}


void GlowWidgetRoot::WRKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRKeyboard");
	
	GlowWidgetKeyboardData filterData;
	filterData.root = this;
	filterData.key = key;
	filterData.x = x;
	filterData.y = y;
	filterData.modifiers = modifiers;
	keyboardFilters_.Send(filterData);
	if (filterData.continue_ &&
		filterData.root->curKeyboardFocus_ != keyboardWidgets_.end())
	{
		GlowWidget* widget = *(filterData.root->curKeyboardFocus_);
		widget->OnWidgetKeyboard(filterData.key,
			filterData.x-widget->RootPositionX(),
			filterData.y-widget->RootPositionY(), filterData.modifiers);
	}
}


bool GlowWidgetRoot::WRBeginPaint()
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRBeginPaint");
	
	::glViewport(0, 0, subwindow_->Width(), subwindow_->Height());
	::glClearColor(float(backColor_[0])/255.0f, float(backColor_[1])/255.0f,
		float(backColor_[2])/255.0f, 0.0f);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::glEnable(GL_SCISSOR_TEST);
	::glScissor(0, 0, subwindow_->Width(), subwindow_->Height());
	return true;
}


void GlowWidgetRoot::WREndPaint()
{
	::glDisable(GL_SCISSOR_TEST);
}


void GlowWidgetRoot::UnregisterMouseWidget_(
	GlowWidget* widget)
{
	mouseWidgets_.remove(widget);
	if (leftButton_ == widget)
	{
		leftButton_ = 0;
	}
	if (middleButton_ == widget)
	{
		middleButton_ = 0;
	}
	if (rightButton_ == widget)
	{
		rightButton_ = 0;
	}
}


void GlowWidgetRoot::UnregisterKeyboardWidget_(
	GlowWidget* widget)
{
	if (curKeyboardFocus_ != keyboardWidgets_.end() && widget == *curKeyboardFocus_)
	{
		widget->SetHasKeyboardFocus_(false);
		++curKeyboardFocus_;
		if (curKeyboardFocus_ == keyboardWidgets_.end())
		{
			curKeyboardFocus_ = keyboardWidgets_.begin();
		}
		if (widget == *curKeyboardFocus_)
		{
			curKeyboardFocus_ = keyboardWidgets_.end();
		}
		if (curKeyboardFocus_ != keyboardWidgets_.end())
		{
			(*curKeyboardFocus_)->SetHasKeyboardFocus_(true);
		}
	}
	keyboardWidgets_.remove(widget);
}


/*
===============================================================================
	Methods of GlowWidgetKeyboardFilter
===============================================================================
*/

void GlowWidgetKeyboardFilter::OnMessage(
	GlowWidgetKeyboardData& message)
{
	if (message.continue_)
	{
		message.continue_ = OnFilter(message);
	}
}


/*
===============================================================================
	Methods of GlowWidgetTabFilter
===============================================================================
*/

bool GlowWidgetTabFilter::OnFilter(
	GlowWidgetKeyboardData& data)
{
	if (data.key == Glow::tabKey)
	{
		data.root->AdvanceKeyboardFocus();
	}
	return data.key != Glow::tabKey;
}


/*
===============================================================================
	Methods of GlowWidgetSubwindow
===============================================================================
*/

void GlowWidgetSubwindow::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseDown(button, x, y, modifiers);
}


void GlowWidgetSubwindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseUp(button, x, y, modifiers);
}


void GlowWidgetSubwindow::OnMouseDrag(
	int x,
	int y)
{
	WRMouseDrag(x, y);
}


void GlowWidgetSubwindow::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRKeyboard(key, x, y, modifiers);
}


bool GlowWidgetSubwindow::OnBeginPaint()
{
	return WRBeginPaint();
}


void GlowWidgetSubwindow::OnEndPaint()
{
	WREndPaint();
}


/*
===============================================================================
	Methods of GlowWidgetWindow
===============================================================================
*/

void GlowWidgetWindow::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseDown(button, x, y, modifiers);
}


void GlowWidgetWindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseUp(button, x, y, modifiers);
}


void GlowWidgetWindow::OnMouseDrag(
	int x,
	int y)
{
	WRMouseDrag(x, y);
}


void GlowWidgetWindow::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRKeyboard(key, x, y, modifiers);
}


bool GlowWidgetWindow::OnBeginPaint()
{
	return WRBeginPaint();
}


void GlowWidgetWindow::OnEndPaint()
{
	WREndPaint();
}


/*
===============================================================================
	Methods of GlowFixedSizeWidgetWindow
===============================================================================
*/

void GlowFixedSizeWidgetWindow::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseDown(button, x, y, modifiers);
}


void GlowFixedSizeWidgetWindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseUp(button, x, y, modifiers);
}


void GlowFixedSizeWidgetWindow::OnMouseDrag(
	int x,
	int y)
{
	WRMouseDrag(x, y);
}


void GlowFixedSizeWidgetWindow::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRKeyboard(key, x, y, modifiers);
}


bool GlowFixedSizeWidgetWindow::OnBeginPaint()
{
	return WRBeginPaint();
}


void GlowFixedSizeWidgetWindow::OnEndPaint()
{
	WREndPaint();
}


/*
===============================================================================
	Methods of GlowSubwindowInWidget
===============================================================================
*/

void GlowSubwindowInWidget::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSubwindowInWidget::OnKeyboard");
	
	Glow::DeliverKeyboardEvt(
		static_cast<GlowWidget*>(Parent())->Root()->Subwindow(),
		key, x, y, modifiers);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

