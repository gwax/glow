/*
===============================================================================

	FILE:  glowViewTransform.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Arcball-based manipulator for GLOW
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <cmath>

using namespace std;

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif
#ifndef GLOW_VIEWTRANSFORM__H
	#include "glowViewTransform.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowViewTransformParams GlowViewTransformParams::defaults(true);

GlowViewTransformParams::GlowViewTransformParams()
{
	*this = defaults;
}

GlowViewTransformParams::GlowViewTransformParams(bool)
{
	connectTo = 0;
	initialScale = 1.0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowViewManipulatorParams GlowViewManipulatorParams::defaults(true);

GlowViewManipulatorParams::GlowViewManipulatorParams()
{
	*this = defaults;
}

GlowViewManipulatorParams::GlowViewManipulatorParams(bool) :
color(0.5, 1.0, 1.0)
{
	connectTo = 0;
	draw = false;
	spinnable = false;
	axisConstraintsActive = false;
	translationThrottle = 1.0;
	rotationThrottle = 1.0;
	scaleThrottle = 1.0;
	initialScale = 1.0;
	spinDataLength = 100;
}


/*
===============================================================================
	Methods of Glow_TransformData_IdleReceiver
===============================================================================
*/

void Glow_TransformData_IdleReceiver::OnMessage(
	const GlowIdleMessage& message)
{
	transform_->StepSpin();
}


/*
===============================================================================
	Methods of Glow_ViewManip_IdleReceiver
===============================================================================
*/

void Glow_ViewManip_IdleReceiver::OnMessage(
	const GlowIdleMessage& message)
{
	manip_->SimDrag_();
}


/*
===============================================================================
	Methods of GlowTransformData
===============================================================================
*/

void GlowTransformData::ApplyToGLMatrix() const
{
	GLOW_DEBUGSCOPE("GlowTransformData::ApplyToGLMatrix");
	
	// Base transform
	Vec3f axis;
	GLfloat angle;
	rotation_.GetRotation(axis, angle);
	glTranslatef(translation_.GetX(), translation_.GetY(), translation_.GetZ());
	glRotatef(angle*Math::radiansToDegrees, axis.GetX(), axis.GetY(), axis.GetZ());
	glScalef(scale_, scale_, scale_);
}


void GlowTransformData::ApplyInverseToGLMatrix() const
{
	GLOW_DEBUGSCOPE("GlowTransformData::ApplyInverseToGLMatrix");
	
	// Base transform
	Vec3f axis;
	GLfloat angle;
	rotation_.GetRotation(axis, angle);
	GLfloat scaleinv = 1.0f/scale_;
	glScalef(scaleinv, scaleinv, scaleinv);
	glRotatef(-angle*Math::radiansToDegrees, axis.GetX(), axis.GetY(), axis.GetZ());
	glTranslatef(-translation_.GetX(), -translation_.GetY(), -translation_.GetZ());
}


void GlowTransformData::GetMatrix(
	Mat4f& matrix) const
{
	GLOW_DEBUGSCOPE("GlowTransformData::GetMatrix");
	
	// Base transform
	Mat4f matrix2;
	Vec3f axis;
	GLfloat angle;
	rotation_.GetRotation(axis, angle);
	matrix2.SetTranslation(translation_);
	matrix *= matrix2;
	matrix2.SetRotation(axis, angle);
	matrix *= matrix2;
	matrix2.SetScale(scale_);
	matrix *= matrix2;
}


void GlowTransformData::GetGLMatrixf(
	GLfloat* matrix) const
{
	Mat4f matrix2;
	GetMatrix(matrix2);
	matrix2.GetGLMatrix(matrix);
}


void GlowTransformData::RefreshAll()
{
	for (GLOW_STD::vector<GlowViewTransform*>::iterator iter = clients_.begin(),
		enditer = clients_.end(); iter != enditer; ++iter)
	{
		(*iter)->ParentWindow()->Refresh();
	}
}


