/*
===============================================================================

	FILE:  glowTextData.cpp
	
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

#include <algorithm>
#include <GL/glut.h>

#ifndef GLOW_TEXTDATA__H
	#include "glowTextData.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of GlowTextData
===============================================================================
*/

int GlowTextData::LineNumOf(
	int pos) const
{
	GLOW_ASSERT(pos >= 0);
	GLOW_ASSERT(pos < int(size()));
	
	int b = 0;
	int e = lineBreaks_.size();
	while (e-b > 1)
	{
		int c = (b+e)/2;
		if (lineBreaks_[c] > pos)
		{
			e = c;
		}
		else if (lineBreaks_[c] < pos)
		{
			b = c;
		}
		else
		{
			return c;
		}
	}
	return b;
}


void GlowTextData::RecalcLineBreaks(
	GlowFont font,
	int pixelWidth,
	int startingLine)
{
	GLOW_ASSERT(startingLine >= 0);
	GLOW_ASSERT(startingLine < int(lineBreaks_.size()));
	
	lineBreaks_.erase(lineBreaks_.begin()+startingLine+1, lineBreaks_.end());
	
	int curWidth = 0;
	int lastBreakable = -1;
	int pos = lineBreaks_.back();
	const char* dat = data();
	
	while (pos < int(size()))
	{
		char ch = dat[pos];
		
		// Check for EOL character
		if (ch == '\r')
		{
			if (pos+1 < int(size()) && dat[pos+1] == '\n')
			{
				lineBreaks_.push_back(pos+2);
				pos += 2;
			}
			else
			{
				lineBreaks_.push_back(pos+1);
				pos += 1;
			}
			lastBreakable = -1;
			curWidth = 0;
			continue;
		}
		if (ch == '\n')
		{
			lineBreaks_.push_back(pos+1);
			pos += 1;
			lastBreakable = -1;
			curWidth = 0;
			continue;
		}
		
		// Check for whitespace
		if (ch == ' ' || ch == '\t')
		{
			lastBreakable = pos;
		}
		
		// Check width
		curWidth += ::glutBitmapWidth(font, ch);
		if (curWidth <= pixelWidth || pixelWidth < 0)
		{
			pos += 1;
			continue;
		}
		
		// Break line due to width
		if (lastBreakable == -1 || lastBreakable == pos)
		{
			lineBreaks_.push_back(pos);
			lastBreakable = -1;
			curWidth = 0;
		}
		else
		{
			lineBreaks_.push_back(lastBreakable+1);
			pos = lastBreakable+1;
			lastBreakable = -1;
			curWidth = 0;
		}
	}
}


int GlowTextData::LinePixelWidth(
	GlowFont font,
	int line) const
{
	GLOW_ASSERT(line >= 0);
	GLOW_ASSERT(line < int(lineBreaks_.size()));
	
	int e = ((line == int(lineBreaks_.size())-1) ?
		size() : lineBreaks_[line+1]);
	int curWidth = 0;
	const char* dat = data();
	for (int pos=lineBreaks_[line]; pos<e; pos++)
	{
		curWidth += ::glutBitmapWidth(font, dat[pos]);
	}
	return curWidth;
}


int GlowTextData::MaxPixelWidth(
	GlowFont font) const
{
	int maxWidth = 0;
	for (int i=0; i<int(lineBreaks_.size()); i++)
	{
		int w = LinePixelWidth(font, i);
		if (w>maxWidth) maxWidth = w;
	}
	return maxWidth;
}


int GlowTextData::AtPixelPos(
	GlowFont font,
	int line,
	int pixpos) const
{
	GLOW_ASSERT(line >= 0);
	GLOW_ASSERT(line < int(lineBreaks_.size()));
	
	int e = ((line == int(lineBreaks_.size())-1) ?
		size() : lineBreaks_[line+1]);
	
	if (pixpos < 0)
	{
		return lineBreaks_[line];
	}
	const char* dat = data();
	
	int curWidth = 0;
	for (int pos=lineBreaks_[line]; pos<e; pos++)
	{
		int nWidth = curWidth+::glutBitmapWidth(font, dat[pos]);
		if (pixpos <= nWidth)
		{
			if (nWidth-pixpos > pixpos-curWidth)
			{
				return pos;
			}
			else
			{
				return pos+1;
			}
		}
		curWidth = nWidth;
	}
	return e;
}


void GlowTextData::CalcLineDrawInfo(
	GlowFont font,
	int line,
	int pixoffset,
	int pixwidth,
	int& start,
	int& end,
	int& leftoffset) const
{
	GLOW_ASSERT(line >= 0);
	GLOW_ASSERT(line < int(lineBreaks_.size()));
	
	int e = ((line == int(lineBreaks_.size())-1) ?
		size() : lineBreaks_[line+1]);
	
	if (pixoffset < 0)
	{
		leftoffset = 0;
		start = e;
		end = e;
		return;
	}
	const char* dat = data();
	
	int curWidth = 0;
	for (start=lineBreaks_[line]; start<e; start++)
	{
		int nWidth = curWidth+::glutBitmapWidth(font, dat[start]);
		if (pixoffset < nWidth)
		{
			break;
		}
		curWidth = nWidth;
	}
	leftoffset = pixoffset-curWidth;
	for (end=start; end<e; end++)
	{
		if (pixoffset+pixwidth < curWidth)
		{
			break;
		}
		curWidth += ::glutBitmapWidth(font, dat[end]);
	}
}


int GlowTextData::PixelPosOf(
	GlowFont font,
	int pos,
	int line) const
{
	GLOW_ASSERT(line >= -1);
	GLOW_ASSERT(line < int(lineBreaks_.size()));
	GLOW_ASSERT(pos >= 0);
	GLOW_ASSERT(pos <= int(size()));
	
	if (line == -1)
	{
		line = LineNumOf(pos);
	}
	
	const char* dat = data();
	int curWidth = 0;
	for (int i=lineBreaks_[line]; i<pos; ++i)
	{
		curWidth += ::glutBitmapWidth(font, dat[i]);
	}
	return curWidth;
}


void GlowTextData::RevalidateSelection()
{
	if (selEnd_ > int(size()))
	{
		selEnd_ = size();
		if (selStart_ > selEnd_)
		{
			selStart_ = selEnd_;
		}
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

