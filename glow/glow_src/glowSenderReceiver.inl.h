/*
===============================================================================

	FILE:  glowSenderReceiver.inl.h
	
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

#include <algorithm>

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of Receiver_Base
===============================================================================
*/

inline Receiver_Base::Receiver_Base()
{
	numActualSenders_ = 0;
}


inline unsigned int Receiver_Base::NumSenders() const
{
	return numActualSenders_;
}


inline unsigned int Receiver_Base::NumTrackers() const
{
	return senders_.size() - numActualSenders_;
}


inline void Receiver_Base::AddSender_(
	Sender_Base* sender)
{
	++numActualSenders_;
	senders_.push_back(sender);
}


inline void Receiver_Base::RemoveSender_(
	Sender_Base* sender)
{
	GLOW_STD::list<Sender_Base*>::iterator iter =
		GLOW_STD::find(senders_.begin(), senders_.end(), sender);
	GLOW_DEBUG(iter == senders_.end(),
		"Sender not found in Receiver_Base::RemoveSender_");
	--numActualSenders_;
	senders_.erase(iter);
}


inline void Receiver_Base::AddTracker_(
	Sender_Base* sender)
{
	senders_.push_back(sender);
}


inline void Receiver_Base::RemoveTracker_(
	Sender_Base* sender)
{
	GLOW_STD::list<Sender_Base*>::iterator iter =
		GLOW_STD::find(senders_.begin(), senders_.end(), sender);
	GLOW_DEBUG(iter == senders_.end(),
		"Tracker not found in Receiver_Base::RemoveTracker_");
	senders_.erase(iter);
}


/*
===============================================================================
	Inline methods of Sender_Base
===============================================================================
*/

inline Sender_Base::Sender_Base()
{
}


inline unsigned int Sender_Base::NumReceivers() const
{
	return receivers_.size();
}


inline bool Sender_Base::IsBoundTo(
	Receiver_Base* receiver) const
{
	return GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver)!=receivers_.end();
}


inline void Sender_Base::RemoveReceiver_(
	Receiver_Base* receiver)
{
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver);
	GLOW_DEBUG(iter == receivers_.end(),
		"Receiver not found in Sender_Base::RemoveReceiver_");
	receivers_.erase(iter);
}


/*
===============================================================================
	Inline methods of ReceiverTracker
===============================================================================
*/

inline ReceiverTracker::ReceiverTracker()
{
	options_ = neverDelete;
}


inline void ReceiverTracker::Bind(
	Receiver_Base* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver)!=receivers_.end(),
		"ReceiverTracker already tracking TReceiver");
	receiver->AddTracker_(this);
	receivers_.push_back(receiver);
}


inline void ReceiverTracker::SetDeletingOptions(
	DeletingOptions options)
{
	options_ = options;
}


inline ReceiverTracker::DeletingOptions ReceiverTracker::GetDeletingOptions() const
{
	return options_;
}


/*
===============================================================================
	Inline methods of TReceiver template
===============================================================================
*/

template <class T>
inline TReceiver<T>::TReceiver()
{
}


/*
===============================================================================
	Inline methods of TReceiver void specialization
===============================================================================
*/

#ifdef GLOW_COMPAT_NOTEMPLATESPECIALIZATIONS

inline TReceiver_Void::TReceiver_Void()
{
}

#else

inline TReceiver<void>::TReceiver()
{
}

#endif


/*
===============================================================================
	Inline methods of TSender template
===============================================================================
*/

template <class T>
inline TSender<T>::TSender()
{
}


template <class T>
inline void TSender<T>::Bind(
	TReceiver<T>* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver)!=receivers_.end(),
		"TSender already bound to TReceiver");
	
	receiver->AddSender_(this);
	receivers_.push_back(receiver);
}


template <class T>
inline void TSender<T>::Unbind(
	TReceiver<T>* receiver)
{
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver);
	GLOW_DEBUG(iter == receivers_.end(),
		"TSender not bound to TReceiver");
	receiver->RemoveSender_(this);
	receivers_.erase(iter);
}


template <class T>
inline void TSender<T>::Send(
	T message) const
{
	GLOW_STD::list<Receiver_Base*>::const_iterator iter = receivers_.begin();
	while (iter != receivers_.end())
	{
		TReceiver<T>* cur = static_cast<TReceiver<T>*>(*iter);
		iter++;
		cur->OnMessage(message);
	}
}


template <class T>
inline void TSender<T>::Send(
	TReceiver<T>* receiver,
	T message)
{
	receiver->OnMessage(message);
}


/*
===============================================================================
	Inline methods of TSender void specialization
===============================================================================
*/

#ifdef GLOW_COMPAT_NOTEMPLATESPECIALIZATIONS

inline TSender_Void::TSender_Void()
{
}


inline void TSender_Void::Bind(
	TReceiver_Void* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver)!=receivers_.end(),
		"TSender already bound to TReceiver");
	
	receiver->AddSender_(this);
	receivers_.push_back(receiver);
}


inline void TSender_Void::Unbind(
	TReceiver_Void* receiver)
{
	receiver->RemoveSender_(this);
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver);
	GLOW_DEBUG(iter == receivers_.end(),
		"TSender not bound to TReceiver");
	receivers_.erase(iter);
}


inline void TSender_Void::Send() const
{
	GLOW_STD::list<Receiver_Base*>::const_iterator iter = receivers_.begin();
	while (iter != receivers_.end())
	{
		TReceiver_Void* cur = static_cast<TReceiver_Void*>(*iter);
		iter++;
		cur->OnMessage();
	}
}


inline void TSender_Void::Send(
	TReceiver_Void* receiver)
{
	receiver->OnMessage();
}

#else

inline TSender<void>::TSender()
{
}


inline void TSender<void>::Bind(
	TReceiver<void>* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver)!=receivers_.end(),
		"TSender already bound to TReceiver");
	
	receiver->AddSender_(this);
	receivers_.push_back(receiver);
}


inline void TSender<void>::Unbind(
	TReceiver<void>* receiver)
{
	receiver->RemoveSender_(this);
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(receivers_.begin(), receivers_.end(), receiver);
	GLOW_DEBUG(iter == receivers_.end(),
		"TSender not bound to TReceiver");
	receivers_.erase(iter);
}


inline void TSender<void>::Send() const
{
	GLOW_STD::list<Receiver_Base*>::const_iterator iter = receivers_.begin();
	while (iter != receivers_.end())
	{
		TReceiver<void>* cur = static_cast<TReceiver<void>*>(*iter);
		iter++;
		cur->OnMessage();
	}
}


inline void TSender<void>::Send(
	TReceiver<void>* receiver)
{
	receiver->OnMessage();
}

#endif


/*
===============================================================================
*/

GLOW_NAMESPACE_END