GlowTransformData::~GlowTransformData()
{
	for (GLOW_STD::vector<GlowViewTransform*>::iterator iter = clients_.begin(),
		enditer = clients_.end(); iter != enditer; ++iter)
	{
		(*iter)->transform_ = new GlowTransformData(
			translation_, rotation_, scale_, false);
	}
}


/*
===============================================================================
	Methods of GlowViewTransform
===============================================================================
*/

void GlowViewTransform::Init(
	GlowComponent *parent,
	const GlowViewTransformParams& params)
{
	GLOW_DEBUGSCOPE("GlowViewTransform::Init");
	GlowComponent::Init(parent);
	
	RawConnect_((params.connectTo != 0) ? params.connectTo :
		new GlowTransformData(params.initialTranslation,
			params.initialRotation, params.initialScale, false));
}


GlowViewTransform::~GlowViewTransform()
{
	RawDisconnect_();
}


bool GlowViewTransform::OnBeginPaint()
{
	::glMatrixMode(GL_MODELVIEW);
	::glPushMatrix();
	ApplyToGLMatrix();
	return true;
}


void GlowViewTransform::OnEndPaint()
{
	::glMatrixMode(GL_MODELVIEW);
	::glPopMatrix();
}


void GlowViewTransform::ConnectTo(
	GlowTransformData* transform)
{
	GLOW_DEBUGSCOPE("GlowViewTransform::ConnectTo");
	GLOW_ASSERT(transform);
	if (transform != transform_)
	{
		RawDisconnect_();
		RawConnect_(transform);
	}
}


void GlowViewTransform::ConnectTo(
	GlowViewTransform* transform)
{
	GLOW_DEBUGSCOPE("GlowViewTransform::ConnectTo");
	GLOW_ASSERT(transform);
	if (transform->TransformData() != transform_)
	{
		RawDisconnect_();
		RawConnect_(transform->TransformData());
	}
}


void GlowViewTransform::Disconnect()
{
	GLOW_DEBUGSCOPE("GlowViewTransform::Disconnect");
	GlowTransformData* newTransform = new GlowTransformData(
		transform_->GetTranslation(), transform_->GetRotation(),
		transform_->GetScale(), false);
	RawDisconnect_();
	RawConnect_(newTransform);
}


/*
===============================================================================
	Methods of GlowViewManipulator
===============================================================================
*/

void GlowViewManipulator::Init(
	GlowComponent *parent,
	const GlowViewManipulatorParams& params)
{
	GLOW_DEBUGSCOPE("GlowViewManip::Init");
	GlowComponent::Init(parent);
	
	color_ = params.color;
	draw_ = params.draw;
	axisConstraintsActive_ = params.axisConstraintsActive;
	axisConstraints_ = params.axisConstraints;
	transThrottle_ = params.translationThrottle;
	rotThrottle_ = params.rotationThrottle * 0.5f;
	scaleThrottle_ = params.scaleThrottle * 0.5f;
	dragType_ = idleState;
	spinnable_ = params.spinnable;
	spinDataLength_ = params.spinDataLength;
	spinStart_ = false;
	
	RawConnect_((params.connectTo != 0) ? params.connectTo :
		new GlowTransformData(params.initialTranslation,
			params.initialRotation, params.initialScale, false));
}


