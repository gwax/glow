/*
===============================================================================

	FILE:  calcengine.cpp
	
	PROJECT:
	
		GLOW-based RPN calculator
	
	CONTENTS:
	
		Data and computation engine
	
	PROGRAMMERS:
	
		Daniel Azuma <dazuma@kagi.com>
	
	COPYRIGHT:
	
		Copyright (C) 2000  Daniel Azuma  (dazuma@kagi.com)
		
		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2.1 of the License, or (at your option) any later version.
		
		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
		GNU Lesser General Public License for more details.
		
		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit tutorial -- version 1.1.2dev  (7 August 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*

This module can be treated as a "black box" for the purposes of this lesson.
It implements all the machinery for making a simple RPN calculator. It just
lacks a user interface.

*/


/*
===============================================================================
	Headers
===============================================================================
*/

#include <cmath>
#include <cstdio>

namespace std {} // For compilers that don't put C libraries in namespace std

using namespace std;

#include "glowDebug.h"

GLOW_NAMESPACE_USING

#include "calcengine.h"


/*
===============================================================================
	Methods of CalcEngine
===============================================================================
*/

CalcEngine::CalcEngine()
{
	GLOW_DEBUGSCOPE("CalcEngine::CalcEngine");
	
	for (int i=0; i<STACKSIZE; ++i)
	{
		stack_[i] = 0;
	}
	state_ = postEntry;
	format_ = decimalFormat;
	angleUnits_ = degrees;
	mantissaNegative_ = exponentNegative_ = false;
}


void CalcEngine::Button(
	int button)
{
	GLOW_DEBUGSCOPE("CalcEngine::Button");
	
	double tempval = 0;
	
	switch (button)
	{
		case enterButton:
			Enter_();
			break;
		
		case pointButton:
			if (state_ == preEntry)
			{
				Enter_();
			}
			if (state_ == postEntry)
			{
				Clear_();
			}
			if (state_ != enterFrac && state_ != enterExpon)
			{
				state_ = enterFrac;
			}
			break;
		
		case exponentButton:
			if (state_ == preEntry)
			{
				Enter_();
			}
			if (state_ == postEntry)
			{
				Clear_();
			}
			if (state_ != enterExpon)
			{
				state_ = enterExpon;
			}
			break;
		
		case negateButton:
			if (state_ == preEntry || state_ == postEntry)
			{
				UnaryOp_(-stack_[0]);
			}
			else if (state_ == enterExpon)
			{
				exponentNegative_ = !exponentNegative_;
			}
			else
			{
				mantissaNegative_ = !mantissaNegative_;
			}
			break;
		
		case deleteButton:
			if (state_ == postEntry)
			{
				Clear_();
			}
			else if (state_ == enterExpon)
			{
				if (exponent_.size() > 0)
				{
					exponent_.erase(exponent_.size()-1, 1);
				}
				else
				{
					exponentNegative_ = false;
					if (fracMantissa_.size() > 0)
					{
						state_ = enterFrac;
					}
					else
					{
						state_ = enterInt;
					}
				}
			}
			else if (state_ == enterFrac)
			{
				if (fracMantissa_.size() > 0)
				{
					fracMantissa_.erase(fracMantissa_.size()-1, 1);
				}
				else
				{
					state_ = enterInt;
				}
			}
			else if (state_ == enterInt)
			{
				if (intMantissa_.size() > 0)
				{
					intMantissa_.erase(intMantissa_.size()-1, 1);
				}
			}
			break;
		
		case plusButton:
			BinaryOp_(stack_[1]+stack_[0]);
			break;
		
		case minusButton:
			BinaryOp_(stack_[1]-stack_[0]);
			break;
		
		case multButton:
			BinaryOp_(stack_[1]*stack_[0]);
			break;
		
		case divButton:
			BinaryOp_(stack_[1]/stack_[0]);
			break;
		
		case recipButton:
			UnaryOp_(Numeric(1)/stack_[0]);
			break;
		
		case sqrtButton:
			UnaryOp_(sqrt(stack_[0]));
			break;
		
		case sqrButton:
			UnaryOp_(stack_[0]*stack_[0]);
			break;
		
		case expButton:
			UnaryOp_(exp(stack_[0]));
			break;
		
		case logButton:
			UnaryOp_(log(stack_[0]));
			break;
		
		case powButton:
			BinaryOp_(pow(stack_[1], stack_[0]));
			break;
		
		case rootButton:
			BinaryOp_(pow(stack_[1], 1.0/stack_[0]));
			break;
		
		case sinButton:
			UnaryOp_(sin(ToRadians_(stack_[0])));
			break;
		
		case cosButton:
			UnaryOp_(cos(ToRadians_(stack_[0])));
			break;
		
		case tanButton:
			UnaryOp_(tan(ToRadians_(stack_[0])));
			break;
		
		case arcsinButton:
			UnaryOp_(FromRadians_(asin(stack_[0])));
			break;
		
		case arccosButton:
			UnaryOp_(FromRadians_(acos(stack_[0])));
			break;
		
		case arctanButton:
			UnaryOp_(FromRadians_(atan(stack_[0])));
			break;
		
		case clearButton:
			Clear_();
			break;
		
		case swapButton:
			tempval = stack_[0];
			stack_[0] = stack_[1];
			stack_[1] = tempval;
			state_ = preEntry;
			break;
		
		case consumeButton:
			for (int i=1; i<STACKSIZE; ++i)
			{
				stack_[i-1] = stack_[i];
			}
			stack_[STACKSIZE-1] = 0;
			state_ = preEntry;
			break;
		
		default:
			GLOW_ASSERT(button >= 0 && button <= 9);
			if (state_ == preEntry)
			{
				Enter_();
			}
			if (state_ == postEntry)
			{
				Clear_();
			}
			if (state_ == enterInt)
			{
				if (!intMantissa_.empty() || button != 0)
				{
					intMantissa_ += char('0'+button);
				}
			}
			else if (state_ == enterFrac)
			{
				fracMantissa_ += char('0'+button);
			}
			else if (state_ == enterExpon)
			{
				if ((!exponent_.empty() || button != 0) &&
					exponent_.size() < 3 &&
					(exponent_.size() < 2 || exponent_[0] < '3'))
				{
					exponent_ += char('0'+button);
				}
			}
	}
	
	if (state_ != preEntry && state_ != postEntry)
	{
		string temp;
		BuildString_(temp);
		stack_[0] = atof(temp.c_str());
	}
}


