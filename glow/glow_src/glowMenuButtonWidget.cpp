/*
===============================================================================

	FILE:  glowMenuButtonWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Menu button for GLOW
	
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
	
		The GLOW Toolkit -- version 1.0.0  (29 June 2000)
	
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

#ifndef GLOW_MENUBUTTONWIDGET__H
	#include "glowMenuButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	GlowMenuButtonParams defaults
===============================================================================
*/

GlowMenuButtonParams GlowMenuButtonParams::defaults(true);

GlowMenuButtonParams::GlowMenuButtonParams()
{
	*this = defaults;
}

GlowMenuButtonParams::GlowMenuButtonParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
iconColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(1.0f, 0.7f, 0.7f),
hiliteIconColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableIconColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	width = 100;
	height = 25;
	menu = 0;
	spacing = 10;
	iconType = GlowMenuButtonWidget::menuIcon;
}


/*
===============================================================================
	GlowPopupMenuParams defaults
===============================================================================
*/

GlowPopupMenuParams GlowPopupMenuParams::defaults(true);

GlowPopupMenuParams::GlowPopupMenuParams()
{
	*this = defaults;
}

GlowPopupMenuParams::GlowPopupMenuParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
iconColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(1.0f, 0.7f, 0.7f),
hiliteIconColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableIconColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	items = 0;
	width = 200;
	height = 24;
	initial = -1;
	mark = "> ";
	font = GLUT_BITMAP_HELVETICA_12;
	spacing = 10;
	receiver = 0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabeledPopupMenuParams GlowLabeledPopupMenuParams::defaults(true);

GlowLabeledPopupMenuParams::GlowLabeledPopupMenuParams()
{
	*this = defaults;
}

GlowLabeledPopupMenuParams::GlowLabeledPopupMenuParams(bool) :
GlowPopupMenuParams(true),
labelColor(0.0f, 0.0f, 0.0f),
hiliteLabelColor(0.8f, 0.0f, 0.0f),
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
	Methods of Glow_MenuButtonSubwindow
===============================================================================
*/

Glow_MenuButtonSubwindow::Glow_MenuButtonSubwindow(
	GlowWidget* parent,
	const char* label,
	GlowFont font,
	GlowMenuButtonWidget::IconType iconType,
	int leftSpacing)
{
	GLOW_DEBUGSCOPE("Glow_MenuButtonSubwindow::Glow_MenuButtonSubwindow");
	
	GlowSubwindowInWidget::Init(parent, 0, 0,
		parent->Width(), parent->Height(),
		Glow::rgbBuffer | Glow::doubleBuffer, Glow::menuEvents);
	label_ = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(label_, label);
	font_ = font;
	labelWidth_ = font_.StringWidth(label_);
	down_ = false;
	iconType_ = iconType;
	leftSpacing_ = leftSpacing;
}


Glow_MenuButtonSubwindow::~Glow_MenuButtonSubwindow()
{
	delete[] label_;
}


void Glow_MenuButtonSubwindow::SetLabel_(
	const char* label)
{
	delete[] label_;
	label_ = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(label_, label);
	labelWidth_ = font_.StringWidth(label_);
	Refresh();
}


void Glow_MenuButtonSubwindow::PreferredSize(
	int& width,
	int& height,
	const char* str)
{
	height = font_.Leading() + 12;
	if (str == 0)
	{
		width = labelWidth_ + leftSpacing_ + leftSpacing_;
	}
	else
	{
		width = font_.StringWidth(str) +
			leftSpacing_ + leftSpacing_;
	}
	if (iconType_ != 0)
	{
		width += height*3/4;
	}
}


