#include "ParticleSystem.hpp"
#include <cmath>
#include <iostream>


void ParticleSystem::init(sf::Vector2u windowSize, size_t noOfParticles)
{
	std::cout << "ParticleSystem::init \n";

	m_windowSize.x = windowSize.x;
	m_windowSize.y = windowSize.y;
	m_numberOfParticles = noOfParticles;

	referenceData.createForcesAndMaxDistances(6);
	referenceData.showForceMatrix();
	referenceData.showMaxDistMatrix();
	m_frictionFactor = std::pow(0.5, 0.02 / 0.04);

	return;
}


std::vector<Particle>& ParticleSystem::getAllParticles() 
{ 
	return m_particles; 
};


void ParticleSystem::restart()
{

	m_particles.clear();
	
	scatterParticles(m_size);
	
	return;

}

void ParticleSystem::pause()
{

		m_pause = !m_pause;

	return;

}


void ParticleSystem::update()
{
	float dt = 0.02f;

	if (m_pause)
		return;

	// Work out every particle's new velocity from the current positions first,
	// then move them all, so each particle sees the same snapshot of the universe.
	for (auto& p : m_particles)
	{
		if (m_lifeReduces)
		{
			//p.lifetime -= m_lifeTimeAdjustment;
			if (p.lifetime == 0)
				p.isAlive = false;
		}

		generateNewVelocity(p);
	}

	//****************************************************************************************************************
	// Accomodate wrapping around universe between boundaries
	//****************************************************************************************************************
	for (auto& p : m_particles)
	{
		p.position.m_X += p.velocity.m_X * dt;
		p.position.m_Y += p.velocity.m_Y * dt;

		if (p.position.m_X > m_windowSize.x)
			p.position.m_X = 0;

		if (p.position.m_X < 0)
			p.position.m_X = m_windowSize.x;

		if (p.position.m_Y > m_windowSize.y)
			p.position.m_Y = 0;

		if (p.position.m_Y < 0)
			p.position.m_Y = m_windowSize.y;

		p.particleSize = m_size;
		setParticleVertices(p);
	}

	return;
}

void ParticleSystem::setParticleVertices(const Particle& particle)
{
	float mx = particle.position.m_X;
	float my = particle.position.m_Y;
	float size = particle.particleSize;
	size_t i = VERTS_PER_PARTICLE * particle.particleIndex;

	// Two triangles: top-left, top-right, bottom-right / top-left, bottom-right, bottom-left
	m_vertices[i + 0].position = sf::Vector2f(mx, my);
	m_vertices[i + 1].position = sf::Vector2f(mx + size, my);
	m_vertices[i + 2].position = sf::Vector2f(mx + size, my + size);
	m_vertices[i + 3].position = sf::Vector2f(mx, my);
	m_vertices[i + 4].position = sf::Vector2f(mx + size, my + size);
	m_vertices[i + 5].position = sf::Vector2f(mx, my + size);
}

sf::Vector2f ParticleSystem::generateNewVelocity(Particle& myParticle)
{
	float dt = 0.02f;
	float totalForceX = 0.0f;
	float totalForceY = 0.0f;

	for (auto& theOtherParticle : m_particles)
	{
		if (theOtherParticle.particleIndex == myParticle.particleIndex)
			continue;

		float rX = theOtherParticle.position.m_X - myParticle.position.m_X;
		float rY = theOtherParticle.position.m_Y - myParticle.position.m_Y;

		// Use the shortest distance across the wrapped edges of the universe
		if (rX > 0.5f * m_windowSize.x)
			rX -= m_windowSize.x;

		if (rX < -0.5f * m_windowSize.x)
			rX += m_windowSize.x;

		if (rY > 0.5f * m_windowSize.y)
			rY -= m_windowSize.y;

		if (rY < -0.5f * m_windowSize.y)
			rY += m_windowSize.y;

		float r = std::sqrt(rX * rX + rY * rY);

		float rMax = referenceData.maxDistances[myParticle.particleType()][theOtherParticle.particleType()];

		rMax *= m_proximity;

		if (r > 0 && r < rMax)
		{
			float attractionFactor = referenceData.forces[myParticle.particleType()][theOtherParticle.particleType()];

			// r / rMax ensures that the distance between particles is always represented between 0 & 1
			float force = forceFunction(r / rMax, attractionFactor);

			// Scale the unit direction back up to the original distance check threshold
			totalForceX += rX / r * force * rMax;
			totalForceY += rY / r * force * rMax;
		}
	}

	totalForceX *= m_forceMultiplier;
	totalForceY *= m_forceMultiplier;

	myParticle.velocity.m_X *= m_frictionFactor;
	myParticle.velocity.m_Y *= m_frictionFactor;

	myParticle.velocity.m_X += totalForceX * dt;
	myParticle.velocity.m_Y += totalForceY * dt;

	return sf::Vector2f(myParticle.velocity.m_X, myParticle.velocity.m_Y);

}


