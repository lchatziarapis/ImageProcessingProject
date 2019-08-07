#include "Block.h"



imaging::Block::Block(const size_t & block_size)
{
	this->data = new Component[block_size];
	this->size = block_size;
	this->error_margin = 0;
}

imaging::Block::Block(const Block & src)
{
	this->data = new Component[src.size];
	this->size = src.size;
	this->error_margin = src.error_margin;
	setData(src.data);

}

imaging::Block::~Block()
{
	if (this->data) delete[] data;
}

imaging::Block * imaging::Block::copyFromImage(imaging::Image & src, imaging::Image::channel_t channel, const ivec2 & pos, const size_t &size)
{
	if (size == 0 || pos.y > src.getHeight() - 1)
	{
		return nullptr;
	}
	//changes start here
	static Block * block;

	size_t sizen = size;
	if (sizen> src.getWidth() - pos.x)
	{
		sizen = src.getWidth() - pos.x;
	}
	block = new Block(sizen);
	//end here

	if (channel == Image::RED)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			block->data[i] = *(src.getRawDataPtr() + pos.x + src.getWidth()*pos.y + i);
		}
	}
	else if (channel == Image::GREEN)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			block->data[i] = *(src.getRawDataPtr() + pos.x + src.getWidth()*pos.y + i + src.getWidth()*src.getHeight());
		}
	}
	else
	{
		for (unsigned int i = 0; i < size; i++)
		{
			block->data[i] = *(src.getRawDataPtr() + pos.x + src.getWidth()*pos.y + i + 2 * src.getWidth()*src.getHeight());
		}
	}

	return block;
}

size_t imaging::Block::getSize() const
{
	return size;
}

imaging::Component * imaging::Block::getDataPtr() const
{
	return data;
}

void imaging::Block::setData(const Component * src)
{
	memcpy(data, src, sizeof(Component)*size);
}

imaging::Block imaging::Block::reverse() const
{
	for (unsigned int i = 0; i < size / 2; i++)
	{
		Component temp = data[i];
		data[i] = data[size - i - 1];
		data[size - i - 1] = temp;
	}
	return *this;
}

void imaging::Block::setErrorMargin(Component err)
{
	this->error_margin = err;
}

imaging::Component & imaging::Block::operator[] (const size_t index) const
{
	return data[index];
}

imaging::Component & imaging::Block::operator() (const size_t & index) const
{
	if (index >= 0 && index < size)
	{
		return data[index];
	}
	else
	{
		return data[size - 1];
	}
}

bool imaging::Block::operator==(const Block& rhs) const
{
	if (error_margin != 0)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if (abs(data[i] - rhs.data[i]) >(int)error_margin)
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if (rhs[i] != data[i])
			{
				return false;
			}
		}
		return true;
	}
}

bool imaging::Block::operator!=(const Block& rhs) const
{
	for (unsigned int i = 0; i < size; i++)
	{
		if (rhs[i] != data[i])
		{
			return true;
		}
	}
	return false;
}

imaging::Block & imaging::Block::operator=(const Block & src)
{
	if (size < src.size)
	{
		memcpy(data, src.data, sizeof(Component)*size);
	}
	else
	{
		memcpy(data, src.data, sizeof(Component)*src.size);
	}
	return *this;
}