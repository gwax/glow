/*
===============================================================================

	FILE:  mandeldata.cpp
	
	PROJECT:
	
		GLOW-based mandelbrot viewer
	
	CONTENTS:
	
		Data and computation engine
	
	PROGRAMMERS:
	
		Daniel Azuma <dazuma@kagi.com>
	
	COPYRIGHT:
	
		Copyright (C) 2000  Daniel Azuma  (dazuma@kagi.com)
		
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
	
		The GLOW Toolkit tutorial -- version 1.1.1dev  (24 July 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*
===============================================================================
	Headers
===============================================================================
*/

#include "mandeldata.h"


/*
===============================================================================
	Methods of MandelData
===============================================================================
*/

MandelData::MandelData()
{
	data_ = new int[201*201];
	width_ = 201;
	height_ = 201;
	xcenter_ = -0.5;
	ycenter_ = 0;
	pixelwidth_ = 0.015;
	valid_ = false;
	linesLeft_ = height_;
	threshhold_ = 1023;
}


void MandelData::SetSize(
	int width,
	int height)
{
	width_ = width;
	height_ = height;
	delete data_;
	data_ = new int[width*height];
	valid_ = false;
	linesLeft_ = height_;
}


void MandelData::Recalc()
{
	while (!valid_)
	{
		RecalcOneLine();
	}
}


void MandelData::RecalcOneLine()
{
	if (valid_) return;
	if (linesLeft_ == height_)
	{
		cury_ = ycenter_-Numeric(height_-1)*0.5*pixelwidth_;
	}
	
	int* data = data_+(height_-linesLeft_)*width_;
	Numeric x = xcenter_-Numeric(width_-1)*0.5*pixelwidth_;
	for (int xindex=0; xindex<width_; ++xindex)
	{
		Numeric a=0;
		Numeric b=0;
		int index=1;
		while (index<threshhold_ && a*a+b*b<4)
		{
			Numeric anew = a*a-b*b+x;
			b = a*b*2+cury_;
			a = anew;
			++index;
		}
		*data = (index == threshhold_) ? 0 : index;
		data++;
		x += pixelwidth_;
	}
	cury_ += pixelwidth_;
	
	--linesLeft_;
	if (linesLeft_ == 0)
	{
		valid_ = true;
	}
}


void MandelData::InvalidateData()
{
	valid_ = false;
	linesLeft_ = height_;
	for (int i=0; i<width_*height_; ++i)
	{
		data_[i] = 0;
	}
}

