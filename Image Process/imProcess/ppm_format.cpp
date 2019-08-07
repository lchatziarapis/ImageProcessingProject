
#include "ppm_format.h"
#include "rle_format.h"
#include <fstream>
#include <iostream>
#include <vector>

// Insert implementation code for the PPMImageWriter and PPMImageReader classes here
//

void imaging::PPMImageWriter::write(std::string filename, const Image &src)
{
	std::ifstream file_exists(filename, std::ios::binary);
	std::cout << "Writing a ppm file with name " + filename + "." << std::endl;
	createLogger("Log.txt");

	if (file_exists)
	{
		std::string answer;
		do
		{
			std::cout << "The file " + filename + " exists.Overwrite? (y)es / (n)o : ";
			std::cin >> answer;
		} while (answer != "y" && answer != "Y" && answer != "n" && answer != "N");

		if (answer == "n" || answer == "N")
		{
			addLogEntry("Didn't overwrite " + filename + " file.");
			std::cout << "Didn't overwrite " + filename + " file." << std::endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}

	std::ofstream file(filename, std::ios::binary);
	file << "P6 " << src.getWidth() << " " << src.getHeight() << " " << "255\n";
	Image image = Image(src);
	file.write((char *)image.getRawDataPtr(), 3 * src.getWidth()*src.getHeight());
	addLogEntry("File successfuly written!!.");
	std::cout << "File " + filename + " successfully has been written!!" << std::endl;

	imaging::destroyLogger();
	file_exists.close();
	file.close();
}


imaging::Image * imaging::PPMImageReader::read(std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	imaging::createLogger("Log.txt");

	if (!file)
	{
		addLogEntry("Couldn't find the file " + filename + ".");
		std::cout << "We couldn't find " + filename + ".Please check your files!!." << std::endl;
		std::system("PAUSE");
		imaging::destroyLogger();
		file.close();
		exit(EXIT_FAILURE);
	}
	else
	{
		std::string version;
		std::string comment;
		char * c = new char[];
		unsigned int width, height, bit;
		int position;
	

		file.seekg(0, file.end);
		int size = file.tellg();
		file.seekg(0, file.beg);

		file >> version;
		if (version == "P6")
		{
			file >> width;

			char next;
			while (file.get(next))
			{
				if (next != '\r')
				{
					if (isdigit(next))
					{
						file.unget();
						file >> height;
						break;
					}
					if (next == '#')
					{
						while (file.get(next))
						{
							if (next == '\r')
							{
								file >> height;
								break;
							}
						}
						break;
					}
					continue;
				}
				else
				{
					file >> height;
					break;
				}
			}

			while (file.get(next))
			{
				if (next != '\r')
				{
					if (isdigit(next))
					{
						file.unget();
						file >> bit;
						break;
					}

					if (next == '#')
					{
						while (file.get(next))
						{
							if (next == '\r')
							{
								break;
							}
						}
						break;
					}
					continue;
				}
				else
				{
					file >> bit;
					break;
				}
			}

			file.ignore();
		}
		
		
		Image * image = new Image(width, height, true);
		const Component * buffer = new Component[size - file.tellg()];
		file.read((char *)buffer, size - file.tellg());
		image->setData(buffer);

		delete buffer;
		addLogEntry("Filename: " + filename + " has been successfully read.");
		file.close();
		imaging::destroyLogger();
		return image;

	}
}


imaging::Image * imaging::RLEImageReader::read(std::string filename)
{
	std::ifstream file_exists(filename, std::ios::binary);
	imaging::createLogger("Log.txt");


	if (!file_exists)
	{
		addLogEntry("Couldn't find the file " + filename + ".");
		std::cout << "We couldn't find " + filename + ".Please check your files!!." << std::endl;
		std::system("PAUSE");
		file_exists.close();
		exit(EXIT_FAILURE);
	}
	else
	{
		std::ifstream file(filename, std::ios::binary);
		unsigned short width, height;
		unsigned short block_size;

		char * CPI = new char[3];
		file.read(CPI, sizeof(char) * 3);

		if (CPI[0] != 'C' && CPI[1] != 'P' && CPI[2] != 'I')
		{
			addLogEntry("Couldn't read the header of the file " + filename + ".");
			std::cout << "Error in Header" << std::endl;
			std::system("PAUSE");
			exit(EXIT_FAILURE);
		}

		char * version = new char[1];
		file.read(version, sizeof(char));
		if (version[0] != 2)
		{
			addLogEntry("Couldn't read the header of the file " + filename + ".");
			std::cout << "Error in Header" << std::endl;
			std::system("PAUSE");
			exit(EXIT_FAILURE);
		}

		unsigned short endian;
		file.read((char *)&endian, sizeof(unsigned short));
		if (endian != 258)
		{
			addLogEntry("Couldn't read the header of the file " + filename + ".");
			std::cout << "Error in Header" << std::endl;
			std::system("PAUSE");
			exit(EXIT_FAILURE);
		}

		file.read((char*)&width, 2);
		file.read((char*)&height, 2);
		file.read((char*)&block_size, 2);

		std::vector<Component> vect;
		while (!file.eof())
		{
			Component x = file.get();
			Component n = file.get();

			for (int i = 0; i < n; i++)
			{
				vect.push_back(x);
			}
		}

		Component * buffer = new Component[vect.size()];

		for (int i = 0; i < vect.size(); i++)
		{
			buffer[i] = vect[i];
		}
		Image * image = new Image(width, height, buffer, false);


		imaging::destroyLogger();
		file.close();
		vect.clear();
		delete[] CPI;
		delete[] buffer;
		delete[] version;
		return image;
	}
	file_exists.close();
	imaging::destroyLogger();
}
