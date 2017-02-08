#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

bool move = false;
bool quit = false;
char kierunek;

typedef struct
{
	sf::View* view;
	sf::Sprite* sprite;
} ViewSprite;

inline float odleglosc3d(sf::Vector3f p1, sf::Vector3f p2)
{
	return sqrtf(powf((p1.x - p2.x), 2) + powf((p1.y - p2.y), 2) + ((p1.z - p2.z), 2));
}

sf::Vector2f& rzutowanie(sf::Vector3f &pkt, sf::Vector3f &kamera)
{
	float Xprim = ((pkt.x - kamera.x) * (kamera.z / pkt.z)) + kamera.x;
	float Yprim = ((pkt.y - kamera.y) * (kamera.z / pkt.z)) + kamera.y;

	return sf::Vector2f(Xprim, Yprim);
}

class Prostokat3d : public sf::Drawable, public sf::Transformable
{
public:

	Prostokat3d(const sf::FloatRect &rect, float wysokosc, float wys_kamery, const std::string &tekstura)
	{
		this->wysokosc = wysokosc;
		this->wys_kamery = wys_kamery;
		this->tekstura.loadFromFile(tekstura);
		this->tekstura.setRepeated(true);
		srodek_x = rect.left + rect.width / 2;
		srodek_y = rect.top + rect.height / 2;

		p1.x = rect.left; 
		p1.y = rect.top; 
		
		p2.x = rect.left + rect.width;
		p2.y = rect.top;
		
		p3.x = rect.left;
		p3.y = rect.top + rect.height;
		
		p4.x = rect.left + rect.width;
		p4.y = rect.top + rect.height;	

		d1 = sf::Vector2f(p1);
		d2 = sf::Vector2f(p2);
		d3 = sf::Vector2f(p3);
		d4 = sf::Vector2f(p4);
	}

	void renderuj(const sf::Vector2f pos)
	{		
		currpos = pos;
		d1 = rzutowanie(sf::Vector3f(p1.x, p1.y, wysokosc), sf::Vector3f(pos.x, pos.y, wys_kamery));
		d2 = rzutowanie(sf::Vector3f(p2.x, p2.y, wysokosc), sf::Vector3f(pos.x, pos.y, wys_kamery));
		d3 = rzutowanie(sf::Vector3f(p3.x, p3.y, wysokosc), sf::Vector3f(pos.x, pos.y, wys_kamery));
		d4 = rzutowanie(sf::Vector3f(p4.x, p4.y, wysokosc), sf::Vector3f(pos.x, pos.y, wys_kamery));
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();		

		sf::VertexArray quad[4];

		// sciana dol
		quad[0] = sf::VertexArray(sf::Quads, 4);
		quad[0][0].position = d3;
		quad[0][1].position = d4;
		quad[0][2].position = p4;
		quad[0][3].position = p3;
		quad[0][0].texCoords = sf::Vector2f(0, 0);
		quad[0][1].texCoords = sf::Vector2f(250, 0);
		quad[0][2].texCoords = sf::Vector2f(250, 100);
		quad[0][3].texCoords = sf::Vector2f(0, 100);

		// sciana gora
		quad[1] = sf::VertexArray(sf::Quads, 4);
		quad[1][0].position = d1;
		quad[1][1].position = d2;
		quad[1][2].position = p2;
		quad[1][3].position = p1;
		quad[1][0].texCoords = sf::Vector2f(0, 0);
		quad[1][1].texCoords = sf::Vector2f(250, 0);
		quad[1][2].texCoords = sf::Vector2f(250, 100);
		quad[1][3].texCoords = sf::Vector2f(0, 100);

		// sciana lewo
		quad[2] = sf::VertexArray(sf::Quads, 4);
		quad[2][0].position = d4;
		quad[2][1].position = d2;
		quad[2][2].position = p2;
		quad[2][3].position = p4;
		quad[2][0].texCoords = sf::Vector2f(0, 0);
		quad[2][1].texCoords = sf::Vector2f(250, 0);
		quad[2][2].texCoords = sf::Vector2f(250, 100);
		quad[2][3].texCoords = sf::Vector2f(0, 100);

		// sciana prawo
		quad[3] = sf::VertexArray(sf::Quads, 4);
		quad[3][0].position = d1;
		quad[3][1].position = d3;
		quad[3][2].position = p3;
		quad[3][3].position = p1;
		quad[3][0].texCoords = sf::Vector2f(0, 0);
		quad[3][1].texCoords = sf::Vector2f(250, 0);
		quad[3][2].texCoords = sf::Vector2f(250, 100);
		quad[3][3].texCoords = sf::Vector2f(0, 100);

		states.texture = &(this->tekstura);

		if (currpos.y <= srodek_y && currpos.x <= srodek_x)
		{
			if (currpos.y <= quad[3][0].position.y)
			{
				target.draw(quad[3], states);
				target.draw(quad[1], states);
			}
			else
			{
				target.draw(quad[1], states);
				target.draw(quad[3], states);
			}

		}
		else if (currpos.y <= srodek_y && currpos.x > srodek_x)
		{
			if (currpos.y <= quad[2][1].position.y)
			{
				target.draw(quad[2], states);
				target.draw(quad[1], states);
			}
			else
			{
				target.draw(quad[1], states);
				target.draw(quad[2], states);
			}
		}
		else if (currpos.y > srodek_y && currpos.x > srodek_x)
		{
			if (currpos.y > quad[2][0].position.y)
			{
				target.draw(quad[2], states);
				target.draw(quad[0], states);
			}
			else
			{
				target.draw(quad[0], states);
				target.draw(quad[2], states);
			}
		}
		else
		{
			if (currpos.y <= quad[3][1].position.y)
			{
				target.draw(quad[0], states);
				target.draw(quad[3], states);
			}
			else
			{
				target.draw(quad[3], states);
				target.draw(quad[0], states);
			}
		}
		
		// dach
		sf::RectangleShape recsh;
		recsh.setPosition(d1);
		recsh.setSize(sf::Vector2f(fabsf(d4.x - d1.x), fabsf(d4.y - d1.y)));
		recsh.setTexture(&tekstura);
		target.draw(recsh, states);
	}

