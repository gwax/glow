/*
===============================================================================

	FILE:  glowImageFile.tem.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Image file I/O for GLOW
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
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
	
		The GLOW Toolkit -- version 1.1.2dev  (7 August 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of template GlowPNMReader
===============================================================================
*/

template <class ImageClass>
void TGlowPNMReader<ImageClass>::Set1Channels_(
	typename ImageClass::ElementType elem,
	unsigned int i,
	unsigned int j)
{
	if (numChannels_ == 1)
	{
		imagePtr_->SetChannel(i, j, 0, elem);
	}
	else if (numChannels_ == 3)
	{
		imagePtr_->SetChannel(i, j, 0, elem);
		imagePtr_->SetChannel(i, j, 1, elem);
		imagePtr_->SetChannel(i, j, 2, elem);
	}
	else //if (numChannels_ == 4)
	{
		imagePtr_->SetChannel(i, j, 0, elem);
		imagePtr_->SetChannel(i, j, 1, elem);
		imagePtr_->SetChannel(i, j, 2, elem);
		imagePtr_->SetChannel(i, j, 3, alphaDefault_);
	}
}


template <class ImageClass>
void TGlowPNMReader<ImageClass>::Set3Channels_(
	typename ImageClass::ElementType elem1,
	typename ImageClass::ElementType elem2,
	typename ImageClass::ElementType elem3,
	unsigned int i,
	unsigned int j)
{
	if (numChannels_ == 3)
	{
		imagePtr_->SetChannel(i, j, 0, elem1);
		imagePtr_->SetChannel(i, j, 1, elem2);
		imagePtr_->SetChannel(i, j, 2, elem3);
	}
	else //if (numChannels_ == 4)
	{
		imagePtr_->SetChannel(i, j, 0, elem1);
		imagePtr_->SetChannel(i, j, 1, elem2);
		imagePtr_->SetChannel(i, j, 2, elem3);
		imagePtr_->SetChannel(i, j, 3, alphaDefault_);
	}
}


template <class ImageClass>
void TGlowPNMReader<ImageClass>::Read(
	ImageClass& image)
{
	class GlowPNMReader_Xlate
	{
		public:
		
			// Determine alpha defaults
			inline static void GetAlphaDefault(
				unsigned char& alpha)
			{
				alpha = UCHAR_MAX;
			}
			inline static void GetAlphaDefault(
				char& alpha)
			{
				alpha = SCHAR_MAX;
			}
			inline static void GetAlphaDefault(
				float& alpha)
			{
				alpha = 1.0f;
			}
			inline static void GetAlphaDefault(
				double& alpha)
			{
				alpha = 1.0;
			}
			
			// Translate input to type
			inline static void TranslateInput(
				unsigned char& c,
				int i)
			{
				c = (unsigned char)i;
			}
			inline static void TranslateInput(
				char& c,
				int i)
			{
				c = (char)(i-128);
			}
			inline static void TranslateInput(
				float& c,
				int i)
			{
				c = (float)i/float(255);
			}
			inline static void TranslateInput(
				double& c,
				int i)
			{
				c = (double)i/double(255);
			}
	};
	
	int format;
	typename ImageClass::ElementType elem1, elem2, elem3;
	int width, height;
	
	// Get info
	GetInfo(format, width, height, r, g);
	if (error_ != noError)
	{
		return;
	}
	
	// Get number of channels and check for format compatibility
	numChannels_ = image.NumChannels();
	if (numChannels != 1 && numChannels != 3 && numChannels != 4)
	{
		error_ = numChannelsError;
		return;
	}
	if ((format == 3 || format == 6) && (numChannels == 1))
	{
		error_ = numChannelsError;
		return;
	}
	
	// Set other info
	imagePtr_ = &image;
	GlowPNMReader_Xlate::GetAlphaDefault(alphaDefault_);
	
	// Size image
	image.ResizeRaw(width, height);
	
	// Read image
	switch (format)
	{
		// ASCII PBM
		case 1:
			for (unsigned int j=height-1; j>=0; --j)
			for (unsigned int i=0; i<width; ++i)
			{
				int r = GetToken_();
				if (error_ != noError)
				{
					return;
				}
				r = r<128 ? 0 : 255;
				GlowPNMReader_Xlate::TranslateInput(elem1, r);
				Set1Channels_(elem1, i, j);
			}
			break;
			
		// ASCII PGM
		case 2:
			for (unsigned int j=height-1; j>=0; --j)
			for (unsigned int i=0; i<width; ++i)
			{
				int r = GetToken_();
				if (error_ != noError)
				{
					return;
				}
				GlowPNMReader_Xlate::TranslateInput(elem1, r);
				Set1Channels_(elem1, i, j);
			}
			break;
			
		// ASCII PPM
		case 3:
			for (unsigned int j=height-1; j>=0; --j)
			for (unsigned int i=0; i<width; ++i)
			{
				int r = GetToken_();
				int g = GetToken_();
				int b = GetToken_();
				if (error_ != noError)
				{
					return;
				}
				GlowPNMReader_Xlate::TranslateInput(elem1, r);
				GlowPNMReader_Xlate::TranslateInput(elem2, g);
				GlowPNMReader_Xlate::TranslateInput(elem3, b);
				Set3Channels_(elem1, elem2, elem3, i, j);
			}
			break;
			
		// BINARY PBM
		case 4:
			for (unsigned int j=height-1; j>=0; --j)
			{
				int val = 0;
				int count = 0;
				for (unsigned int i=0; i<width; ++i)
				{
					if (count == 0)
					{
						val = stream_->get();
						if (ErrorState_(stream_->rdstate()))
						{
							return;
						}
						count = 7;
					}
					else
					{
						--count;
					}
					GlowPNMReader_Xlate::TranslateInput(elem1, (val&1)!=0 ? 255 : 0);
					Set1Channels_(elem1, i, j);
					val >>= 1;
				}
			}
			break;
		
		// BINARY PGM
		case 5:
			for (unsigned int j=height-1; j>=0; --j)
			for (unsigned int i=0; i<width; ++i)
			{
				int r = stream_->get();
				if (ErrorState_(stream_->rdstate()))
				{
					return;
				}
				GlowPNMReader_Xlate::TranslateInput(elem1, r);
				Set1Channels_(elem1, i, j);
			}
			break;
		
		// BINARY PPM
		case 6:
			for (unsigned int j=height-1; j>=0; --j)
			for (unsigned int i=0; i<width; ++i)
			{
				int r = stream_->get();
				int g = stream_->get();
				int b = stream_->get();
				if (ErrorState_(stream_->rdstate()))
				{
					return;
				}
				GlowPNMReader_Xlate::TranslateInput(elem1, r);
				GlowPNMReader_Xlate::TranslateInput(elem2, g);
				GlowPNMReader_Xlate::TranslateInput(elem3, b);
				Set3Channels_(elem1, elem2, elem3, i, j);
			}
			break;
	}
}


