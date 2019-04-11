#ifndef GRAPHIC_HPP
# define GRAPHIC_HPP

# include <SFML/Window.hpp>
# include <SFML/Graphics.hpp>
# include <vector>
# include <map>
# include <array>
# include <list>
# include <utility>
# include <iostream>
# include <unistd.h>

typedef std::vector<std::vector<int> > Grid;

class Graphic
{
	public:
		Graphic();
		~Graphic(void);
		bool	display_grid(const Grid&);
		std::pair<int, int>		getEvent(Grid, int);

	private:
		int							_windowSizeX;
		int							_windowSizeY;
		int							_windowDefaultSize;
		int							_size;
		sf::RenderWindow			*_window;
		sf::Texture					*_t_p1;
		sf::Texture					*_t_p2;
		sf::Texture					*_t_grid;
		sf::Sprite					*_s_p1;
		sf::Sprite					*_s_p2;
		sf::Sprite					*_s_grid;
		sf::Font					_font;
		sf::Text					_text;

		bool						_fill_textures_sprites();
		bool						_readEvent(int &) const;
};

#endif
