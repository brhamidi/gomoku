#include "Graphic.hpp"
#include <unordered_set>
#include <algorithm>
#define MAXDEPTH 4
#define GRIDSIZE 19

class PairHash {
	public:
		size_t operator()(const std::pair<int, int>& pos) const {
			return pos.first * 100 + pos.second;
		}
};

Graphic		g;

void	isCaptured(Grid &grid, std::pair<int, int> pos, int x, int y, int ennemy,
		std::list<std::pair<int, int> > &list)
{
	if (grid[pos.first + y][pos.second + x] == grid[pos.first][pos.second]
	&& grid[pos.first + (y % 2)][pos.second + (x % 2)] == ennemy
	&& grid[pos.first + (y % 2) ][pos.second + (x % 2)] ==
	grid[pos.first + ((y % 2) * 2)][pos.second + ((x % 2) * 2)])
	{
		list.push_front(std::make_pair<int, int>(pos.first + (y % 2), pos.second + (x % 2)));
		list.push_front(std::make_pair<int, int>(pos.first + ((y % 2) * 2), pos.second + ((x % 2) * 2)));
	}
}

bool	checkThreeLine(std::vector<char> line)
{
	int	middle = 1;
	int	filled;
	int	pos = 4;

	while(++pos < 9 && line[pos])
		++middle;
	if (middle > 3 || (pos < 9 && line[pos] == 3))
		return false;
	pos = 4;
	while(--pos >= 0 && line[pos])
		++middle;
	if (middle > 3 || (pos >= 0 && line[pos] == 3))
		return false;

	filled = middle;
	pos = 4;
	while(++pos < 9 && line[pos]);
	if (pos + 1 < 9)
		while(++pos < 9 && line[pos])
			++filled;
	if (filled == 3 && pos < 9 && !line[pos])
		return true;

	filled = middle;
	pos = 4;
	while(--pos >= 0 && line[pos]);
	if (pos - 1 >= 0)
		while(--pos >= 0 && line[pos])
			++filled;
	if (filled == 3 && pos >= 0 && !line[pos])
		return true;
	return false;
}

bool	isDoubleThree(Grid &grid, std::pair<int, int> pos, int player)
{
	int					i = 0;
	int					three = 0;
	int					vpos = 4;
	std::vector<char>	line(9, 3);

	line[4] = player;
	while (pos.first - ++i >= 0 && (grid[pos.first - i][pos.second] == player
				|| grid[pos.first - i][pos.second] == 0) && vpos)
		line[--vpos] = grid[pos.first - i][pos.second];
	i = 0;
	vpos = 4;
	while (pos.first + ++i < GRIDSIZE && (grid[pos.first + i][pos.second] == player
			|| grid[pos.first + i][pos.second] == 0) && vpos < 9)
		line[++vpos] = grid[pos.first + i][pos.second];
	
	three += checkThreeLine(line) ? 1 : 0;
	line.assign(9, 3);
	i = 0;
	vpos = 4;
	while (pos.second - ++i >= 0 && (grid[pos.first][pos.second - i] == player
				|| grid[pos.first][pos.second - i] == 0) && vpos)
		line[--vpos] = grid[pos.first][pos.second - i];
	i = 0;
	vpos = 4;
	while (pos.second + ++i < GRIDSIZE && (grid[pos.first][pos.second + i] == player
				|| grid[pos.first][pos.second + i] == 0) && vpos < 9)
		line[++vpos] = grid[pos.first][pos.second + i];

	three += checkThreeLine(line) ? 1 : 0;
	if (three == 2)
		return true;
	line.assign(9, 3);
	i = 0;
	vpos = 4;
	while (pos.second - ++i >= 0 && pos.first - i >= 0 &&
			(grid[pos.first - i][pos.second - i] == player
			 || grid[pos.first - i][pos.second - i] == 0) && vpos)
		line[--vpos] = grid[pos.first - i][pos.second - i];
	i = 0;
	vpos = 4;
	while (pos.second + ++i >= 0 && pos.first + i < GRIDSIZE &&
			(grid[pos.first + i][pos.second + i] == player
			|| grid[pos.first + i][pos.second + i] == 0) && vpos < 9)
		line[++vpos] = grid[pos.first + i][pos.second + i];

	three += checkThreeLine(line) ? 1 : 0;
	if (three == 2)
		return true;
	line.assign(9, 3);
	i = 0;
	vpos = 4;
	while (pos.second - ++i >= 0 && pos.first + i < GRIDSIZE &&
			(grid[pos.first + i][pos.second - i] == player
			|| grid[pos.first + i][pos.second - i] == 0) && vpos)
		line[--vpos] = grid[pos.first + i][pos.second - i];
	i = 0;
	vpos = 4;
	while (pos.second + ++i >= 0 && pos.first - i >= 0 &&
			(grid[pos.first - i][pos.second + i] == player
			|| grid[pos.first - i][pos.second + i] == 0) && vpos < 9)
		line[++vpos] = grid[pos.first - i][pos.second + i];
	three += checkThreeLine(line) ? 1 : 0;
	return (three == 2 ? true : false);
}

