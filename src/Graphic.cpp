#include "Graphic.hpp"

bool	Graphic::_fill_textures_sprites()
{
	this->_t_grid = new sf::Texture();
	if (!this->_t_grid->loadFromFile("grid.png"))
		return false;
	this->_s_grid = new sf::Sprite();
	this->_s_grid->setTexture(*this->_t_grid);
	this->_t_p1 = new sf::Texture();
	if (!this->_t_p1->loadFromFile("black.png"))
		return false;
	this->_s_p1 = new sf::Sprite();
	this->_s_p1->setTexture(*this->_t_p1);
	this->_t_p2 = new sf::Texture();
	if (!this->_t_p2->loadFromFile("white.png"))
		return false;
	this->_s_p2 = new sf::Sprite();
	this->_s_p2->setTexture(*this->_t_p2);
	return true;
}

Graphic::Graphic(): _size(19), _windowDefaultSize(1000), _windowSizeX(1000), _windowSizeY(1000)
{
	if (!_fill_textures_sprites())
		throw std::runtime_error(std::string("Error while loading texture, please make sure 'grid.png', 'black.png' and 'white.png' exist/got permissions"));
	std::cout << "ok\n";
	if (!(this->_window = new sf::RenderWindow(sf::VideoMode(this->_windowSizeX, this->_windowSizeY), "Gomoku")))
		throw std::runtime_error(std::string("Error while loading SFML window"));
	if (!(this->_font.loadFromFile("font.ttf")))
		throw std::runtime_error(std::string("Error while loading font for SFML, please make sure font exist/got permissions"));
	this->_text.setFont(this->_font);
//	this->_text.setString(" Press:\n\tArrow key to move\n\tEscape to exit\n\tEnter to resolve\n\tSpace to pause/unpause\n\t< to slow\n\t> to accelarate");
//	this->_text.setCharacterSize(38);
//	this->_text.setPosition(sf::Vector2f(600, 0));
	
	sf::Vector2u grid_size = this->_t_grid->getSize();
	this->_s_grid->setScale(static_cast<float>(this->_windowSizeX) / grid_size.x, static_cast<float>(this->_windowSizeY) / grid_size.y);this->_text.setFillColor(sf::Color::White);
}

Graphic::~Graphic(void)
{
	delete this->_s_grid;
	delete this->_s_p1;
	delete this->_s_p2;

	delete this->_t_grid;
	delete this->_t_p1;
	delete this->_t_p2;

	this->_window->close();
	delete this->_window;
}

bool	Graphic::display_grid(const Grid &grid)
{
	this->_s_grid->setPosition(0, 0);
	this->_window->draw(*this->_s_grid);
	int x = 0, y = 0;
	for(auto l: grid)
	{
		for(auto n: l)
		{
			sf::Sprite	*tmp = nullptr;
			if (n == 1)
				tmp = this->_s_p1;
			else if (n == 2)
				tmp = this->_s_p2;
			if (tmp)
			{
				tmp->setPosition(x * this->_windowDefaultSize / this->_size, y * this->_windowDefaultSize / this->_size);
				this->_window->draw(*tmp);
			}
			++x;
		}
		x = 0;
		++y;
	}
	this->_window->display();
	return true;
}

std::pair<int, int> Graphic::getEvent(Grid grid, int player)
{
	sf::Event	event;
	int			xp = 0, yp = 0;

	while (_window->waitEvent(event))
	{
		if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed
					&& event.key.code == sf::Keyboard::Escape))
			return std::make_pair(-1, -1);
		else if (event.type == sf::Event::MouseMoved)
		{
			if (event.mouseMove.x / (this->_windowSizeX / this->_size) != xp
			|| yp != event.mouseMove.y / (this->_windowSizeY / this->_size))
			{
				xp = event.mouseMove.x / (this->_windowSizeX / this->_size);
				yp = event.mouseMove.y / (this->_windowSizeY / this->_size);
				if (xp < this->_size && xp >= 0 && yp < this->_size && yp >= 0)
				{
					if (!grid[yp][xp])
					{
						grid[yp][xp] = player;
						this->display_grid(grid);
						grid[yp][xp] = 0;
					}
					else
						this->display_grid(grid);
				}
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				int x = event.mouseButton.x / (this->_windowSizeX / this->_size);
				int y = event.mouseButton.y / (this->_windowSizeY / this->_size);
				if (x < this->_size && x >= 0 && y < this->_size && y >= 0 && !grid[y][x])
					return (std::make_pair(x, y));
			}
		}
		else if (event.type == sf::Event::Resized)
		{
			sf::Vector2u	tmp = this->_window->getSize();
			this->_windowSizeX = tmp.x;
			this->_windowSizeY = tmp.y;
		}
	}
	return std::make_pair(-1, -1);
}
