#include "Graphic.hpp"
#include <unordered_set>
#include <algorithm>

class PairHash {
	public:
		size_t operator()(const std::pair<int, int>& pos) const {
			return pos.first * 100 + pos.second;
		}
};

Graphic		g;

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

int qwer;
int	h(Grid &grid)
{
	static int	i = -1;
	int		tab[] = {-1, 3, 5, 1, -6, -4, 0, 9};
	static int	qwe = 0;

	if (!(qwe % 10000))
	{
		std::cout << qwe << " h" << std::endl;
		g.display_grid(grid);
	}
	qwer = qwe;
	++qwe;
	if (i >= 7)
		i = -1;
	return -1;
}

int	minmax(Grid &grid, int depth, bool maximizingPlayer, int max, int min, std::unordered_set<std::pair<int, int>, PairHash> &set);

std::pair<int, std::pair<int, int> >	setUnorderedSet(Grid &grid, int depth, bool maximizingPlayer, int max, int min, std::unordered_set<std::pair<int, int>, PairHash> &set, int x, int y)
{
	std::list<std::pair<int, int> > l;

	grid[y][x] = maximizingPlayer ? 1 : 2;
	for(int i = y - 1; i < y + 1; i++)
		for(int j = x - 1; j < x + 1; j++)
			if (i >= 0 && j >= 0 && i < 19 && j < 19 && !grid[i][j])
				if (set.insert(std::make_pair(i, j)).second)
					l.push_front(std::make_pair(i, j));
	std::pair<int, std::pair<int, int> >	res = std::make_pair(minmax(grid, depth, maximizingPlayer, max, min, set),
			std::make_pair(y, x));
	for(auto pos: l)
		set.erase(pos);
	grid[y][x] = 0;
	return res;
}

int		minmax(Grid &grid, int depth, bool maximizingPlayer, int max, int min, std::unordered_set<std::pair<int, int>, PairHash> &set)
{
	if (depth <= 0)
		return h(grid);
	int	res = std::numeric_limits<int>::min();
	int	tmp;
	bool i = false;

	for(std::pair<int, int> p: set)
	{
		if (grid[p.first][p.second] == 0)
		{
			tmp = setUnorderedSet(grid, depth - 1, !maximizingPlayer, max, min, set, p.second, p.first).first;
			if (i)
			{
				res = maximizingPlayer ? (res > tmp ? res : tmp) : (res < tmp ? res : tmp);
				(maximizingPlayer ? max : min) = res;
				if (min <= max)
					return res;
			}
			else
			{
				res = tmp;
				i = true;
			}
		}
	}
	return res;
}

int main()
{
	Grid 		grid(19, std::vector<char>(19, 0));
	
	//g.getEvent(grid, 1);
	grid[9][9] = 2;
	int		player = 1;
	int		d = 2;

/*	while (1)
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
	}*/
	std::unordered_set<std::pair<int, int>, PairHash> set;
	std::cout << setUnorderedSet(grid, 10, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), set, 9, 9).first << "\n";
	std::cout << qwer << "\n";
}
