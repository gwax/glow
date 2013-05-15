/*
===============================================================================

	FILE:  mandeldata.h
	
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
	
		The GLOW Toolkit tutorial -- version 1.1.2dev  (7 August 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/

#ifndef MANDELDATA__H
#define MANDELDATA__H


/*
===============================================================================
	Headers
===============================================================================
*/

#include <utility>

using namespace std;


/*
===============================================================================
	Types
===============================================================================
*/

typedef double Numeric;


/*
===============================================================================
	Data structure
===============================================================================
*/

class MandelData
{
	public:
	
		MandelData();
		
		// Size of the image to compute in pixels
		inline pair<int, int> GetSize() const;
		inline int Width() const;
		inline int Height() const;
		void SetSize(
			int width,
			int height);
		
		// Location of the center of the image in the complex plane
		inline pair<Numeric, Numeric> GetCenter() const;
		inline Numeric CenterX() const;
		inline Numeric CenterY() const;
		inline void SetCenter(
			Numeric x,
			Numeric y);
		inline void MoveCenter(
			Numeric deltax,
			Numeric deltay);
		
		// Width of a pixel in the complex plane
		// (i.e. the zoom of the image)
		inline Numeric GetPixelWidth() const;
		inline void SetPixelWidth(
			Numeric width);
		inline void ScalePixelWidth(
			Numeric factor);
		
		// The computation threshhold, or how many iterations to check
		// before assuming an orbit doesn't escape
		inline int GetThreshhold() const;
		inline void SetThreshhold(
			int thresh);
		
		// Recalculation
		inline bool IsDataValid() const;
		void Recalc();
		inline int LinesRemaining() const;
		void RecalcOneLine();
		void InvalidateData();
		
		// Data access
		inline int DataAt(
			int i,
			int j) const;
		inline const int* Data() const;
	
	private:
	
		int* data_;
		int width_;
		int height_;
		Numeric xcenter_;
		Numeric ycenter_;
		Numeric pixelwidth_;
		int threshhold_;
		bool valid_;
		int linesLeft_;
		Numeric cury_;
};


/*
===============================================================================
	Inline methods
===============================================================================
*/

inline pair<int, int> MandelData::GetSize() const
{
	return make_pair(width_, height_);
}


inline int MandelData::Width() const
{
	return width_;
}


inline int MandelData::Height() const
{
	return height_;
}


inline pair<Numeric, Numeric> MandelData::GetCenter() const
{
	return make_pair(xcenter_, ycenter_);
}


inline Numeric MandelData::CenterX() const
{
	return xcenter_;
}


inline Numeric MandelData::CenterY() const
{
	return ycenter_;
}


inline void MandelData::SetCenter(
	Numeric x,
	Numeric y)
{
	xcenter_ = x;
	ycenter_ = y;
	InvalidateData();
}


inline void MandelData::MoveCenter(
	Numeric deltax,
	Numeric deltay)
{
	xcenter_ += deltax;
	ycenter_ += deltay;
	InvalidateData();
}


inline Numeric MandelData::GetPixelWidth() const
{
	return pixelwidth_;
}


inline void MandelData::SetPixelWidth(
	Numeric width)
{
	pixelwidth_ = width;
	InvalidateData();
}


inline void MandelData::ScalePixelWidth(
	Numeric factor)
{
	pixelwidth_ *= factor;
	InvalidateData();
}


inline void MandelData::SetThreshhold(
	int thresh)
{
	threshhold_ = thresh;
	InvalidateData();
}


inline int MandelData::GetThreshhold() const
{
	return threshhold_;
}


inline bool MandelData::IsDataValid() const
{
	return valid_;
}


inline int MandelData::LinesRemaining() const
{
	return linesLeft_;
}


inline int MandelData::DataAt(
	int i,
	int j) const
{
	return data_[i*width_+j];
}


inline const int* MandelData::Data() const
{
	return static_cast<const int*>(data_);
}


#endif
