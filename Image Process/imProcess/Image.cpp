//------------------------------------------------------------
//
// 
//
// G. Papaioannou, 2013
//
//

#include "Image.h"
#include <iostream>
#include <algorithm>



namespace imaging
{

	//-------------------------------- Image class implementation goes here ---------------------------------------------------

	Image::Image(unsigned int width, unsigned int height, bool interleaved)
	{
		this->width = width;
		this->height = height;
		this->interleaved = interleaved;
		buffer = new Component[3 * width*height];
	}

	Image::Image(unsigned int width, unsigned int height, const Component * data_ptr, bool interleaved)
	{
		this->width = width;
		this->height = height;
		this->interleaved = interleaved;
		buffer = new Component[3 * width*height];
		this->setData(data_ptr);
	}

	Image::Image(const Image &src)
	{

		this->width = src.width;
		this->height = src.height;
		this->interleaved = src.interleaved;
		buffer = new Component[3 * width*height];
		const Component * point = src.buffer;
		setData(point);
	}

	Image::~Image() //  Delete the buffer in order to prevent memory leak.
	{
		if (buffer) delete[] buffer;
	}

	Component * Image::getRawDataPtr() //  Return the data pointer of the image.
	{
		return buffer;
	}

	Color Image::getPixel(unsigned int x, unsigned int y) const // Return a color with three colors(RED,GREEN,BLUE)
	{
		Color c;
		if (x < 0 || y < 0 || x > width || y > height)
		{
			c.x = 0;
			c.y = 0;
			c.z = 0;
			return c;
		}
		else
		{

			if (isInterleaved())
			{
				int position = 3 * x + 3 * y * width; // Three bit for the three colors(RED,GREEN,BLUE) plus the number of these
				c.x = *(buffer + position);
				c.y = *(buffer + position + 1);
				c.z = *(buffer + position + 2);
			}
			else
			{
				int position = x + y*width;
				c.x = *(buffer + position);
				c.y = *(buffer + position + width*height);
				c.z = *(buffer + position + 2 * width*height);

			}
		}
		return c;
	}


	void Image::setPixel(unsigned int x, unsigned int y, Color value)
	{
		if (x < 0 || y < 0 || x > width || y > height || value.x < 0 || value.x > 255 || value.y < 0 || value.y > 255 || value.z < 0 || value.z >255)
		{
			std::cout << "You can't set a pixel outside the boundaries.";
			return;
		}

		if (isInterleaved())
		{
			Component * p1 = (buffer + 3 * x + 3 * y*width);
			*p1 = value.x;
			*(p1 + 1) = value.y;
			*(p1 + 2) = value.z;
		}
		else
		{
			Component * p2 = (buffer + x + y*width);
			*p2 = value.x;
			*(p2 + width*height) = value.y;
			*(p2 + 2 * width*height) = value.z;
		}
	}

	void Image::setData(const Component * &data_ptr)
	{
		int size = 3 * width * height;
		memcpy(buffer, data_ptr, sizeof(Component)*size);
	}

	void Image::convertToInterleavedImage()
	{
		Image * image = new Image(*this);
		image->interleaved = true;
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				image->setPixel(x, y, getPixel(x, y));
			}

		}
		buffer = image->buffer;
		interleaved = true;
	}

	void Image::convertToColorPlaneImage()
	{
		Image * image = new Image(*this);
		image->interleaved = false;
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				image->setPixel(x, y, getPixel(x, y));
			}
		}

		buffer = image->buffer;
		interleaved = false;
	}


	//-------------------------------- Helper functions -----------------------------------------------------------------------

	bool isSuffix(std::string & filename, const std::string suffix)
	{
		std::string low_caps_filename = filename;
		std::string dot_suffix = '.' + suffix;

		// make the string lowercase.
		std::transform(low_caps_filename.begin(), low_caps_filename.end(), low_caps_filename.begin(), ::tolower);

		//make the suffix lowercase.
		std::transform(dot_suffix.begin(), dot_suffix.end(), dot_suffix.begin(), ::tolower);

		size_t pos = low_caps_filename.rfind(dot_suffix);

		if (pos == std::string::npos) // did not find it
			return false;

		if (pos < low_caps_filename.length() - dot_suffix.length()) // found it, but not at the end
			return false;

		return true; // now we are positive. :)
	}

	void replaceSuffix(std::string & filename, const std::string suffix)
	{
		size_t pos = filename.rfind('.');
		if (pos == std::string::npos)
			return;

		filename.resize(pos + 1);
		filename += suffix;
	}


} // namespace imaging