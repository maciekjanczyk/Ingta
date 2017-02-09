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
		view.zoom(2.0f);
		window->setView(view);
		gracz = sprite;
		obiekty3d.push_back(Prostokat3d(sf::FloatRect(400, 300, 300, 300), 100, wys_kamery, "static/budynek.png"));
		obiekty3d.push_back(Prostokat3d(sf::FloatRect(800, 300, 300, 300), 100, wys_kamery, "static/budynek.png"));
		obiekty3d.push_back(Prostokat3d(sf::FloatRect(400, 700, 300, 300), 100, wys_kamery, "static/budynek.png"));
		obiekty3d.push_back(Prostokat3d(sf::FloatRect(800, 700, 300, 300), 100, wys_kamery, "static/budynek.png"));
		vs.sprite = gracz;
		vs.view = &view;
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

		target.draw(*gracz);
		
		for (auto i = obiekty3d.begin(); i != obiekty3d.end(); i++)
		{
			target.draw(*i);
		}

		window->setView(view);
	}

	std::vector<Prostokat3d> obiekty3d;
	sf::View view;
	float wys_kamery;
	sf::RenderWindow* window;
	sf::Sprite* gracz;
	ViewSprite vs;
};
