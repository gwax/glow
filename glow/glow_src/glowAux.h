/*
===============================================================================

	FILE:  glowAux.h
	
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


#ifndef GLOW_AUX__H
#define GLOW_AUX__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

// Microsoft cluelessness: the win32 gl.h depends on windows.h but
// doesn't include it.
#if defined(_WIN32) || defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
#endif

#include <GL/glut.h>

#include <cstddef>


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS GlowFont
===============================================================================
*/

class GlowFont
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static GlowFont fixed8by13;
		static GlowFont fixed9by15;
		static GlowFont helvetica10;
		static GlowFont helvetica12;
		static GlowFont helvetica18;
		static GlowFont timesRoman10;
		static GlowFont timesRoman24;
	
	public:
	
		inline GlowFont();
		inline GlowFont(
			void* glutFont);
		
		inline operator void*();
		inline operator const void*() const;
		inline GlowFont& operator=(
			const GlowFont& font);
		inline GlowFont& operator=(
			void* glutFont);
		inline bool operator==(
			void* op2) const;
	
	public:
	
		int Leading() const;
		int BaselinePos() const;
		int StringWidth(
			const char* str) const;
		inline int CharWidth(
			int ch) const;
	
	public:
	
		static void SetLeadingMetric(
			GlowFont font,
			int value);
		static void SetBaselineMetric(
			GlowFont font,
			int value);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		void* glutFont_;
	
	private:
	
		static int leading8by13_;
		static int leading9by15_;
		static int leadingHelve10_;
		static int leadingHelve12_;
		static int leadingHelve18_;
		static int leadingTimes10_;
		static int leadingTimes24_;
		
		static int baseline8by13_;
		static int baseline9by15_;
		static int baselineHelve10_;
		static int baselineHelve12_;
		static int baselineHelve18_;
		static int baselineTimes10_;
		static int baselineTimes24_;
};



/*
===============================================================================
	CLASS GlowColor

	Color value for Glow components
===============================================================================
*/

class GlowColor
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const GlowColor black;
		static const GlowColor blue;
		static const GlowColor green;
		static const GlowColor cyan;
		static const GlowColor red;
		static const GlowColor magenta;
		static const GlowColor yellow;
		static const GlowColor white;
		static const GlowColor gray;
	
	public:
	
		inline explicit GlowColor(
			GLubyte rgbval = 0,
			GLubyte aval = 255);
		inline explicit GlowColor(
			GLubyte r,
			GLubyte g,
			GLubyte b,
			GLubyte a = 255);
		inline explicit GlowColor(
			GLfloat r,
			GLfloat g,
			GLfloat b,
			GLfloat a = 1.0f);
		inline explicit GlowColor(
			GLdouble r,
			GLdouble g,
			GLdouble b,
			GLdouble a = 1.0);
		
		inline void Set(
			GLubyte rgbval = 0,
			GLubyte aval = 255);
		inline void Set(
			GLubyte r,
			GLubyte g,
			GLubyte b,
			GLubyte a = 255);
		inline void Set(
			GLfloat r,
			GLfloat g,
			GLfloat b,
			GLfloat a = 1.0f);
		inline void Set(
			GLdouble r,
			GLdouble g,
			GLdouble b,
			GLdouble a = 1.0);
		
		inline operator GLubyte*();
		inline operator const GLubyte*() const;
		inline GLubyte& operator[](
			GLOW_CSTD::ptrdiff_t i);
		inline const GLubyte& operator[](
			GLOW_CSTD::ptrdiff_t i) const;
		
		inline bool operator==(
			const GlowColor& op2) const;
		inline bool operator!=(
			const GlowColor& op2) const;
		
		inline void Apply() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GLubyte rgba_[4];
};


/*
===============================================================================
	CLASS GlowImage

	Image template class
===============================================================================
*/

