/*
===============================================================================

	FILE:  glowMenuButtonWidget.inl.h
	
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

#include <cstring>

GLOW_INTERNAL_USINGSTD

#include "glowDebug.h"

GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS Glow_MenuButtonSubwindow
	
	Menu button subwindow class (private)
===============================================================================
*/

class Glow_MenuButtonSubwindow :
	public GlowSubwindowInWidget
{
	//-------------------------------------------------------------------------
	//	Private interface
	//-------------------------------------------------------------------------
	
	friend class GlowMenuButtonWidget;
	friend class GlowPopupMenuWidget;
	
	
	protected:
	
		Glow_MenuButtonSubwindow(
			GlowWidget* parent,
			const char* label,
			GlowFont font,
			GlowMenuButtonWidget::IconType iconType,
			int leftSpacing);
		
		virtual ~Glow_MenuButtonSubwindow();
	
	protected:
	
		inline const char* GetLabel_() const;
		void SetLabel_(
			const char* label);
		inline GlowMenu* GetMenu_() const;
		inline void SetMenu_(
			GlowMenu* menu);
		
		void PreferredSize(
			int& width,
			int& height,
			const char* str = 0);
	
	protected:
	
		virtual void OnMenuDown(
			int x,
			int y);
		virtual void OnMenuUp();
		
		virtual void OnEndPaint();
	
	private:
	
		char* label_;
		GlowFont font_;
		GlowMenuButtonWidget::IconType iconType_;
		int labelWidth_;
		int leftSpacing_;
	
	protected:
	
		bool down_;
		
		GlowColor boxColor_;
		GlowColor textColor_;
		GlowColor iconColor_;
		GlowColor hiliteBoxColor_;
		GlowColor hiliteTextColor_;
		GlowColor hiliteIconColor_;
		GlowColor disableBoxColor_;
		GlowColor disableTextColor_;
		GlowColor disableIconColor_;
		GlowColor disableOutlineColor_;
		GlowColor lightBevelColor_;
		GlowColor darkBevelColor_;
};


/*
===============================================================================
	CLASS Glow_PopupMenuSubwindow
	
	Menu button subwindow class (private)
===============================================================================
*/

class Glow_PopupMenuSubwindow :
	public Glow_MenuButtonSubwindow
{
	friend class GlowPopupMenuWidget;
	
	protected:
	
		inline Glow_PopupMenuSubwindow(
			GlowPopupMenuWidget* parent,
			const char* label,
			GlowFont font,
			GlowMenuButtonWidget::IconType iconType,
			int leftSpacing);
	
	protected:
	
		virtual void OnMenuDown(
			int x,
			int y);
		virtual void OnMenuUp();
		virtual void OnDirectMenuHit(
			const GlowMenuMessage& message);
	
	private:
	
		GlowPopupMenuWidget* widget_;
};


/*
===============================================================================
	Inline methods of Glow_MenuButtonSubwindow
===============================================================================
*/

inline const char* Glow_MenuButtonSubwindow::GetLabel_() const
{
	return label_;
}


inline void Glow_MenuButtonSubwindow::SetMenu_(
	GlowMenu* menu)
{
	SetMenu(Glow::leftButton, menu);
	SetMenu(Glow::middleButton, menu);
	SetMenu(Glow::rightButton, menu);
}


inline GlowMenu* Glow_MenuButtonSubwindow::GetMenu_() const
{
	return GetMenu(Glow::rightButton);
}


/*
===============================================================================
	Inline methods of Glow_PopupMenuSubwindow
===============================================================================
*/

inline Glow_PopupMenuSubwindow::Glow_PopupMenuSubwindow(
	GlowPopupMenuWidget* parent,
	const char* label,
	GlowFont font,
	GlowMenuButtonWidget::IconType iconType,
	int leftSpacing) :
Glow_MenuButtonSubwindow(parent, label, font, iconType, leftSpacing)
{
	widget_ = parent;
}


/*
===============================================================================
	Inline methods of GlowMenuButtonWidget
===============================================================================
*/

inline GlowMenuButtonWidget::GlowMenuButtonWidget()
{
}


