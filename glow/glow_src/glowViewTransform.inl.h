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
	
		The GLOW Toolkit -- version 0.9.7  (1 May 2000)
	
	CHANGE HISTORY:
	
		27 March 2000 -- DA -- Initial CVS checkin
		10 April 2000 -- DA -- Version 0.9.6 update
		1 May 2000 -- DA -- Version 0.9.7 update
	
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
	_transform = transform;
}


/*
===============================================================================
	Inline methods of GlowViewManip_IdleReceiver
===============================================================================
*/

inline Glow_ViewManip_IdleReceiver::Glow_ViewManip_IdleReceiver(
	GlowViewManipulator* manip)
{
	_manip = manip;
}


/*
===============================================================================
	Inline methods of GlowTransformData
===============================================================================
*/

inline GlowTransformData::GlowTransformData(
	bool persistent) :
_receiver(this)
{
	_spinning = false;
	_scale = 1;
	_persistent = persistent;
}


inline GlowTransformData::GlowTransformData(
	GLfloat scale,
	const Quatf& rot,
	const Vec3f& trans,
	bool persistent) :
_receiver(this)
{
	_spinning = false;
	_scale = scale;
	_rotation = rot;
	_translation = trans;
	_persistent = persistent;
}


inline void GlowTransformData::Set(
	GLfloat scale,
	const Quatf& rot,
	const Vec3f& trans)
{
	_scale = scale;
	_rotation = rot;
	_translation = trans;
	RefreshAll();
}


inline void GlowTransformData::SetScale(
	GLfloat scale)
{
	_scale = scale;
	RefreshAll();
}


inline void GlowTransformData::SetTranslation(
	const Vec3f& trans)
{
	_translation = trans;
	RefreshAll();
}


inline void GlowTransformData::SetRotation(
	const Quatf& rot)
{
	_rotation = rot;
	RefreshAll();
}


inline void GlowTransformData::SetIdentity()
{
	_scale = 1.0f;
	_rotation.SetIdentity();
	_translation.SetZero();
	RefreshAll();
}


inline void GlowTransformData::AddScale(
	GLfloat scale)
{
	_translation *= scale;
	_scale *= scale;
	RefreshAll();
}


inline void GlowTransformData::AddTranslation(
	const Vec3f& trans)
{
	_translation += trans;
	RefreshAll();
}


inline void GlowTransformData::AddRotation(
	const Quatf& rot)
{
	_translation = rot * _translation;
	_rotation = rot % _rotation;
	_rotation.Normalize();
	RefreshAll();
}


inline bool GlowTransformData::IsSpinning() const
{
	return _spinning;
}


inline void GlowTransformData::StopSpinning()
{
	if (_spinning)
	{
		Glow::UnregisterIdle(&_receiver);
		_spinning = false;
	}
}


inline void GlowTransformData::StartSpinning(
	const Quatf& spin)
{
	_curSpin = spin;
	if (!_spinning)
	{
		Glow::RegisterIdle(&_receiver);
		_spinning = true;
	}
}


inline void GlowTransformData::StepSpin()
{
	AddRotation(_curSpin);
}


inline GLfloat GlowTransformData::GetScale() const
{
	return _scale;
}


inline const Vec3f& GlowTransformData::GetTranslation() const
{
	return _translation;
}


inline const Quatf& GlowTransformData::GetRotation() const
{
	return _rotation;
}


inline Vec3f GlowTransformData::Apply(
	const Vec3f& vec) const
{
	return _rotation*(vec*_scale)+_translation;
}


inline Vec3f GlowTransformData::ApplyInverse(
	const Vec3f& vec) const
{
	return (_rotation.Conjugate()*(vec-_translation))/_scale;
}


inline bool GlowTransformData::IsPersistent() const
{
	return _persistent;
}


inline void GlowTransformData::SetPersistent(
	bool persist)
{
	_persistent = persist;
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
	GLfloat scale,
	const Quatf& rot,
	const Vec3f& trans)
{
	_transform->Set(scale, rot, trans);
}


inline void GlowViewTransform::SetScale(
	GLfloat scale)
{
	_transform->SetScale(scale);
}


inline void GlowViewTransform::SetTranslation(
	const Vec3f& trans)
{
	_transform->SetTranslation(trans);
}


inline void GlowViewTransform::SetRotation(
	const Quatf& rot)
{
	_transform->SetRotation(rot);
}


inline void GlowViewTransform::SetIdentity()
{
	_transform->SetIdentity();
}


inline void GlowViewTransform::AddScale(
	GLfloat scale)
{
	_transform->AddScale(scale);
}


inline void GlowViewTransform::AddTranslation(
	const Vec3f& trans)
{
	_transform->AddTranslation(trans);
}