void Glow_MenuButtonSubwindow::OnEndPaint()
{
	GLOW_DEBUGSCOPE("Glow_MenuButtonSubwindow::OnEndPaint");
	
	float bevelHeight = float(4)/float(Height());
	float bevelWidth = float(4)/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	
	// Box
	if (down_)
	{
		hiliteBoxColor_.Apply();
	}
	else if (!IsActive())
	{
		disableBoxColor_.Apply();
	}
	else
	{
		boxColor_.Apply();
	}
	::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	
	// Bevels
	if (IsActive())
	{
		if (down_)
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
		::glVertex2f(-1.0f, 1.0f);
		::glVertex2f(-1.0f+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		if (down_)
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
		::glVertex2f(1.0f, -1.0f);
		::glVertex2f(1.0f-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
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
	
	// Label
	if (down_)
	{
		hiliteTextColor_.Apply();
	}
	else if (!IsActive())
	{
		disableTextColor_.Apply();
	}
	else
	{
		textColor_.Apply();
	}
	::glRasterPos2f(float(-1)+float(leftSpacing_*2)/float(Width()),
		float(font_.Leading()-font_.BaselinePos()*2)/float(Height()));
	int labellen = GLOW_CSTD::strlen(label_);
	for (int i=0; i<labellen; i++)
	{
		::glutBitmapCharacter(font_, label_[i]);
	}
	
	// Menu icon
	if (iconType_ & (GlowMenuButtonWidget::arrowIcon | GlowMenuButtonWidget::menuIcon))
	{
		if (down_)
		{
			hiliteIconColor_.Apply();
		}
		else if (!IsActive())
		{
			disableIconColor_.Apply();
		}
		else
		{
			iconColor_.Apply();
		}
		if (iconType_ == GlowMenuButtonWidget::arrowIcon)
		{
			float icontop = 0.3f;
			float iconbottom = -0.3f;
			float iconright = 1.0f-0.5f*float(Height())/float(Width());
			float iconleft = iconright - float(Height())/float(Width());
			::glBegin(GL_TRIANGLES);
			::glVertex2f(iconright, icontop);
			::glVertex2f(iconleft, icontop);
			::glVertex2f((iconleft+iconright)*0.5f, iconbottom);
			::glEnd();
		}
		else if (iconType_ == GlowMenuButtonWidget::menuIcon)
		{
			float icontop = 0.65f;
			float iconbottom = -0.5f;
			float iconmid = 0.3f;
			float iconright = 1.0f-0.5f*float(Height())/float(Width());
			float iconleft = iconright - float(Height())/float(Width());
			::glBegin(GL_QUADS);
			::glVertex2f(iconleft, iconmid);
			::glVertex2f(iconleft, icontop);
			::glVertex2f(iconright, icontop);
			::glVertex2f(iconright, iconmid);
			::glVertex2f(iconleft+bevelWidth, iconbottom-bevelHeight);
			::glVertex2f(iconleft+bevelWidth, iconmid-bevelHeight);
			::glVertex2f(iconright+bevelWidth, iconmid-bevelHeight);
			::glVertex2f(iconright+bevelWidth, iconbottom-bevelHeight);
			GlowColor::white.Apply();
			::glVertex2f(iconleft, iconbottom);
			::glVertex2f(iconleft, iconmid);
			::glVertex2f(iconright, iconmid);
			::glVertex2f(iconright, iconbottom);
			::glEnd();
			if (down_)
			{
				hiliteIconColor_.Apply();
			}
			else if (!IsActive())
			{
				disableIconColor_.Apply();
			}
			else
			{
				iconColor_.Apply();
			}
			::glBegin(GL_LINE_LOOP);
			::glVertex2f(iconleft, iconbottom);
			::glVertex2f(iconleft, iconmid);
			::glVertex2f(iconright, iconmid);
			::glVertex2f(iconright, iconbottom);
			::glEnd();
		}
	}
}


void Glow_MenuButtonSubwindow::OnMenuDown(
	int x,
	int y)
{
	down_ = true;
	Refresh();
}


void Glow_MenuButtonSubwindow::OnMenuUp()
{
	down_ = false;
	Refresh();
}


/*
===============================================================================
	Methods of Glow_PopupMenuSubwindow
===============================================================================
*/

void Glow_PopupMenuSubwindow::OnMenuDown(
	int x,
	int y)
{
	down_ = true;
	Refresh();
	widget_->Refresh();
}


void Glow_PopupMenuSubwindow::OnMenuUp()
{
	down_ = false;
	Refresh();
	widget_->Refresh();
}


void Glow_PopupMenuSubwindow::OnDirectMenuHit(
	const GlowMenuMessage& message)
{
	widget_->OnHit(message.code);
}


/*
===============================================================================
	Methods of GlowMenuButton
===============================================================================
*/

void GlowMenuButtonWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowMenuButtonParams& params)
{
	GLOW_DEBUGSCOPE("GlowMenuButtonWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	subwindow_ = new Glow_MenuButtonSubwindow(this, params.text,
		params.font, params.iconType, params.spacing);
	subwindow_->boxColor_ = params.boxColor;
	subwindow_->textColor_ = params.textColor;
	subwindow_->iconColor_ = params.iconColor;
	subwindow_->hiliteBoxColor_ = params.hiliteBoxColor;
	subwindow_->hiliteTextColor_ = params.hiliteTextColor;
	subwindow_->hiliteIconColor_ = params.hiliteIconColor;
	subwindow_->disableBoxColor_ = params.disableBoxColor;
	subwindow_->disableTextColor_ = params.disableTextColor;
	subwindow_->disableIconColor_ = params.disableIconColor;
	subwindow_->disableOutlineColor_ = params.disableOutlineColor;
	subwindow_->lightBevelColor_ = params.lightBevelColor;
	subwindow_->darkBevelColor_ = params.darkBevelColor;
	if (params.menu != 0)
	{
		subwindow_->SetMenu_(params.menu);
	}
}


GlowWidget::AutoPackError GlowMenuButtonWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowMenuButtonWidget::OnAutoPack");
	
	int pwidth = 0, pheight = 0;
	subwindow_->PreferredSize(pwidth, pheight);
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	
	int vnew = Height();
	if (vSize != unspecifiedSize && vSize < pheight)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == expandPreferredSize || vOption == preferredSize)
	{
		vnew = pheight;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowMenuButtonWidget::OnWidgetMove()
{
	subwindow_->Move(RootPositionX(), RootPositionY());
}


void GlowMenuButtonWidget::OnWidgetReshape()
{
	subwindow_->Reshape(Width(), Height());
}


void GlowMenuButtonWidget::OnWidgetVisible()
{
	subwindow_->Show();
}


void GlowMenuButtonWidget::OnWidgetInvisible()
{
	subwindow_->Hide();
}


/*
===============================================================================
	Methods of GlowPopupMenu
===============================================================================
*/

void GlowPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowPopupMenuParams& params)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	
	// Mark
	if (params.mark != 0)
	{
		mark_ = new char[GLOW_CSTD::strlen(params.mark)+1];
		GLOW_CSTD::strcpy(mark_, params.mark);
	}
	else
	{
		mark_ = 0;
	}
	
	// Menu
	menu_ = new GlowMenu;
	menu_->SetBindState(GlowMenu::bindSubwindow);
	
	// Fill in items
	if (params.items != 0)
	{
		char* tempbuf = new char[GLOW_CSTD::strlen(params.items)+1];
		GLOW_CSTD::strcpy(tempbuf, params.items);
		for (int i=0; ; ++i)
		{
			char* tok = GLOW_CSTD::strtok(i==0 ? tempbuf : 0, "\t");
			if (tok == 0) break;
			menu_->AddEntry(tok, i);
		}
		delete[] tempbuf;
	}
	
	// Find and mark initial current item
	curItem_ = params.initial;
	if (curItem_ < 0) curItem_ = 0;
	if (curItem_ >= menu_->NumItems()) curItem_ = menu_->NumItems()-1;
	if (mark_ != 0 && curItem_ != -1)
	{
		menu_->SetItemMark(curItem_, mark_);
	}
	
	// Menubutton subwindow
	subwindow_ = new Glow_PopupMenuSubwindow(this, "", params.font,
		GlowMenuButtonWidget::arrowIcon, params.spacing);
	subwindow_->boxColor_ = params.boxColor;
	subwindow_->textColor_ = params.textColor;
	subwindow_->iconColor_ = params.iconColor;
	subwindow_->hiliteBoxColor_ = params.hiliteBoxColor;
	subwindow_->hiliteTextColor_ = params.hiliteTextColor;
	subwindow_->hiliteIconColor_ = params.hiliteIconColor;
	subwindow_->disableBoxColor_ = params.disableBoxColor;
	subwindow_->disableTextColor_ = params.disableTextColor;
	subwindow_->disableIconColor_ = params.disableIconColor;
	subwindow_->disableOutlineColor_ = params.disableOutlineColor;
	subwindow_->lightBevelColor_ = params.lightBevelColor;
	subwindow_->darkBevelColor_ = params.darkBevelColor;
	subwindow_->SetMenu_(menu_);
	if (curItem_ >= 0)
	{
		subwindow_->SetLabel_(menu_->GetItemLabel(curItem_));
	}
	
	if (params.receiver != 0)
	{
		sender_.Bind(params.receiver);
	}
}


