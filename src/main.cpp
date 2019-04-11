#include "Graphic.hpp"

int main()
{
	Graphic		g;
	Grid 		grid(19, std::vector<int>(19, 0));
	while (1)
	{
		g.display_grid(grid);
		std::pair<int,int> e = g.getEvent(grid, 2);
		if (e.first == -1)
			break ;
		std::cout << "x: " << e.first << " y: " << e.second << std::endl;
		grid[e.second][e.first] = 2;
	}
}
