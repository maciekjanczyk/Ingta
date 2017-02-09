#include <iostream>
#include <SFML/Graphics.hpp>
#include "Gra.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ingta 0.01a");	
	
	sf::Texture tekstura;
	tekstura.loadFromFile("static/pl_sprite.png");
	tekstura.setSmooth(true);
	sf::Sprite sprite(tekstura);
	sprite.setTextureRect(sf::IntRect(0, 0, 110, 130));
	sf::IntRect textRect = sprite.getTextureRect();
	sprite.setPosition(sf::Vector2f(400, 300));
	sprite.setOrigin(sf::Vector2f(textRect.width / 2, textRect.height / 2));
	sprite.setRotation(0);
	Mapa mapa(&window, &sprite);

	sf::Thread th(&Mapa::poruszanieSprite, &mapa);
	sf::Thread th2(&Mapa::poruszanieKeyboard, &mapa);
	th.launch();
	th2.launch();

	while (window.isOpen())
	{
		sf::Event event;

		mapa.renderuj(); 

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

			default:
				break;
			}
		}

		if (mapa.move)
		{
			mapa.renderuj();
		}
		
		window.clear();
		window.draw(mapa);
		window.display();
	}

	th.wait();
	th2.wait();

	return EXIT_SUCCESS;
}