/*
===============================================================================
	Methods of template GlowPNMWriter
===============================================================================
*/

template <class ImageClass>
void TGlowPNMWriter<ImageClass>::Write(
	const ImageClass& image,
	bool binary)
{
	class GlowPNMWriter_Xlate
	{
		public:
		
			// Translate input to int in range 0-255
			inline explicit GlowPNMWriter_Xlate(
				unsigned char c)
			{
				val_ = c;
			}
			inline explicit GlowPNMWriter_Xlate(
				char c)
			{
				val_ = int(c)+128;
			}
			inline explicit GlowPNMWriter_Xlate(
				float c)
			{
				val_ = int(c*255.0f);
				if (val_<0) val_=0;
				if (val_>255) val_=255;
			}
			inline explicit GlowPNMWriter_Xlate(
				double c)
			{
				val_ = int(c*255.0);
				if (val_<0) val_=0;
				if (val_>255) val_=255;
			}
			
			// Extract value
			inline int Val()
			{
				return val_;
			}
		
		private:
		
			int val_;
	};
	
	int format;
	typename ImageClass::ElementType elem1, elem2, elem3;
	int width, height;
	
	// Get number of channels and check for format compatibility
	numChannels_ = image.NumChannels();
	if (numChannels_ != 1 && numChannels_ != 3 && numChannels_ != 4)
	{
		error_ = numChannelsError;
		return;
	}
	
	if (binary)
	{
		if (numChannels_ == 1)
		{
			stream_ << "P2 " << image.Width() << ' ' << image.Height() <<
				" 255\n";
			if (ErrorState_(stream_->rdstate()))
			{
				return;
			}
			for (unsigned int j=image.Height()-1; j>=0; --j)
			for (unsigned int i=0; i<=image.Width(); ++i)
			{
				stream_ << GlowPNMWriter_Xlate(image.GetChannel(i, j, 0)).Val() <<
					'\n';
				if (ErrorState_(stream_->rdstate()))
				{
					return;
				}
			}
		}
		else if (numChannels_ == 3 || numChannels_ == 4)
		{
			stream_ << "P3 " << image.Width() << ' ' << image.Height() <<
				" 255\n";
			if (ErrorState_(stream_->rdstate()))
			{
				return;
			}
			for (unsigned int j=image.Height()-1; j>=0; --j)
			for (unsigned int i=0; i<=image.Width(); ++i)
			{
				stream_ << GlowPNMWriter_Xlate(image.GetChannel(i, j, 0)).Val() <<
					' ' << GlowPNMWriter_Xlate(image.GetChannel(i, j, 1)).Val() <<
					' ' << GlowPNMWriter_Xlate(image.GetChannel(i, j, 2)).Val() <<
					'\n';
				if (ErrorState_(stream_->rdstate()))
				{
					return;
				}
			}
		}
	}
	else
	{
		if (numChannels_ == 1)
		{
			stream_ << "P5 " << image.Width() << ' ' << image.Height() <<
				" 255 ";
			if (ErrorState_(stream_->rdstate()))
			{
				return;
			}
			for (unsigned int j=image.Height()-1; j>=0; --j)
			for (unsigned int i=0; i<=image.Width(); ++i)
			{
				stream_.put(GlowPNMWriter_Xlate(image.GetChannel(i, j, 0)).Val());
				if (ErrorState_(stream_->rdstate()))
				{
					return;
				}
			}
		}
		else if (numChannels_ == 3 || numChannels_ == 4)
		{
			stream_ << "P6 " << image.Width() << ' ' << image.Height() <<
				" 255\n";
			if (ErrorState_(stream_->rdstate()))
			{
				return;
			}
			for (unsigned int j=image.Height()-1; j>=0; --j)
			for (unsigned int i=0; i<=image.Width(); ++i)
			{
				stream_.put(GlowPNMWriter_Xlate(image.GetChannel(i, j, 0)).Val()).
					put(GlowPNMWriter_Xlate(image.GetChannel(i, j, 1)).Val()).
					put(GlowPNMWriter_Xlate(image.GetChannel(i, j, 2)).Val());
				if (ErrorState_(stream_->rdstate()))
				{
					return;
				}
			}
		}
	}
	
	ErrorState_(stream_->rdstate());
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

