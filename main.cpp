#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "ParticleSystem.hpp"
//#include <time.h>


// Font Properties
struct FontProps {
	std::string name;
	sf::Font gameFont;
	uint16_t size;
	uint16_t r;
	uint16_t g;
	uint16_t b;
};

class Entity {
public:
	std::shared_ptr<sf::Shape> shape;
	std::shared_ptr<sf::Text> text;
	float sx;
	float sy;
};

// Game config object
class Game 
{
public:
	FontProps font;
	int windowWidth;
	int windowHeight;
	sf::RenderWindow window;
	sf::Text GameText;
	size_t noOfParticles;
	size_t particleSize;
	size_t frameCount;
	size_t runTime = 0;
	float frictionFactor = 1.0;
	sf::Clock Clock;


	void loadConfigFile(const std::string& filename, std::vector<std::shared_ptr<Entity> >& entities);
	void render(sf::RenderWindow& win, std::vector<std::shared_ptr<Entity> >& entities);
	void update(std::vector<std::shared_ptr<Entity> >& entities);
	void checkInput(sf::RenderWindow& win, std::vector<std::shared_ptr<Entity> >& entities);
};




int main(int argc, char* argv[])
{
	Game game;
	ParticleSystem particles;
	std::vector<std::shared_ptr<Entity> > entities;
	game.loadConfigFile("bin/config.txt", entities);
		

	if (!game.font.gameFont.loadFromFile(game.font.name)) {
		std::cerr << "Could not load font!\n";
		exit(-1);
	}

	sf::RenderWindow window(sf::VideoMode(game.windowWidth, game.windowHeight), "SFML works!");
	
	sf::Vector2f controls = sf::Vector2f(400, 400);
	ImGui::SFML::Init(window, controls);

	//sf::Vector2f controls = sf::Vector2f(game.windowWidth, 800);


	ImGui::LoadIniSettingsFromDisk("bin/universe.ini");

	std::string m_title;

	// 60 FPS
	//window.setFramerateLimit(60);
	particles.init(window.getSize(), game.noOfParticles);
	//particles.m_frictionFactor = game.frictionFactor;

	particles.scatterParticles(game.particleSize);

	////std::vector<Particle> Particles = particles.getAllParticles();
	////for (auto& p : Particles)
	////	std::cout << "Particle ID " << p.particleIndex << " \n";
	////return 0;

	sf::Text sampleText("Press x to reverse velocities.",
			    game.font.gameFont, game.font.size);

	sampleText.setPosition(0, 0);

	game.GameText = sampleText;

	float Framerate;
	float StartTime, EndTime, TotalSecs = 0, milliSecs = 0;
	float dt = 0;

	game.frameCount = 0;

	while (window.isOpen()) 
	{
		game.frameCount++;
		//game.checkInput(window, entities);
		//game.update(entities);
		//game.render(window, entities);
		ImGui::SFML::Update(window, game.Clock.restart());

		StartTime = game.Clock.getElapsedTime().asMilliseconds();

		ImGui::Begin("Universe Control");

		ImGui::SliderInt("Number of Particles", &particles.m_numberOfParticles, 5, 5000);
		ImGui::SliderInt("Proximity Sensitivity", &particles.m_proximity, 1, 100);
		ImGui::SliderFloat("Force Multiplier", &particles.m_forceMultiplier, 1, 100);
		ImGui::SliderFloat("Size of Particles", &particles.m_size, 0.1, 3);
		ImGui::SliderFloat("Friction Factor", &particles.m_frictionFactor, 0, 1.5);

		//ImGui::Text("Particle Masses");
		//ImGui::SliderFloat("RED", &particles.m_RED_mass, 1, 100);
		//ImGui::SliderFloat("BLUE", &particles.m_BLUE_mass, 1, 100);
		//ImGui::SliderFloat("YELLOW", &particles.m_YELLOW_mass, 1, 100);
		//ImGui::SliderFloat("GREEN", &particles.m_GREEN_mass, 1, 100);
		//ImGui::SliderFloat("WHITE", &particles.m_WHITE_mass, 1, 100);
		//ImGui::SliderFloat("MAGENTA", &particles.m_MAGENTA_mass, 1, 100);

		//ImGui::Checkbox("Allow to Die", &particles.m_lifeReduces);
		//ImGui::SliderInt("Age Rate", &particles.m_lifeTimeAdjustment, 1, 100);

		//ImGui::SaveIniSettingsToDisk("bin/universe.ini");

		if (ImGui::Button("Restart"))
			particles.restart();

		if (ImGui::Button("Pause"))
			particles.pause();

		if (ImGui::Button("Reset Forces/Max Distances"))
		{
			particles.referenceData.createForcesAndMaxDistances(6);
			particles.referenceData.showForceMatrix();
			particles.referenceData.showMaxDistMatrix();
		}


		//ImGui::SliderFloat("rMax", &particles.rMax, 0.1, 100);

		ImGui::End();


		game.checkInput(window, entities);
		particles.update();
		window.clear(sf::Color::Black);
		particles.draw(window);
		ImGui::SFML::Render(window);

		window.display();
				
		particles.dt = game.Clock.restart().asMilliseconds() - StartTime;

		milliSecs += particles.dt;

		TotalSecs = milliSecs / 1000;

		window.setTitle("Particle Life ... @ FPS " + std::to_string(game.frameCount / TotalSecs) + " for : " + std::to_string(TotalSecs) + " secs");
		
		//std::cout << "Timestep : " << particles.dt << std::endl;

	}

	ImGui::SFML::Shutdown();

	return 0;
}