template <class T>
class TGlowImage
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef T ElementType;
	
	public:
	
		// Constructors
		inline TGlowImage();
		TGlowImage(
			const TGlowImage<T>& orig);
		TGlowImage(
			unsigned int width,
			unsigned int height,
			T defelem = T(0));
		
		// Destructor
		virtual ~TGlowImage();
		
		// Set
		TGlowImage<T>& operator=(
			const TGlowImage<T>& orig);
		
		// Element access
		inline T& At(
			unsigned int x,
			unsigned int y);
		inline const T& At(
			unsigned int x,
			unsigned int y) const;
		inline T& operator()(
			unsigned int x,
			unsigned int y);
		inline const T& operator()(
			unsigned int x,
			unsigned int y) const;
		
		// Global mutator
		void Clear(
			T defelem = T(0));
		
		// Array access
		inline T* Array();
		inline const T* Array() const;
		inline void* RawArray();
		inline const void* RawArray() const;
		inline T* ReleaseArray();
		inline void GrabArray(
			T* array,
			unsigned int width,
			unsigned int height);
		
		// Size access
		inline unsigned int Width() const;
		inline unsigned int Height() const;
		
		// Size mutators
		void ResizeRaw(
			unsigned int width,
			unsigned int height);
		inline void ResizeClear(
			unsigned int width,
			unsigned int height,
			T defelem = T(0));
		void ResizeCopy(
			unsigned int width,
			unsigned int height);
		void ResizeClearCopy(
			unsigned int width,
			unsigned int height,
			T defelem = T(0));
	
	public:
	
		// Subrect copy
		static void CopyRect(
			unsigned int width,
			unsigned int height,
			TGlowImage<T>& to,
			unsigned int xto,
			unsigned int yto,
			const TGlowImage<T>& from,
			unsigned int xfrom,
			unsigned int yfrom);
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		T* data_;
		unsigned int length_;
		unsigned int width_;
		unsigned int height_;
};


/*
===============================================================================
	Common image types
===============================================================================
*/

class GlowColorImage :
	public TGlowImage<GlowColor>
{
	public:
	
		// Constructors
		inline GlowColorImage();
		inline GlowColorImage(
			const GlowColorImage& orig);
		inline GlowColorImage(
			unsigned int width,
			unsigned int height,
			GlowColor defelem = GlowColor());
		
		// I/O information
		typedef unsigned char ChannelType;
		inline unsigned int NumChannels() const;
		inline ChannelType GetChannel(
			unsigned int x,
			unsigned int y,
			unsigned int c) const;
		inline void SetChannel(
			unsigned int x,
			unsigned int y,
			unsigned int c,
			ChannelType data);
		
		// OpenGL integration
		void ReadFromBuffer(
			int x,
			int y,
			unsigned int width,
			unsigned int height);
		void DrawAtRasterPos() const;
		
		// Scale
		void Scale(
			unsigned int nWidth,
			unsigned int nHeight);
};


class GlowUcharImage :
	public TGlowImage<unsigned char>
{
	public:
	
		// Constructors
		inline GlowUcharImage();
		inline GlowUcharImage(
			const GlowUcharImage& orig);
		inline GlowUcharImage(
			unsigned int width,
			unsigned int height,
			unsigned char defelem = 0);
		
		// I/O information
		typedef unsigned char ChannelType;
		inline unsigned int NumChannels() const;
		inline ChannelType GetChannel(
			unsigned int x,
			unsigned int y,
			unsigned int c) const;
		inline void SetChannel(
			unsigned int x,
			unsigned int y,
			unsigned int c,
			ChannelType data);
		
		// OpenGL integration
		void ReadFromBuffer(
			int x,
			int y,
			unsigned int width,
			unsigned int height,
			GLenum which);
		void DrawAtRasterPos(
			GLenum which) const;
};


class GlowGLfloatImage :
	public TGlowImage<GLfloat>
{
	public:
	
		// Constructors
		inline GlowGLfloatImage();
		inline GlowGLfloatImage(
			const GlowGLfloatImage& orig);
		inline GlowGLfloatImage(
			unsigned int width,
			unsigned int height,
			GLfloat defelem = 0.0f);
		
		// I/O information
		typedef float ChannelType;
		inline unsigned int NumChannels() const;
		inline ChannelType GetChannel(
			unsigned int x,
			unsigned int y,
			unsigned int c) const;
		inline void SetChannel(
			unsigned int x,
			unsigned int y,
			unsigned int c,
			ChannelType data);
		
		// OpenGL integration
		void ReadFromBuffer(
			int x,
			int y,
			unsigned int width,
			unsigned int height,
			GLenum which);
		void DrawAtRasterPos(
			GLenum which) const;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowAux.inl.h"
// TODO: compatibility with export
#include "glowAux.tem.h"


#endif

