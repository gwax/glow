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
	int i)
{
	GLOW_ASSERT(i>=0 && i<4);
	return rgba_[i];
}


inline const GLubyte& GlowColor::operator[](
	int i) const
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
*/

GLOW_NAMESPACE_END

