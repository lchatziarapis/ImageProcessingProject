//------------------------------------------------------------
//
// 
//
// G. Papaioannou, 2014
//
//
//-------------------------------------------------------------

#ifndef _FILTER
#define _FILTER

#include "Image.h"

namespace imaging
{
	class Filter
	{
		// Προσθέστε αν θέλετε πεδία και μεθόδους
		Image * img;
	public:

		Filter(Image & image)
		{
			img = new Image(image);
		}
		Filter() {};
		virtual ~Filter() { delete img; }
		virtual Image * getImage() { return img; }
		virtual void setImage(Image & image) { img = new Image(image); }
	};

}
#endif