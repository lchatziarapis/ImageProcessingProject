#include "LocalFilter.h"
#include "NeighborFilter.h"
#include "ppm_format.h"
#include "rle_format.h"
#include "Color.h"
#include <iostream>
#include <fstream>
#include <vector>


void addFilter(imaging::Image & img, std::vector<int> vec);
imaging::Color c;

int main(int argc, char * argv[])
{
	imaging::createLogger("Log.txt");
	std::vector<int> vec;
	std::cout << "Filtering Operation!!!!.";
	// Numbers for the filters
	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-f")
		{
			if (i != argc - 1)
			{
				if (std::string(argv[i + 1]) == "gray")
				{
					vec.push_back(0);
				}
				else if (std::string(argv[i + 1]) == "color")
				{
					vec.push_back(1);
					if (i + 2 < argc && i + 3 < argc && i + 4 < argc)
					{
						c.x = atoi(argv[i + 2]);
						c.x = atoi(argv[i + 3]);
						c.x = atoi(argv[i + 4]);
					}
					else
					{
						break;
					}
				}
				else if (std::string(argv[i + 1]) == "blur")
				{
					vec.push_back(2);
				}
				else if (std::string(argv[i + 1]) == "median")
				{
					vec.push_back(3);
				}
				else if (std::string(argv[i + 1]) == "diff")
				{
					vec.push_back(4);
				}
				else
				{
					std::cout << "Error : missing argument" << std::endl;
				}
				i++;
			}
			
		}
	}

	if (imaging::isSuffix(std::string(argv[argc - 1]), "ppm"))
	{
		imaging::PPMImageReader reader;
		imaging::Image * image = reader.read(std::string(argv[argc - 1]));

		std::string filename = std::string(argv[argc - 1]);
		imaging::replaceSuffix(filename, "filtered");

		addFilter(*image, vec);

		imaging::PPMImageWriter writer;
		writer.write(filename+".ppm",*image);
	}
	else if (imaging::isSuffix(std::string(argv[argc - 1]), "rle"))
	{
		imaging::RLEImageReader reader;
		imaging::Image * image = reader.read(std::string(argv[argc - 1]));

		std::string filename = std::string(argv[argc - 1]);
		imaging::replaceSuffix(filename, "filtered");

		addFilter(*image, vec);

		imaging::PPMImageWriter writer;
		writer.write(filename + ".rle", *image);
	}
	else
	{
		std::cout << "No filename!!." << std::endl;
	}

	system("PAUSE");
	return 0;
}

void addFilter(imaging::Image & img,std::vector<int> vec)
{
	imaging::LocalFilter l_filter;
	imaging::NeighborFilter n_filter;

	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == 0) // Filter for gray
		{
			l_filter.toGrayEffect(img);
		}
		else if (vec[i] == 1) // Filter for color
		{
			l_filter.toColorEffect(img, c);
		}
		else if (vec[i] == 2) // Filter for Blur
		{
			n_filter.addBlurEffect(img);
		}
		else if (vec[i] == 3) // Filter for Median
		{
			n_filter.addMedianEffect(img);
		}
		else if (vec[i] == 4) // Filter for Diff
		{
			n_filter.addDiffEffect(img);
		}
	}
}