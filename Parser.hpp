#ifndef _PARSER_H
#define _PARSER_H

#include <cstdio>
#include <string>
#include "Format.hpp"
#include "Color.hpp"

class Parser
{	private:
	FILE	*fp;
	uint16_t width, height;

	public:
	Parser(std::string&, Format);
	~Parser();

	void	setOffset(int)	const;
	uint8_t getByte() const;
	void	getColor(Color&) const;

	uint16_t getWidth() const;
	uint16_t getHeight() const;
};

#endif