sf::Vector2f ParticleSystem::generateNewVelocityWithAccn(Particle& myParticle)
{

	Vec2 direction = Vec2(0.0f, 0.0f);
	Vec2 directionNormalised = Vec2(0.0f, 0.0f);
	Vec2 totalForce = Vec2(0.0f, 0.0f);
	Vec2 velocity = myParticle.velocity;
	Vec2 acceleration = Vec2(0.0f, 0.0f);

	for (auto& theOtherParticle : m_particles)
	{
		if (theOtherParticle.particleIndex == myParticle.particleIndex)
			continue;

		// copy the particle being checked's position then subtract the current particle position to get the new position vector.
		direction = theOtherParticle.position - myParticle.position;

		if (direction.m_X > 0.5 * m_windowSize.x)
			direction.m_X -= m_windowSize.x;

		if (direction.m_X < -0.5 * m_windowSize.x)
			direction.m_X += m_windowSize.x;

		if (direction.m_Y > 0.5 * m_windowSize.y)
			direction.m_Y -= m_windowSize.y;

		if (direction.m_Y < -0.5 * m_windowSize.y)
			direction.m_Y += m_windowSize.y;

		float r = direction.length();

		float rMax = referenceData.maxDistances[myParticle.particleType()][theOtherParticle.particleType()] * m_proximity;

		if (r > 0 && r < rMax)
		{
			directionNormalised = direction / r;

			float attractionFactor = referenceData.forces[myParticle.particleType()][theOtherParticle.particleType()];

			// r / rMax ensures that the distance between particles is always represented between 0 & 1
			float forceFactor = forceFunction(r / rMax, attractionFactor);

			// Scale back to the original distance check threshold
			totalForce += directionNormalised * (forceFactor * rMax);
		}
	}

	totalForce = totalForce * m_forceMultiplier;

	if (myParticle.type == pType::RED)		{ myParticle.mass = m_RED_mass; }
	if (myParticle.type == pType::BLUE)		{ myParticle.mass = m_BLUE_mass; }
	if (myParticle.type == pType::YELLOW)	{ myParticle.mass = m_YELLOW_mass; }
	if (myParticle.type == pType::GREEN)	{ myParticle.mass = m_GREEN_mass; }
	if (myParticle.type == pType::WHITE)	{ myParticle.mass = m_WHITE_mass; }
	if (myParticle.type == pType::MAGENTA)	{ myParticle.mass = m_MAGENTA_mass; }

	myParticle.particleSize = myParticle.mass * m_size;

	velocity = velocity.scale(m_frictionFactor);

	acceleration = totalForce / myParticle.mass;
	
	// Increment velocity by acceleration & timestep
	velocity += acceleration * this->dt;

	myParticle.velocity = velocity;
	
	return sf::Vector2f(myParticle.velocity.m_X, myParticle.velocity.m_Y);
}

float ParticleSystem::forceFunction(float r, float attractionFactor)
{
	float beta = 0.3;
	// beta represents the threshold distance between the two particles where the force between them goes from 
	// repulsive to attractive.
	// > 0.3 it is a negative force and represents repulsion
	// < 0.3 the force between the particles becomes attractive - reducing as the particles increase away from each other
	// up until we reach a further threshold of distance between the particles where the force of attraction is now zero

	float forceFactor = 0.0f;

	if (r < beta)
	{
		forceFactor = r / beta - 1;
	}
	else if (beta < r && r < 1)
	{
		forceFactor = attractionFactor * (1 - std::abs(2 * r - 1 - beta) / (1 - beta));
	}
	else
		forceFactor = 0;
		
	return forceFactor;
}

void ParticleSystem::draw(sf::RenderWindow& window)
{
	//std::cout << "ParticleSystem::draw \n";
	window.draw(m_vertices);
	return;

}


void ParticleSystem::scatterParticles(float size)
{

	std::cout << "ParticleSystem::scatterParticles \n";

	//m_particles = std::vector<Particle>(m_numberOfParticles);
	m_vertices = sf::VertexArray(sf::PrimitiveType::Triangles, m_numberOfParticles * VERTS_PER_PARTICLE);

	for (int p = 0; p < m_numberOfParticles; p++)
	{
		Particle newParticle = createParticle(p, size);

		m_particles.push_back(newParticle);

	}
	return;

}

