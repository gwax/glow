/*
===============================================================================

	FILE:  mandelwind.cpp
	
	PROJECT:
	
		GLOW-based mandelbrot viewer
	
	CONTENTS:
	
		Viewer window
	
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
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit tutorial -- version 1.1.2dev  (7 August 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*
===============================================================================
	Headers
===============================================================================
*/

#include "mandelwind.h"
#include "mandeldata.h"


/*
===============================================================================
	Constructor for MandelWind
===============================================================================
*/

MandelWind::MandelWind(
	MandelData* data) :
// We first construct the window itself.
GlowWindow("Mandelglow", GlowWindow::autoPosition, GlowWindow::autoPosition,
	data->Width(), data->Height(), Glow::rgbBuffer | Glow::doubleBuffer,
	Glow::noEvents)
{
	// Get data
	data_ = data;
	data_->Recalc();
	
	// Make image
	image_ = new unsigned char[data_->Width()*data_->Height()*4];
	const int* rawimage = data_->Data();
	int mx = data_->Width()*data_->Height();
	for (int i=0; i<mx; ++i)
	{
		if (rawimage[i] == 0)
		{
			// Black color
			image_[i*4] = 0;
			image_[i*4+1] = 0;
			image_[i*4+2] = 0;
			image_[i*4+3] = 0;
		}
		else
		{
			// Red color
			image_[i*4] = 255;
			image_[i*4+1] = 0;
			image_[i*4+2] = 0;
			image_[i*4+3] = 0;
		}
	}
}


/*
===============================================================================
	Destructor for MandelWind
===============================================================================
*/

MandelWind::~MandelWind()
{
	delete data_;
	delete image_;
}


/*
===============================================================================
	Redraw method. Called when the window needs to be redrawn
===============================================================================
*/

void MandelWind::OnEndPaint()
{
	// Put OpenGL calls here.
	::glRasterPos2f(-1.0f, -1.0f);
	::glDrawPixels(data_->Width(), data_->Height(), GL_RGBA, GL_UNSIGNED_BYTE, image_);
}

