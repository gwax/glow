/*
===============================================================================

	FILE:  glowImageFile.h
	
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


#ifndef GLOW_IMAGEFILE__H
#define GLOW_IMAGEFILE__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_AUX__H
	#include "glowAux.h"
#endif

#include <iostream>


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS GlowPNMFile_Base
===============================================================================
*/

class GlowPNMFile_Base
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum PNMError
		{
			noError = 0,
			eofError = 1,
			failIOError = 2,
			badIOError = 3,
			otherIOError = 4,
			magicNumberError = 10,
			numChannelsError = 11,
			rangeError = 12
		};
	
	public:
	
		inline PNMError GetError() const;
		inline void ResetError();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		inline GlowPNMFile_Base();
		
		bool ErrorState_(
			GLOW_STD::ios::iostate state);
	
	protected:
	
		PNMError error_;
};


/*
===============================================================================
	CLASS GlowPNMReader_Base
===============================================================================
*/

class GlowPNMReader_Base :
	public GlowPNMFile_Base
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GLOW_STD::istream& GetStream() const;
		inline void SetStream(
			GLOW_STD::istream& stream);
		
		void GetInfo(
			int& format,
			int& width,
			int& height,
			int& maxVal,
			int& headerLength);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		inline GlowPNMReader_Base(
			GLOW_STD::istream& stream);
	
	protected:
	
		int GetToken_();
	
	protected:
	
		GLOW_STD::istream* stream_;
};


/*
===============================================================================
	CLASS GlowPNMWriter_Base
===============================================================================
*/

class GlowPNMWriter_Base :
	public GlowPNMFile_Base
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GLOW_STD::ostream& GetStream() const;
		inline void SetStream(
			GLOW_STD::ostream& stream);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		inline GlowPNMWriter_Base(
			GLOW_STD::ostream& stream);
	
	protected:
	
		GLOW_STD::ostream* stream_;
};


/*
===============================================================================
	TEMPLATE CLASS GlowPNMReader
===============================================================================
*/

template <class ImageClass>
class TGlowPNMReader :
	public GlowPNMReader_Base
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline TGlowPNMReader(
			GLOW_STD::istream& stream);
		
		void Read(
			ImageClass& image);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		void Set1Channels_(
			typename ImageClass::ElementType elem,
			unsigned int i,
			unsigned int j);
		void Set3Channels_(
			typename ImageClass::ElementType elem1,
			typename ImageClass::ElementType elem2,
			typename ImageClass::ElementType elem3,
			unsigned int i,
			unsigned int j);
	
	private:
	
		unsigned int numChannels_;
		typename ImageClass::ElementType alphaDefault_;
		ImageClass* imagePtr_;
};


/*
===============================================================================
	TEMPLATE CLASS GlowPNMWriter
===============================================================================
*/

template <class ImageClass>
class TGlowPNMWriter :
	public GlowPNMWriter_Base
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline TGlowPNMWriter(
			GLOW_STD::ostream& stream);
		
		void Write(
			const ImageClass& image,
			bool binary = true);
		
		void WriteBinary(
			const ImageClass& image);
		void WriteAscii(
			const ImageClass& image);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowImageFile.inl.h"
// TODO: compatibility with export
#include "glowImageFile.tem.h"


#endif