Particle ParticleSystem::createParticle(size_t index, float size)
{
	// set the particle's initial position

	Particle theParticle;

	Vec2 startPos = rn.RandomVec2i(0, m_windowSize.x, 0, m_windowSize.y);
	//Vec2 startVel = rn.RandomVec2f(-0.3, 0.3, -0.3, 0.3);
	Vec2 startVel = Vec2(0,0);

	//Vec2 screenCtrPos = Vec2(m_windowSize.x, m_windowSize.y);

	//startVel = screenCtrPos - startPos;
	//startVel = startVel.scale(0.01);

	int life = rn.RandInt(0, 1000);
	int type = rn.RandInt(0, 5);

	theParticle.position = startPos;
	theParticle.velocity = startVel;
	theParticle.type = (pType)type;
	theParticle.particleIndex = index;
	theParticle.lifetime = life;
	theParticle.particleSize = size;
	
	setParticleVertices(theParticle);

	sf::Color colour;

	switch (theParticle.type)
	{
	case pType::RED:		colour = sf::Color::Red;		break;
	case pType::BLUE:		colour = sf::Color::Blue;		break;
	case pType::YELLOW:		colour = sf::Color::Yellow;		break;
	case pType::GREEN:		colour = sf::Color::Green;		break;
	case pType::WHITE:		colour = sf::Color::White;		break;
	case pType::MAGENTA:	colour = sf::Color::Magenta;	break;

	default:

		std::cout << "Invalid type generated.. \n";
		break;
	}

	for (size_t v = 0; v < VERTS_PER_PARTICLE; v++)
		m_vertices[VERTS_PER_PARTICLE * index + v].color = colour;

	return theParticle;
}


void Data::createForcesAndMaxDistances(int number)
{

	RandNum dRn;

	for (int i = 0; i < number; i++)
	{

		for (int j = 0; j < number; j++)
		{
			forces[i][j] = dRn.RandFloat(-1.0f, 1.0f);
			maxDistances[i][j] = dRn.RandInt(5, 20);
		}

	}

	return;

}


void Data::showForceMatrix()
{

	int numTypes = 6;
	std::cout << "********************** Force Matrix ****************************\n";

	std::string output = "\t\tRED\t\tYEL\t\tBLUE\t\tGREEN\t\tWHITE\t\tMAGENTA\n";
	

	//	//			RED		YEL		BLUE	GREEN	WHITE	MAGENTA	
	//	/*RED*/		1.3f,	1.6f,	1.5f,	5.2f,	1.8f,	1.1f,
	//	/*YEL*/		1.1f,	1.2f,	1.5f,	1.2f,	1.1f,	1.8f,
	//	/*BLUE*/	1.8f,	1.8f,	4.5f,	1.2f,	1.8f,	1.6f,
	//	/*GREEN*/	1.5f,	0.6f,	1.5f,	1.2f,	1.8f,	1.1f,
	//	/*WHITE*/	1.3f,	1.6f,	1.5f,	1.2f,	1.4f,	3.5f,
	//	/*MAGENTA*/	1.9f,	1.9f,	1.5f,	1.2f,	1.8f,	0.5f


	std::cout << output;


	for (int i = 0; i < numTypes; i++)
	{

		switch ((pType)i)
		{
		case pType::RED:
			output = "/*RED*/\t\t";
			break;
		case pType::BLUE:
			output = "/*BLUE*/\t";
			break;
		case pType::YELLOW:
			output = "/*YELLOW*/\t";
			break;
		case pType::GREEN:
			output = "/*GREEN*/\t";
			break;
		case pType::WHITE:
			output = "/*WHITE*/\t";
			break;
		case pType::MAGENTA:
			output = "/*MAGENTA*/\t";
			break;
		default:
			output += "something went wrong\n";
			break;

		}


		for (int j = 0; j < numTypes; j++)
		{
			output += std::to_string(forces[i][j]) + "\t";

		}

		std::cout << output + "\n";


	}


}

void Data::showMaxDistMatrix()
{
	int numTypes = 6;
	std::cout << "\n\n********************** Distance Matrix ****************************\n";
	std::string output = "\tRED\tYEL\tBLUE\tGREEN\tWHITE\tMAGENTA\n";


	//	//			RED		YEL		BLUE	GREEN	WHITE	MAGENTA	
	//	/*RED*/		1.3f,	1.6f,	1.5f,	5.2f,	1.8f,	1.1f,
	//	/*YEL*/		1.1f,	1.2f,	1.5f,	1.2f,	1.1f,	1.8f,
	//	/*BLUE*/	1.8f,	1.8f,	4.5f,	1.2f,	1.8f,	1.6f,
	//	/*GREEN*/	1.5f,	0.6f,	1.5f,	1.2f,	1.8f,	1.1f,
	//	/*WHITE*/	1.3f,	1.6f,	1.5f,	1.2f,	1.4f,	3.5f,
	//	/*MAGENTA*/	1.9f,	1.9f,	1.5f,	1.2f,	1.8f,	0.5f


	std::cout << output;


	for (int i = 0; i < numTypes; i++)
	{

		switch ((pType)i)
		{
		case pType::RED:
			output = "/*RED*/\t";
			break;
		case pType::BLUE:
			output = "/*BLU*/\t";
			break;
		case pType::YELLOW:
			output = "/*YEL*/\t";
			break;
		case pType::GREEN:
			output = "/*GRN*/\t";
			break;
		case pType::WHITE:
			output = "/*WHI*/\t";
			break;
		case pType::MAGENTA:
			output = "/*MAG*/\t";
			break;
		default:
			output += "something went wrong\n";
			break;

		}


		for (int j = 0; j < numTypes; j++)
		{
			output += std::to_string(maxDistances[i][j]) + "\t";

		}

		std::cout << output + "\n";


	}




}