inline GlowMenuButtonWidget::GlowMenuButtonWidget(
	GlowWidget* parent,
	const GlowMenuButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowMenuButtonWidget::GlowMenuButtonWidget(
	GlowWidgetRoot* root,
	const GlowMenuButtonParams& params)
{
	Init(root, 0, params);
}


inline void GlowMenuButtonWidget::Init(
	GlowWidget* parent,
	const GlowMenuButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowMenuButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowMenuButtonParams& params)
{
	Init(root, 0, params);
}


inline bool GlowMenuButtonWidget::IsDown() const
{
	return subwindow_->down_;
}


inline void GlowMenuButtonWidget::SetText(
	const char* text)
{
	subwindow_->SetLabel_(text);
}


inline const char* GlowMenuButtonWidget::GetText() const
{
	return subwindow_->GetLabel_();
}


inline GlowFont GlowMenuButtonWidget::GetFont() const
{
	return subwindow_->font_;
}


inline void GlowMenuButtonWidget::SetFont(
	GlowFont font)
{
	subwindow_->font_ = font;
	subwindow_->Refresh();
}


inline int GlowMenuButtonWidget::GetSpacing() const
{
	return subwindow_->leftSpacing_;
}


inline void GlowMenuButtonWidget::SetSpacing(
	int spacing)
{
	subwindow_->leftSpacing_ = spacing;
	subwindow_->Refresh();
}


inline GlowMenuButtonWidget::IconType GlowMenuButtonWidget::GetIconType() const
{
	return subwindow_->iconType_;
}


inline void GlowMenuButtonWidget::SetIconType(
	IconType iconType)
{
	subwindow_->iconType_ = iconType;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetMenu(
	GlowMenu* menu)
{
	subwindow_->SetMenu_(menu);
}


inline GlowMenu* GlowMenuButtonWidget::GetMenu() const
{
	return subwindow_->GetMenu_();
}


inline GlowColor GlowMenuButtonWidget::GetBoxColor() const
{
	return subwindow_->boxColor_;
}


inline GlowColor GlowMenuButtonWidget::GetIconColor() const
{
	return subwindow_->iconColor_;
}


inline GlowColor GlowMenuButtonWidget::GetTextColor() const
{
	return subwindow_->textColor_;
}


inline GlowColor GlowMenuButtonWidget::GetHiliteBoxColor() const
{
	return subwindow_->hiliteBoxColor_;
}


inline GlowColor GlowMenuButtonWidget::GetHiliteIconColor() const
{
	return subwindow_->hiliteIconColor_;
}


inline GlowColor GlowMenuButtonWidget::GetHiliteTextColor() const
{
	return subwindow_->hiliteTextColor_;
}


inline GlowColor GlowMenuButtonWidget::GetDisableBoxColor() const
{
	return subwindow_->disableBoxColor_;
}


inline GlowColor GlowMenuButtonWidget::GetDisableIconColor() const
{
	return subwindow_->disableIconColor_;
}


inline GlowColor GlowMenuButtonWidget::GetDisableTextColor() const
{
	return subwindow_->disableTextColor_;
}


inline GlowColor GlowMenuButtonWidget::GetDisableOutlineColor() const
{
	return subwindow_->disableOutlineColor_;
}


inline GlowColor GlowMenuButtonWidget::GetLightBevelColor() const
{
	return subwindow_->lightBevelColor_;
}


inline GlowColor GlowMenuButtonWidget::GetDarkBevelColor() const
{
	return subwindow_->darkBevelColor_;
}


inline void GlowMenuButtonWidget::SetBoxColor(
	GlowColor c)
{
	subwindow_->boxColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetTextColor(
	GlowColor c)
{
	subwindow_->textColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetIconColor(
	GlowColor c)
{
	subwindow_->iconColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	subwindow_->hiliteBoxColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	subwindow_->hiliteTextColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetHiliteIconColor(
	GlowColor c)
{
	subwindow_->hiliteIconColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableBoxColor(
	GlowColor c)
{
	subwindow_->disableBoxColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	subwindow_->disableTextColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableIconColor(
	GlowColor c)
{
	subwindow_->disableIconColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	subwindow_->disableOutlineColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	subwindow_->lightBevelColor_ = c;
	subwindow_->Refresh();
}


inline void GlowMenuButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	subwindow_->darkBevelColor_ = c;
	subwindow_->Refresh();
}


/*
===============================================================================
	Inline methods of GlowPopupMenuWidget
===============================================================================
*/

inline GlowPopupMenuWidget::GlowPopupMenuWidget()
{
}


inline GlowPopupMenuWidget::GlowPopupMenuWidget(
	GlowWidget* parent,
	const GlowPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowPopupMenuWidget::GlowPopupMenuWidget(
	GlowWidgetRoot* root,
	const GlowPopupMenuParams& params)
{
	Init(root, 0, params);
}


inline void GlowPopupMenuWidget::Init(
	GlowWidget* parent,
	const GlowPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	const GlowPopupMenuParams& params)
{
	Init(root, 0, params);
}


inline bool GlowPopupMenuWidget::IsDown() const
{
	return subwindow_->down_;
}


inline const char* GlowPopupMenuWidget::GetMark() const
{
	return mark_;
}


inline GlowFont GlowPopupMenuWidget::GetFont() const
{
	return subwindow_->font_;
}


inline void GlowPopupMenuWidget::SetFont(
	GlowFont font)
{
	subwindow_->font_ = font;
	subwindow_->Refresh();
}


inline int GlowPopupMenuWidget::GetSpacing() const
{
	return subwindow_->leftSpacing_;
}


inline void GlowPopupMenuWidget::SetSpacing(
	int spacing)
{
	subwindow_->leftSpacing_ = spacing;
	subwindow_->Refresh();
}


inline int GlowPopupMenuWidget::NumItems() const
{
	return menu_->NumItems();
}


inline int GlowPopupMenuWidget::GetCurItem() const
{
	return curItem_;
}


inline const char* GlowPopupMenuWidget::GetItemLabel(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < menu_->NumItems());
	
	return menu_->GetItemLabel(itemNum);
}


inline GlowColor GlowPopupMenuWidget::GetBoxColor() const
{
	return subwindow_->boxColor_;
}


inline GlowColor GlowPopupMenuWidget::GetIconColor() const
{
	return subwindow_->iconColor_;
}


inline GlowColor GlowPopupMenuWidget::GetTextColor() const
{
	return subwindow_->textColor_;
}


inline GlowColor GlowPopupMenuWidget::GetHiliteBoxColor() const
{
	return subwindow_->hiliteBoxColor_;
}


inline GlowColor GlowPopupMenuWidget::GetHiliteIconColor() const
{
	return subwindow_->hiliteIconColor_;
}


inline GlowColor GlowPopupMenuWidget::GetHiliteTextColor() const
{
	return subwindow_->hiliteTextColor_;
}


inline GlowColor GlowPopupMenuWidget::GetDisableBoxColor() const
{
	return subwindow_->disableBoxColor_;
}


inline GlowColor GlowPopupMenuWidget::GetDisableIconColor() const
{
	return subwindow_->disableIconColor_;
}


inline GlowColor GlowPopupMenuWidget::GetDisableTextColor() const
{
	return subwindow_->disableTextColor_;
}


inline GlowColor GlowPopupMenuWidget::GetDisableOutlineColor() const
{
	return subwindow_->disableOutlineColor_;
}


inline GlowColor GlowPopupMenuWidget::GetLightBevelColor() const
{
	return subwindow_->lightBevelColor_;
}


inline GlowColor GlowPopupMenuWidget::GetDarkBevelColor() const
{
	return subwindow_->darkBevelColor_;
}


inline void GlowPopupMenuWidget::SetBoxColor(
	GlowColor c)
{
	subwindow_->boxColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetIconColor(
	GlowColor c)
{
	subwindow_->iconColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetTextColor(
	GlowColor c)
{
	subwindow_->textColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetHiliteBoxColor(
	GlowColor c)
{
	subwindow_->hiliteBoxColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetHiliteIconColor(
	GlowColor c)
{
	subwindow_->hiliteIconColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetHiliteTextColor(
	GlowColor c)
{
	subwindow_->hiliteTextColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableBoxColor(
	GlowColor c)
{
	subwindow_->disableBoxColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableIconColor(
	GlowColor c)
{
	subwindow_->disableIconColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableTextColor(
	GlowColor c)
{
	subwindow_->disableTextColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableOutlineColor(
	GlowColor c)
{
	subwindow_->disableOutlineColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetLightBevelColor(
	GlowColor c)
{
	subwindow_->lightBevelColor_ = c;
	subwindow_->Refresh();
}


inline void GlowPopupMenuWidget::SetDarkBevelColor(
	GlowColor c)
{
	subwindow_->darkBevelColor_ = c;
	subwindow_->Refresh();
}


inline TSender<const GlowPopupMenuMessage&>& GlowPopupMenuWidget::Notifier()
{
	return sender_;
}


/*
===============================================================================
	Inline methods of GlowLabeledPopupMenuWidget
===============================================================================
*/

inline GlowLabeledPopupMenuWidget::GlowLabeledPopupMenuWidget()
{
}


inline GlowLabeledPopupMenuWidget::GlowLabeledPopupMenuWidget(
	GlowWidget* parent,
	const GlowLabeledPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabeledPopupMenuWidget::GlowLabeledPopupMenuWidget(
	GlowWidgetRoot* root,
	const GlowLabeledPopupMenuParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabeledPopupMenuWidget::Init(
	GlowWidget* parent,
	const GlowLabeledPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabeledPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledPopupMenuParams& params)
{
	Init(root, 0, params);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