inline void GlowViewTransform::AddRotation(
	const Quatf& rot)
{
	_transform->AddRotation(rot);
}


inline GLfloat GlowViewTransform::GetScale() const
{
	return _transform->GetScale();
}


inline const Vec3f& GlowViewTransform::GetTranslation() const
{
	return _transform->GetTranslation();
}


inline const Quatf& GlowViewTransform::GetRotation() const
{
	return _transform->GetRotation();
}


inline Vec3f GlowViewTransform::Apply(
	const Vec3f& vec) const
{
	return _transform->Apply(vec);
}


inline Vec3f GlowViewTransform::ApplyInverse(
	const Vec3f& vec) const
{
	return _transform->ApplyInverse(vec);
}


inline void GlowViewTransform::ApplyToGLMatrix() const
{
	_transform->ApplyToGLMatrix();
}


inline void GlowViewTransform::ApplyInverseToGLMatrix() const
{
	_transform->ApplyInverseToGLMatrix();
}


inline void GlowViewTransform::GetMatrix(
	Mat4f& matrix) const
{
	_transform->GetMatrix(matrix);
}


inline void GlowViewTransform::GetGLMatrixf(
	GLfloat* matrix) const
{
	_transform->GetGLMatrixf(matrix);
}


inline bool GlowViewTransform::IsSpinning() const
{
	return _transform->IsSpinning();
}


inline void GlowViewTransform::StopSpinning()
{
	_transform->StopSpinning();
}


inline void GlowViewTransform::StartSpinning(
	const Quatf& spin)
{
	_transform->StartSpinning(spin);
}


inline void GlowViewTransform::StepSpin()
{
	_transform->StepSpin();
}


inline GlowTransformData* GlowViewTransform::TransformData() const
{
	return _transform;
}


inline void GlowViewTransform::RefreshAllConnected() const
{
	_transform->RefreshAll();
}


inline void GlowViewTransform::_RawDisconnect()
{
	_transform->_clients.erase(
		GLOW_STD::find(_transform->_clients.begin(), _transform->_clients.end(), this));
	if (!_transform->_persistent && _transform->_clients.empty())
	{
		delete _transform;
	}
}


inline void GlowViewTransform::_RawConnect(
	GlowTransformData* transform)
{
	_transform = transform;
	_transform->_clients.push_back(this);
}


/*
===============================================================================
	Inline methods of GlowViewManipulator
===============================================================================
*/

inline GlowViewManipulator::GlowViewManipulator() :
_receiver(this)
{
}


inline GlowViewManipulator::GlowViewManipulator(
	GlowComponent *parent,
	const GlowViewManipulatorParams& params) :
_receiver(this)
{
	Init(parent, params);
}


inline void GlowViewManipulator::SetColor(
	GlowColor c)
{
	_color = c;
	ParentWindow()->Refresh();
}


inline bool GlowViewManipulator::IsDragging() const
{
	return _dragType != idleState;
}


inline bool GlowViewManipulator::IsSpinnable() const
{
	return _spinnable;
}


inline GlowViewManipulator::State GlowViewManipulator::GetState() const
{
	return _dragType;
}


inline bool GlowViewManipulator::IsDrawing() const
{
	return _draw;
}


inline void GlowViewManipulator::SetDrawing(
	bool draw)
{
	_draw = draw;
}


inline float GlowViewManipulator::GetScaleThrottle() const
{
	return _scaleThrottle;
}


inline void GlowViewManipulator::SetScaleThrottle(
	float throttle)
{
	_scaleThrottle = throttle;
}


inline float GlowViewManipulator::GetTranslationThrottle() const
{
	return _transThrottle;
}


inline void GlowViewManipulator::SetTranslationThrottle(
	float throttle)
{
	_transThrottle = throttle;
}


inline float GlowViewManipulator::GetRotationThrottle() const
{
	return _rotThrottle * 2.0f;
}


inline void GlowViewManipulator::SetRotationThrottle(
	float throttle)
{
	_rotThrottle = throttle * 0.5f;
}


inline GLOW_STD::vector<Vec3f>& GlowViewManipulator::AxisConstraints()
{
	return _axisConstraints;
}


inline const GLOW_STD::vector<Vec3f>& GlowViewManipulator::AxisConstraints() const
{
	return _axisConstraints;
}


inline void GlowViewManipulator::SetAxisConstraintsActive(
	bool active)
{
	_axisConstraintsActive = active;
}


inline bool GlowViewManipulator::IsAxisConstraintsActive() const
{
	return _axisConstraintsActive;
}


inline void GlowViewManipulator::_SimDrag()
{
	InDrag(_xCur, _yCur);
}



/*
===============================================================================
*/

GLOW_NAMESPACE_END

