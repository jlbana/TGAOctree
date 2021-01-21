#ifndef _OCTREE_H
#define _OCTREE_H

#include <list>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include "Color.hpp"

#define	 LEAFDEPTH	8

struct Octree
{
	bool	 Leaf;

	union
	{
		struct {
			uint32_t hitCount;
			uint32_t R, G, B;
		} Data;

		struct Octree *Next[8];
	} Fields;
};

void	 initializeNode(Octree **, uint32_t, Color&);
void	 createPalette(Octree *, Color [], int *);

#endif
