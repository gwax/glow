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
	
		The GLOW Toolkit -- version 0.9.6  (10 April 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
		10 April 2000 -- DA -- Version 0.9.6 update

===============================================================================
*/


#ifndef GLOW_HEADER__H
#define GLOW_HEADER__H


/*
===============================================================================
	Macros
===============================================================================
*/

// Macros for std namespace
#ifdef GLOW_COMPAT_NOSTDNAMESPACE
	#define GLOW_STD
	#define GLOW_CSTD
#else
	#define GLOW_STD std
	#ifdef GLOW_COMPAT_CLIBNOSTDNAMESPACE
		#define GLOW_CSTD
	#else
		#define GLOW_CSTD std
	#endif
#endif

// This is probably unnecessary now...
#ifdef GLOW_COMPAT_INTERNALUSINGSTD
	#define GLOW_INTERNAL_USINGSTD namespace std {} using namespace std;
#else
	#define GLOW_INTERNAL_USINGSTD
#endif

// Macros for glow namespace
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

// Compatibility for old-style for loop scoping (msvc)
#ifdef GLOW_COMPAT_BADFORSCOPING
	#define for if(0){}else for
#endif

// Compatibility for missing min and max templates (msvc)
#if defined(GLOW_COMPAT_NOSTDMINMAX) && !defined(VIRTUOSO_INTERNAL_MINMAXDEFINED)
	#ifndef GLOW_COMPAT_NOSTDNAMESPACE
		namespace std {
	#endif
		template <class T>
			inline const T& min(const T& a, const T& b)
				{ return b < a ? b : a; }
		template <class T>
			inline const T& max(const T& a, const T& b)
				{ return a < b ? b : a; }
	#ifndef GLOW_COMPAT_NOSTDNAMESPACE
		}
	#endif
	#define VIRTUOSO_INTERNAL_MINMAXDEFINED
#endif

// Integral release number (0.9.5 == 3)
#define GLOW_TOOLKIT 5

// Fractional version number
#define GLOW_VERSION 0.97

// Internal utility
#define GLOW_INTERNAL_SETUPENUMBITFIELD(T) \
	inline T operator|(T op1, T op2) {return T(int(op1)|int(op2));} \
	inline T operator&(T op1, T op2) {return T(int(op1)&int(op2));} \
	inline T& operator|=(T& op1, T op2) {op1=T(int(op1)|int(op2)); return op1;} \
	inline T& operator&=(T& op1, T op2) {op1=T(int(op1)&int(op2)); return op1;}

// Establish namespaces so using directive doesn't barf
GLOW_NAMESPACE_BEGIN
GLOW_NAMESPACE_END
namespace std {}


#endif

