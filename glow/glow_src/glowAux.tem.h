/*
===============================================================================

	FILE:  glowAux.tem.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Auxiliary classes for GLOW
	
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

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods and data for GlowImage
===============================================================================
*/

template <class T>
GlowImage<T>::GlowImage(
	const GlowImage<T>& orig)
{
	width_ = orig.width_;
	height_ = orig.height_;
	data_ = new T[width_*height_];
	GLOW_CSTD::memcpy(data_, orig.data_, width_*height_*sizeof(T));
}


template <class T>
GlowImage<T>::GlowImage(
	unsigned int width,
	unsigned int height,
	T defelem)
{
	width_ = width;
	height_ = height;
	int length = width_*height_;
	data_ = new T[length];
	for (int i=0; i<length; ++i)
	{
		data_[i] = defelem;
	}
}


template <class T>
GlowImage<T>::~GlowImage()
{
	delete[] data_;
}


template <class T>
GlowImage<T>& GlowImage<T>::operator=(
	const GlowImage<T>& orig)
{
	if (orig.width_ != width_ || orig.height_ != height_)
	{
		delete[] data_;
		width_ = orig.width_;
		height_ = orig.height_;
		data_ = new T[width_*height_];
	}
	GLOW_CSTD::memcpy(data_, orig.data_, width_*height_*sizeof(T));
}


template <class T>
void GlowImage<T>::Clear(
	T defelem)
{
	for (int i=0, length=width_*height_; i<length; ++i)
	{
		data_[i] = defelem;
	}
}


template <class T>
void GlowImage<T>::ResizeRaw(
	unsigned int width,
	unsigned int height)
{
	if (width != width_ || height != height_)
	{
		delete[] data_;
		width_ = width;
		height_ = height;
		data_ = new T[width_*height_];
	}
}


template <class T>
void GlowImage<T>::ResizeCopy(
	unsigned int width,
	unsigned int height)
{
	if (width != width_ || height != height_)
	{
		T* data = new T[width*height];
		
		int kwidth = GLOW_CSTD::min(width, width_);
		int kheight = GLOW_CSTD::min(height, height_);
		for (int j=0; j<kheight; j++)
		for (int i=0; i<kwidth; i++)
		{
			data[j*width+i] = data_[j*width_+i];
		}
		
		delete[] data_;
		width_ = width;
		height_ = height;
		data_ = data;
	}
}


template <class T>
void GlowImage<T>::ResizeClearCopy(
	unsigned int width,
	unsigned int height,
	T defelem)
{
	if (width != width_ || height != height_)
	{
		unsigned int length = width*height;
		T* data = new T[length];
		for (int i=0; i<length; ++i)
		{
			data[i] = defelem;
		}
		
		int kwidth = GLOW_CSTD::min(width, width_);
		int kheight = GLOW_CSTD::min(height, height_);
		for (int j=0; j<kheight; j++)
		for (int i=0; i<kwidth; i++)
		{
			data[j*width+i] = data_[j*width_+i];
		}
		
		delete[] data_;
		width_ = width;
		height_ = height;
		data_ = data;
	}
}


template <class T>
void GlowImage<T>::CopyRect(
	unsigned int width,
	unsigned int height,
	GlowImage<T>& to,
	unsigned int xto,
	unsigned int yto,
	const GlowImage<T>& from,
	unsigned int xfrom,
	unsigned int yfrom)
{
	for (int j=0; j<height; ++j)
	for (int i=0; i<width; ++i)
	{
		to.data_[(yto+j)*to.width_+xto+i] =
			from.data_[(yfrom+j)*from.width_+xfrom+i];
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

