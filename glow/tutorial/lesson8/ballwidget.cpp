/*
===============================================================================

	FILE:  ballwidget.cpp
	
	PROJECT:
	
		GLOW widgets for glutmech demo
	
	CONTENTS:
	
		Ball widget
	
	PROGRAMMERS:
	
		Daniel Azuma <dazuma@kagi.com>
		John Nagle <nagle@animats.com> 
	
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
	
		The GLOW Toolkit tutorial -- version 1.0.3  (10 September 2000)
	
	CHANGES:
	
		Original release (DA)
		2001-09-25 Converting to texture-mapped trackball. (JN)

===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#include <algorithm>

using namespace std;

#include "glowViewTransform.h"

GLOW_NAMESPACE_USING

#include "ballwidget.h"
//
//	class DrawCircle  --  draw a filled circle
//
class DrawCircle {
private:
	float radius_;
	GLuint displayList_; 
private:
	void Init();
public:
	DrawCircle(float radius):
		radius_(radius),displayList_(0) {}
	void Draw();
};
//
//	Init  --  called to build display list.
//
void DrawCircle::Init()
{	if (displayList_ != 0) return;		// already built	
	displayList_ = glGenLists(1);		// get a new list
	if (displayList_ == 0) throw("OpenGL/Glow: can't allocate display list.");
	glNewList(displayList_,GL_COMPILE);	// circle display list
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_POLYGON);
	const int sides = 32;				// number of sides for circle
	for (int i=0; i<sides; i++)
	{	float angle = i*((Math::pi*2.0)/sides);	// angle of circle
		float x = sin(angle)*radius_;	// circle of indicated radius
		float y = cos(angle)*radius_;
		glVertex2f(x,y);
	}
	glEnd();
	glEndList();						// end the list
}
//
//	Draw  --  draws a circle
//
void DrawCircle::Draw()
{	Init();								// set up as needed
	glCallList(displayList_);			// draw the circle
}
static DrawCircle unitCircle(1.0);		// set up to draw circle
static DrawCircle innerCircle(0.95);	// smaller, inner circle
//	
//	class CheckerboardTexture  --  a texture object of a checkerboard
//
//	This object cannot be initialized statically, because OpenGL isn't ready
//	at static construction time.
//
class CheckerboardTexture {
private:
	GLuint _textureID;					// texture ID if any
	int _darkColor;						// color of lines
	int _lightColor;					// color of background
private:
	Init();								// called on first GetID to initialize
public:
	CheckerboardTexture(int dark, int light):
	  _darkColor(dark),_lightColor(light),_textureID(0) {}
	GLuint GetID()
	{	if (_textureID == 0) Init();	// initialize if needed
		return(_textureID);
	}
};
//
//	Init  --  initial setup of a checkerboard texture.
//
//	Returns: OpenGL texture "name" (an integer ID)
//	Called at first GetID; can't be called until OpenGL is running.
//
//	Modelled after code in "OpenGL Programming Guide" and GLUI.
//
CheckerboardTexture::Init()
{	//	Build checkerboard image procedurally.
	const int kCheckerboardSize = 64;		// typical 64x64 texture
	unsigned char texture_image[kCheckerboardSize][kCheckerboardSize][3];
	for(int i=0; i<kCheckerboardSize; i++ ) 
		for(int j=0; j<kCheckerboardSize; j++ ) {
		{	bool isdark = ((i&0x8)==0) != ((j&0x8)==0);  
			char texbyte = isdark ? _darkColor : _lightColor;	// pick texture byte
			texture_image[i][j][0] = texbyte;		// all colors same, thus grey
			texture_image[i][j][1] = texbyte;
			texture_image[i][j][2] = texbyte;
		}    
	}
	//	Usual OpenGL ritual to setup a texture.
	GLuint textureNames[1] = { 0 };					// we need one texture name
	glPushAttrib(GL_ALL_ATTRIB_BITS);				// save OpenGL state
	glGenTextures(1,textureNames);					// get one
	glBindTexture(GL_TEXTURE_2D,textureNames[0]);	// bind it to next texture
	glColor3f( 1.0, 1.0, 1.0 );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glEnable( GL_TEXTURE_2D);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, kCheckerboardSize, 
		kCheckerboardSize, 0, GL_RGB, GL_UNSIGNED_BYTE,
		texture_image );
	glPopAttrib();									// restore OpenGL state
	_textureID = textureNames[0];					// set name of new texture
	if (_textureID == 0) throw("OpenGL/GLOW: unable to create texture.");

}
//	A checkerboard texture to wrap around the arcball
static CheckerboardTexture darkCheckerboard(110,220);
/*
===============================================================================
	Component that draws a solid sphere with a checkerboard texture.
===============================================================================
*/

