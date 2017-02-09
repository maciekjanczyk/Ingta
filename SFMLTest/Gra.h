#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Animacja3D.h"

#define M_PI       3.14159265358979323846

typedef struct
{
	sf::View* view;
	sf::Sprite* sprite;
} ViewSprite;

bool sortComparer(Prostokat3d p1, Prostokat3d p2)
{
	return (p1.dist > p2.dist);
}

class Mapa : public sf::Drawable, public sf::Transformable
{
public:

	bool move = false;
	bool quit = false;

	Mapa(sf::RenderWindow* window, sf::Sprite* sprite)
	{		
		this->window = window;
		wys_kamery = 300;
		view = sf::View(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
		view.zoom(1.0f);
		window->setView(view);
		kolekcjaTekstur();
		gracz = sprite;
		gracz->setScale(0.35, 0.35);
		czytajMapeZPliku("static/betamap.dat");		
		vs.sprite = gracz;
		vs.view = &view;
		poruszanieWodyth = new sf::Thread(&Mapa::poruszanieWoda, this);
		poruszanieWodyth->launch();
	}

	~Mapa()
	{
		poruszanieWodyth->wait();
		delete poruszanieWodyth;

		for (int i = 0; i < obiekty2d.size(); i++)
		{
			delete obiekty2d[i];
		}

		for (int i = 0; i < woda.size(); i++)
		{
			delete woda[i];
		}

		delete kafel_t;
		delete blok_t;
		delete blok_dach_t;
		delete wiezowiec_t;
		delete krzak_t;
		delete ulica_prosto_t;
		delete woda_t;
	}

	void kolekcjaTekstur()
	{
		kafel_t = new sf::Texture();
		kafel_t->loadFromFile("static/kafel.png");

		blok_t = new sf::Texture();
		blok_t->loadFromFile("static/blok.png");

		blok_dach_t = new sf::Texture();
		blok_dach_t->loadFromFile("static/blok_dach.png");

		wiezowiec_t = new sf::Texture();
		wiezowiec_t->loadFromFile("static/wiezowiec.png");

		krzak_t = new sf::Texture();
		krzak_t->loadFromFile("static/krzak.png");

		ulica_prosto_t = new sf::Texture();
		ulica_prosto_t->loadFromFile("static/ulica_prosto.png");

		woda_t = new sf::Texture();
		woda_t->loadFromFile("static/woda.png");
	}

	const sf::View getView() const
	{
		return view;
	}

	void renderuj()
	{
		sortByDistance();

		for (auto i = obiekty3d.begin(); i < obiekty3d.end(); i++)
		{
			i->renderuj(gracz->getPosition());
		}
	}

	sf::Sprite& getPlayer()
	{
		return *gracz;
	}

	const sf::View& getCamera() const
	{
		return view;
	}

	sf::View* getCameraPointer()
	{
		return &view;
	}

	void obrocPostac(sf::Event ev)
	{
		gracz->setRotation(liczRotacje(gracz->getPosition(), ev.mouseMove, window, &view));
	}

	ViewSprite* getVSPointer()
	{
		return &vs;
	}

	void poruszanieSprite()
	{
		sf::Sprite* spr = gracz;
		sf::Clock clock;
		int x = 220;
		bool ruchprzedost = false;

		while (!quit)
		{
			if (move)
			{
				ruchprzedost = true;
				if (clock.getElapsedTime().asMilliseconds() > 100.0f)
				{
					x = x == 660 ? 220 : x + 110;
					spr->setTextureRect(sf::IntRect(x, 0, 110, 130));
					clock.restart();
				}
			}
			else if (ruchprzedost)
			{
				ruchprzedost = false;
				spr->setTextureRect(sf::IntRect(110, 0, 110, 130));
			}
		}
	}

	void poruszanieWoda()
	{
		sf::Clock clock;
		int x = 0;

		while (!quit)
		{
			if (clock.getElapsedTime().asMilliseconds() > 100.0f)
			{
				x += 50;

				if (x >= 600)
				{
					x = 0;
				}

				for (int i = 0; i < woda.size(); i++)
				{					
					woda[i]->setTextureRect(sf::IntRect(x, 0, 50, 50));
				}

				clock.restart();
			}
		}
	}

	void poruszanieKeyboard()
	{
		ViewSprite* vs = &(this->vs);
		sf::Sprite* spr = vs->sprite;
		sf::View* view = vs->view;
		float predkosc = 0.0005;

		while (!quit)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				move = true;
				double angle = (view->getRotation() - 45) * M_PI / 2;
				double move_x = predkosc * cos(angle);
				double move_y = predkosc * sin(angle);
				sf::Vector2f vec = sf::Vector2f(move_x, move_y);
				spr->move(
					sin(M_PI * view->getRotation() / 180.f) * predkosc,
					-1 * cos(M_PI * view->getRotation() / 180.f) * predkosc
				);
				view->move(
					sin(M_PI * view->getRotation() / 180.f) * predkosc,
					-1 * cos(M_PI * view->getRotation() / 180.f) * predkosc
				);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				move = true;
				sf::Vector2f vec = sf::Vector2f(0, predkosc);;
				spr->move(
					sin(M_PI * (view->getRotation() - 180) / 180.f) * predkosc,
					-1 * cos(M_PI * (view->getRotation() - 180) / 180.f) * predkosc
				);
				view->move(
					sin(M_PI * (view->getRotation() - 180) / 180.f) * predkosc,
					-1 * cos(M_PI * (view->getRotation() - 180) / 180.f) * predkosc
				);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				move = true;
				sf::Vector2f vec = sf::Vector2f(-predkosc, 0);
				spr->move(
					sin(M_PI * (view->getRotation() - 90) / 180.f) * predkosc,
					-1 * cos(M_PI * (view->getRotation() - 90) / 180.f) * predkosc
				);
				view->move(
					sin(M_PI * (view->getRotation() - 90) / 180.f) * predkosc,
					-1 * cos(M_PI * (view->getRotation() - 90) / 180.f) * predkosc
				);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				move = true;
				sf::Vector2f vec = sf::Vector2f(predkosc, 0);
				spr->move(
					sin(M_PI * (view->getRotation() + 90) / 180.f) * predkosc,
					-1 * cos(M_PI * (view->getRotation() + 90) / 180.f) * predkosc
				);
				view->move(
					sin(M_PI * (view->getRotation() + 90) / 180.f) * predkosc,
					-1 * cos(M_PI * (view->getRotation() + 90) / 180.f) * predkosc
				);
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				spr->setTextureRect(sf::IntRect(770, 0, 110, 130));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				view->rotate(predkosc);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				view->rotate(-predkosc);
			}
		}
	}