void GlowViewManipulator::OnEndPaint()
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::OnEndPaint");
	
	// Finish up
	if (dragType_ != idleState && draw_)
	{
		::glMatrixMode(GL_MODELVIEW);
		::glLoadIdentity();
		
		::glMatrixMode(GL_PROJECTION);
		::glPushMatrix();
		::glLoadIdentity();
		
		GLboolean lightingSave;
		GLint depthFuncSave;
		::glGetBooleanv(GL_LIGHTING, &lightingSave);
		::glGetIntegerv(GL_DEPTH_FUNC, &depthFuncSave);
		
		::glDisable(GL_LIGHTING);
		::glDepthFunc(GL_ALWAYS);
		
		color_.Apply();
		::glBegin(GL_LINES);
		DrawArc_(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
		DrawArc_(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(-1.0f, 0.0f, 0.0f));
		DrawArc_(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f));
		DrawArc_(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f));
		if (dragType_ == scalingState)
		{
			::glVertex2f(xStart_, yStart_);
			::glVertex2f(xStart_, yCur_);
		}
		else if (dragType_ == translatingState)
		{
			::glVertex2f(xStart_, yStart_);
			::glVertex2f(xCur_, yCur_);
		}
		else //if (dragType_ == rotatingState)
		{
			DrawArc_(ballDown_, ballCur_);
		}
		::glEnd();
		
		::glDepthFunc((GLenum)depthFuncSave);
		if (lightingSave == GL_TRUE)
		{
			::glEnable(GL_LIGHTING);
		}
		
		::glPopMatrix();
		::glMatrixMode(GL_MODELVIEW);
	}
	
	// Compute spin data, and start spinning
	if (spinnable_)
	{
		if (dragType_ == rotatingState &&
			(!axisConstraintsActive_ || axisConstraints_.size() < 2))
		{
			// While rotating
			int curTime = glutGet(GLenum(GLUT_ELAPSED_TIME));
			spinData_.push_back(GLOW_STD::pair<Vec3f, int>(ballCur_, curTime));
			while (spinData_.front().second < curTime-spinDataLength_ &&
				spinData_.size() > 3)
			{
				spinData_.pop_front();
			}
		}
		else if (spinStart_ && spinData_.size() > 0)
		{
			spinStart_ = false;
			int curTime = glutGet(GLenum(GLUT_ELAPSED_TIME));
			while (spinData_.front().second < curTime-spinDataLength_ &&
				spinData_.size() > 2)
			{
				spinData_.pop_front();
			}
			if (ballCur_ != spinData_.front().first)
			{
				// Ready to begin spinning
				Quatf curSpin;
				if (spinData_.size() < 2)
				{
					curSpin.SetImaginary(spinData_.front().first % ballCur_);
					curSpin.SetW(spinData_.front().first * ballCur_);
					curSpin.Normalize();
					curSpin.ScaleRotation(rotThrottle_/float(spinData_.size()));
					transform_->StartSpinning(curSpin);
				}
				else
				{
					curSpin.SetImaginary(spinData_.front().first % spinData_.back().first);
					curSpin.SetW(spinData_.front().first * spinData_.back().first);
					curSpin.Normalize();
					curSpin.ScaleRotation(rotThrottle_/float(spinData_.size()-1));
					transform_->StartSpinning(curSpin);
				}
			}
			spinData_.clear();
		}
	}
	::glPopMatrix();
}


void GlowViewManipulator::BeginScaleDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::BeginScaleDrag");
	
	transform_->StopSpinning();
	xStart_ = xn;
	yStart_ = yn;
	xCur_ = xn;
	yCur_ = yn;
	dragType_ = scalingState;
	oldTranslation_ = transform_->GetTranslation();
	oldRotation_ = transform_->GetRotation();
	oldScale_ = transform_->GetScale();
}


void GlowViewManipulator::BeginTranslationDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::BeginTranslationDrag");
	
	transform_->StopSpinning();
	xStart_ = xn;
	yStart_ = yn;
	xCur_ = xn;
	yCur_ = yn;
	dragType_ = translatingState;
	oldTranslation_ = transform_->GetTranslation();
	oldRotation_ = transform_->GetRotation();
	oldScale_ = transform_->GetScale();
}


void GlowViewManipulator::BeginRotationDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::BeginRotationDrag");
	
	transform_->StopSpinning();
	xStart_ = xn;
	yStart_ = yn;
	xCur_ = xn;
	yCur_ = yn;
	dragType_ = rotatingState;
	ballDown_ = ballCur_ = MouseToBall_(xStart_, yStart_);
	oldTranslation_ = transform_->GetTranslation();
	oldRotation_ = transform_->GetRotation();
	oldScale_ = transform_->GetScale();
	if (spinnable_)
	{
		Glow::RegisterIdle(&receiver_);
	}
}