class DrawTexturedSphere :
	public GlowComponent
{
	private:
		GLUquadric* _texturedSphere;			// ID of sphere object
	public:
	
		DrawTexturedSphere(
			GlowComponent* parent) :
		GlowComponent(parent),_texturedSphere(0) {}
		virtual ~DrawTexturedSphere()			// destructor
		{	if (_texturedSphere) gluDeleteQuadric(_texturedSphere); 
		}
	
	protected:
		virtual void OnEndPaint();
		virtual void Init();
};
//
//	Init  -- initialization, called on first draw
//
void DrawTexturedSphere::Init()
{
	if (_texturedSphere != 0) return;	// already initialized
	_texturedSphere = gluNewQuadric();
	if (!_texturedSphere) throw("OpenGL/GLOW: unable to create sphere.");
	gluQuadricDrawStyle(_texturedSphere, GLU_FILL);
    gluQuadricNormals(_texturedSphere, GLU_SMOOTH);
    gluQuadricTexture(_texturedSphere, true );
}

void DrawTexturedSphere::OnEndPaint()
{	const float radius = 0.9;				// sphere fills 0.9 of unit square
	Init();									// build sphere object if needed
	::glPushAttrib(GL_ALL_ATTRIB_BITS);		// save state
	::glEnable(GL_TEXTURE_2D);
	::glEnable(GL_CULL_FACE);
	::glDisable(GL_DEPTH_TEST);
	::glBindTexture(GL_TEXTURE_2D,darkCheckerboard.GetID());	// use specified texture
	::glColor3ub(255, 255, 255);			// Color gets modulated by texture
    ::gluSphere(_texturedSphere, radius, 16, 16);
	::glPopAttrib();
}


/*
===============================================================================
	Methods of BallWidget
===============================================================================
*/

BallWidget::BallWidget(
	GlowWidget* parent,
	int x,
	int y,
	int width)
{
	GLOW_DEBUGSCOPE("BallWidget::BallWidget");
	
	// Initialize widget
	GlowWidgetParams params;
	params.x = x;
	params.y = y;
	params.width = params.height = width;
	GlowWidget::Init(parent->Root(), parent, params);
	
	// Make view manipulator and drawing component
	manip_ = new GlowViewManipulator(this, GlowViewManipulatorParams::defaults);
	new DrawTexturedSphere(manip_);
	
	// Register to receive mouse events
	RegisterMouseEvents();
}


// Override this method to draw the widget
void BallWidget::OnWidgetPaint()
{	const float trackballBackgroundGrey = 0.9;	// outer ring around trackball
	const float trackballInnerGrey = 0.3;	// inner ring around trackball
		//	Start by drawing a circle around the control.
	::glPushAttrib(GL_ALL_ATTRIB_BITS);		// save state
	::glDisable(GL_DEPTH_TEST);
	::glColor3f(trackballBackgroundGrey,trackballBackgroundGrey,trackballBackgroundGrey);						
	unitCircle.Draw();						// circle
	::glColor3f(trackballInnerGrey,trackballInnerGrey,trackballInnerGrey);					
	innerCircle.Draw();						// inner circle
	::glPopAttrib();

}


// Handle mouse down in widget events
void BallWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnWidgetMouseDown");
	
	if (!manip_->IsDragging())
	{
		GLfloat xn;
		GLfloat yn;
		NormalizeCoordinates(x, y, xn, yn);
		manip_->BeginRotationDrag(xn, yn);
		button_ = button;
		modifiers_ = modifiers;
	}
}


