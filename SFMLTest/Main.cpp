#include <iostream>
#include <SFML/Graphics.hpp>
#include "Gra.h"

/*extern "C" {
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}*/

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ingta 0.01a");	
	window.setFramerateLimit(50);
	
	sf::Texture tekstura;
	tekstura.loadFromFile("static/pl_sprite.png");
	tekstura.setSmooth(true);
	sf::Sprite sprite(tekstura);
	sprite.setTextureRect(sf::IntRect(0, 0, 110, 130));
	sf::IntRect textRect = sprite.getTextureRect();
	sprite.setPosition(sf::Vector2f(400, 300));
	sprite.setOrigin(sf::Vector2f(textRect.width / 2, textRect.height / 2));
	sprite.setRotation(0);
	Mapa mapa(&window, &sprite, "static/betamap.dat");

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			float kat = 0;

			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				mapa.quit = true;
				break;

			case sf::Event::MouseMoved:
				mapa.obrocPostac(event);
				break;

			case sf::Event::KeyReleased:
				mapa.move = false;
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					mapa.pause = !mapa.pause;
				}
				break;

			default:
				break;
			}
		}		
		
		window.clear();
		window.draw(mapa);
		window.display();
	}

	return EXIT_SUCCESS;
}