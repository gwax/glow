/*
===============================================================================

	FILE:  glowHeader.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		General pre-header stuff
	
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
	
		The GLOW Toolkit -- version 0.95  (27 March 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin

===============================================================================
*/


#ifndef GLOW_HEADER__H
#define GLOW_HEADER__H


/*
===============================================================================
	Macros
===============================================================================
*/

#ifdef GLOW_COMPAT_NOSTDNAMESPACE
	#define GLOW_STD
#else
	#define GLOW_STD std
#endif

#ifdef GLOW_COMPAT_INTERNALUSINGSTD
	#define GLOW_INTERNAL_USINGSTD namespace std {} using namespace std;
#else
	#define GLOW_INTERNAL_USINGSTD
#endif

#ifdef GLOW_OPTION_USEGLOBALNAMESPACE
	#define GLOW_NAMESPACE
	#define GLOW_NAMESPACE_BEGIN
	#define GLOW_NAMESPACE_END
	#define GLOW_NAMESPACE_USING
#else
	#define GLOW_NAMESPACE glow
	#define GLOW_NAMESPACE_BEGIN namespace glow {
	#define GLOW_NAMESPACE_END }
	#define GLOW_NAMESPACE_USING using namespace glow;
#endif

#ifdef GLOW_COMPAT_BADFORSCOPING
	#define for if(0){}else for
#endif

#define GLOW_TOOLKIT 3
#define GLOW_VERSION (0.95)


GLOW_NAMESPACE_BEGIN
GLOW_NAMESPACE_END


#endif