void	capture(Grid &grid, std::pair<int, int> pos, int ennemy,
		std::list<std::pair<int, int> > &list)
{
	if (pos.first - 3 >= 0)
	{
		if (pos.second - 3 >= 0)
			isCaptured(grid, pos, -3, -3, ennemy, list);
		isCaptured(grid, pos, 0, -3, ennemy, list);
		if (pos.second + 3 < GRIDSIZE)
			isCaptured(grid, pos, 3, -3, ennemy, list);
	}
	if (pos.first + 3 < GRIDSIZE)
	{
		if (pos.second - 3 >= 0)
			isCaptured(grid, pos, -3, 3, ennemy, list);
		isCaptured(grid, pos, 0, 3, ennemy, list);
		if (pos.second + 3 < GRIDSIZE)
			isCaptured(grid, pos, 3, 3, ennemy, list);
	}
	if (pos.second - 3 >= 0)
		isCaptured(grid, pos, -3, 0, ennemy, list);
	if (pos.second + 3 < GRIDSIZE)
		isCaptured(grid, pos, 3, 0, ennemy, list);
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
	while (pos.first + ++i < GRIDSIZE && grid[pos.first + i][pos.second] == player
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
	while (pos.second + ++i < GRIDSIZE && grid[pos.first][pos.second + i] == player
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
	while (pos.second + ++i < GRIDSIZE && pos.first + i < GRIDSIZE
			&& grid[pos.first + i][pos.second + i] == player && count < 5)
		++count;
	if (count == 5)
		return true;

	count = 1;
	i = 0;
	while (pos.second - ++i >= 0 && pos.first + i < GRIDSIZE
			&& grid[pos.first + i][pos.second - i] == player && count < 5)
		++count;
	i = 0;
	while (pos.second + ++i < GRIDSIZE && pos.first - i >= 0
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
	for (int y = 0; y < GRIDSIZE; ++y) {
		for (int x = 0; x < GRIDSIZE; ++x) {
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
int	getLine(Grid &grid, int x, int y, int xm, int ym)
{
	int	line = 10;
	int	mode = 2;
	bool	buff = false;

	if ((x - xm < 0 || y - ym < 0 ||
			x - xm >= grid.size() || y - ym >= grid.size()) ||
			(grid[y - ym][x - xm] && grid[y - ym][x - xm] != grid[y][x]))
		--mode;
	for(int yy = y + ym, xx = x + xm; yy < grid.size() && xx < grid.size() && xx >= 0 && yy >= 0; yy += ym, xx += xm)
	{
		if (grid[y][x] == grid[yy][xx])
		{
			line *= 10;
			if (buff && line == 10000)
				return (mode == 2 ? line : line / 2);
			if (line == 100000)
			{
				std::cout << "max\n";
				break;
			}
		}
		else if (!grid[yy][xx])
		{
			if (buff || line == 10000)
				return (mode == 2 ? line : line / 2);
			else
				buff = !buff;
		}
		else
			return (--mode == 1 ? line / 2 : 0);
	}
	return (--mode == 1 ? line / 2 : 0);
}

int	h(Grid &grid, int mincaptured, int maxcaptured)
{
	static int	qwe = 0;
	int			total = 0;

	qwer = qwe;
	++qwe;
//	return -1;
//	if ((asd > 9) && grid[12][11] == 2)
//	{
//		std::cout << qwe << " h" << std::endl;
//		g.display_grid(grid);
//		g.getEvent(grid, 1);
//	}
	for(int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid.size(); ++x)
			if (grid[y][x])
			{
				total += getLine(grid, x, y, 1, 0) * (grid[y][x] == 1 ? 1 : -1);
				total += getLine(grid, x, y, 1, 1) * (grid[y][x] == 1 ? 1 : -1);
				total += getLine(grid, x, y, 0, 1) * (grid[y][x] == 1 ? 1 : -1);
				total += getLine(grid, x, y, -1, 1) * (grid[y][x] == 1 ? 1 : -1);
			}
	}
	return total;
}

std::pair <int, std::pair<int, int> >	minmax(Grid &grid, int depth, bool maximizingPlayer, int max, int min,
		std::unordered_set<std::pair<int, int>, PairHash> &set,
		int mincaptured, int maxcaptured);

std::pair<int, std::pair<int, int> >	setUnorderedSet(Grid &grid, int depth,
		bool maximizingPlayer, int max, int min, std::unordered_set<std::pair<int, int>,
		PairHash> &set, int x, int y, int mincaptured, int maxcaptured)
{
	std::list<std::pair<int, int> > l;

	if (x != -1)
		for(int i = y - 1; i <= y + 1; i++)
			for(int j = x - 1; j <= x + 1; j++)
				if (i >= 0 && j >= 0 && i < GRIDSIZE && j < GRIDSIZE/* && !grid[i][j]*/)
					if (set.insert(std::make_pair(i, j)).second)
						l.push_front(std::make_pair(i, j));
	std::pair<int, std::pair<int, int> >	res = minmax(grid,
		depth, maximizingPlayer, max, min, set, mincaptured, maxcaptured);
	if (x != -1)
		for(std::pair<int, int> pos: l)
		{
			set.erase(pos);
			l.pop_front();
		}
	return res;
}

std::pair <int, std::pair<int, int> >	minmax(Grid &grid, int depth, bool maximizingPlayer, int max, int min,
		std::unordered_set<std::pair<int, int>, PairHash> &set, int mincaptured, int maxcaptured)
{
	if (depth <= 0)
		return std::make_pair(h(grid, mincaptured, maxcaptured), std::make_pair(0,0));
	std::pair <int, std::pair<int, int > >	res = std::make_pair(0, std::make_pair(-1, -1));
	int	tmp;
	bool i = false;
	std::list<std::pair<int, int> > list;

	for(std::pair<int, int> p: set)
	{
		if (grid[p.first][p.second] == 0)
		{
			grid[p.first][p.second] = maximizingPlayer ? 1 : 2;
			capture(grid, p, maximizingPlayer ? 2 : 1, list);
			if (list.size() || !isDoubleThree(grid, p, maximizingPlayer ? 1 : 2))
			{
				if (isWin(grid, p))
				{
					res.first = 100000 * (maximizingPlayer ? 1 : -1) * (MAXDEPTH + 1 - depth);
					res.second = p;
				}
				else
				{
					(maximizingPlayer ? maxcaptured : mincaptured) += list.size();
					for(std::pair<int, int> lp: list)
						grid[lp.first][lp.second] = 0;
					tmp = setUnorderedSet(grid, depth - 1, !maximizingPlayer, max,
							min, set, p.second, p.first, mincaptured, maxcaptured).first;
					for(std::pair<int, int> lp: list)
					{
						grid[lp.first][lp.second] = maximizingPlayer ? 2 : 1;
						list.pop_front();
					}
					grid[p.first][p.second] = 0;
					if (i)
					{
						res.second = maximizingPlayer ? (res.first > tmp ? res.second : p) : (res.first < tmp ? res.second : p);
						res.first = maximizingPlayer ? (res.first > tmp ? res.first : tmp) : (res.first < tmp ? res.first : tmp);
					}
					else
					{
						res.first = tmp;
						res.second = p;
						i = true;
					}
				}
				(maximizingPlayer ? max : min) = res.first;
				if (min <= max/* || max > 50000 || min < 50000*//* || max > 100000*/)
				{
					grid[p.first][p.second] = 0;
					return res;
				}
			}
			grid[p.first][p.second] = 0;
		}
	}
	return res;
}

void	addToSet(std::unordered_set<std::pair<int, int>, PairHash> &set,
		int x, int y)
{
	for(int i = y - 1; i <= y + 1; i++)
		for(int j = x - 1; j <= x + 1; j++)
			if (i >= 0 && j >= 0 && i < GRIDSIZE && j < GRIDSIZE/* && !grid[i][j]*/)
				set.insert(std::make_pair(i, j));
}

int main()
{
	Grid 		grid(19, std::vector<char>(19, 0));
	int		player = 2;
	int		d = 2;
	std::unordered_set<std::pair<int, int>, PairHash> set;
	std::list<std::pair<int, int> > list;

//	g.getEvent(grid, 1);

//	grid[9][9] = 2;
//	addToSet(set, 9, 9);

//	auto	a = setUnorderedSet(grid, MAXDEPTH, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), set, -1, -1, 0, 0);
	while (1)
	{
//		g.display_grid(grid);
		std::pair<int,int> e = g.getEvent(grid, player);
//		int a, b;
//		std::cin >> a;
//		std::cin >> b;
//		std::pair<int,int> e = std::make_pair(a, b);
		if (e.second == -1)
			break ;
		grid[e.first][e.second] = player;
		addToSet(set, e.second, e.first);
		std::cout << "x: " << e.second << " y: " << e.first << std::endl;
		capture(grid, e, player == 1 ? 2 : 1, list);
		if(!list.size() && isDoubleThree(grid, e, player) )
			grid[e.first][e.second] = 0;
		else
		{
			player = player == 1 ? 2 : 1;
			for(auto n: list)
			{
				grid[n.first][n.second] = 0;
				list.pop_front();
			}
			if (isWin(grid, e))
			{
				std::cout << "P1 WIN STRIKE\n";
				break;
			}
			std::cout << "H " << h(grid, 0, 0) << " \n";

			auto	a = setUnorderedSet(grid, MAXDEPTH, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), set, -1, -1, 0, 0);
			std::cout << "A = " << a.first << "\n";
			std::pair<int, int>	p = a.second;
			std::cout << "\n" << p.first << " " << p.second << "\n";
			grid[p.first][p.second] = player;
			if (isWin(grid, p))
			{
				std::cout << "P2 WIN STRIKE\n";
				break;
			}
			addToSet(set, p.second, p.first);
			player = player == 1 ? 2 : 1;
			capture(grid, p, player, list);
			for(auto n: list)
			{
				grid[n.first][n.second] = 0;
				list.pop_front();
			}
		}
	}
	std::cout << qwer << "\n";
	std::pair<int,int> e = g.getEvent(grid, player);
}