private:

	void czytajMapeZPliku(std::string mapname)
	{
		FILE* plik;
		fopen_s(&plik, mapname.data(), "r");
		int sizex, sizey;
		char znak;
		fscanf_s(plik, "%d", &sizex);
		fscanf_s(plik, "%d", &sizey);

		for (int i = 0; i < sizex; i++)
		{
			for (int j = 0; j < sizey; j++)
			{
				fscanf_s(plik, "%c", &znak);
				switch (znak)
				{
				case 'k':
					obiekty2d.push_back(kafel(j * 50, i * 50, kafel_t));
					break;
				case 'W':
					obiekty3d.push_back(wiezowiec(j * 50, i * 50, wys_kamery, wiezowiec_t, wiezowiec_t));
					break;
				case 'b':
					obiekty3d.push_back(blok(j * 50, i * 50, wys_kamery, blok_t, blok_dach_t));
					break;
				case 'K':
					obiekty3d.push_back(krzak(j * 50, i * 50, wys_kamery, krzak_t));
					break;
				case '1':
					obiekty2d.push_back(ulicaProsto(j * 50, i * 50, ulica_prosto_t));
					break;
				case 'w':
					woda.push_back(wodiczka(j * 50, i * 50, woda_t));
					break;
				case '!':
					//gracz->setPosition(sf::Vector2f(j * 50, i * 50));
					break;
				default:
					break;
				}
			}
		}

		fclose(plik);
	}

	float liczRotacje(sf::Vector2f curPos, sf::Event::MouseMoveEvent m_position, sf::RenderWindow* gameWindow, sf::View* playerView)
	{
		sf::Vector2i position(m_position.x, m_position.y);
		position = sf::Vector2i(gameWindow->mapPixelToCoords(position, *playerView));
		float dx = curPos.x - position.x;
		float dy = curPos.y - position.y;
		float rotation = (atan2(dy, dx)) * 180 / M_PI;

		return rotation - 180;
	}

	void sortByDistance()
	{
		sf::Vector2f currpos = gracz->getPosition();

		for (int i = 0; i < obiekty3d.size(); i++)
		{
			std::vector<sf::Vector2f> wektory = obiekty3d[i].wektoryPodstawy();
			obiekty3d[i].dist = fminf(
				fminf(
					sqrtf((wektory[0].x - currpos.x) * (wektory[0].x - currpos.x) + (wektory[0].y - currpos.y) * (wektory[0].y - currpos.y)),
					sqrtf((wektory[1].x - currpos.x) * (wektory[1].x - currpos.x) + (wektory[1].y - currpos.y) * (wektory[1].y - currpos.y))
				),
				fminf(
					sqrtf((wektory[2].x - currpos.x) * (wektory[2].x - currpos.x) + (wektory[2].y - currpos.y) * (wektory[2].y - currpos.y)),
					sqrtf((wektory[3].x - currpos.x) * (wektory[3].x - currpos.x) + (wektory[3].y - currpos.y) * (wektory[3].y - currpos.y))
				)
			);
		}

		std::sort(obiekty3d.begin(), obiekty3d.end(), sortComparer);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (auto i = woda.begin(); i != woda.end(); i++)
		{
			target.draw(**i);
		}

		for (auto i = obiekty2d.begin(); i != obiekty2d.end(); i++)
		{
			target.draw(**i);
		}

		target.draw(*gracz);

		for (auto i = obiekty3d.begin(); i != obiekty3d.end(); i++)
		{
			target.draw(*i);
		}

		window->setView(view);
	}

	std::vector<Prostokat3d> obiekty3d;
	std::vector<sf::Drawable*> obiekty2d;
	std::vector<sf::RectangleShape*> woda;
	sf::View view;
	float wys_kamery;
	sf::RenderWindow* window;
	sf::Sprite* gracz;
	ViewSprite vs;

	// galeria tekstur
	sf::Texture* kafel_t;
	sf::Texture* blok_t;
	sf::Texture* blok_dach_t;
	sf::Texture* wiezowiec_t;
	sf::Texture* krzak_t;
	sf::Texture* ulica_prosto_t;
	sf::Texture* woda_t;

	// watki
	sf::Thread* poruszanieWodyth;
};