	float wysokosc, wys_kamery, srodek_x, srodek_y;
	sf::Vector2f p1, p2, p3, p4;
	sf::Vector2f d1, d2, d3, d4;
	sf::Vector2f currpos;
	sf::Texture tekstura;
};

float liczRotacje(sf::Vector2f curPos, sf::Event::MouseMoveEvent m_position, sf::RenderWindow* gameWindow, sf::View* playerView)
{
	sf::Vector2i position(m_position.x, m_position.y);
	position = sf::Vector2i(gameWindow->mapPixelToCoords(position, *playerView));
	float dx = curPos.x - position.x;
	float dy = curPos.y - position.y;
	float rotation = (atan2(dy, dx)) * 180 / M_PI;
	
	return rotation - 180;
}

void poruszanieSprite(sf::Sprite* spr)
{
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

void poruszanieKeyboard(ViewSprite* vs)
{
	sf::Sprite* spr = vs->sprite;
	sf::View* view = vs->view;
	float predkosc = 0.0005;

	while (!quit)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			move = true;			
			kierunek = 'w';
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
			kierunek = 's';
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
			kierunek = 'a';
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
			kierunek = 'd';
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

int main()
{
	float wys_kamery = 300;
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::View view(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	window.setView(view);
	view.zoom(2.0f);
	sf::Texture tekstura;

	Prostokat3d klocek2(sf::FloatRect(100, 50, 500, 500), 100, wys_kamery, "static/budynek.png");

	tekstura.loadFromFile("static/pl_sprite.png");
	tekstura.setSmooth(true);

	sf::Sprite sprite(tekstura);
	sprite.setTextureRect(sf::IntRect(0, 0, 110, 130));
	sf::IntRect textRect = sprite.getTextureRect();
	sprite.setPosition(sf::Vector2f(400, 300));
	sprite.setOrigin(sf::Vector2f(textRect.width / 2, textRect.height / 2));
	sprite.setRotation(0);

	ViewSprite vs;
	vs.sprite = &sprite;
	vs.view = &view;
	sf::Thread th(&poruszanieSprite, &sprite);
	sf::Thread th2(&poruszanieKeyboard, &vs);
	th.launch();
	th2.launch();

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
				break;

			case sf::Event::MouseMoved:			
				sprite.setRotation(liczRotacje(sprite.getPosition(), event.mouseMove, &window, &view));			
				break;

			case sf::Event::KeyReleased:
				move = false;
				break;

			default:
				break;
			}
		}

		if (move)
		{
			klocek2.renderuj(sprite.getPosition());
		}
		window.setView(view);
		window.clear();
		window.draw(sprite);
		window.draw(klocek2);
		window.display();
	}

	quit = true;
	th.wait();
	th2.wait();

	return 0;
}