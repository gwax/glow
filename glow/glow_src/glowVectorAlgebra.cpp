/*
===============================================================================

	FILE:  glowVectorAlgebra.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Vector, matrix and quaternion classes
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_OPTION_NOIOSTREAMS
	#include <iostream>
#endif

#ifndef GLOW_VECTORALGEBRA__H
	#include "glowVectorAlgebra.h"
#endif

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of Vec3f and friends
===============================================================================
*/

#ifndef GLOW_OPTION_NOIOSTREAMS

//-----------------------------------------------------------------------------
// Templated vector input function
//-----------------------------------------------------------------------------
GLOW_STD::istream& operator>>(
	GLOW_STD::istream& stream,
	Vec3f& v)
{
	char c;
	
	stream >> c;
	if (c == '[')
	{
		GLfloat val1, val2, val3;
		stream >> val1 >> val2 >> val3;
		v.Set(val1, val2, val3);
		stream >> c;
	}
	return stream;
}

#endif



/*
===============================================================================
	Methods of Mat4f
===============================================================================
*/

//-----------------------------------------------------------------------------
// Binary operations
//-----------------------------------------------------------------------------

Mat4f Mat4f::operator*(
	GLfloat op2) const
{
	Mat4f result;
	short i, j;
	
	for (j=0; j<4; j++)
	for (i=0; i<4; i++)
	{
		result._vals[i][j] = _vals[i][j] * op2;
	}
	
	return result;
}


