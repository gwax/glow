/*
===============================================================================

	FILE:  glowSliderWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Slider widget for GLOW
	
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


#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowSliderWidget
===============================================================================
*/

inline GlowSliderWidget::GlowSliderWidget()
{
}


inline GlowSliderWidget::GlowSliderWidget(
	GlowWidget* parent,
	const GlowSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowSliderWidget::GlowSliderWidget(
	GlowWidgetRoot* root,
	const GlowSliderParams& params)
{
	Init(root, 0, params);
}


inline void GlowSliderWidget::Init(
	GlowWidget* parent,
	const GlowSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowSliderWidget::Init(
	GlowWidgetRoot* root,
	const GlowSliderParams& params)
{
	Init(root, 0, params);
}


inline GlowColor GlowSliderWidget::GetStripColor() const
{
	return stripColor_;
}


inline GlowColor GlowSliderWidget::GetIndicatorColor() const
{
	return indicatorColor_;
}


inline GlowColor GlowSliderWidget::GetTickMarkColor() const
{
	return tickMarkColor_;
}


inline GlowColor GlowSliderWidget::GetHiliteIndicatorColor() const
{
	return hiliteIndicatorColor_;
}


inline GlowColor GlowSliderWidget::GetDisableStripColor() const
{
	return disableStripColor_;
}


inline GlowColor GlowSliderWidget::GetDisableIndicatorColor() const
{
	return disableIndicatorColor_;
}


inline GlowColor GlowSliderWidget::GetDisableTickMarkColor() const
{
	return disableTickMarkColor_;
}


inline GlowColor GlowSliderWidget::GetDisableOutlineColor() const
{
	return disableOutlineColor_;
}


inline GlowColor GlowSliderWidget::GetLightBevelColor() const
{
	return lightBevelColor_;
}


inline GlowColor GlowSliderWidget::GetDarkBevelColor() const
{
	return darkBevelColor_;
}


inline void GlowSliderWidget::SetStripColor(
	GlowColor c)
{
	stripColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetIndicatorColor(
	GlowColor c)
{
	indicatorColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetTickMarkColor(
	GlowColor c)
{
	tickMarkColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetHiliteIndicatorColor(
	GlowColor c)
{
	hiliteIndicatorColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableStripColor(
	GlowColor c)
{
	disableStripColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableIndicatorColor(
	GlowColor c)
{
	disableIndicatorColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableTickMarkColor(
	GlowColor c)
{
	disableTickMarkColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableOutlineColor(
	GlowColor c)
{
	disableOutlineColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetLightBevelColor(
	GlowColor c)
{
	lightBevelColor_ = c;
	Refresh();
}


inline void GlowSliderWidget::SetDarkBevelColor(
	GlowColor c)
{
	darkBevelColor_ = c;
	Refresh();
}


inline double GlowSliderWidget::GetValue() const
{
	return value_;
}


inline double GlowSliderWidget::GetMaximum() const
{
	return max_;
}


inline double GlowSliderWidget::GetMinimum() const
{
	return min_;
}


inline void GlowSliderWidget::SetValue(
	double val)
{
	value_ = val;
	if (value_ > max_) value_ = max_;
	if (value_ < min_) value_ = min_;
	Refresh();
}


inline void GlowSliderWidget::SetMaximum(
	double val)
{
	GLOW_DEBUG(val < min_, "max < min in GlowSliderWidget::SetMaximum");
	max_ = val;
	if (value_ > max_) value_ = max_;
	Refresh();
}


inline void GlowSliderWidget::SetMinimum(
	double val)
{
	GLOW_DEBUG(max_ < val, "min > max in GlowSliderWidget::SetMinimum");
	min_ = val;
	if (value_ < min_) value_ = min_;
	Refresh();
}


inline int GlowSliderWidget::GetNumTickMarks() const
{
	return numTicks_;
}


inline void GlowSliderWidget::SetNumTickMarks(
	int numTicks)
{
	numTicks_ = numTicks;
	Refresh();
}


inline GlowSliderWidget::Options GlowSliderWidget::GetOptions() const
{
	return type_;
}


inline void GlowSliderWidget::SetOptions(
	Options options)
{
	type_ = options;
	Refresh();
}


inline TSender<const GlowSliderMessage&>& GlowSliderWidget::Notifier()
{
	return sender_;
}


inline void GlowSliderWidget::LeftBevelColor_()
{
	if ((type_ & decreasing) != 0)
	{
		darkBevelColor_.Apply();
	}
	else
	{
		lightBevelColor_.Apply();
	}
}


inline void GlowSliderWidget::RightBevelColor_()
{
	if ((type_ & decreasing) != 0)
	{
		lightBevelColor_.Apply();
	}
	else
	{
		darkBevelColor_.Apply();
	}
}


inline void GlowSliderWidget::TopBevelColor_()
{
	if ((type_ & ticksOnTop) != 0)
	{
		darkBevelColor_.Apply();
	}
	else
	{
		lightBevelColor_.Apply();
	}
}


inline void GlowSliderWidget::BottomBevelColor_()
{
	if ((type_ & ticksOnTop) != 0)
	{
		lightBevelColor_.Apply();
	}
	else
	{
		darkBevelColor_.Apply();
	}
}


/*
===============================================================================
	Inline methods of GlowLabeledSliderWidget
===============================================================================
*/

inline GlowLabeledSliderWidget::GlowLabeledSliderWidget()
{
}


inline GlowLabeledSliderWidget::GlowLabeledSliderWidget(
	GlowWidget* parent,
	const GlowLabeledSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabeledSliderWidget::GlowLabeledSliderWidget(
	GlowWidgetRoot* root,
	const GlowLabeledSliderParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabeledSliderWidget::Init(
	GlowWidget* parent,
	const GlowLabeledSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabeledSliderWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledSliderParams& params)
{
	Init(root, 0, params);
}


inline bool GlowLabeledSliderWidget::HasLabel() const
{
	return label_->GetText() != 0;
}


inline GlowColor GlowLabeledSliderWidget::GetLabelTextColor() const
{
	return label_->GetTextColor();
}


inline void GlowLabeledSliderWidget::SetLabelTextColor(
	GlowColor c)
{
	label_->SetTextColor(c);
}


inline GlowColor GlowLabeledSliderWidget::GetLabelDisableTextColor() const
{
	return label_->GetDisableTextColor();
}


inline void GlowLabeledSliderWidget::SetLabelDisableTextColor(
	GlowColor c)
{
	label_->SetDisableTextColor(c);
}


inline const char* GlowLabeledSliderWidget::GetLabelTemplate() const
{
	return labelTemplate_;
}


inline GlowFont GlowLabeledSliderWidget::GetLabelFont() const
{
	return label_->GetFont();
}


inline void GlowLabeledSliderWidget::SetLabelFont(
	GlowFont font)
{
	label_->SetFont(font);
	RepositionLabel();
}


inline int GlowLabeledSliderWidget::GetLabelSpacing() const
{
	return labelSpacing_;
}


inline void GlowLabeledSliderWidget::SetLabelSpacing(
	int spacing)
{
	if (spacing != labelSpacing_)
	{
		labelSpacing_ = spacing;
		RepositionLabel();
	}
}


inline GlowLabeledSliderWidget::LabelPosition GlowLabeledSliderWidget::GetLabelPosition() const
{
	return labelPosition_;
}


inline void GlowLabeledSliderWidget::SetLabelPosition(
	LabelPosition position)
{
	if (position != labelPosition_)
	{
		labelPosition_ = position;
		RepositionLabel();
	}
}


inline int GlowLabeledSliderWidget::GetLabelWidth() const
{
	return labelWidth_;
}


inline void GlowLabeledSliderWidget::SetLabelWidth(
	int width)
{
	if (width != labelWidth_)
	{
		labelWidth_ = width;
		RepositionLabel();
	}
}


inline int GlowLabeledSliderWidget::GetLabelHeight() const
{
	return labelHeight_;
}


inline void GlowLabeledSliderWidget::SetLabelHeight(
	int height)
{
	if (height != labelHeight_)
	{
		labelHeight_ = height;
		RepositionLabel();
	}
}


inline bool GlowLabeledSliderWidget::HasMinmax() const
{
	return minmaxTemplate_ != 0;
}


inline const char* GlowLabeledSliderWidget::GetMinmaxTemplate() const
{
	return minmaxTemplate_;
}


inline GlowFont GlowLabeledSliderWidget::GetMinmaxFont() const
{
	return minLabel_->GetFont();
}


inline void GlowLabeledSliderWidget::SetMinmaxFont(
	GlowFont font)
{
	minLabel_->SetFont(font);
	maxLabel_->SetFont(font);
	if (minmaxTemplate_ != 0)
	{
		RepositionMinmax();
	}
}


inline GlowColor GlowLabeledSliderWidget::GetMinmaxTextColor() const
{
	return minLabel_->GetTextColor();
}


inline void GlowLabeledSliderWidget::SetMinmaxTextColor(
	GlowColor c)
{
	minLabel_->SetTextColor(c);
	maxLabel_->SetTextColor(c);
}


inline GlowColor GlowLabeledSliderWidget::GetMinmaxDisableTextColor() const
{
	return minLabel_->GetDisableTextColor();
}


inline void GlowLabeledSliderWidget::SetMinmaxDisableTextColor(
	GlowColor c)
{
	minLabel_->SetDisableTextColor(c);
	maxLabel_->SetDisableTextColor(c);
}


inline int GlowLabeledSliderWidget::GetMinmaxSize() const
{
	return minmaxSize_;
}


inline void GlowLabeledSliderWidget::SetMinmaxSize(
	int size)
{
	minmaxSize_ = size;
	if (minmaxTemplate_ != 0)
	{
		RepositionMinmax();
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