// Handle mouse up in widget events
void BallWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnWidgetMouseUp");
	
	if (manip_->IsDragging())
	{
		GLfloat xn;
		GLfloat yn;
		NormalizeCoordinates(x, y, xn, yn);
		manip_->EndDrag(xn, yn);
		OnReleased(button_, modifiers_);
	}
}


// Handle mouse drag in widget events
void BallWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("BallWidget::OnWidgetMouseDrag");
	
	if (manip_->IsDragging())
	{
		GLfloat xn;
		GLfloat yn;
		NormalizeCoordinates(x, y, xn, yn);
		manip_->InDrag(xn, yn);
		OnDragged(button_, modifiers_);
	}
}


// Default method for handling trackball dragged events
void BallWidget::OnDragged(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnDragged");
	
	// Send event message to receivers
	BallMessage message;
	message.widget = this;
	message.rotation = manip_->GetRotation();
	message.released = false;
	message.mouseButton = button;
	message.modifiers = modifiers;
	sender_.Send(message);
}


// Default method for handling trackball released events
void BallWidget::OnReleased(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("BallWidget::OnReleased");
	
	// Send event message to receivers
	BallMessage message;
	message.widget = this;
	message.rotation = manip_->GetRotation();
	message.released = true;
	message.mouseButton = button;
	message.modifiers = modifiers;
	sender_.Send(message);
}


// Automatically resize widget according to constraints
GlowWidget::AutoPackError BallWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("BallWidget::OnAutoPack");
	
	int hnew;
	int vnew;
	
	// noReshape is kind of like forcedSize for our purposes. We'll
	// fold the two options into one to make our life easier.
	if (hOption == noReshape)
	{
		hOption = forcedSize;
		hSize = Width();
	}
	if (vOption == noReshape)
	{
		vOption = forcedSize;
		vSize = Height();
	}
	
	// Handle forcedSize first
	// If forcing different sizes, fail because we want the widget square
	if (hOption == forcedSize && vOption == forcedSize && hSize != vSize)
	{
		return hAutoPackError;
	}
	// Set to the forced size, if it's not too small
	if (hOption == forcedSize)
	{
		if (hSize < 20)
		{
			return hAutoPackError;
		}
		hnew = hSize;
	}
	if (vOption == forcedSize)
	{
		if (vSize < 20)
		{
			return vAutoPackError;
		}
		vnew = vSize;
	}
	
	// Handle preferredSize and expandPreferredSize
	if (hOption == preferredSize || hOption == expandPreferredSize)
	{
		// Make sure the constraint isn't too small
		if (hSize != unspecifiedSize && hSize < 20)
		{
			return hAutoPackError;
		}
		// Adjust constraint if vertical size is already forced
		if (vOption == forcedSize)
		{
			// Do vertical forced size and horizontal constraint conflict?
			if (hSize != unspecifiedSize && hSize < vnew)
			{
				return hAutoPackError;
			}
			hnew = vnew;
		}
		else
		{
			// If using expandPreferredSize or constraint is smaller than 100
			// then set the size to the constraint, otherwise set it to 100.
			if (hOption == expandPreferredSize ||
				(hSize != unspecifiedSize && hSize <= 100))
			{
				hnew = hSize;
			}
			else
			{
				hnew = 100;
			}
		}
	}
	if (vOption == preferredSize || vOption == expandPreferredSize)
	{
		if (vSize != unspecifiedSize && vSize < 20)
		{
			return vAutoPackError;
		}
		if (hOption == forcedSize)
		{
			if (vSize != unspecifiedSize && vSize < hnew)
			{
				return vAutoPackError;
			}
			vnew = hnew;
		}
		else
		{
			if (vOption == expandPreferredSize ||
				(vSize != unspecifiedSize && vSize <= 100))
			{
				vnew = vSize;
			}
			else
			{
				vnew = 100;
			}
		}
	}
	
	// Handle one more case: neither vertical nor horizontal were forced,
	// but they evaluated to different sizes. To make it square, pick the
	// smaller size
	if (hnew != vnew)
	{
		hnew = vnew = min(hnew, vnew);
	}
	
	// Resize widget
	Reshape(hnew, vnew);
	
	// Done!
	return noAutoPackError;
}


/*
===============================================================================
*/

