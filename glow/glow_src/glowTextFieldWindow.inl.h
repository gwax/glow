/*
===============================================================================

	FILE:  glowTextFieldWindow.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Text field window for GLOW
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowTextFieldWindow
===============================================================================
*/

inline GlowTextFieldWindow::GlowTextFieldWindow()
{
}


inline GlowTextFieldWindow::GlowTextFieldWindow(
	const GlowTextFieldWindowParams& params)
{
	Init(params);
}


inline GlowTextFieldWindow::GlowTextFieldWindow(
	const char* windowTitle,
	int x,
	int y,
	const char* labelText,
	int fieldWidth,
	const char* fieldText,
	const char* buttonLabels,
	GlowTextFieldWindowReceiver* receiver)
{
	Init(windowTitle, x, y, labelText, fieldWidth, fieldText,
		buttonLabels, receiver);
}


inline TSender<const GlowTextFieldWindowMessage&>& GlowTextFieldWindow::Notifier()
{
	return _sender;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

