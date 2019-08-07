
#include "rle_format.h"
#include "Block.h"
#include <fstream>
#include <iostream>


imaging::Block * to_Rle(imaging::Block & block);
imaging::Block * to_lossyRle(imaging::Block & block, imaging::Component error);

void imaging::RLEImageWriter::write(std::string filename, const Image & src)
{

	std::ifstream file_exists(filename, std::ios::binary);
	std::cout << "Writing a rle file with name " + filename + "." << std::endl;
	createLogger("Log.txt");
	addLogEntry("Begin the process for writing a rle file!.");
	if (file_exists) // check for overwriting
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

	char cpi[3] = { 'C', 'P', 'I' };
	unsigned char version = 2;
	unsigned short width = src.getWidth();
	unsigned short height = src.getHeight();
	unsigned short bit = 258;
	unsigned short b_size = block_length;

	//Writing the header
	file.write(cpi, 3);
	file.write((char*)&version, 1);
	file.write((char*)&bit, 2);
	file.write((char*)&width, 2);
	file.write((char*)&height, 2);
	file.write((char*)&b_size, 2);

	int n_blocks = (int)ceil(width / (float)b_size); // Calculate the number of blocks.
	int b_last = width - (n_blocks - 1)* b_size; // Calculate the size of the last in block.

	Image im = src;
	ivec2 vector(0, 0);
	Image::channel_t arr[] = { src.RED, src.GREEN, src.BLUE };

	if (threshold != 0)
	{
		for (Image::channel_t c : arr)
		{
			for (int y = 0; y < height; y++)
			{
				vector.x = 0;
				vector.y = y;
				for (int i = 0; i < n_blocks - 1; i++)
				{
					Block * blk = to_lossyRle(*Block::copyFromImage(im, c, vector, b_size), threshold);
					vector.x += b_size;
					file.write((char*)blk->getDataPtr(), blk->getSize());
					delete blk;
				}
				Block * blk = to_lossyRle(*Block::copyFromImage(im, c, vector, b_last), threshold);
				vector.x += b_last;
				file.write((char*)blk->getDataPtr(), blk->getSize());
				delete blk;
			}
		}
	}
	else
	{
		for (Image::channel_t c : arr)
		{
			for (int y = 0; y < height; y++)
			{
				vector.x = 0;
				vector.y = y;
				for (int i = 0; i < n_blocks - 1; i++)
				{
					Block * blk = to_Rle(*Block::copyFromImage(im, c, vector, b_size));
					vector.x += b_size;
					file.write((char*)blk->getDataPtr(), blk->getSize());
					delete blk;
				}
				Block * blk = to_Rle(*Block::copyFromImage(im, c, vector, b_last));
				vector.x += b_last;
				file.write((char*)blk->getDataPtr(), blk->getSize());
				delete blk;
			}
		}
	}
	addLogEntry("Operation Complete!!!.");
	imaging::destroyLogger();
	file.close();
	file_exists.close();
}

imaging::Block  * to_Rle(imaging::Block & block)
{
	int counter = 0;
	size_t i = 0;
	int index = 0;
	imaging::Component * arr = new imaging::Component[2 * block.getSize()];

	while (i < block.getSize())
	{
		counter = 1;
		imaging::Component value = block.getDataPtr()[i];
		i++;
		while (i < block.getSize() && value == block.getDataPtr()[i])
		{
			counter++;
			i++;
		}
		arr[index] = value;
		arr[index + 1] = counter;
		index = index + 2;
	}
	imaging::Block * blk = new imaging::Block(index);

	blk->setData(arr);
	delete[] arr;
	return blk;
}

imaging::Block * to_lossyRle(imaging::Block & block, imaging::Component error)
{
	int counter = 0;
	size_t i = 0;
	int index = 0;
	imaging::Component * arr = new imaging::Component[2 * block.getSize()];

	while (i < block.getSize())
	{
		counter = 1;
		imaging::Component value = block[i];
		i++;
		while (i < block.getSize() && abs((block[i] - value)) <= (int)error)
		{
			counter++;
			i++;
		}
		arr[index] = value;
		arr[index + 1] = counter;
		index = index + 2;
	}
	imaging::Block * blk = new imaging::Block(index);
	blk->setData(arr);
	delete[] arr;
	return blk;

}