Mat4f Mat4f::operator/(
	GLfloat op2) const
{
	Mat4f result;
	short i, j;
	
	for (j=0; j<4; j++)
	for (i=0; i<4; i++)
	{
		result._vals[i][j] = _vals[i][j] / op2;
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix inverse by Gauss-Jordan elimination
//-----------------------------------------------------------------------------

Mat4f& Mat4f::SetInverse(
	Mat4f& res) const
{
	Mat4f temp = *this;
	int i, j;
	GLfloat fac;
	GLfloat pivot, pivot2;
	int pivrow;
	
	res.SetIdentity();
	for (j=0; j<4; j++)
	{
		// Find largest pivot
		pivrow = j;
		pivot = GLOW_STD::fabs(temp._vals[j][j]);
		for (i=j+1; i<4; i++)
		{
			pivot2 = GLOW_STD::fabs(temp._vals[j][i]);
			if (pivot2 > pivot)
			{
				pivot = pivot2;
				pivrow = i;
			}
		}
		
		// Check for singular matrix
		if (pivot == GLfloat(0))
		{
			res.SetZero();
			return res;
		}
		
		// Get pivot into jth row
		if (pivrow != j)
		{
			temp._GJSwapRows(pivrow, j);
			res._GJSwapRows(pivrow, j);
		}
		
		// Turn the pivot into 1
		fac = GLfloat(1)/temp._vals[j][j];
		temp._GJScaleRow(j, fac);
		res._GJScaleRow(j, fac);
		
		// Zero out this element in other rows
		for (i=0; i<4; i++)
		{
			if (j != i && temp._vals[j][i] != GLfloat(0))
			{
				fac = -(temp._vals[j][i]);
				temp._GJAddToRow(j, fac, i);
				res._GJAddToRow(j, fac, i);
			}
		}
	}
	
	return res;
}


Mat4f Mat4f::Inverse() const
{
	Mat4f res;
	SetInverse(res);
	return res;
}

/*{
	const GLfloat TINY = 0.0000001;
	Mat4f result;
	Mat4f temp = *this;
	short i, j;
	GLfloat fac;
	
	for (i=0; i<4; i++)
	{
		// First get a 1 into this element
		if (temp._vals[i][i] < TINY && temp._vals[i][i] > -TINY)
		{
			for (j=i+1; j<4; j++)
			{
				if (temp._vals[i][j] > TINY || temp._vals[i][j] < -TINY)
				{
					temp._GJSwapRows(i, j);
					result._GJSwapRows(i, j);
					break;
				}
			}
			if (temp._vals[i][i] == 0.0)
			{
				// singular matrix
				result.SetZero();
				return result;
			}
		}
		fac = 1.0/temp._vals[i][i];
		temp._GJScaleRow(i, fac);
		result._GJScaleRow(i, fac);
		
		// Next, zero out this element in other rows
		for (j=0; j<4; j++)
		{
			if (j != i && temp._vals[i][j] != 0.0)
			{
				fac = -(temp._vals[i][j]);
				temp._GJAddToRow(i, fac, j);
				result._GJAddToRow(i, fac, j);
			}
		}
	}
	
	return result;
}*/


//-----------------------------------------------------------------------------
// Matrix cofactors
//-----------------------------------------------------------------------------

Mat4f& Mat4f::SetCofactors(
	Mat4f& res) const
{
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		res._vals[i][j] = _CofactorElem(i, j);
	}
	
	return res;
}


Mat4f Mat4f::Cofactors() const
{
	Mat4f result;
	
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		result._vals[i][j] = _CofactorElem(i, j);
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix determinant
//-----------------------------------------------------------------------------

GLfloat Mat4f::Determinant() const
{
	GLfloat result = 0;
	
	for (int i=0; i<4; i++)
	{
		result += _CofactorElem(i, 0) * GetVal(0, i);
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix transpose
//-----------------------------------------------------------------------------

Mat4f& Mat4f::SetTranspose(
	Mat4f& res) const
{
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		res._vals[i][j] = _vals[j][i];
	}
	
	return res;
}


Mat4f Mat4f::Transpose() const
{
	Mat4f result;
	
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		result._vals[i][j] = _vals[j][i];
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Set a rotation matrix given axis as three values, and angle
//-----------------------------------------------------------------------------

void Mat4f::SetRotation(
	GLfloat x,
	GLfloat y,
	GLfloat z,
	GLfloat angle)
{
	_vals[0][0] = x*x+(GLfloat(1)-x*x)*GLOW_STD::cos(angle);
	_vals[1][0] = x*y*(GLfloat(1)-GLOW_STD::cos(angle))-z*GLOW_STD::sin(angle);
	_vals[2][0] = z*x*(GLfloat(1)-GLOW_STD::cos(angle))+y*GLOW_STD::sin(angle);
	_vals[3][0] = GLfloat(0);
	_vals[0][1] = x*y*(GLfloat(1)-GLOW_STD::cos(angle))+z*GLOW_STD::sin(angle);
	_vals[1][1] = y*y+(GLfloat(1)-y*y)*GLOW_STD::cos(angle);
	_vals[2][1] = y*z*(GLfloat(1)-GLOW_STD::cos(angle))-x*GLOW_STD::sin(angle);
	_vals[3][1] = GLfloat(0);
	_vals[0][2] = z*x*(GLfloat(1)-GLOW_STD::cos(angle))-y*GLOW_STD::sin(angle);
	_vals[1][2] = y*z*(GLfloat(1)-GLOW_STD::cos(angle))+x*GLOW_STD::sin(angle);
	_vals[2][2] = z*z+(GLfloat(1)-z*z)*GLOW_STD::cos(angle);
	_vals[3][2] = GLfloat(0);
	_vals[0][3] = GLfloat(0);
	_vals[1][3] = GLfloat(0);
	_vals[2][3] = GLfloat(0);
	_vals[3][3] = GLfloat(1);
}


//-----------------------------------------------------------------------------
// Create a translation transform
//-----------------------------------------------------------------------------

void Mat4f::SetTranslation(
	GLfloat x,
	GLfloat y,
	GLfloat z)
{
	_vals[0][0] = GLfloat(1);
	_vals[1][0] = GLfloat(0);
	_vals[2][0] = GLfloat(0);
	_vals[3][0] = x;
	_vals[0][1] = GLfloat(0);
	_vals[1][1] = GLfloat(1);
	_vals[2][1] = GLfloat(0);
	_vals[3][1] = y;
	_vals[0][2] = GLfloat(0);
	_vals[1][2] = GLfloat(0);
	_vals[2][2] = GLfloat(1);
	_vals[3][2] = z;
	_vals[0][3] = GLfloat(0);
	_vals[1][3] = GLfloat(0);
	_vals[2][3] = GLfloat(0);
	_vals[3][3] = GLfloat(1);
}


//-----------------------------------------------------------------------------
// Create a scaling transform
//-----------------------------------------------------------------------------

void Mat4f::SetScale(
	GLfloat x,
	GLfloat y,
	GLfloat z)
{
	_vals[0][0] = x;
	_vals[1][0] = GLfloat(0);
	_vals[2][0] = GLfloat(0);
	_vals[3][0] = GLfloat(0);
	_vals[0][1] = GLfloat(0);
	_vals[1][1] = y;
	_vals[2][1] = GLfloat(0);
	_vals[3][1] = GLfloat(0);
	_vals[0][2] = GLfloat(0);
	_vals[1][2] = GLfloat(0);
	_vals[2][2] = z;
	_vals[3][2] = GLfloat(0);
	_vals[0][3] = GLfloat(0);
	_vals[1][3] = GLfloat(0);
	_vals[2][3] = GLfloat(0);
	_vals[3][3] = GLfloat(1);
}


//-----------------------------------------------------------------------------
// Set axis transformation matrix
//-----------------------------------------------------------------------------

void Mat4f::SetAxisTransformation(
	const Vec3f& x,
	const Vec3f& y,
	const Vec3f& z)
{
	_vals[0][0] = x.GetX();
	_vals[1][0] = y.GetX();
	_vals[2][0] = z.GetX();
	_vals[3][0] = 0.0f;
	_vals[0][1] = x.GetY();
	_vals[1][1] = y.GetY();
	_vals[2][1] = z.GetY();
	_vals[3][1] = 0.0f;
	_vals[0][2] = x.GetZ();
	_vals[1][2] = y.GetZ();
	_vals[2][2] = z.GetZ();
	_vals[3][2] = 0.0f;
	_vals[0][3] = 0.0f;
	_vals[1][3] = 0.0f;
	_vals[2][3] = 0.0f;
	_vals[3][3] = 1.0f;
}


//-----------------------------------------------------------------------------
// Apply matrix transform to a vector
//-----------------------------------------------------------------------------

Vec3f Mat4f::operator*(
	const Vec3f& v)
{
	return Vec3f(
		v.GetX()*GLfloat(_vals[0][0]) + v.GetY()*GLfloat(_vals[1][0]) +
			v.GetZ()*GLfloat(_vals[2][0]) + GLfloat(_vals[3][0]),
		v.GetX()*GLfloat(_vals[0][1]) + v.GetY()*GLfloat(_vals[1][1]) +
			v.GetZ()*GLfloat(_vals[2][1]) + GLfloat(_vals[3][1]),
		v.GetX()*GLfloat(_vals[0][2]) + v.GetY()*GLfloat(_vals[1][2]) +
			v.GetZ()*GLfloat(_vals[2][2]) + GLfloat(_vals[3][2])) /
		GLfloat(v.GetX()*GLfloat(_vals[0][3]) + v.GetY()*GLfloat(_vals[1][3]) +
	      v.GetZ()*GLfloat(_vals[2][3]) + GLfloat(_vals[3][3]));
}


//-----------------------------------------------------------------------------
// Matrix binary operations
//-----------------------------------------------------------------------------

Mat4f& Mat4f::operator*=(
	const Mat4f& op2)
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result._vals[i][j] =
			_vals[0][j]*op2._vals[i][0] +
			_vals[1][j]*op2._vals[i][1] +
			_vals[2][j]*op2._vals[i][2] +
			_vals[3][j]*op2._vals[i][3];
	}
	
	*this = result;
	
	return *this;
}


Mat4f Mat4f::operator+(
	const Mat4f& op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result._vals[j][i] = _vals[j][i] + op2._vals[j][i];
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix subtract
//-----------------------------------------------------------------------------

Mat4f Mat4f::operator-(
	const Mat4f& op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result._vals[j][i] = _vals[j][i] - op2._vals[j][i];
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix multiply
//-----------------------------------------------------------------------------

Mat4f Mat4f::operator*(
	const Mat4f& op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result._vals[i][j] =
			_vals[0][j]*op2._vals[i][0] +
			_vals[1][j]*op2._vals[i][1] +
			_vals[2][j]*op2._vals[i][2] +
			_vals[3][j]*op2._vals[i][3];
	}
	
	return result;
}



//************************************************ NON-METHOD DEFINITIONS *****


//********************************************************* I/O FUNCTIONS *****

#ifndef GLOW_OPTION_NOIOSTREAMS

//-----------------------------------------------------------------------------
// Templated matrix output function
//-----------------------------------------------------------------------------

GLOW_STD::ostream& operator<<(
	GLOW_STD::ostream& stream,
	const Mat4f& m)
{
	stream << "[ ";
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		stream << m.GetVal(j, i) << " ";
	}
	stream << "]";
	
	return stream;
}


//-----------------------------------------------------------------------------
// Templated matrix input function
//-----------------------------------------------------------------------------

GLOW_STD::istream& operator>>(
	GLOW_STD::istream& stream,
	Mat4f& m)
{
	char c;
	GLfloat val;
	
	stream >> c;
	if (c == '[')
	{
		for (short j=0; j<4; j++)
		for (short i=0; i<4; i++)
		{
			stream >> val;
			m.SetVal(j, i, val);
		}
	}
	stream >> c;
	
	return stream;
}

#endif



/*
===============================================================================
	Methods of template Quatf
===============================================================================
*/

//-----------------------------------------------------------------------------
// Extract info from rotation quaternion
//-----------------------------------------------------------------------------

void Quatf::GetRotation(
	Vec3f& axis,
	GLfloat& angle) const
{
	GLfloat val = _vals[0];
	if (val > 1.0)
	{
		val = 1.0;
	}
	else if (val < -1.0)
	{
		val = -1.0;
	}
	angle = GLfloat(2.0)*GLOW_STD::acos(val);
	if (val < 0)
	{
		angle -= GLfloat(2)*GLfloat(3.1415926535897932384626433832795);
	}
	if (_vals[1] == 0.0 && _vals[2] == 0.0 && _vals[3] == 0.0)
	{
		axis.Set(GLfloat(1), GLfloat(0), GLfloat(0));
	}
	else
	{
		axis.Set(_vals[1], _vals[2], _vals[3]);
		axis.Normalize();
	}
}


//-----------------------------------------------------------------------------
// Scale rotation in a rotation quaternion
//-----------------------------------------------------------------------------

void Quatf::ScaleRotation(
	GLfloat factor)
{
	GLfloat l = GLOW_STD::sqrt(GLfloat(1)-_vals[0]*_vals[0]);
	if (l>FLT_EPSILON || l<-FLT_EPSILON)
	{
		Normalize();
		GLfloat a = GLOW_STD::acos(_vals[0])*factor;
		GLfloat b = GLOW_STD::sin(a)/l;
		_vals[0] = GLOW_STD::cos(a);
		_vals[1] = _vals[1]*b;
		_vals[2] = _vals[2]*b;
		_vals[3] = _vals[3]*b;
	}
}


//-----------------------------------------------------------------------------
// Apply quaternion rotation to a vector
//-----------------------------------------------------------------------------

Vec3f Quatf::operator*(
	const Vec3f& v) const
{
	return Vec3f((*this % Quatf(v) % Conjugate()).GetImaginary());
}


//-----------------------------------------------------------------------------
// Quatf multiply
//-----------------------------------------------------------------------------

Quatf Quatf::operator%(
	const Quatf& op2) const
{
	return Quatf(
		_vals[0]*op2._vals[0]-
			_vals[1]*op2._vals[1]-
			_vals[2]*op2._vals[2]-
			_vals[3]*op2._vals[3],
		_vals[0]*op2._vals[1]+
			_vals[1]*op2._vals[0]+
			_vals[2]*op2._vals[3]-
			_vals[3]*op2._vals[2],
		_vals[0]*op2._vals[2]-
			_vals[1]*op2._vals[3]+
			_vals[2]*op2._vals[0]+
			_vals[3]*op2._vals[1],
		_vals[0]*op2._vals[3]+
			_vals[1]*op2._vals[2]-
			_vals[2]*op2._vals[1]+
			_vals[3]*op2._vals[0]);
}


Quatf& Quatf::operator%=(
	const Quatf& op2)
{
	Quatf result(
		_vals[0]*op2._vals[0]-
			_vals[1]*op2._vals[1]-
			_vals[2]*op2._vals[2]-
			_vals[3]*op2._vals[3],
		_vals[0]*op2._vals[1]+
			_vals[1]*op2._vals[0]+
			_vals[2]*op2._vals[3]-
			_vals[3]*op2._vals[2],
		_vals[0]*op2._vals[2]-
			_vals[1]*op2._vals[3]+
			_vals[2]*op2._vals[0]+
			_vals[3]*op2._vals[1],
		_vals[0]*op2._vals[3]+
			_vals[1]*op2._vals[2]-
			_vals[2]*op2._vals[1]+
			_vals[3]*op2._vals[0]);
	*this = result;
	return *this;
}


//************************************************ NON-METHOD DEFINITIONS *****


//********************************************************* I/O FUNCTIONS *****

#ifndef GLOW_OPTION_NOIOSTREAMS

//-----------------------------------------------------------------------------
// Templated quaternion output function
//-----------------------------------------------------------------------------

inline GLOW_STD::ostream& operator<<(
	GLOW_STD::ostream& stream,
	const Quatf& q)
{
	stream << "[ ";
	
	for (short i=0; i<4; i++)
	{
		stream << q.GetVal(i) << " ";
	}
	stream << "]";
	
	return stream;
}


//-----------------------------------------------------------------------------
// Templated quaternion input function
//-----------------------------------------------------------------------------

inline GLOW_STD::istream& operator>>(
	GLOW_STD::istream& stream,
	Quatf& q)
{
	char c;
	GLfloat val;
	
	stream >> c;
	if (c == '[')
	{
		for (short i=0; i<4; i++)
		{
			stream >> val;
			q.SetVal(i, val);
		}
	}
	stream >> c;
	
	return stream;
}

#endif


/*
===============================================================================
*/


GLOW_NAMESPACE_END