GlowPopupMenuWidget::~GlowPopupMenuWidget()
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::~GlowPopupMenuWidget");
	
	delete[] mark_;
}


void GlowPopupMenuWidget::SetMark(
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::SetMark");
	
	delete mark_;
	mark_ = new char[GLOW_CSTD::strlen(mark)+1];
	GLOW_CSTD::strcpy(mark_, mark);
	menu_->UnmarkItem(curItem_);
	if (mark_ != 0)
	{
		menu_->SetItemMark(curItem_, mark_);
	}
}


GlowWidget::AutoPackError GlowPopupMenuWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::OnAutoPack");
	
	int pwidth = 0, pheight = 0;
	subwindow_->PreferredSize(pwidth, pheight, "");
	for (int i=0; i<NumItems(); i++)
	{
		int w = 0;
		subwindow_->PreferredSize(w, pheight, GetItemLabel(i));
		if (w > pwidth) pwidth = w;
	}
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	
	int vnew = Height();
	if (vSize != unspecifiedSize && vSize < pheight)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == expandPreferredSize || vOption == preferredSize)
	{
		vnew = pheight;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


int GlowPopupMenuWidget::AddItem(
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::AddItem");
	
	menu_->AddEntry(label, menu_->NumItems());
	if (menu_->NumItems() == 1)
	{
		curItem_ = 0;
		subwindow_->SetLabel_(menu_->GetItemLabel(curItem_));
		if (mark_ != 0)
		{
			menu_->SetItemMark(curItem_, mark_);
		}
		subwindow_->Refresh();
	}
	return menu_->NumItems()-1;
}


void GlowPopupMenuWidget::RemoveItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::RemoveItem");
	
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < menu_->NumItems());
	
	int max = menu_->NumItems();
	for (int i=itemNum+1; i<max; i++)
	{
		menu_->SetItemCode(i, i-1);
	}
	if (curItem_ == itemNum)
	{
		if (max == 1)
		{
			curItem_ = -1;
			subwindow_->SetLabel_("");
		}
		else
		{
			curItem_ = 0;
			subwindow_->SetLabel_(menu_->GetItemLabel(curItem_));
		}
		subwindow_->Refresh();
	}
	else if (curItem_ > itemNum)
	{
		curItem_--;
	}
	menu_->RemoveItem(itemNum);
}


