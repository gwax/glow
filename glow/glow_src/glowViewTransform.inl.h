/*
===============================================================================

	FILE:  glowViewTransform.inl.h
	
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

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowTransformData_IdleReceiver
===============================================================================
*/

inline Glow_TransformData_IdleReceiver::Glow_TransformData_IdleReceiver(
	GlowTransformData* transform)
{
	transform_ = transform;
}


/*
===============================================================================
	Inline methods of GlowViewManip_IdleReceiver
===============================================================================
*/

inline Glow_ViewManip_IdleReceiver::Glow_ViewManip_IdleReceiver(
	GlowViewManipulator* manip)
{
	manip_ = manip;
}


/*
===============================================================================
	Inline methods of GlowTransformData
===============================================================================
*/

inline GlowTransformData::GlowTransformData(
	bool persistent) :
receiver_(this)
{
	spinning_ = false;
	scale_ = 1;
	persistent_ = persistent;
}


inline GlowTransformData::GlowTransformData(
	const Vec3f& trans,
	const Quatf& rot,
	GLfloat scale,
	bool persistent) :
translation_(trans),
rotation_(rot),
receiver_(this)
{
	spinning_ = false;
	scale_ = scale;
	persistent_ = persistent;
}


inline void GlowTransformData::Set(
	const Vec3f& trans,
	const Quatf& rot,
	GLfloat scale)
{
	translation_ = trans;
	rotation_ = rot;
	scale_ = scale;
	RefreshAll();
}


inline void GlowTransformData::SetScale(
	GLfloat scale)
{
	scale_ = scale;
	RefreshAll();
}


inline void GlowTransformData::SetTranslation(
	const Vec3f& trans)
{
	translation_ = trans;
	RefreshAll();
}


inline void GlowTransformData::SetRotation(
	const Quatf& rot)
{
	rotation_ = rot;
	RefreshAll();
}


inline void GlowTransformData::SetIdentity()
{
	scale_ = 1.0f;
	rotation_.SetIdentity();
	translation_.SetZero();
	RefreshAll();
}


inline void GlowTransformData::AddScale(
	GLfloat scale)
{
	translation_ *= scale;
	scale_ *= scale;
	RefreshAll();
}


inline void GlowTransformData::AddTranslation(
	const Vec3f& trans)
{
	translation_ += trans;
	RefreshAll();
}


inline void GlowTransformData::AddRotation(
	const Quatf& rot)
{
	translation_ = rot * translation_;
	rotation_ = rot % rotation_;
	rotation_.Normalize();
	RefreshAll();
}


inline bool GlowTransformData::IsSpinning() const
{
	return spinning_;
}


inline void GlowTransformData::StopSpinning()
{
	if (spinning_)
	{
		Glow::UnregisterIdle(&receiver_);
		spinning_ = false;
	}
}


inline void GlowTransformData::StartSpinning(
	const Quatf& spin)
{
	curSpin_ = spin;
	if (!spinning_)
	{
		Glow::RegisterIdle(&receiver_);
		spinning_ = true;
	}
}


inline void GlowTransformData::StepSpin()
{
	AddRotation(curSpin_);
}


inline GLfloat GlowTransformData::GetScale() const
{
	return scale_;
}


inline const Vec3f& GlowTransformData::GetTranslation() const
{
	return translation_;
}


inline const Quatf& GlowTransformData::GetRotation() const
{
	return rotation_;
}


inline Vec3f GlowTransformData::Apply(
	const Vec3f& vec) const
{
	return rotation_*(vec*scale_)+translation_;
}


inline Vec3f GlowTransformData::ApplyInverse(
	const Vec3f& vec) const
{
	return (rotation_.Conjugate()*(vec-translation_))/scale_;
}


inline bool GlowTransformData::IsPersistent() const
{
	return persistent_;
}


inline void GlowTransformData::SetPersistent(
	bool persist)
{
	persistent_ = persist;
}


/*
===============================================================================
	Inline methods of GlowViewTransform
===============================================================================
*/

inline GlowViewTransform::GlowViewTransform()
{
}


inline GlowViewTransform::GlowViewTransform(
	GlowComponent *parent,
	const GlowViewTransformParams& params)
{
	Init(parent, params);
}


inline void GlowViewTransform::Set(
	const Vec3f& trans,
	const Quatf& rot,
	GLfloat scale)
{
	transform_->Set(trans, rot, scale);
}


inline void GlowViewTransform::SetScale(
	GLfloat scale)
{
	transform_->SetScale(scale);
}


inline void GlowViewTransform::SetTranslation(
	const Vec3f& trans)
{
	transform_->SetTranslation(trans);
}


inline void GlowViewTransform::SetRotation(
	const Quatf& rot)
{
	transform_->SetRotation(rot);
}


inline void GlowViewTransform::SetIdentity()
{
	transform_->SetIdentity();
}


inline void GlowViewTransform::AddScale(
	GLfloat scale)
{
	transform_->AddScale(scale);
}


