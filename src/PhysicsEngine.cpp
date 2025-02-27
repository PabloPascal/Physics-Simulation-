#include "../headers/PhysicsEngine.h"
#include "../headers/calculate_collisions.h"
#include "../headers/calculate_Force.h"

#include <iostream>


void controlView(sf::View& view, float dt);


Physics* Physics::instance_engine = nullptr;


Physics* Physics::getInstance(float gravity) {
	
	if (instance_engine == nullptr) {
		instance_engine = new Physics;
		instance_engine->m_gravity = gravity;
		instance_engine->m_window.create(sf::VideoMode(ScreenWidth, ScreenHeight), "Simulation");
	}

	return instance_engine;

}


void Physics::run() {

	sf::View view(sf::FloatRect(0,0, ScreenWidth, ScreenHeight));
	view.setCenter(ScreenWidth / 2, ScreenHeight / 2);

	sf::Clock clock;

	while (m_window.isOpen()) {//main cycle

		sf::Time time = clock.restart();
		float dt = time.asSeconds();


		sf::Event event;
		while (m_window.pollEvent(event)) { 

			if (event.type == sf::Event::Closed) {
				m_window.close();
			}
			
			//control
			controlView(view, dt);
		}
				
		//phsycis

		collision();
		
		gravityUpdate(dt);

		for (size_t i = 0; i < circles.size(); i++) {
			circles[i]->update(dt);
		}

		ForceUpdate();

		//draw

		m_window.clear();

		render();

		m_window.setView(view);
		m_window.display();
		

	}

}


void controlView(sf::View& view, float dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		view.zoom(1 + 5*dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		view.zoom(1-5*dt);
		//view.setCenter(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		view.move(0, -20000*dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		view.move(0, 20000 *dt);
		//view.setCenter(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		view.move(-20000*dt, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		view.move(20000*dt,0);
		//view.setCenter(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
	}


}



/*
							COLLISIONS
*/

//check all collisions
void Physics::collision()
{
	//collisions with each other (circles)
	for (size_t i = 0; i < circles.size(); i++) {

		for (size_t j = i+1; j < circles.size(); j++) {

			if (circles[i]->getCollisionIndicate() && circles[j]->getCollisionIndicate() && 
				checkBallsCollision(*circles[i], *circles[j]))
			{
				hitBalls(*circles[i], *circles[j]);
				separateBalls(*circles[i], *circles[j]);
			}

		}

	}
	//boundary collisions

	if (boundaryCollisionHandle) {

		for (auto& c : circles) {
			boundaryCollision(*c);
			
		}


	}

	//circle collision with rect
	for (size_t i = 0; i < circles.size(); i++) {
		for (size_t j = 0; j < rects.size(); j++) {

			if (circles[i]->getCollisionIndicate() && rects[j]->getCollisionIndicate()) {

				CircleHitRect(*circles[i], *rects[j]);

			}

		}


	}


}


void Physics::boundaryCollisionTurn(bool handle) {
	boundaryCollisionHandle = handle;
}

/*
							RENDER
*/

void Physics::render() {

	for (size_t i = 0; i < circles.size(); i++)
	{
		circles[i]->draw(m_window);
	}

	for (size_t i = 0; i < rects.size(); i++) {
		rects[i]->draw(m_window);
	}
	

}


/*
		ADD OBJECTS
*/

void Physics::addCircle(Circle& c) {
	circles.push_back(std::make_unique<Circle>(c));
}


void Physics::addRect(Rectangle& r) {
	rects.push_back(std::make_unique<Rectangle>(r));
}


void Physics::generateBalls(size_t count) {

	srand(std::time(0));

	for (size_t i = 0; i < count; i++) {
		float x = (rand() % (ScreenWidth - 20)) + 10;
		float y = (rand() % (ScreenHeight - 20)) + 10;

		float vx = (rand() % 30) + 10;
		float vy = (rand() % 30) + 10;

		float radius = (rand() % 5) + 10;
		float mass = (rand() % 10) + 25;

		int color = rand() % 2;

		Circle c({ x, y }, radius, mass);

		c.setVelocity({ vx, vy });
		c.setFillColor(sf::Color((1 - color) * 255, 0, color * 255));
		//c.activateGravity(true);
		c.activateGravityForce(true);
		//c.activateCollision(true);
		//c.setElastic(0.9);

		addCircle(c);
	}

}


/*
		GRAVITY
*/

void Physics::gravityUpdate(float dt)
{
	for (size_t i = 0; i < circles.size(); i++) {
		if(circles[i]->getGravityIndicate())
			gravity(*circles[i], dt);
	}
}


void Physics::gravity(Circle &c, float dt)
{
	float vx = c.getVelocity().getX();
	float vy = c.getVelocity().getY();
	
	vy += m_gravity * dt;

	c.setVelocity({ vx, vy });
}


float Physics::getGravity() {
	return m_gravity;
}

void Physics::setGravity(float g) {
	m_gravity = g;
}



void Physics::ForceUpdate() {

	for (size_t i = 0; i < circles.size(); i++) {

		for (size_t j = i + 1; j < circles.size(); j++) {
			if (circles[i]->getGravityForceIndicate() && circles[j]->getGravityForceIndicate())
			{
				calcGravityForce(*circles[i], *circles[j]);
			}
		}

	}

}