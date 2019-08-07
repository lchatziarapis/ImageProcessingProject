#include "LocalFilter.h"


void imaging::LocalFilter::toGrayEffect(Image & image)
{
	for (int y = 0; y < image.getHeight(); y++)
	{
		for (int x = 0; x < image.getWidth(); x++)
		{
			Color c = image.getPixel(x, y);
			int m = ((float)c.x + (float)c.y + (float)c.z) / 3;
			c.x = c.y = c.z = m;
			image.setPixel(x, y, c);
		}
	}
}

void imaging::LocalFilter::toColorEffect(Image & image,Color color)
{
	for (int y = 0; y < image.getHeight(); y++)
	{
		for (int x = 0; x < image.getWidth(); x++)
		{
			Color cc = image.getPixel(x, y);
			vec3_t<float> * new_color = new vec3_t<float>((color.x / 255.0f)*cc.x,(color.y / 255.0f)*cc.y,(color.z / 255.0f)*cc.z);
			Color col;
			col.x = new_color->x;
			col.y = new_color->y;
			col.z = new_color->z;
			image.setPixel(x, y, col);
			delete new_color;
		}
	}
}