void GlowPopupMenuWidget::SetItemLabel(
	int itemNum,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::SetItemLabel");
	
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < menu_->NumItems());
	
	menu_->SetItemLabel(itemNum, label);
	if (curItem_ == itemNum)
	{
		subwindow_->SetLabel_(label);
		subwindow_->Refresh();
	}
}


void GlowPopupMenuWidget::SetCurItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::SetCurItem");
	
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < menu_->NumItems());
	
	if (itemNum != curItem_)
	{
		menu_->UnmarkItem(curItem_);
		curItem_ = itemNum;
		subwindow_->SetLabel_(menu_->GetItemLabel(curItem_));
		if (mark_ != 0)
		{
			menu_->SetItemMark(curItem_, mark_);
		}
	}
}


void GlowPopupMenuWidget::OnWidgetMove()
{
	subwindow_->Move(RootPositionX(), RootPositionY());
}


void GlowPopupMenuWidget::OnWidgetReshape()
{
	subwindow_->Reshape(Width(), Height());
}


void GlowPopupMenuWidget::OnWidgetVisible()
{
	subwindow_->Show();
}


void GlowPopupMenuWidget::OnWidgetInvisible()
{
	subwindow_->Hide();
}


void GlowPopupMenuWidget::OnHit(
	int item)
{
	GlowPopupMenuMessage outmsg;
	outmsg.oldItem = curItem_;
	if (curItem_ != item)
	{
		menu_->UnmarkItem(curItem_);
		curItem_ = item;
		subwindow_->SetLabel_(menu_->GetItemLabel(curItem_));
		if (mark_ != 0)
		{
			menu_->SetItemMark(curItem_, mark_);
		}
	}
	outmsg.item = item;
	outmsg.widget = this;
	sender_.Send(outmsg);
}


/*
===============================================================================
	Methods of GlowLabeledPopupMenuWidget
===============================================================================
*/

void GlowLabeledPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledPopupMenuParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabeledPopupMenuWidget::Init");
	
	GlowPopupMenuWidget::Init(root, parent, params);
	InitLabel(this, params.labelPosition, params.labelWidth,
		params.labelHeight, params.labelSpacing, params.labelText,
		params.labelFont, params.labelColor, params.disableLabelColor);
	upLabelColor_ = params.labelColor;
	downLabelColor_ = params.hiliteLabelColor;
}


void GlowLabeledPopupMenuWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowLabeledPopupMenuWidget::OnWidgetPaint");
	
	Label()->SetRefreshEnabled(false);
	if (IsDown())
	{
		Label()->SetTextColor(downLabelColor_);
	}
	else
	{
		Label()->SetTextColor(upLabelColor_);
	}
	Label()->SetRefreshEnabled(true);
}


GlowWidget::AutoPackError GlowLabeledPopupMenuWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledPopupMenuWidget::OnAutoPack");
	
	// Use helper
	AutoPackError result = HelpAutoPack(hSize, vSize, leftMargin, rightMargin,
		topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	result = GlowPopupMenuWidget::OnAutoPack(hSize, vSize, hOption,
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

