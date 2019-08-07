#include "cpi_format.h"
#include <fstream>
#include <iostream>

//Insert implementation code for the CPIImageWriter and CPIImageReader classes here

void imaging::CPIImageWriter::write(std::string filename, const Image &src)
{
	std::ifstream file_exists(filename, std::ios::binary);


	if (file_exists)
	{
		std::ofstream file(filename, std::ios::binary);
		createLogger("Log.txt");
		std::cout << "The file " + filename + " exists.Overwrite? (y)es / (n)o : ";
		std::string answer;
		std::cin >> answer;

		if (answer == "y" || answer == "Y")
		{
			char cpi[3] = { 'C', 'P', 'I' };
			unsigned char num = 1;
			unsigned short width = src.getWidth();
			unsigned short height = src.getHeight();
			unsigned short eden = 258;
			file.write(cpi, sizeof(cpi));
			file.write((char*)&num, 1);
			file.write((char*)&eden, 2);
			file.write((char*)&width, 2);
			file.write((char*)&height, 2);
			Image image = Image(src);
			file.write((char *)image.getRawDataPtr(), 3 * src.getWidth()*src.getHeight());
		}
		else if (answer == "n" || answer == "N")
		{
			std::cout << "File " + filename + " didn't overwritten!!" << std::endl;
			addLogEntry("Didn't overwrite " + filename + " file.");
		}
		file.close();
	}
	else
	{
		std::ofstream file(filename, std::ios::binary);
		createLogger("Log.txt");
		char cpi[3] = { 'C', 'P', 'I' };
		unsigned char num = 1;
		unsigned short width = src.getWidth();
		unsigned short height = src.getHeight();
		unsigned short eden = 258;
		file.write(cpi, sizeof(cpi));
		file.write((char*)&num, 1);
		file.write((char*)&eden, 2);
		file.write((char*)&width, 2);
		file.write((char*)&height, 2);
		Image image = Image(src);
		file.write((char *)image.getRawDataPtr(), 3 * src.getWidth()*src.getHeight());
		addLogEntry("File " + filename + " is written!.");
		file.close();
	}
	imaging::destroyLogger();
	file_exists.close();
}


imaging::Image * imaging::CPIImageReader::read(std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	imaging::createLogger("Log.txt");

	if (!file)
	{
		addLogEntry("Couldn't find the file " + filename + ".");
		std::cout << "We couldn't find " + filename + ".Please check your files!!." << std::endl;
		std::cin.get();
		imaging::destroyLogger();
		file.close();
		exit(EXIT_FAILURE);
	}
	else
	{
		char  cpi[3];
		unsigned short width, height;

		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);

		for (int i = 0; i < 6; i++)
		{
			file.ignore();
		}

		file.read((char*)&width, 2);
		file.read((char*)&height, 2);

		Image * image = new Image(width, height, false);
		const Component * buffer = new Component[length - file.tellg()];
		file.read((char *)buffer, length - file.tellg());
		image->setData(buffer);

		delete buffer;
		addLogEntry("Filename: " + filename + " has been successfully read.");
		file.close();
		imaging::destroyLogger();
		return image;
	}
}

