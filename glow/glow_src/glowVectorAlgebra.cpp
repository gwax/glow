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

#include <cfloat>
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
	Math constants
===============================================================================
*/

const double Math::pi = 3.1415926535897932384626433832795;
const double Math::twopi = 3.1415926535897932384626433832795*2.0;
const double Math::halfpi = 3.1415926535897932384626433832795*0.5;
const double Math::radiansToDegrees = 180.0/3.1415926535897932384626433832795;
const double Math::degreesToRadians = 3.1415926535897932384626433832795/180.0;


/*
===============================================================================
	Methods of Vec3f
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

const Mat4f Mat4f::operator*(
	GLfloat op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result.vals_[i][j] = vals_[i][j] * op2;
	}
	
	return result;
}


const Mat4f operator*(
	GLfloat op1,
	const Mat4f& op2)
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result.SetVal(i, j, op1 * op2.GetVal(i, j));
	}
	
	return result;
}


const Mat4f Mat4f::operator/(
	GLfloat op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result.vals_[i][j] = vals_[i][j] / op2;
	}
	
	return result;
}


const Mat4f Mat4f::operator-() const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result.vals_[i][j] = -vals_[i][j];
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix inverse by Gauss-Jordan elimination
//-----------------------------------------------------------------------------

void Mat4f::SetInverse(
	const Mat4f& orig)
{
	Mat4f temp = orig;
	int i, j;
	GLfloat fac;
	GLfloat pivot, pivot2;
	int pivrow;
	
	SetIdentity();
	for (j=0; j<4; j++)
	{
		// Find largest pivot
		pivrow = j;
		pivot = GLOW_CSTD::fabs(temp.vals_[j][j]);
		for (i=j+1; i<4; i++)
		{
			pivot2 = GLOW_CSTD::fabs(temp.vals_[j][i]);
			if (pivot2 > pivot)
			{
				pivot = pivot2;
				pivrow = i;
			}
		}
		
		// Check for singular matrix
		if (pivot == GLfloat(0))
		{
			SetZero();
			return;
		}
		
		// Get pivot into jth row
		if (pivrow != j)
		{
			temp._GJSwapRows(pivrow, j);
			_GJSwapRows(pivrow, j);
		}
		
		// Turn the pivot into 1
		fac = GLfloat(1)/temp.vals_[j][j];
		temp._GJScaleRow(j, fac);
		_GJScaleRow(j, fac);
		
		// Zero out this element in other rows
		for (i=0; i<4; i++)
		{
			if (j != i && temp.vals_[j][i] != GLfloat(0))
			{
				fac = -(temp.vals_[j][i]);
				temp._GJAddToRow(j, fac, i);
				_GJAddToRow(j, fac, i);
			}
		}
	}
}


const Mat4f Mat4f::Inverse() const
{
	Mat4f res;
	res.SetInverse(*this);
	return res;
}


//-----------------------------------------------------------------------------
// Matrix cofactors
//-----------------------------------------------------------------------------

void Mat4f::SetCofactors(
	const Mat4f& orig)
{
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		vals_[i][j] = orig._CofactorElem(i, j);
	}
}


const Mat4f Mat4f::Cofactors() const
{
	Mat4f result;
	
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		result.vals_[i][j] = _CofactorElem(i, j);
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

void Mat4f::SetTranspose(
	const Mat4f& orig)
{
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		vals_[i][j] = orig.vals_[j][i];
	}
}


const Mat4f Mat4f::Transpose() const
{
	Mat4f result;
	
	for (int j=0; j<4; j++)
	for (int i=0; i<4; i++)
	{
		result.vals_[i][j] = vals_[j][i];
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
	vals_[0][0] = x*x+(GLfloat(1)-x*x)*GLOW_CSTD::cos(angle);
	vals_[1][0] = x*y*(GLfloat(1)-GLOW_CSTD::cos(angle))-z*GLOW_CSTD::sin(angle);
	vals_[2][0] = z*x*(GLfloat(1)-GLOW_CSTD::cos(angle))+y*GLOW_CSTD::sin(angle);
	vals_[3][0] = GLfloat(0);
	vals_[0][1] = x*y*(GLfloat(1)-GLOW_CSTD::cos(angle))+z*GLOW_CSTD::sin(angle);
	vals_[1][1] = y*y+(GLfloat(1)-y*y)*GLOW_CSTD::cos(angle);
	vals_[2][1] = y*z*(GLfloat(1)-GLOW_CSTD::cos(angle))-x*GLOW_CSTD::sin(angle);
	vals_[3][1] = GLfloat(0);
	vals_[0][2] = z*x*(GLfloat(1)-GLOW_CSTD::cos(angle))-y*GLOW_CSTD::sin(angle);
	vals_[1][2] = y*z*(GLfloat(1)-GLOW_CSTD::cos(angle))+x*GLOW_CSTD::sin(angle);
	vals_[2][2] = z*z+(GLfloat(1)-z*z)*GLOW_CSTD::cos(angle);
	vals_[3][2] = GLfloat(0);
	vals_[0][3] = GLfloat(0);
	vals_[1][3] = GLfloat(0);
	vals_[2][3] = GLfloat(0);
	vals_[3][3] = GLfloat(1);
}


//-----------------------------------------------------------------------------
// Create a translation transform
//-----------------------------------------------------------------------------

void Mat4f::SetTranslation(
	GLfloat x,
	GLfloat y,
	GLfloat z)
{
	vals_[0][0] = GLfloat(1);
	vals_[1][0] = GLfloat(0);
	vals_[2][0] = GLfloat(0);
	vals_[3][0] = x;
	vals_[0][1] = GLfloat(0);
	vals_[1][1] = GLfloat(1);
	vals_[2][1] = GLfloat(0);
	vals_[3][1] = y;
	vals_[0][2] = GLfloat(0);
	vals_[1][2] = GLfloat(0);
	vals_[2][2] = GLfloat(1);
	vals_[3][2] = z;
	vals_[0][3] = GLfloat(0);
	vals_[1][3] = GLfloat(0);
	vals_[2][3] = GLfloat(0);
	vals_[3][3] = GLfloat(1);
}


//-----------------------------------------------------------------------------
// Create a scaling transform
//-----------------------------------------------------------------------------

void Mat4f::SetScale(
	GLfloat x,
	GLfloat y,
	GLfloat z)
{
	vals_[0][0] = x;
	vals_[1][0] = GLfloat(0);
	vals_[2][0] = GLfloat(0);
	vals_[3][0] = GLfloat(0);
	vals_[0][1] = GLfloat(0);
	vals_[1][1] = y;
	vals_[2][1] = GLfloat(0);
	vals_[3][1] = GLfloat(0);
	vals_[0][2] = GLfloat(0);
	vals_[1][2] = GLfloat(0);
	vals_[2][2] = z;
	vals_[3][2] = GLfloat(0);
	vals_[0][3] = GLfloat(0);
	vals_[1][3] = GLfloat(0);
	vals_[2][3] = GLfloat(0);
	vals_[3][3] = GLfloat(1);
}


//-----------------------------------------------------------------------------
// Set axis transformation matrix
//-----------------------------------------------------------------------------

void Mat4f::SetAxisTransformation(
	const Vec3f& x,
	const Vec3f& y,
	const Vec3f& z)
{
	vals_[0][0] = x.GetX();
	vals_[1][0] = y.GetX();
	vals_[2][0] = z.GetX();
	vals_[3][0] = 0.0f;
	vals_[0][1] = x.GetY();
	vals_[1][1] = y.GetY();
	vals_[2][1] = z.GetY();
	vals_[3][1] = 0.0f;
	vals_[0][2] = x.GetZ();
	vals_[1][2] = y.GetZ();
	vals_[2][2] = z.GetZ();
	vals_[3][2] = 0.0f;
	vals_[0][3] = 0.0f;
	vals_[1][3] = 0.0f;
	vals_[2][3] = 0.0f;
	vals_[3][3] = 1.0f;
}


//-----------------------------------------------------------------------------
// Apply matrix transform to a vector
//-----------------------------------------------------------------------------

const Vec3f Mat4f::operator*(
	const Vec3f& v)
{
	return Vec3f(
		v.GetX()*GLfloat(vals_[0][0]) + v.GetY()*GLfloat(vals_[1][0]) +
			v.GetZ()*GLfloat(vals_[2][0]) + GLfloat(vals_[3][0]),
		v.GetX()*GLfloat(vals_[0][1]) + v.GetY()*GLfloat(vals_[1][1]) +
			v.GetZ()*GLfloat(vals_[2][1]) + GLfloat(vals_[3][1]),
		v.GetX()*GLfloat(vals_[0][2]) + v.GetY()*GLfloat(vals_[1][2]) +
			v.GetZ()*GLfloat(vals_[2][2]) + GLfloat(vals_[3][2])) /
		GLfloat(v.GetX()*GLfloat(vals_[0][3]) + v.GetY()*GLfloat(vals_[1][3]) +
	      v.GetZ()*GLfloat(vals_[2][3]) + GLfloat(vals_[3][3]));
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
		result.vals_[i][j] =
			vals_[0][j]*op2.vals_[i][0] +
			vals_[1][j]*op2.vals_[i][1] +
			vals_[2][j]*op2.vals_[i][2] +
			vals_[3][j]*op2.vals_[i][3];
	}
	
	*this = result;
	
	return *this;
}


const Mat4f Mat4f::operator+(
	const Mat4f& op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result.vals_[j][i] = vals_[j][i] + op2.vals_[j][i];
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix subtract
//-----------------------------------------------------------------------------

const Mat4f Mat4f::operator-(
	const Mat4f& op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result.vals_[j][i] = vals_[j][i] - op2.vals_[j][i];
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Matrix multiply
//-----------------------------------------------------------------------------

const Mat4f Mat4f::operator*(
	const Mat4f& op2) const
{
	Mat4f result;
	
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		result.vals_[i][j] =
			vals_[0][j]*op2.vals_[i][0] +
			vals_[1][j]*op2.vals_[i][1] +
			vals_[2][j]*op2.vals_[i][2] +
			vals_[3][j]*op2.vals_[i][3];
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
	GLfloat val = vals_[0];
	if (val > 1.0)
	{
		val = 1.0;
	}
	else if (val < -1.0)
	{
		val = -1.0;
	}
	angle = GLfloat(2.0)*GLOW_CSTD::acos(val);
	if (val < 0)
	{
		angle -= Math::twopi;
	}
	if (vals_[1] == 0.0 && vals_[2] == 0.0 && vals_[3] == 0.0)
	{
		axis.Set(GLfloat(1), GLfloat(0), GLfloat(0));
	}
	else
	{
		axis.Set(vals_[1], vals_[2], vals_[3]);
		axis.Normalize();
	}
}


//-----------------------------------------------------------------------------
// Scale rotation in a rotation quaternion
//-----------------------------------------------------------------------------

void Quatf::ScaleRotation(
	GLfloat factor)
{
	GLfloat l = GLOW_CSTD::sqrt(GLfloat(1)-vals_[0]*vals_[0]);
	if (l>FLT_EPSILON || l<-FLT_EPSILON)
	{
		Normalize();
		GLfloat a = GLOW_CSTD::acos(vals_[0])*factor;
		GLfloat b = GLOW_CSTD::sin(a)/l;
		vals_[0] = GLOW_CSTD::cos(a);
		vals_[1] = vals_[1]*b;
		vals_[2] = vals_[2]*b;
		vals_[3] = vals_[3]*b;
	}
}


//-----------------------------------------------------------------------------
// Apply quaternion rotation to a vector
//-----------------------------------------------------------------------------

const Vec3f Quatf::operator*(
	const Vec3f& v) const
{
	return Vec3f((*this % Quatf(v) % Conjugate()).GetImaginary());
}


//-----------------------------------------------------------------------------
// Quatf multiply
//-----------------------------------------------------------------------------

const Quatf Quatf::operator%(
	const Quatf& op2) const
{
	return Quatf(
		vals_[0]*op2.vals_[0]-
			vals_[1]*op2.vals_[1]-
			vals_[2]*op2.vals_[2]-
			vals_[3]*op2.vals_[3],
		vals_[0]*op2.vals_[1]+
			vals_[1]*op2.vals_[0]+
			vals_[2]*op2.vals_[3]-
			vals_[3]*op2.vals_[2],
		vals_[0]*op2.vals_[2]-
			vals_[1]*op2.vals_[3]+
			vals_[2]*op2.vals_[0]+
			vals_[3]*op2.vals_[1],
		vals_[0]*op2.vals_[3]+
			vals_[1]*op2.vals_[2]-
			vals_[2]*op2.vals_[1]+
			vals_[3]*op2.vals_[0]);
}


Quatf& Quatf::operator%=(
	const Quatf& op2)
{
	Quatf result(
		vals_[0]*op2.vals_[0]-
			vals_[1]*op2.vals_[1]-
			vals_[2]*op2.vals_[2]-
			vals_[3]*op2.vals_[3],
		vals_[0]*op2.vals_[1]+
			vals_[1]*op2.vals_[0]+
			vals_[2]*op2.vals_[3]-
			vals_[3]*op2.vals_[2],
		vals_[0]*op2.vals_[2]-
			vals_[1]*op2.vals_[3]+
			vals_[2]*op2.vals_[0]+
			vals_[3]*op2.vals_[1],
		vals_[0]*op2.vals_[3]+
			vals_[1]*op2.vals_[2]-
			vals_[2]*op2.vals_[1]+
			vals_[3]*op2.vals_[0]);
	*this = result;
	return *this;
}


//************************************************ NON-METHOD DEFINITIONS *****


//********************************************************* I/O FUNCTIONS *****

#ifndef GLOW_OPTION_NOIOSTREAMS

//-----------------------------------------------------------------------------
// Templated quaternion output function
//-----------------------------------------------------------------------------

GLOW_STD::ostream& operator<<(
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

GLOW_STD::istream& operator>>(
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