void Game::render(sf::RenderWindow& window, std::vector<std::shared_ptr<Entity> >& entities)
{
	// Rendering
	window.clear();
	for (const auto& e : entities) {
		window.draw(*e->shape);
		window.draw(*e->text);
	}
	window.draw(GameText);
	window.display();

}

void Game::update(std::vector<std::shared_ptr<Entity> >& entities)
{
	// Update position
	for (auto& e : entities) {
		float shapeWidth = e->shape->getLocalBounds().width;
		float shapeHeight = e->shape->getLocalBounds().height;
		float shapeX = e->shape->getPosition().x;
		float shapeY = e->shape->getPosition().y;
		float textWidth = e->text->getLocalBounds().width;
		float textHeight = e->text->getLocalBounds().height;

		// Check for collisions
		// Left or right collision
		if (shapeX < 0 ||
			(shapeX + shapeWidth > windowWidth)) {
			e->sx *= -1.0f;
		}

		// Top or bottom collision
		if (shapeY < 0 ||
			(shapeY + shapeHeight > windowHeight)) {
			e->sy *= -1.0f;
		}

		e->shape->setPosition(e->shape->getPosition().x + e->sx, e->shape->getPosition().y + e->sy);
		// center the text with the entity
		e->text->setPosition(
			shapeX + (shapeWidth / 2) - (textWidth / 2),
			shapeY + (shapeHeight / 2) - (textHeight / 2));
	}

}


void Game::checkInput(sf::RenderWindow& window, std::vector<std::shared_ptr<Entity>>& entities)
{
	sf::Event event;
	while (window.pollEvent(event)) 
	{

		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::KeyPressed) {
			std::cout << "Key pressed with code = "
				<< event.key.code << "\n";

			// Move in opposite direction
			if (event.key.code == sf::Keyboard::X) 
			{
				for (const auto& e : entities) 
				{
					e->sx *= -1.0f;
					e->sy *= -1.0f;
				}
			}

			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();

			}
			if (event.key.code == sf::Keyboard::R)
			{
				//particle

			}

		}
	}


}
// Set game object & load entities.
void Game::loadConfigFile(const std::string& filename, std::vector<std::shared_ptr<Entity> >& entities)
{
	std::ifstream fin(filename);
	std::string token;

	fin >> token >> windowWidth >> windowHeight;
	fin >> token >> font.name >> font.size >> font.r >>
		font.b >> font.g;

	while (fin >> token) {
		auto e = std::make_shared<Entity>();

		if (token == "Circle") {
			auto pCircle = std::make_shared<sf::CircleShape>();
			// Name
			std::string name;
			fin >> name;
			e->text = std::make_shared<sf::Text>(
				name, font.gameFont, font.size);
			e->text->setFillColor(sf::Color(
				font.r, font.g, font.b));

			// Initial co-ordinates
			int x, y;
			fin >> x >> y;
			pCircle->setPosition(x, y);

			// Speed
			fin >> e->sx >> e->sy;

			// Color
			int r, g, b;
			fin >> r >> g >> b;
			pCircle->setFillColor(sf::Color(r, g, b));

			// Radius
			float radius;
			fin >> radius;
			pCircle->setRadius(radius);

			e->shape = pCircle;
			entities.push_back(e);
		} else if (token == "Rectangle") {
			auto pRect = std::make_shared<sf::RectangleShape>();
			// Name
			std::string name;
			fin >> name;
			e->text = std::make_shared<sf::Text>(
				name, font.gameFont, font.size);
			e->text->setFillColor(sf::Color(
				font.r, font.g, font.b));

			// Initial co-ordinates
			int x, y;
			fin >> x >> y;
			pRect->setPosition(x, y);

			// Speed
			fin >> e->sx >> e->sy;

			// Color
			int r, g, b;
			fin >> r >> g >> b;
			pRect->setFillColor(sf::Color(r, g, b));

			// Size
			float width, height;
			fin >> width >> height;
			pRect->setSize(sf::Vector2f(width, height));

			e->shape = pRect;
			entities.push_back(e);
		}
		else if(token == "Particles")
		{
			fin >> noOfParticles >> particleSize;
		}
		else if (token == "FrictionFactor")
		{
			fin >> frictionFactor;
		}

	}
}