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

int	h(Grid &grid)
{
	static int	i = -1;
	int		tab[] = {-1, 3, 5, 1, -6, -4, 0, 9};
	static int	qwe = 0;

//	if (!(qwe % 100000))
		std::cout << qwe << " h" << std::endl;
	++qwe;
	if (i >= 7)
		i = -1;
	return -1;
}

std::pair<int, Grid >	minmax(Grid &grid, int depth, bool maximizingPlayer, int, int);

std::pair<int, Grid>	getfirst(Grid &grid, int depth, bool maximizingPlayer, int min, int max, std::pair<int, Grid> &res, int &y, int &x)
{
	for(y = 0; y < grid.size(); ++y)
	{
		for(x = 0; x < grid.size(); ++x)
		{
			if (grid[y][x] == 0)
				return minmax(grid, depth, !maximizingPlayer, min, max);
		}
	}
	return res;
}

std::pair<int, Grid >	getmaxmin(Grid &grid, int depth, bool maximizingPlayer, int max, int min)
{
	std::pair<int, Grid >	res = std::make_pair(std::numeric_limits<int>::min(), grid);
	std::pair<int, Grid>	tmp;

	int	y, x;
	static int	i = -1;

	res = getfirst(grid, depth - 1, maximizingPlayer, max, min, res, y, x);
	if (res.first == std::numeric_limits<int>::max())
		return res;
	(maximizingPlayer ? max : min) = res.first;
	if (min <= max)
	{
		std::cout << "breaking\n";
		return res;
	}
	for(; y < grid.size(); ++y)
	{
		for(; x < grid.size(); ++x)
		{
			if (grid[y][x] == 0)
			{
				tmp = minmax(grid, depth - 1, !maximizingPlayer, max, min);
				std::cout << "loop\n";
				if (maximizingPlayer)
				{
					res = res.first > tmp.first ? res : tmp;
					max = res.first;
					if (min <= max)
					{
						std::cout << "breaking\n";
						return res;
					}
					else
						std::cout << max << " " << min << " depth: " << depth - 1 << " 1 HERE\n";
				}
				else
				{
					res = res.first < tmp.first ? res : tmp;
					min = res.first;
					if (min <= max)
					{
						std::cout << "breaking\n";
						return res;
					}
					else
						std::cout << max << " " << min << " depth: " << depth - 1 << " 2 HERE\n";
				}
			}
		}
		x = 0;
	}
	return res;
}

std::pair<int, Grid >	minmax(Grid &grid, int depth, bool maximizingPlayer, int max, int min)
{
	if (depth <= 0)
		return std::make_pair(h(grid), grid);
	std::pair<int, Grid >	res = std::make_pair(std::numeric_limits<int>::min(), grid);
	std::pair<int, Grid>	tmp;

	int	y, x;
	static int	i = -1;

	res = getfirst(grid, depth - 1, maximizingPlayer, max, min, res, y, x);
	if (res.first == std::numeric_limits<int>::max())
		return res;
	(maximizingPlayer ? max : min) = res.first;
	if (min <= max)
	{
		std::cout << max << " " << min << "\n";
		std::cout << "breaking\n";
		return res;
	}
	std::cout << "qweqweqweasd max: " << max << " min: " << min << "\n";
	for(; y < grid.size(); ++y)
	{
		for(; x < grid.size(); ++x)
		{
			if (grid[y][x] == 0)
			{
				tmp = minmax(grid, depth - 1, !maximizingPlayer, max, min);
				if (maximizingPlayer)
				{
					res = res.first > tmp.first ? res : tmp;
					max = res.first;
					if (min <= max)
					{
						std::cout << "breaking 2 \n";
						return res;
					}
					else
						std::cout << "max: " << max << " min: " << min << "   1\n";
				}
				else
				{
					res = res.first < tmp.first ? res : tmp;
					min = res.first;
					if (min <= max)
					{
						std::cout << "breaking 2\n";
						return res;
					}
					else
						std::cout << "max: " << max << " min: " << min << "   2\n";
				}
			}
		}
		x = 0;
	}
	return res;
}

int main()
{
	Graphic		g;
	Grid 		grid(19, std::vector<int>(19, 0));
	int			player = 1;

/*	while (1)
	{
		g.display_grid(grid);
		std::pair<int,int> e = g.getEvent(grid, player);
		if (e.second == -1)
			break ;
		std::cout << "x: " << e.second << " y: " << e.first << std::endl;
		grid[e.first][e.second] = player;
		player = player == 1 ? 2 : 1;
		std::cout << capture(grid, e, player) << std::endl;
		if (isWin(grid, e))
			std::cout << "STRIKE\n";
	}*/
	std::cout << minmax(grid, 3, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()).first << "\n";
}
