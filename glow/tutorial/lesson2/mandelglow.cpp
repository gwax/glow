/*
===============================================================================

	FILE:  mandelglow.cpp
	
	PROJECT:
	
		GLOW-based mandelbrot viewer
	
	CONTENTS:
	
		Main program
	
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
		License along with this library; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit tutorial -- version 0.9.9  (14 June 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
		10 April 2000 -- DA -- Version 0.9.6 update
		1 May 2000 -- DA -- Version 0.9.7 update
		23 May 2000 -- DA -- Version 0.9.8 update
		14 June 2000 -- DA -- Version 0.9.9 update

===============================================================================
*/


/*
===============================================================================
	Headers
===============================================================================
*/

#include <iostream>

using namespace std;

#include "glow.h"

GLOW_NAMESPACE_USING

#include "mandeldata.h"
#include "mandelwind.h"


/*
===============================================================================
	Entry point
===============================================================================
*/

int main(
	int argc,
	char **argv)
{
	cout << "Mandelglow (lesson 2)" << endl;
	
	// Initialize GLOW runtime
	Glow::Init(argc, argv);
	
	// Create one window to display one data object
	new MandelWind(new MandelData);
	
	// Enter event loop
	Glow::MainLoop();
	
	// Control actually never gets here, but we'll put this here so
	// compilers don't complain.
	return 0;
}

