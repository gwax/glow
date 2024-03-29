/*
===============================================================================

	FILE:  glowScrollBarWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Scroll bar widget for GLOW
	
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


#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowScrollBarWidget
===============================================================================
*/

inline GlowScrollBarWidget::GlowScrollBarWidget()
{
}


inline GlowScrollBarWidget::GlowScrollBarWidget(
	GlowWidget* parent,
	const GlowScrollBarParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowScrollBarWidget::GlowScrollBarWidget(
	GlowWidgetRoot* root,
	const GlowScrollBarParams& params)
{
	Init(root, 0, params);
}


inline void GlowScrollBarWidget::Init(
	GlowWidget* parent,
	const GlowScrollBarParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowScrollBarWidget::Init(
	GlowWidgetRoot* root,
	const GlowScrollBarParams& params)
{
	Init(root, 0, params);
}


inline GlowColor GlowScrollBarWidget::GetStripColor() const
{
	return stripColor_;
}


inline GlowColor GlowScrollBarWidget::GetIndicatorColor() const
{
	return indicatorColor_;
}


inline GlowColor GlowScrollBarWidget::GetShadowColor() const
{
	return shadowColor_;
}


inline GlowColor GlowScrollBarWidget::GetButtonColor() const
{
	return buttonColor_;
}


inline GlowColor GlowScrollBarWidget::GetButtonIconColor() const
{
	return buttonIconColor_;
}


inline GlowColor GlowScrollBarWidget::GetHiliteStripColor() const
{
	return hiliteStripColor_;
}


inline GlowColor GlowScrollBarWidget::GetHiliteIndicatorColor() const
{
	return hiliteIndicatorColor_;
}


inline GlowColor GlowScrollBarWidget::GetHiliteButtonColor() const
{
	return hiliteButtonColor_;
}


inline GlowColor GlowScrollBarWidget::GetHiliteButtonIconColor() const
{
	return hiliteButtonIconColor_;
}


inline GlowColor GlowScrollBarWidget::GetDisableStripColor() const
{
	return disableStripColor_;
}


inline GlowColor GlowScrollBarWidget::GetDisableIndicatorColor() const
{
	return disableIndicatorColor_;
}


inline GlowColor GlowScrollBarWidget::GetDisableButtonColor() const
{
	return disableButtonColor_;
}


inline GlowColor GlowScrollBarWidget::GetDisableButtonIconColor() const
{
	return disableButtonIconColor_;
}


inline GlowColor GlowScrollBarWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowScrollBarWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowScrollBarWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowScrollBarWidget::SetStripColor(
	GlowColor c)
{
	stripColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetIndicatorColor(
	GlowColor c)
{
	indicatorColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetShadowColor(
	GlowColor c)
{
	shadowColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetButtonColor(
	GlowColor c)
{
	buttonColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetButtonIconColor(
	GlowColor c)
{
	buttonIconColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteStripColor(
	GlowColor c)
{
	hiliteStripColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteIndicatorColor(
	GlowColor c)
{
	hiliteIndicatorColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteButtonColor(
	GlowColor c)
{
	hiliteButtonColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteButtonIconColor(
	GlowColor c)
{
	hiliteButtonIconColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableStripColor(
	GlowColor c)
{
	disableStripColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableIndicatorColor(
	GlowColor c)
{
	disableIndicatorColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableButtonColor(
	GlowColor c)
{
	disableButtonColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableButtonIconColor(
	GlowColor c)
{
	disableButtonIconColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


inline long GlowScrollBarWidget::GetTopValue() const
{
	return topValue_;
}


inline long GlowScrollBarWidget::GetBottomValue() const
{
	return topValue_+span_;
}


inline long GlowScrollBarWidget::GetMaximum() const
{
	return max_;
}


inline long GlowScrollBarWidget::GetMinimum() const
{
	return min_;
}


inline long GlowScrollBarWidget::GetSpan() const
{
	return span_;
}


inline void GlowScrollBarWidget::SetTopValue(
	long val)
{
	topValue_ = val;
	if (topValue_ < min_) topValue_ = min_;
	if (topValue_+span_ > max_) topValue_ = max_-span_;
	Refresh();
}


inline void GlowScrollBarWidget::MoveTopValue(
	long delta)
{
	topValue_ += delta;
	if (topValue_ < min_) topValue_ = min_;
	if (topValue_+span_ > max_) topValue_ = max_-span_;
	Refresh();
}


inline void GlowScrollBarWidget::SetMaximum(
	long val)
{
	GLOW_DEBUG(val < min_+span_, "max < min in GlowScrollBarWidget::SetMaximum");
	max_ = val;
	if (topValue_+span_ > max_) topValue_ = max_-span_;
	Refresh();
}


inline void GlowScrollBarWidget::SetMinimum(
	long val)
{
	GLOW_DEBUG(max_-span_ < val, "min > max in GlowScrollBarWidget::SetMinimum");
	min_ = val;
	if (topValue_ < min_) topValue_ = min_;
	Refresh();
}


inline void GlowScrollBarWidget::SetSpan(
	long val)
{
	GLOW_DEBUG(val < 1, "span<1 in GlowScrollBarWidget::SetSpan");
	GLOW_DEBUG(val > max_-min_, "span > max-min in GlowScrollBarWidget::SetSpan");
	span_ = val;
	if (topValue_+span_ > max_) topValue_ = max_-span_;
	Refresh();
}


inline int GlowScrollBarWidget::GetFirstDelay() const
{
	return firstDelay_;
}


inline void GlowScrollBarWidget::SetFirstDelay(
	int msecs)
{
	firstDelay_ = msecs;
}


inline int GlowScrollBarWidget::GetSecondDelay() const
{
	return secondDelay_;
}


inline void GlowScrollBarWidget::SetSecondDelay(
	int msecs)
{
	secondDelay_ = msecs;
}


inline long GlowScrollBarWidget::GetPageStep() const
{
	return pageStep_;
}


inline void GlowScrollBarWidget::SetPageStep(
	long pageStep)
{
	pageStep_ = pageStep;
}


inline long GlowScrollBarWidget::GetArrowStep() const
{
	return arrowStep_;
}


inline void GlowScrollBarWidget::SetArrowStep(
	long arrowStep)
{
	arrowStep_ = arrowStep;
}


inline TSender<const GlowScrollBarMessage&>& GlowScrollBarWidget::Notifier()
{
	return sender_;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

