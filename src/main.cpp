#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "ParticleSystem.hpp"


// Game config object
class Game
{
public:
	unsigned int windowWidth = 1600;
	unsigned int windowHeight = 1000;
	size_t noOfParticles = 500;
	float particleSize = 2.0f;
	float frictionFactor = 0.707f;

	void loadConfigFile(const std::string& filename);
	void checkInput(sf::RenderWindow& window, ParticleSystem& particles);
};



int main()
{
	Game game;
	game.loadConfigFile("res/config.txt");

	sf::RenderWindow window(sf::VideoMode({ game.windowWidth, game.windowHeight }), "Particle Life");

	if (!ImGui::SFML::Init(window)) {
		std::cerr << "Could not initialise ImGui!\n";
		return -1;
	}

	// Persist the control window layout between runs
	ImGui::GetIO().IniFilename = "res/imgui.ini";

	// 60 FPS
	//window.setFramerateLimit(60);

	ParticleSystem particles;
	particles.init(window.getSize(), game.noOfParticles);
	particles.m_frictionFactor = game.frictionFactor;
	particles.m_size = game.particleSize;
	particles.scatterParticles(game.particleSize);

	sf::Clock deltaClock;
	sf::Clock runClock;
	sf::Clock titleClock;
	size_t frameCount = 0;

	while (window.isOpen())
	{
		frameCount++;

		game.checkInput(window, particles);
		if (!window.isOpen())
			break;

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Universe Control");

		ImGui::SliderInt("Number of Particles", &particles.m_numberOfParticles, 5, 5000);
		ImGui::SliderInt("Proximity Sensitivity", &particles.m_proximity, 1, 100);
		ImGui::SliderFloat("Force Multiplier", &particles.m_forceMultiplier, 1, 100);
		ImGui::SliderFloat("Size of Particles", &particles.m_size, 1, 5);
		ImGui::SliderFloat("Friction Factor", &particles.m_frictionFactor, 0, 1.5);

		if (ImGui::Button("Restart"))
			particles.restart();

		if (ImGui::Button(particles.m_pause ? "Resume" : "Pause"))
			particles.pause();

		if (ImGui::Button("Reset Forces/Max Distances"))
		{
			particles.referenceData.createForcesAndMaxDistances(6);
			particles.referenceData.showForceMatrix();
			particles.referenceData.showMaxDistMatrix();
		}

		ImGui::Text("Keys: R restart, Space pause, Esc quit");

		ImGui::End();

		particles.update();

		window.clear(sf::Color::Black);
		particles.draw(window);
		ImGui::SFML::Render(window);
		window.display();

		// Updating the title every frame is expensive, so refresh it twice a second
		if (titleClock.getElapsedTime().asSeconds() >= 0.5f)
		{
			titleClock.restart();
			float totalSecs = runClock.getElapsedTime().asSeconds();
			window.setTitle("Particle Life ... @ FPS " + std::to_string(frameCount / totalSecs) + " for : " + std::to_string(totalSecs) + " secs");
		}
	}

	ImGui::SFML::Shutdown();

	return 0;
}



void Game::checkInput(sf::RenderWindow& window, ParticleSystem& particles)
{
	while (const std::optional event = window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(window, *event);

		if (event->is<sf::Event::Closed>()) {
			window.close();
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			// Don't treat typing into an ImGui widget as a shortcut
			if (ImGui::GetIO().WantCaptureKeyboard)
				continue;

			if (keyPressed->code == sf::Keyboard::Key::Escape)
			{
				window.close();
			}

			if (keyPressed->code == sf::Keyboard::Key::R)
			{
				particles.restart();
			}

			if (keyPressed->code == sf::Keyboard::Key::Space)
			{
				particles.pause();
			}
		}
	}
}

// Load window size and particle settings. Missing file or tokens keep the defaults.
void Game::loadConfigFile(const std::string& filename)
{
	std::ifstream fin(filename);
	if (!fin) {
		std::cerr << "Could not open " << filename << ", using defaults\n";
		return;
	}

	std::string token;
	while (fin >> token) {
		if (token == "Window")
		{
			fin >> windowWidth >> windowHeight;
		}
		else if (token == "Particles")
		{
			fin >> noOfParticles >> particleSize;
		}
		else if (token == "FrictionFactor")
		{
			fin >> frictionFactor;
		}
	}
}
