/*
===============================================================================

	FILE:  glowUtilities.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Extensions and utilities for GLOW
	
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

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods for GlowWindowSetting
===============================================================================
*/

inline GlowWindowSetting::GlowWindowSetting()
{
	saveWindowNum_ = 0;
}


inline GlowWindowSetting::GlowWindowSetting(
	int windowNum)
{
	saveWindowNum_ = ::glutGetWindow();
	::glutSetWindow(windowNum);
}


inline GlowWindowSetting::~GlowWindowSetting()
{
	if (saveWindowNum_ != 0)
	{
		::glutSetWindow(saveWindowNum_);
	}
}


inline void GlowWindowSetting::Set(
	int windowNum)
{
	GLOW_DEBUG(saveWindowNum_ != 0, "Attempt to re-enter GlowWindowSetting");
	saveWindowNum_ = ::glutGetWindow();
	::glutSetWindow(windowNum);
}


inline void GlowWindowSetting::Reset()
{
	GLOW_DEBUG(saveWindowNum_ == 0, "Attempt to re-exit GlowWindowSetting");
	::glutSetWindow(saveWindowNum_);
	saveWindowNum_ = 0;
}


/*
===============================================================================
	Inline methods for GlowDeferredTask
===============================================================================
*/

inline GlowDeferredTask::GlowDeferredTask()
{
	curTimerID_ = 0;
}


inline void GlowDeferredTask::Schedule(
	unsigned int msecs)
{
	if (curTimerID_ != 0)
	{
		Glow::UnregisterTimer(curTimerID_);
	}
	curTimerID_ = Glow::RegisterTimer(msecs, this);
}


inline void GlowDeferredTask::Unschedule()
{
	if (curTimerID_ != 0)
	{
		Glow::UnregisterTimer(curTimerID_);
		curTimerID_ = 0;
	}
}


inline bool GlowDeferredTask::IsScheduled()
{
	return curTimerID_ != 0;
}


/*
===============================================================================
	Inline methods of GlowFixedSizeWindow
===============================================================================
*/

inline GlowFixedSizeWindow::GlowFixedSizeWindow()
{
}


inline GlowFixedSizeWindow::GlowFixedSizeWindow(
	const GlowWindowParams& params)
{
	canonicalWidth_ = params.width;
	canonicalHeight_ = params.height;
	GlowWindow::Init(params);
}


inline GlowFixedSizeWindow::GlowFixedSizeWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	canonicalWidth_ = width;
	canonicalHeight_ = height;
	GlowWindow::Init(title, x, y, width, height, mode, eventMask);
}


#ifndef GLOW_OPTION_STRICTGLUT3

inline GlowFixedSizeWindow::GlowFixedSizeWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	const char* modeString,
	Glow::EventMask eventMask)
{
	canonicalWidth_ = width;
	canonicalHeight_ = height;
	GlowWindow::Init(title, x, y, width, height, modeString, eventMask);
}

#endif


inline void GlowFixedSizeWindow::Init(
	const GlowWindowParams& params)
{
	canonicalWidth_ = params.width;
	canonicalHeight_ = params.height;
	GlowWindow::Init(params);
}


inline void GlowFixedSizeWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	canonicalWidth_ = width;
	canonicalHeight_ = height;
	GlowWindow::Init(title, x, y, width, height, mode, eventMask);
}


#ifndef GLOW_OPTION_STRICTGLUT3

inline void GlowFixedSizeWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	const char* modeString,
	Glow::EventMask eventMask)
{
	canonicalWidth_ = width;
	canonicalHeight_ = height;
	GlowWindow::Init(title, x, y, width, height, modeString, eventMask);
}

#endif


inline void GlowFixedSizeWindow::ForceReshape(
	int width,
	int height)
{
	canonicalWidth_ = width;
	canonicalHeight_ = height;
	Reshape(width, height);
}


inline int GlowFixedSizeWindow::FixedWidth() const
{
	return canonicalWidth_;
}


inline int GlowFixedSizeWindow::FixedHeight() const
{
	return canonicalHeight_;
}


/*
===============================================================================
	Inline methods of GlowReceiverTrackerComponent
===============================================================================
*/

inline GlowReceiverTrackerComponent::GlowReceiverTrackerComponent(
	GlowComponent* parent) :
GlowComponent(parent)
{
	SetDeletingOptions(referenceCountDelete);
}


inline GlowReceiverTrackerComponent::GlowReceiverTrackerComponent()
{
	SetDeletingOptions(referenceCountDelete);
}


inline void GlowReceiverTrackerComponent::Init(
	GlowComponent* parent)
{
	GlowComponent::Init(parent);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

