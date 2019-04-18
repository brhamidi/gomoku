#include "Graphic.hpp"

bool	isCaptured(Grid &grid, std::pair<int, int> pos, int x, int y, int ennemy)
{
	if (grid[pos.first + y][pos.second + x] == grid[pos.first][pos.second]
	&& grid[pos.first + y % 2][pos.second + x % 2] == ennemy
	&& grid[pos.first + y % 2 ][pos.second + x % 2] ==
	grid[pos.first + (y % 2) * 2][pos.second + (x % 2) * 2])
	{
		grid[pos.first + y % 2][pos.second + x % 2] = 0;
		grid[pos.first + (y % 2) * 2][pos.second + (x % 2) * 2] = 0;
		return true;
	}
	return false;
}

bool	capture(Grid &grid, std::pair<int, int> pos, int ennemy)
{
	bool	res = false;

	if (pos.first - 3 >= 0)
	{
		if (pos.second - 3 >= 0)
		{
			res |= isCaptured(grid, pos, -3, -3, ennemy);
		}
		res |= isCaptured(grid, pos, 0, -3, ennemy);
		if (pos.second + 3 < 19)
			res |= isCaptured(grid, pos, 3, -3, ennemy);
	}
	if (pos.first + 3 < 19)
	{
		if (pos.second - 3 >= 0)
			res |= isCaptured(grid, pos, -3, 3, ennemy);
		res |= isCaptured(grid, pos, 0, 3, ennemy);
		if (pos.second + 3 < 19)
			res |= isCaptured(grid, pos, 3, 3, ennemy);
	}
	if (pos.second - 3 >= 0)
		res |= isCaptured(grid, pos, -3, 0, ennemy);
	if (pos.second + 3 < 19)
		res |= isCaptured(grid, pos, 3, 0, ennemy);
	return res;
}

bool	isWin(Grid &grid, std::pair<int, int> pos)
{
	int		player = grid[pos.first][pos.second];
	int		i = 0;
	int		count = 1;

	while (pos.first - ++i >= 0 && grid[pos.first - i][pos.second] == player
			&& count < 5)
		++count;
	i = 0;
	while (pos.first + ++i < 19 && grid[pos.first + i][pos.second] == player
			&& count < 5)
		++count;
	if (count == 5)
		return true;

	count = 1;
	i = 0;
	while (pos.second - ++i >= 0 && grid[pos.first][pos.second - i] == player
			&& count < 5)
		++count;
	i = 0;
	while (pos.second + ++i < 19 && grid[pos.first][pos.second + i] == player
			&& count < 5)
		++count;
	if (count == 5)
		return true;

	count = 1;
	i = 0;
	while (pos.second - ++i >= 0 && pos.first - i >= 0
			&& grid[pos.first - i][pos.second - i] == player && count < 5)
		++count;
	i = 0;
	while (pos.second + ++i >= 0 && pos.first + i < 19
			&& grid[pos.first + i][pos.second + i] == player && count < 5)
		++count;
	if (count == 5)
		return true;

	count = 1;
	i = 0;
	while (pos.second - ++i >= 0 && pos.first + i < 19
			&& grid[pos.first + i][pos.second - i] == player && count < 5)
		++count;
	i = 0;
	while (pos.second + ++i >= 0 && pos.first - i >= 0
			&& grid[pos.first - i][pos.second + i] == player && count < 5)
		++count;
	if (count == 5)
		return true;
	return false;
}
#include <algorithm>
int	distance(const Grid & grid, int & distance)
{
	int	dy;
	int	dx;

	if (distance == 9) return distance;
	for (int y = 0; y < 19; ++y) {
		for (int x = 0; x < 19; ++x) {
			if (grid[y][x] != 0) {
				dy = std::abs(9 - y) + 2;
				dx = std::abs(9 - x) + 2;
				distance = std::max(distance, std::max(dy, dx));
			}
		}
	}
	if (distance > 9) return 9;
	if (distance < 2) return 2;
	return distance;
}

int main()
{
	Graphic		g;
	Grid 		grid(19, std::vector<int>(19, 0));
	int		player = 1;
	int		d = 2;

	while (1)
	{
		g.display_grid(grid);
		std::cout << "distance " << distance(grid, d) << std::endl;
		std::pair<int,int> e = g.getEvent(grid, player);
		if (e.second == -1)
			break ;
		std::cout << "x: " << e.second << " y: " << e.first << std::endl;
		grid[e.first][e.second] = player;
		player = player == 1 ? 2 : 1;
		std::cout << capture(grid, e, player) << std::endl;
		if (isWin(grid, e))
			std::cout << "STRIKE\n";
	}
}
