/*
===============================================================================

	FILE:  glowSenderReceiver.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Messaging system
	
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
	
		The GLOW Toolkit -- version 1.0.0  (29 June 2000)
	
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

#ifndef GLOW_SENDERRECEIVER__H
	#include "glowSenderReceiver.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of Receiver_Base
===============================================================================
*/

Receiver_Base::~Receiver_Base()
{
	for (GLOW_STD::list<Sender_Base*>::iterator iter = senders_.begin(),
		enditer = senders_.end(); iter != enditer; ++iter)
	{
		(*iter)->RemoveReceiver_(this);
	}
}


/*
===============================================================================
	Methods of Sender_Base
===============================================================================
*/

void Sender_Base::UnbindAll()
{
	for (GLOW_STD::list<Receiver_Base*>::iterator iter = receivers_.begin(),
		enditer = receivers_.end(); iter != enditer; ++iter)
	{
		(*iter)->RemoveSender_(this);
	}
	receivers_.erase(receivers_.begin(), receivers_.end());
}


Sender_Base::~Sender_Base()
{
	for (GLOW_STD::list<Receiver_Base*>::iterator iter = receivers_.begin(),
		enditer = receivers_.end(); iter != enditer; ++iter)
	{
		(*iter)->RemoveSender_(this);
	}
}


/*
===============================================================================
	Methods of ReceiverTracker
===============================================================================
*/

ReceiverTracker::~ReceiverTracker()
{
	UnbindAll();
}


void ReceiverTracker::DeleteAllReceivers()
{
	while (!receivers_.empty())
	{
		delete receivers_.front();
	}
}


void ReceiverTracker::Unbind(
	Receiver_Base* receiver)
{
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver);
	GLOW_DEBUG(iter == receivers_.end(),
		"ReceiverTracker not tracking TReceiver");
	receiver->RemoveTracker_(this);
	receivers_.erase(iter);
	if ((receiver->NumTrackers() == 0 && options_ == referenceCountDelete) ||
		options_ == alwaysDelete)
	{
		delete receiver;
	}
}


void ReceiverTracker::UnbindAll()
{
	for (GLOW_STD::list<Receiver_Base*>::iterator iter = receivers_.begin(),
		enditer = receivers_.end(); iter != enditer; ++iter)
	{
		(*iter)->RemoveTracker_(this);
		if (((*iter)->NumTrackers() == 0 && options_ == referenceCountDelete) ||
			options_ == alwaysDelete)
		{
			delete (*iter);
		}
	}
	receivers_.erase(receivers_.begin(), receivers_.end());
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

