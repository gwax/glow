/*
===============================================================================

	FILE:  glowAux.inl.h
	
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
	Inline methods for GlowFont
===============================================================================
*/

inline GlowFont::GlowFont()
{
	glutFont_ = GLUT_BITMAP_HELVETICA_12;
}


inline GlowFont::GlowFont(
	void* glutFont)
{
	GLOW_DEBUG(glutFont != GLUT_BITMAP_HELVETICA_10 &&
		glutFont != GLUT_BITMAP_HELVETICA_12 &&
		glutFont != GLUT_BITMAP_HELVETICA_18 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_10 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_24 &&
		glutFont != GLUT_BITMAP_8_BY_13 &&
		glutFont != GLUT_BITMAP_9_BY_15, "Unknown GLUT font");
	glutFont_ = glutFont;
}


inline GlowFont::operator void*()
{
	return glutFont_;
}


inline GlowFont::operator const void*() const
{
	return glutFont_;
}


inline GlowFont& GlowFont::operator=(
	const GlowFont& font)
{
	glutFont_ = font.glutFont_;
	return *this;
}


inline GlowFont& GlowFont::operator=(
	void* glutFont)
{
	GLOW_DEBUG(glutFont != GLUT_BITMAP_HELVETICA_10 &&
		glutFont != GLUT_BITMAP_HELVETICA_12 &&
		glutFont != GLUT_BITMAP_HELVETICA_18 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_10 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_24 &&
		glutFont != GLUT_BITMAP_8_BY_13 &&
		glutFont != GLUT_BITMAP_9_BY_15, "Unknown GLUT font");
	glutFont_ = glutFont;
	return *this;
}


inline bool GlowFont::operator==(
	void* op2) const
{
	return glutFont_ == op2;
}


inline int GlowFont::CharWidth(
	int ch) const
{
	return ::glutBitmapWidth(glutFont_, ch);
}


/*
===============================================================================
	Inline methods for GlowColor
===============================================================================
*/

inline GlowColor::GlowColor(
	GLubyte rgbval,
	GLubyte aval)
{
	rgba_[0] = rgba_[1] = rgba_[2] = rgbval;
	rgba_[3] = aval;
}


inline GlowColor::GlowColor(
	GLubyte r,
	GLubyte g,
	GLubyte b,
	GLubyte a)
{
	rgba_[0] = r;
	rgba_[1] = g;
	rgba_[2] = b;
	rgba_[3] = a;
}


inline GlowColor::GlowColor(
	GLfloat r,
	GLfloat g,
	GLfloat b,
	GLfloat a)
{
	rgba_[0] = GLubyte(int(r*255.0f));
	rgba_[1] = GLubyte(int(g*255.0f));
	rgba_[2] = GLubyte(int(b*255.0f));
	rgba_[3] = GLubyte(int(a*255.0f));
}


inline GlowColor::GlowColor(
	GLdouble r,
	GLdouble g,
	GLdouble b,
	GLdouble a)
{
	rgba_[0] = GLubyte(int(r*255.0));
	rgba_[1] = GLubyte(int(g*255.0));
	rgba_[2] = GLubyte(int(b*255.0));
	rgba_[3] = GLubyte(int(a*255.0));
}


inline void GlowColor::Set(
	GLubyte rgbval,
	GLubyte aval)
{
	rgba_[0] = rgba_[1] = rgba_[2] = rgbval;
	rgba_[3] = aval;
}


inline void GlowColor::Set(
	GLubyte r,
	GLubyte g,
	GLubyte b,
	GLubyte a)
{
	rgba_[0] = r;
	rgba_[1] = g;
	rgba_[2] = b;
	rgba_[3] = a;
}


inline void GlowColor::Set(
	GLfloat r,
	GLfloat g,
	GLfloat b,
	GLfloat a)
{
	rgba_[0] = GLubyte(int(r*255.0f));
	rgba_[1] = GLubyte(int(g*255.0f));
	rgba_[2] = GLubyte(int(b*255.0f));
	rgba_[3] = GLubyte(int(a*255.0f));
}


inline void GlowColor::Set(
	GLdouble r,
	GLdouble g,
	GLdouble b,
	GLdouble a)
{
	rgba_[0] = GLubyte(int(r*255.0));
	rgba_[1] = GLubyte(int(g*255.0));
	rgba_[2] = GLubyte(int(b*255.0));
	rgba_[3] = GLubyte(int(a*255.0));
}


inline GlowColor::operator GLubyte*()
{
	return rgba_;
}


inline GlowColor::operator const GLubyte*() const
{
	return rgba_;
}


inline GLubyte& GlowColor::operator[](
	GLOW_CSTD::ptrdiff_t i)
{
	GLOW_ASSERT(i>=0 && i<4);
	return rgba_[i];
}


inline const GLubyte& GlowColor::operator[](
	GLOW_CSTD::ptrdiff_t i) const
{
	GLOW_ASSERT(i>=0 && i<4);
	return rgba_[i];
}


inline bool GlowColor::operator==(
	const GlowColor& op2) const
{
	return rgba_[0] == op2.rgba_[0] &&
		rgba_[1] == op2.rgba_[1] &&
		rgba_[2] == op2.rgba_[2] &&
		rgba_[3] == op2.rgba_[3];
}