void CalcEngine::Enter_()
{
	for (int i=STACKSIZE-1; i>0; --i)
	{
		stack_[i] = stack_[i-1];
	}
	state_ = postEntry;
}


void CalcEngine::Clear_()
{
	state_ = enterInt;
	mantissaNegative_ = exponentNegative_ = false;
	intMantissa_ = "";
	fracMantissa_ = "";
	exponent_ = "";
	stack_[0] = 0;
}


void CalcEngine::BinaryOp_(
	Numeric result)
{
	state_ = preEntry;
	stack_[0] = result;
	for (int i=2; i<STACKSIZE; ++i)
	{
		stack_[i-1] = stack_[i];
	}
	stack_[STACKSIZE-1] = 0;
}


void CalcEngine::UnaryOp_(
	Numeric result)
{
	state_ = preEntry;
	stack_[0] = result;
}


void CalcEngine::BuildString_(
	string& str) const
{
	GLOW_DEBUGSCOPE("CalcEngine::BuildString_");
	
	if (mantissaNegative_)
	{
		str = '-';
	}
	if (intMantissa_.empty())
	{
		str += '0';
	}
	else
	{
		str.append(intMantissa_);
	}
	if (!fracMantissa_.empty() || state_ == enterFrac)
	{
		str += '.';
		str.append(fracMantissa_);
	}
	if (!exponent_.empty() || state_ == enterExpon)
	{
		if (exponentNegative_)
		{
			str += "e-";
		}
		else
		{
			str += "e+";
		}
		str.append(exponent_);
	}
}


string CalcEngine::Display(
	int level) const
{
	GLOW_DEBUGSCOPE("CalcEngine::Display");
	GLOW_ASSERT(level >= 0 && level < STACKSIZE);
	
	string result;
	if (state_ == preEntry || state_ == postEntry || level != 0)
	{
		char buf[200];
		if (format_ == scientificFormat || stack_[level] >= 1e10 ||
			stack_[level] <= -1e10 || (stack_[level] < 1e-10 &&
			stack_[level] > -1e-10 && stack_[level] != 0))
		{
			sprintf(buf, "%.15e", stack_[level]);
		}
		else
		{
			sprintf(buf, "%.15f", stack_[level]);
			buf[21] = 0;
		}
		result.assign(buf);
	}
	else
	{
		BuildString_(result);
	}
	return result;
}

