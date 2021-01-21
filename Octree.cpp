#include "Octree.hpp"

Octree	 *Root = nullptr;
uint32_t currentSize =  0;
std::array<std::list<Octree *>, LEAFDEPTH> rList;

void	createPalette(Octree *current, Color Palette[], int *numLeafs)
{
	if (current == nullptr)
		return;

	if (! current->Leaf)
	{
		for( int i = 0; i < 8; i++)
			createPalette(
			current->Fields.Next[i],
			Palette,
			numLeafs);
	}
	else
	{
		int hitCount = current->Fields.Data.hitCount;
		Palette[*numLeafs].r = current->Fields.Data.R / hitCount;
		Palette[*numLeafs].g = current->Fields.Data.G / hitCount;
		Palette[*numLeafs].b = current->Fields.Data.B / hitCount;

		(*numLeafs)++;
	}
}

void	createNode(Octree **node, uint32_t depth)
{
	*node = (Octree *) calloc(1, sizeof(Octree));
	(*node)->Leaf  = (depth >= LEAFDEPTH);

	if ((*node)->Leaf)
		currentSize++;
}

Octree	*terminateNode(	Octree *node,
			int *children,
			Color &color,
			int *hitcount)
{
	if (node == nullptr)
		return NULL;

	if (node->Leaf)
	{
		(*children)++;
		color.r += node->Fields.Data.R;
		color.g += node->Fields.Data.G;
		color.b += node->Fields.Data.B;
		*hitcount += node->Fields.Data.hitCount;
	} else {
		for (int i = 0; i < 8; i++)
		{
			node->Fields.Next[i] =
			terminateNode(node->Fields.Next[i], children,
				      color, hitcount);
		}
	}

	free(node);
	return NULL;
}

void reduceNodes()
{
	int	index	  = LEAFDEPTH - 1,
		hitCount  = 0, nLeafs = 0;
	Color	color;

	while	(index >= 0 && rList[index].empty())
		index--;
	if	(index < 0) return;
	Octree *node	= rList[index].back();
	rList[index].pop_back();

	for (int i = 0; i < 8; i++)
	{
		node->Fields.Next[i] = terminateNode(
		node->Fields.Next[i], &nLeafs, color, &hitCount);
	}

	node->Leaf		= true;
	node->Fields.Data.R	= color.r;
	node->Fields.Data.G	= color.g;
	node->Fields.Data.B	= color.b;
	node->Fields.Data.hitCount = hitCount;

	currentSize = currentSize - nLeafs + 1;
}

void	initializeNode(Octree **node, uint32_t depth, Color& color)
{
	uint32_t branch, nChildren = 0;

	if (*node == nullptr)
	{
		createNode(node, depth);
	}

	if ((*node)->Leaf)
	{
		(*node)->Fields.Data.hitCount++;
		(*node)->Fields.Data.R += color.r;
		(*node)->Fields.Data.G += color.g;
		(*node)->Fields.Data.B += color.b;

		while (currentSize > 256)
		{
			reduceNodes();
		}
	} else if (depth < LEAFDEPTH) {
		branch = (((color.r >> (LEAFDEPTH - 1 - depth)) & 1) << 2)
		|	 (((color.g >> (LEAFDEPTH - 1 - depth)) & 1) << 1)
		|	 (((color.b >> (LEAFDEPTH - 1 - depth)) & 1));

		if ((*node)->Fields.Next[branch] == nullptr)
		{
			for(int i = 0; i < 8; i++)
			{
				if ((*node)->Fields.Next[i] != NULL)
					nChildren++;
			}

			if (nChildren == 1)
				rList[depth].push_back(*node);
		}

		initializeNode(
		&((*node)->Fields.Next[branch]),
		depth + 1,
		color);
	}
}
