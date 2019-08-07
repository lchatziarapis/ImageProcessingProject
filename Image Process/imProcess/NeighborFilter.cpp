#include "NeighborFilter.h"
#include <vector>
#include <algorithm>
#include <array>
#include <iostream>


void insertionSort(imaging::Component a[], int array_size)
{
	int i, j, index;
	for (i = 1; i < array_size; ++i)
	{
		index = a[i];
		for (j = i; j > 0 && a[j - 1] > index; j--)
			a[j] = a[j - 1];

		a[j] = index;
	}
}

void imaging::NeighborFilter::addBlurEffect(Image & image)
{
	Image new_img = image;
	float counter = 0;
	vec3_t<float> color;
	
	for (unsigned int y = 0; y < image.getHeight(); y++)
	{
		for (int x = 0; x < image.getWidth(); x++)
		{
			
			color.x = 0;
			color.y = 0;
			color.z = 0;

			for (int m = -1; m <= 1; m++)
			{
				for (int n = -1; n <= 1; n++)
				{
					if (x + m >= 0 && x + m < image.getWidth() && y + n >= 0 && y + n < image.getHeight())
					{
						Color c = image.getPixel(x + m, y + n);
						color.x += c.x;
						color.y += c.y;
						color.z += c.z;
						counter++;
					}
				}
			}
			vec3_t<float> * new_color = new vec3_t<float>((color.x / counter), (color.y / counter), (color.z / counter));
			Color col;
			col.x = new_color->x;
			col.y = new_color->y;
			col.z = new_color->z;


			counter = 0;
			new_img.setPixel(x, y, col);
			delete new_color;
		}
	}
	const Component * point = new_img.getRawDataPtr();
	image.setData(point);
}

void imaging::NeighborFilter::addMedianEffect(Image & image)
{
	Image new_img = image;
	
	Color c;
	
	Color color;
	Component RED[9];
	Component GREEN[9];
	Component BLUE[9];
	for (int y = 0; y < image.getHeight(); y++)
	{
		for (int x = 0; x < image.getWidth(); x++)
		{
			

			int Ai = 0;
			int Bi = 0;
			int Ci = 0;

			for (int m = -1; m <= 1; m++)
			{
				for (int n = -1; n <= 1; n++)
				{
					if (x + m >= 0 && x + m < image.getWidth() && y + n >= 0 && y + n < image.getHeight())
					{
						c = image.getPixel(x + m, y + n);
						RED[Ai++] = c.x;
						GREEN[Bi++] = c.y;
						BLUE[Ci++] = c.z;
					}
				}
			}
			insertionSort(RED, Ai);
			insertionSort(GREEN, Bi);
			insertionSort(BLUE, Ci);

			
			color.x = (int)RED[Ai/2];
			color.y = (int)GREEN[Bi / 2];
			color.z = (int)BLUE[Ci / 2];
			new_img.setPixel(x, y, color);
		}
	}
	const Component * point = new_img.getRawDataPtr();
	image.setData(point);
}

void imaging::NeighborFilter::addDiffEffect(Image & image)
{
	Image new_img = image;

	Component RED[9];
	Component GREEN[9];
	Component BLUE[9];
	Color c;


	for (int y = 0; y < image.getHeight(); y++)
	{
		for (int x = 0; x < image.getWidth(); x++)
		{
			int Ai = 0;
			int Bi = 0;
			int Ci = 0;

			for (int m = -1; m <= 1; m++)
			{
				for (int n = -1; n <= 1; n++)
				{
					if (x + m >= 0 && x + m < image.getWidth() && y + n >= 0 && y + n < image.getHeight())
					{
						Color c = image.getPixel(x + m, y + n);
						RED[Ai++] = c.x;
						GREEN[Bi++] = c.y;
						BLUE[Ci++] = c.z;
					}
				}
			}
			insertionSort(RED, Ai);
			insertionSort(GREEN, Bi);
			insertionSort(BLUE, Ci);
			
			c.x = RED[Ai-1] - RED[0];
			c.y = GREEN[Bi-1] - GREEN[0];
			c.z = BLUE[Ci-1] - BLUE[0];

			new_img.setPixel(x, y, c);
			
		}
	}
	const Component * point = new_img.getRawDataPtr();
	image.setData(point);
}