void GlowViewManipulator::InDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::InDrag");
	
	xCur_ = xn;
	yCur_ = yn;
	if (dragType_ == scalingState)
	{
		float curScale = GLOW_CSTD::pow(2.0f, (yCur_-yStart_)*scaleThrottle_);
		transform_->Set(oldTranslation_ * curScale, oldRotation_,
			oldScale_ * curScale);
	}
	else if (dragType_ == translatingState)
	{
		Vec3f curTranslation((xCur_ - xStart_) * transThrottle_,
			(yCur_ - yStart_) * transThrottle_, 0);
		transform_->Set(oldTranslation_ + curTranslation, oldRotation_, oldScale_);
	}
	else if (dragType_ == rotatingState)
	{
		ballDown_ = MouseToBall_(xStart_, yStart_);
		ballCur_ = MouseToBall_(xCur_, yCur_);
		Quatf curRotation;
		if (xCur_ != xStart_ || yCur_ != yStart_)
		{
			if (axisConstraintsActive_ && axisConstraints_.size() != 0)
			{
				GLOW_STD::vector<Vec3f>::iterator best = axisConstraints_.begin();
				float bestValue = 2;
				for (GLOW_STD::vector<Vec3f>::iterator iter = best,
					enditer = axisConstraints_.end(); iter != enditer; ++iter)
				{
					float curValue = GLOW_CSTD::fabs(ballCur_*(*iter));
					if (curValue < bestValue)
					{
						bestValue = curValue;
						best = iter;
					}
				}
				ballDown_ = (ballDown_-(*best)*(ballDown_*(*best))).Normalized();
				ballCur_ = (ballCur_-(*best)*(ballCur_*(*best))).Normalized();
			}
			curRotation.SetImaginary(ballDown_%ballCur_);
			curRotation.SetW(ballDown_*ballCur_);
			curRotation.Normalize();
			curRotation.ScaleRotation(rotThrottle_);
		}
		transform_->Set(curRotation * oldTranslation_,
			curRotation % oldRotation_, oldScale_);
	}
}


void GlowViewManipulator::EndDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::EndDrag");
	
	if (IsDragging())
	{
		InDrag(xn, yn);
		if (dragType_ == rotatingState && spinnable_)
		{
			Glow::UnregisterIdle(&receiver_);
			spinStart_ = true;
		}
		dragType_ = idleState;
	}
}


void GlowViewManipulator::SetSpinnable(
	bool spinnable)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::SetSpinnable");
	if (!spinnable && spinnable_)
	{
		transform_->StopSpinning();
	}
	if (dragType_ == rotatingState)
	{
		if (spinnable && !spinnable_)
		{
			Glow::RegisterIdle(&receiver_);
		}
		else if (!spinnable && spinnable_)
		{
			Glow::UnregisterIdle(&receiver_);
			spinData_.clear();
		}
	}
	spinnable_ = spinnable;
}


Vec3f GlowViewManipulator::MouseToBall_(
	GLfloat xn,
	GLfloat yn)
{
	GLfloat mag = xn*xn+yn*yn;
	if (mag >= 1.0f)
	{
		return Vec3f(xn, yn, 0.0f);
	}
	else
	{
		return Vec3f(xn, yn, GLOW_CSTD::sqrt(1.0f-mag));
	}
}


void GlowViewManipulator::DrawArc_(
	const Vec3f& start,
	const Vec3f& end,
	int level)
{
	if (level <= 0)
	{
		glVertex2fv(start.Array());
		glVertex2fv(end.Array());
	}
	else
	{
		Vec3f mid = (start+end).Normalized();
		DrawArc_(start, mid, level-1);
		DrawArc_(mid, end, level-1);
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

