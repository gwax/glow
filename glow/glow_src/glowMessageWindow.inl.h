/*
===============================================================================

	FILE:  glowMessageWindow.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Message window for GLOW
	
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

#ifndef GLOW_PUSHBUTTONWIDGET__H
	#include "glowPushButtonWidget.h"
#endif

GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowMessageWindow
===============================================================================
*/

inline GlowMessageWindow::GlowMessageWindow()
{
}


inline GlowMessageWindow::GlowMessageWindow(
	const GlowMessageWindowParams& params)
{
	Init(params);
}


inline GlowMessageWindow::GlowMessageWindow(
	const char* windowTitle,
	int x,
	int y,
	const char* text,
	const char* buttonLabels,
	GlowMessageWindowReceiver* receiver)
{
	Init(windowTitle, x, y, text, buttonLabels, receiver);
}


inline TSender<const GlowMessageWindowMessage&>& GlowMessageWindow::Notifier()
{
	return sender_;
}


inline void GlowMessageWindow::SetEnterButton(
	int num)
{
	GLOW_DEBUG(num >= int(buttons_.size()), "enterButton ID is too high");
	enterFilter_->SetPushButton((num < 0) ? 0 : buttons_[num]);
}


inline void GlowMessageWindow::SetEscapeButton(
	int num)
{
	GLOW_DEBUG(num >= int(buttons_.size()), "escapeButton ID is too high");
	escapeFilter_->SetPushButton((num < 0) ? 0 : buttons_[num]);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

