/*
===============================================================================

	FILE:  glowImageFile.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Image file I/O for GLOW
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
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

inline GlowPNMFile_Base::GlowPNMFile_Base()
{
	error_ = noError;
}


inline GlowPNMFile_Base::PNMError GlowPNMFile_Base::GetError() const
{
	return error_;
}


inline void GlowPNMFile_Base::ResetError()
{
	error_ = noError;
}


/*
===============================================================================
	Inline methods for GlowPNMReader_Base
===============================================================================
*/

inline GlowPNMReader_Base::GlowPNMReader_Base(
	GLOW_STD::istream& stream)
{
	stream_ = &stream;
}


inline GLOW_STD::istream& GlowPNMReader_Base::GetStream() const
{
	return *stream_;
}


inline void GlowPNMReader_Base::SetStream(
	GLOW_STD::istream& stream)
{
	stream_ = &stream;
}


/*
===============================================================================
	Inline methods for GlowPNMWriter_Base
===============================================================================
*/

inline GlowPNMWriter_Base::GlowPNMWriter_Base(
	GLOW_STD::ostream& stream)
{
	stream_ = &stream;
}


inline GLOW_STD::ostream& GlowPNMWriter_Base::GetStream() const
{
	return *stream_;
}


inline void GlowPNMWriter_Base::SetStream(
	GLOW_STD::ostream& stream)
{
	stream_ = &stream;
}


/*
===============================================================================
	Inline methods for template GlowPNMReader
===============================================================================
*/

template <class ImageClass>
inline TGlowPNMReader<ImageClass>::TGlowPNMReader(
	GLOW_STD::istream& stream) :
GlowPNMReader_Base(stream)
{
}


/*
===============================================================================
	Inline methods for template GlowPNMWriter
===============================================================================
*/

template <class ImageClass>
inline TGlowPNMWriter<ImageClass>::TGlowPNMWriter(
	GLOW_STD::ostream& stream) :
GlowPNMWriter_Base(stream)
{
}


template <class ImageClass>
void TGlowPNMWriter<ImageClass>::WriteAscii(
	const ImageClass& image)
{
	Write(image, false);
}


template <class ImageClass>
void TGlowPNMWriter<ImageClass>::WriteBinary(
	const ImageClass& image)
{
	Write(image, true);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