inline void GlowViewTransform::AddTranslation(
	const Vec3f& trans)
{
	transform_->AddTranslation(trans);
}


inline void GlowViewTransform::AddRotation(
	const Quatf& rot)
{
	transform_->AddRotation(rot);
}


inline GLfloat GlowViewTransform::GetScale() const
{
	return transform_->GetScale();
}


inline const Vec3f& GlowViewTransform::GetTranslation() const
{
	return transform_->GetTranslation();
}


inline const Quatf& GlowViewTransform::GetRotation() const
{
	return transform_->GetRotation();
}


inline Vec3f GlowViewTransform::Apply(
	const Vec3f& vec) const
{
	return transform_->Apply(vec);
}


inline Vec3f GlowViewTransform::ApplyInverse(
	const Vec3f& vec) const
{
	return transform_->ApplyInverse(vec);
}


inline void GlowViewTransform::ApplyToGLMatrix() const
{
	transform_->ApplyToGLMatrix();
}


inline void GlowViewTransform::ApplyInverseToGLMatrix() const
{
	transform_->ApplyInverseToGLMatrix();
}


inline void GlowViewTransform::GetMatrix(
	Mat4f& matrix) const
{
	transform_->GetMatrix(matrix);
}


inline void GlowViewTransform::GetGLMatrixf(
	GLfloat* matrix) const
{
	transform_->GetGLMatrixf(matrix);
}


inline bool GlowViewTransform::IsSpinning() const
{
	return transform_->IsSpinning();
}


inline void GlowViewTransform::StopSpinning()
{
	transform_->StopSpinning();
}


inline void GlowViewTransform::StartSpinning(
	const Quatf& spin)
{
	transform_->StartSpinning(spin);
}


inline void GlowViewTransform::StepSpin()
{
	transform_->StepSpin();
}


inline GlowTransformData* GlowViewTransform::TransformData() const
{
	return transform_;
}


inline void GlowViewTransform::RefreshAllConnected() const
{
	transform_->RefreshAll();
}


inline void GlowViewTransform::RawDisconnect_()
{
	transform_->clients_.erase(
		GLOW_STD::find(transform_->clients_.begin(), transform_->clients_.end(), this));
	if (!transform_->persistent_ && transform_->clients_.empty())
	{
		delete transform_;
	}
}


inline void GlowViewTransform::RawConnect_(
	GlowTransformData* transform)
{
	transform_ = transform;
	transform_->clients_.push_back(this);
}


/*
===============================================================================
	Inline methods of GlowViewManipulator
===============================================================================
*/

inline GlowViewManipulator::GlowViewManipulator() :
receiver_(this)
{
}


inline GlowViewManipulator::GlowViewManipulator(
	GlowComponent *parent,
	const GlowViewManipulatorParams& params) :
receiver_(this)
{
	Init(parent, params);
}


inline void GlowViewManipulator::SetColor(
	GlowColor c)
{
	color_ = c;
	ParentWindow()->Refresh();
}


inline bool GlowViewManipulator::IsDragging() const
{
	return dragType_ != idleState;
}


inline bool GlowViewManipulator::IsSpinnable() const
{
	return spinnable_;
}


inline GlowViewManipulator::State GlowViewManipulator::GetState() const
{
	return dragType_;
}


inline bool GlowViewManipulator::IsDrawing() const
{
	return draw_;
}


inline void GlowViewManipulator::SetDrawing(
	bool draw)
{
	draw_ = draw;
}


inline float GlowViewManipulator::GetScaleThrottle() const
{
	return scaleThrottle_;
}


inline void GlowViewManipulator::SetScaleThrottle(
	float throttle)
{
	scaleThrottle_ = throttle;
}


inline float GlowViewManipulator::GetTranslationThrottle() const
{
	return transThrottle_;
}


inline void GlowViewManipulator::SetTranslationThrottle(
	float throttle)
{
	transThrottle_ = throttle;
}


inline float GlowViewManipulator::GetRotationThrottle() const
{
	return rotThrottle_ * 2.0f;
}


inline void GlowViewManipulator::SetRotationThrottle(
	float throttle)
{
	rotThrottle_ = throttle * 0.5f;
}


inline GLOW_STD::vector<Vec3f>& GlowViewManipulator::AxisConstraints()
{
	return axisConstraints_;
}


inline const GLOW_STD::vector<Vec3f>& GlowViewManipulator::AxisConstraints() const
{
	return axisConstraints_;
}


inline void GlowViewManipulator::SetAxisConstraintsActive(
	bool active)
{
	axisConstraintsActive_ = active;
}


inline bool GlowViewManipulator::IsAxisConstraintsActive() const
{
	return axisConstraintsActive_;
}


inline void GlowViewManipulator::SimDrag_()
{
	InDrag(xCur_, yCur_);
}



/*
===============================================================================
*/

GLOW_NAMESPACE_END

