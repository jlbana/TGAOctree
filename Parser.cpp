#include <stdexcept>
#include "Parser.hpp"

Parser::Parser(std::string &filename, Format fmt) : fp(NULL)
{
	if (fmt != Format::B24)
	{
		throw std::runtime_error("unsupported TGA format");
	}

	fp = fopen(filename.c_str(), "r");

	if (fp)
	{
		setOffset(12);
		width	= (getByte() | (getByte() << 8));
		height	= (getByte() | (getByte() << 8));
		setOffset(18);
	}
	else
	{
		throw std::runtime_error("File not found");
	}
}

Parser::~Parser()
{
	if (fp) fclose(fp);
}

void Parser::setOffset(int offset) const
{
	if (fp == NULL)
	{
		return;
	}

	fseek(fp, offset, SEEK_SET);
}

uint8_t Parser::getByte() const
{
	return static_cast<uint8_t>(fgetc(fp));
}

void Parser::getColor(Color& color) const
{
	color.b = getByte();
	color.g = getByte();
	color.r = getByte();
}

uint16_t Parser::getWidth() const
{
	return width;
}

uint16_t Parser::getHeight() const
{
	return height;
}
