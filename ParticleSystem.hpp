#pragma once
#include <SFML/Graphics.hpp>
#include "RandNum.h"
#include "Vec2.h"
#include <vector>
#include <string>


enum class pType
{
	RED = 0,
	BLUE = 1,
	YELLOW = 2,
	GREEN = 3,
	WHITE = 4,
	MAGENTA = 5

};


class Data
{

public:
	Data();
	~Data();
	
	//float forces[6][6];

	void createForcesAndMaxDistances(int n);
	void showForceMatrix();
	void showMaxDistMatrix();
	
	float forces[6][6] = {};
	//{

	//	//			RED		YEL		BLUE	GREEN	WHITE	MAGENTA	
	//	/*RED*/		1.3f,	1.6f,	1.5f,	5.2f,	1.8f,	1.1f,
	//	/*YEL*/		1.1f,	1.2f,	1.5f,	1.2f,	1.1f,	1.8f,
	//	/*BLUE*/	1.8f,	1.8f,	4.5f,	1.2f,	1.8f,	1.6f,
	//	/*GREEN*/	1.5f,	0.6f,	1.5f,	1.2f,	1.8f,	1.1f,
	//	/*WHITE*/	1.3f,	1.6f,	1.5f,	1.2f,	1.4f,	3.5f,
	//	/*MAGENTA*/	1.9f,	1.9f,	1.5f,	1.2f,	1.8f,	0.5f
	//};

	int maxDistances[6][6] =
	{
		1,	1,	1,	1,	1,	1,
		1,	1,	1,	1,	1,	1,
		1,	1,	1,	1,	1,	1,
		1,	1,	1,	1,	1,	1,
		1,	1,	1,	1,	1,	1,
		1,	1,	1,	1,	1,	1
	};

	/*
		3,	6,	5,	2,	1,	1,
		1,	2,	5,	2,	1,	8,
		1,	1,	8,	2,	2,	6,
		5,	6,	2,	2,	3,	1,
		1,	6,	2,	2,	4,	5,
		1,	1,	2,	2,	3,	1

	*/


};

inline Data::Data()
{
}

inline Data::~Data()
{
}




class Particle
{

private:


public:

	Vec2 velocity;
	Vec2 position;
	Vec2 acceleration;
	pType type;
	std::string name = "blank";
	int lifetime = 0;
	int particleIndex;
	int particleSize;
	float mass = 1;
	bool isAlive = true;

	const int particleType() { return (int)type; };
		

};



class ParticleSystem
{


	std::vector<Particle>	m_particles;
	sf::VertexArray			m_vertices;
	sf::Vector2u			m_windowSize;
	
	

public:
	int						m_numberOfParticles = 500;
	float					m_size = 1;
	int						m_proximity = 10;
	float					rMax = 2.0f;
	
	float					m_forceMultiplier = 20.0f;
	float					m_frictionFactor = 0.85f;
	bool					m_pause = false;
	float					m_RED_mass = 1;
	float					m_BLUE_mass = 1;
	float					m_YELLOW_mass = 1;
	float					m_GREEN_mass = 1;
	float					m_WHITE_mass = 1;
	float					m_MAGENTA_mass = 1;
	bool					m_lifeReduces = false;
	int						m_lifeTimeAdjustment = 1;
	float					dt = 0.02;
	Data					referenceData;

	RandNum					rn;

	//std::vector<std::shared_ptr<Entity> > entities;
	void init(sf::Vector2u windowSize, size_t noOfParticles);
	void update();
	void draw(sf::RenderWindow& window);
	Particle createParticle(size_t index, size_t size);
	std::vector<Particle>& getAllParticles();
	void scatterParticles(size_t size);
	sf::Vector2f generateNewVelocityWithAccn(Particle& particle);
	sf::Vector2f generateNewVelocity(Particle& myParticle);
	float forceFunction(float r, float attractionFactor);
	void restart();
	void pause();



	/*

RED = 0,
BLUE = 1,
YELLOW = 2,
GREEN = 3,
WHITE = 4,
MAGENTA = 5

*/


};