inline bool GlowColor::operator!=(
	const GlowColor& op2) const
{
	return rgba_[0] != op2.rgba_[0] ||
		rgba_[1] != op2.rgba_[1] ||
		rgba_[2] != op2.rgba_[2] ||
		rgba_[3] != op2.rgba_[3];
}


inline void GlowColor::Apply() const
{
	::glColor4ubv(rgba_);
}


/*
===============================================================================
	Inline methods for GlowColor
===============================================================================
*/

template <class T>
inline TGlowImage<T>::TGlowImage()
{
	data_ = 0;
	width_ = 0;
	height_ = 0;
}


template <class T>
inline T& TGlowImage<T>::At(
	unsigned int x,
	unsigned int y)
{
	return data_[y*width_+x];
}


template <class T>
inline const T& TGlowImage<T>::At(
	unsigned int x,
	unsigned int y) const
{
	return data_[y*width_+x];
}


template <class T>
inline T& TGlowImage<T>::operator()(
	unsigned int x,
	unsigned int y)
{
	return data_[y*width_+x];
}


template <class T>
inline const T& TGlowImage<T>::operator()(
	unsigned int x,
	unsigned int y) const
{
	return data_[y*width_+x];
}


template <class T>
inline T* TGlowImage<T>::Array()
{
	return data_;
}


template <class T>
inline const T* TGlowImage<T>::Array() const
{
	return data_;
}


template <class T>
inline void* TGlowImage<T>::RawArray()
{
	return static_cast<void*>(data_);
}


template <class T>
inline const void* TGlowImage<T>::RawArray() const
{
	return static_cast<const void*>(data_);
}


template <class T>
inline T* ReleaseArray()
{
	T* ret = data_;
	data_ = 0;
	width_ = 0;
	height_ = 0;
	return ret;
}


inline void GrabArray(
	T* array,
	unsigned int width,
	unsigned int height)
{
	delete data_;
	data_ = array;
	width_ = width;
	height_ = height;
}


template <class T>
inline unsigned int TGlowImage<T>::Width() const
{
	return width_;
}


template <class T>
inline unsigned int TGlowImage<T>::Height() const
{
	return height_;
}


template <class T>
inline void TGlowImage<T>::ResizeClear(
	unsigned int width,
	unsigned int height,
	T defelem)
{
	ResizeRaw(width, height);
	Clear(defelem);
}


/*
===============================================================================
	Common image types
===============================================================================
*/

// TODO: This really should use traits classes, but MSVC doesn't seem to
// like specializations right now.

inline GlowColorImage::GlowColorImage()
{
}


inline GlowColorImage::GlowColorImage(
	const GlowColorImage& orig) :
TGlowImage<GlowColor>(orig)
{
}


inline GlowColorImage::GlowColorImage(
	unsigned int width,
	unsigned int height,
	GlowColor defelem) :
TGlowImage<GlowColor>(width, height, defelem)
{
}


inline unsigned int GlowColorImage::NumChannels() const
{
	return 4;
}


inline unsigned char GlowColorImage::GetChannel(
	unsigned int x,
	unsigned int y,
	unsigned int c) const
{
	return At(x, y)[c];
}


inline void GlowColorImage::SetChannel(
	unsigned int x,
	unsigned int y,
	unsigned int c,
	unsigned char data)
{
	At(x, y)[c] = data;
}


// GlowUcharImage

inline GlowUcharImage::GlowUcharImage()
{
}


inline GlowUcharImage::GlowUcharImage(
	const GlowUcharImage& orig) :
TGlowImage<unsigned char>(orig)
{
}


inline GlowUcharImage::GlowUcharImage(
	unsigned int width,
	unsigned int height,
	unsigned char defelem) :
TGlowImage<unsigned char>(width, height, defelem)
{
}


inline unsigned int GlowUcharImage::NumChannels() const
{
	return 1;
}


inline unsigned char GlowUcharImage::GetChannel(
	unsigned int x,
	unsigned int y,
	unsigned int c) const
{
	return At(x, y);
}


inline void GlowUcharImage::SetChannel(
	unsigned int x,
	unsigned int y,
	unsigned int c,
	unsigned char data)
{
	At(x, y) = data;
}


// GlowGLfloatImage

inline GlowGLfloatImage::GlowGLfloatImage()
{
}


inline GlowGLfloatImage::GlowGLfloatImage(
	const GlowGLfloatImage& orig) :
TGlowImage<GLfloat>(orig)
{
}


inline GlowGLfloatImage::GlowGLfloatImage(
	unsigned int width,
	unsigned int height,
	GLfloat defelem) :
TGlowImage<GLfloat>(width, height, defelem)
{
}


inline unsigned int GlowGLfloatImage::NumChannels() const
{
	return 1;
}


inline GLfloat GlowGLfloatImage::GetChannel(
	unsigned int x,
	unsigned int y,
	unsigned int c) const
{
	return At(x, y);
}


inline void GlowGLfloatImage::SetChannel(
	unsigned int x,
	unsigned int y,
	unsigned int c,
	GLfloat data)
{
	At(x, y) = data;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

