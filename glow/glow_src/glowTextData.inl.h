/*
===============================================================================

	FILE:  glowTextData.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Text data for GLOW
	
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

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowTextData
===============================================================================
*/

inline GlowTextData::GlowTextData()
{
	lineBreaks_.push_back(0);
	selStart_ = selEnd_ = 0;
}


// MSVC doesn't like the base class initializers here. I have no idea why.
// Luckily there's a workaround, but probably with worse performance.

#if 0

inline GlowTextData::GlowTextData(
	const GLOW_STD::string& str) :
GLOW_STD::string(str)
{
	lineBreaks_.push_back(0);
	selStart_ = selEnd_ = 0;
}


inline GlowTextData::GlowTextData(
	const char* str) :
GLOW_STD::string(str)
{
	lineBreaks_.push_back(0);
	selStart_ = selEnd_ = 0;
}

#else

inline GlowTextData::GlowTextData(
	const GLOW_STD::string& str)
{
	assign(str);
	lineBreaks_.push_back(0);
	selStart_ = selEnd_ = 0;
}


inline GlowTextData::GlowTextData(
	const char* str)
{
	assign(str);
	lineBreaks_.push_back(0);
	selStart_ = selEnd_ = 0;
}

#endif


inline void GlowTextData::ClearLineBreaks()
{
	lineBreaks_.erase(lineBreaks_.begin(), lineBreaks_.end());
	lineBreaks_.push_back(0);
}


inline int GlowTextData::NumLines() const
{
	return lineBreaks_.size();
}


inline GLOW_STD::string GlowTextData::Line(
	int num) const
{
	GLOW_ASSERT(num > 0);
	GLOW_ASSERT(num < int(lineBreaks_.size()));
	return substr(lineBreaks_[num],
		(num == int(lineBreaks_.size())-1) ? GLOW_STD::string::npos :
		lineBreaks_[num+1]-lineBreaks_[num]);
}


inline GLOW_STD::string GlowTextData::ToEndOfLine(
	int pos) const
{
	int num = LineNumOf(pos);
	return substr(lineBreaks_[num],
		(num == int(lineBreaks_.size())-1) ? GLOW_STD::string::npos :
		lineBreaks_[num+1]-lineBreaks_[num]);
}


inline int GlowTextData::SelectionStart() const
{
	return selStart_;
}


inline int GlowTextData::SelectionEnd() const
{
	return selEnd_;
}


inline int GlowTextData::SelectionLength() const
{
	return selEnd_ - selStart_;
}


inline void GlowTextData::SetSelection(
	int start,
	int end)
{
	if (start > end)
	{
		GLOW_ASSERT(end >= 0);
		GLOW_ASSERT(start <= int(size()));
		selStart_ = end;
		selEnd_ = start;
	}
	else
	{
		GLOW_ASSERT(start >= 0);
		GLOW_ASSERT(end <= int(size()));
		selStart_ = start;
		selEnd_ = end;
	}
}


inline void GlowTextData::SetSelection(
	int pos)
{
	GLOW_ASSERT(pos >= 0);
	GLOW_ASSERT(pos <= int(size()));
	selStart_ = pos;
	selEnd_ = pos;
}


inline GLOW_STD::string GlowTextData::SelectedText() const
{
	return substr(selStart_, selEnd_-selStart_);
}


inline void GlowTextData::ReplaceSelectionWith(
	const char* str)
{
	int len = GLOW_CSTD::strlen(str);
	replace(selStart_, selEnd_-selStart_, str, len);
	selEnd_ = selStart_+len;
}


inline void GlowTextData::ReplaceSelectionWith(
	char ch)
{
	replace(selStart_, selEnd_-selStart_, 1, ch);
	selEnd_ = selStart_+1;
}


inline void GlowTextData::DeleteSelection()
{
	erase(selStart_, selEnd_-selStart_);
	selEnd_ = selStart_;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

