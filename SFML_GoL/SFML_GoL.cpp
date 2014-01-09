// SFML_GoL.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "GameBoardRenderer.h"

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

	~ParticleSystem()
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
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
#endif

	sf::RenderWindow window(sf::VideoMode(800, 800), "Game Of Life");

	sf::Texture tiles;
	if (!tiles.loadFromFile("tiles.png")) {
		sf::err() << "Failed to load tileset texture." << std::endl;
		return 1;
	}
	
	ParticleSystem psys(5);
	GameBoardRenderer board(sf::Vector2u(100, 100), sf::Vector2u(8, 8), tiles);

	sf::Clock clock;
	sf::Vector2i click;

	sf::View game_view(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	game_view.setViewport(sf::FloatRect(0, 0, 1, 0.75));
	window.setView(game_view);

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
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				// Re-center view
				case sf::Keyboard::C:
					game_view.setCenter(sf::Vector2f(400, 300));
					window.setView(game_view);
					break;

				// Change speed:
				case sf::Keyboard::P:
					board.setSpeed(GameSpeed::SPEED_PAUSED);
					break;
				case sf::Keyboard::Num1:
					board.setSpeed(GameSpeed::SPEED_SLOW);
					break;
				case sf::Keyboard::Num2:
					board.setSpeed(GameSpeed::SPEED_FAST);
					break;
				}
				break;
			case sf::Event::MouseButtonPressed:
				click = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
				if (event.mouseButton.button == sf::Mouse::Button::Left) { 
					board.clickCell(sf::Vector2i(window.mapPixelToCoords(click)));
				} else if (event.mouseButton.button == sf::Mouse::Button::Right) {
					game_view.setCenter(sf::Vector2f(click));
					window.setView(game_view);
				}
				break;
			}
		}
		
		sf::Time elapsed = clock.restart();
		board.update(elapsed);
		psys.update(elapsed);

		window.clear(sf::Color::Blue);
		window.draw(board);
		window.draw(psys);
		window.display();
		sf::sleep(sf::milliseconds(50) - elapsed);
	}
	return 0;
}

