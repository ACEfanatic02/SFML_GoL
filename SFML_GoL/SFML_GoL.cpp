// SFML_GoL.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{
private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_tileset;

		target.draw(m_vertices, states);
	}

public:

	bool load(const std::string& tileset, sf::Vector2u tile_size, 
		const int * tiles, unsigned int width, unsigned int height)
	{
		if (!m_tileset.loadFromFile(tileset))
		{
			return false;
		}

		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		// Number of tiles per row in tileset texture.
		const int tileset_width = (m_tileset.getSize().x / tile_size.x);
		
		for (unsigned int i = 0; i < width; ++i)
		{
			for (unsigned int j = 0; j < height; ++j)
			{
				int tile_number = tiles[i + j * width];

				int tu = tile_number % tileset_width;
				int tv = tile_number / tileset_width;

				sf::Vertex * quad = &m_vertices[(i + j * width) * 4];

				quad[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
				quad[1].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
				quad[2].position = sf::Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);
				quad[3].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);

				quad[0].texCoords = sf::Vector2f(tu * tile_size.x, tv * tile_size.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
				quad[3].texCoords = sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);	
			}
		}

		return true;
	}
};


class ParticleEmitter : public sf::Drawable, public sf::Transformable
{
private:
	struct Particle {
		Particle() {}
		Particle(const Particle& p) : velocity(p.velocity), lifetime(p.lifetime) {}
		sf::Vector2f velocity;
		sf::Time lifetime;
	};

	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;

	void resetParticle(std::size_t index)
	{
		float angle = (std::rand() % 360) * 3.14f / 180.0f;
		float speed = (std::rand() % 50) + 50.0f;
		m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

		m_vertices[index].position = m_emitter;
		sf::Color pcol(std::rand() % 255, std::rand() % 255, std::rand() % 255, 255);
		m_vertices[index].color = pcol;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = NULL;
		target.draw(m_vertices, states);
	}

public:
	
	ParticleEmitter() :
		m_particles(0),
		m_vertices(sf::Points, 0),
		m_lifetime(sf::seconds(3)),
		m_emitter(0, 0)
	{
	}

	ParticleEmitter(unsigned int count, sf::Vector2i pos) :
		m_particles(count),
		m_vertices(sf::Points, count),
		m_lifetime(sf::seconds(3)),
		m_emitter(pos)
	{
	}

	void update(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < m_particles.size(); ++i)
		{
			Particle& p = m_particles[i];
			p.lifetime -= elapsed;

			if (p.lifetime <= sf::Time::Zero) 
			{
				resetParticle(i);
			}

			m_vertices[i].position += p.velocity * elapsed.asSeconds();

			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}
};

class ParticleSystem : public sf::Drawable
{
private:
	std::vector<ParticleEmitter> m_emitters;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_emitters.size(); ++i)
		{
			target.draw(m_emitters[i], states);
		}
	}

public:
	ParticleSystem() :
		m_emitters(10)
	{
	}

	ParticleSystem(std::size_t emitter_count) :
		m_emitters(emitter_count)
	{
	}

	void addEmitter(unsigned int particles, sf::Vector2i pos)
	{
		m_emitters.push_back(ParticleEmitter(particles, pos));
	}

	void update(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < m_emitters.size(); ++i)
		{
			m_emitters[i].update(elapsed);
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window(sf::VideoMode(1024, 512), "Particles");

	ParticleSystem psys;

	sf::Clock clock;
	sf::Vector2i click;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				click = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
				psys.addEmitter(100000, click);
				break;
			}
		}
		
		sf::Time elapsed = clock.restart();
		psys.update(elapsed);

		window.clear(sf::Color::Black);
		window.draw(psys);
		window.display();
	}

	return 0;
}

