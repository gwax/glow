/*
===============================================================================

	FILE:  glowImageFile.cpp
	
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
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
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

#include <cstring>
#include <cstdlib>

#ifndef GLOW_IMAGEFILE__H
	#include "glowImageFile.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of GlowPNMFile_Base
===============================================================================
*/

bool GlowPNMFile_Base::ErrorState_(
	GLOW_STD::ios::iostate state)
{
	if (state | GLOW_STD::ios::eofbit)
	{
		error_ = eofError;
	}
	else if (state | GLOW_STD::ios::badbit)
	{
		error_ = badIOError;
	}
	else if (state | GLOW_STD::ios::failbit)
	{
		error_ = failIOError;
	}
	else if (state != GLOW_STD::ios::goodbit)
	{
		error_ = otherIOError;
	}
	error_ = noError;
	return (error_ != noError);
}


/*
===============================================================================
	Methods of GlowPNMReader_Base
===============================================================================
*/

void GlowPNMReader_Base::GetInfo(
	int& format,
	int& width,
	int& height,
	int& maxVal,
	int& headerLength)
{
	if (error_ != noError)
	{
		return;
	}
	
	static char buf[72];
	
#ifdef GLOW_COMPAT_USEOLDSTREAMPOS
	GLOW_STD::streampos startpos = stream_->tellg();
#else
	GLOW_STD::istream::pos_type startpos = stream_->tellg();
#endif
	
	// Get magic nuber and format
	while (true)
	{
		*stream_ >> buf;
		if (ErrorState_(stream_->rdstate()))
		{
			return;
		}
		if (buf[0] == '#')
		{
			stream_->getline(buf, 70);
		}
		else
		{
			break;
		}
	}
	if (GLOW_CSTD::strlen(buf) != 2)
	{
		error_ = magicNumberError;
		return;
	}
	if (buf[0] != 'P')
	{
		error_ = magicNumberError;
		return;
	}
	format = buf[1] - '0';
	if (format < 1 || format > 6)
	{
		error_ = magicNumberError;
		return;
	}
	
	// Get width and height
	width = GetToken_();
	if (error_ != noError)
	{
		return;
	}
	height = GetToken_();
	if (error_ != noError)
	{
		return;
	}
	
	// Get maxVal if applicable
	if (format != 1 && format != 4)
	{
		maxVal = GetToken_();
		if (error_ != noError)
		{
			return;
		}
	}
	
	// One extra byte for binary formats (format quirk)
	if (format >= 4)
	{
		stream_->get();
		if (ErrorState_(stream_->rdstate()))
		{
			return;
		}
	}
	
	headerLength = stream_->tellg() - startpos;
}


int GlowPNMReader_Base::GetToken_()
{
	if (error_ != noError)
	{
		return 0;
	}
	
	static char buf[72];
	
	while (true)
	{
		*stream_ >> buf;
		if (ErrorState_(stream_->rdstate()))
		{
			return 0;
		}
		if (buf[0] == '#')
		{
			stream_->getline(buf, 70);
		}
		else
		{
			return GLOW_CSTD::atoi(buf);
		}
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

