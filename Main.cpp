#include <unistd.h>
#include "Parser.hpp"
#include "Octree.hpp"

extern	Octree *Root;
Color	Palette[256];

void nextColor(Parser& p, Color &color)
{
	p.getColor(color);
	initializeNode(&Root, 0, color);
}

int main(int argc, char *argv[])
{
	int leafCount = 0;

	if (argc != 2)
	{
		fprintf(stdout, "usage: %s image.tga\n", argv[0]);
		exit(0);
	}

	std::string	filename = argv[1];
	Parser		p { filename, Format::B24 };
	Color		currentColor;

	for(int y = 0; y < p.getHeight(); y++)
	{
		for(int x = 0; x < p.getWidth(); x++)
			nextColor(p, currentColor);
	}

	createPalette(Root, Palette, &leafCount);
	fprintf(
	stdout,
	"Obtained an octree of %d leafs for a %dx%d TGA.\n",
	leafCount,
	p.getWidth(),
	p.getHeight());

	for (int i = 0; i < leafCount; i++)
	{
		fprintf(
		stdout,
		"#%02X%02X%02X\n",
		Palette[i].r,
		Palette[i].g,
		Palette[i].b
		);
	}
}
