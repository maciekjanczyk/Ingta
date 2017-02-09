#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

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