/*
===============================================================================

	FILE:  glowAux.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Auxiliary classes in GLOW
	
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

#include <cstring>

#ifndef GLOW_AUX__H
	#include "glowAux.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	GlowColors
===============================================================================
*/

const GlowColor GlowColor::black(0);
const GlowColor GlowColor::blue(GLubyte(0), GLubyte(0), GLubyte(255));
const GlowColor GlowColor::green(GLubyte(0), GLubyte(255), GLubyte(0));
const GlowColor GlowColor::cyan(GLubyte(0), GLubyte(255), GLubyte(255));
const GlowColor GlowColor::red(GLubyte(255), GLubyte(0), GLubyte(0));
const GlowColor GlowColor::magenta(GLubyte(255), GLubyte(0), GLubyte(255));
const GlowColor GlowColor::yellow(GLubyte(255), GLubyte(255), GLubyte(0));
const GlowColor GlowColor::white(255);
const GlowColor GlowColor::gray(GLubyte(128), GLubyte(128), GLubyte(128));


/*
===============================================================================
	Methods and data for GlowFont
===============================================================================
*/

GlowFont GlowFont::fixed8by13 = GLUT_BITMAP_8_BY_13;
GlowFont GlowFont::fixed9by15 = GLUT_BITMAP_9_BY_15;
GlowFont GlowFont::helvetica10 = GLUT_BITMAP_HELVETICA_10;
GlowFont GlowFont::helvetica12 = GLUT_BITMAP_HELVETICA_12;
GlowFont GlowFont::helvetica18 = GLUT_BITMAP_HELVETICA_18;
GlowFont GlowFont::timesRoman10 = GLUT_BITMAP_TIMES_ROMAN_10;
GlowFont GlowFont::timesRoman24 = GLUT_BITMAP_TIMES_ROMAN_24;

int GlowFont::leading8by13_ = 15;
int GlowFont::leading9by15_ = 18;
int GlowFont::leadingHelve10_ = 12;
int GlowFont::leadingHelve12_ = 14;
int GlowFont::leadingHelve18_ = 22;
int GlowFont::leadingTimes10_ = 12;
int GlowFont::leadingTimes24_ = 28;

int GlowFont::baseline8by13_ = 11;
int GlowFont::baseline9by15_ = 13;
int GlowFont::baselineHelve10_ = 10;
int GlowFont::baselineHelve12_ = 11;
int GlowFont::baselineHelve18_ = 17;
int GlowFont::baselineTimes10_ = 9;
int GlowFont::baselineTimes24_ = 21;


int GlowFont::Leading() const
{
	if (glutFont_ == GLUT_BITMAP_8_BY_13)
	{
		return leading8by13_;
	}
	else if (glutFont_ == GLUT_BITMAP_9_BY_15)
	{
		return leading9by15_;
	}
	else if (glutFont_ == GLUT_BITMAP_HELVETICA_10)
	{
		return leadingHelve10_;
	}
	else if (glutFont_ == GLUT_BITMAP_HELVETICA_12)
	{
		return leadingHelve12_;
	}
	else if (glutFont_ == GLUT_BITMAP_HELVETICA_18)
	{
		return leadingHelve18_;
	}
	else if (glutFont_ == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		return leadingTimes10_;
	}
	else if (glutFont_ == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		return leadingTimes24_;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::Leading");
	return 0;
}


int GlowFont::BaselinePos() const
{
	if (glutFont_ == GLUT_BITMAP_8_BY_13)
	{
		return baseline8by13_;
	}
	else if (glutFont_ == GLUT_BITMAP_9_BY_15)
	{
		return baseline9by15_;
	}
	else if (glutFont_ == GLUT_BITMAP_HELVETICA_10)
	{
		return baselineHelve10_;
	}
	else if (glutFont_ == GLUT_BITMAP_HELVETICA_12)
	{
		return baselineHelve12_;
	}
	else if (glutFont_ == GLUT_BITMAP_HELVETICA_18)
	{
		return baselineHelve18_;
	}
	else if (glutFont_ == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		return baselineTimes10_;
	}
	else if (glutFont_ == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		return baselineTimes24_;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::BaselinePos");
	return 0;
}


void GlowFont::SetLeadingMetric(
	GlowFont font,
	int value)
{
	if (font == GLUT_BITMAP_8_BY_13)
	{
		leading8by13_ = value;
	}
	else if (font == GLUT_BITMAP_9_BY_15)
	{
		leading9by15_ = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_10)
	{
		leadingHelve10_ = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_12)
	{
		leadingHelve12_ = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_18)
	{
		leadingHelve18_ = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		leadingTimes10_ = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		leadingTimes24_ = value;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::SetLeadingMetric");
}


void GlowFont::SetBaselineMetric(
	GlowFont font,
	int value)
{
	if (font == GLUT_BITMAP_8_BY_13)
	{
		baseline8by13_ = value;
	}
	else if (font == GLUT_BITMAP_9_BY_15)
	{
		baseline9by15_ = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_10)
	{
		baselineHelve10_ = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_12)
	{
		baselineHelve12_ = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_18)
	{
		baselineHelve18_ = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		baselineTimes10_ = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		baselineTimes24_ = value;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::SetBaselineMetric");
}


int GlowFont::StringWidth(
	const char* str) const
{
	GLOW_DEBUGSCOPE("GlowFont::StringWidth");
	
	int ret = 0;
	int len = GLOW_CSTD::strlen(str);
	for (int i=0; i<len; i++)
	{
		ret += ::glutBitmapWidth(glutFont_, str[i]);
	}
	return ret;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

