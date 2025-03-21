#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Object.h"


class Rectangle : public Object
{
private:

	sf::RectangleShape m_rect;

	vec2 m_position;
	vec2 m_size;
	vec2 m_velosity;
	float m_angle = 0;

	float m_mass;

	bool m_gravityHandle = false;
	bool m_collisionHandle = false;

	vec2 m_localCoordsX;
	vec2 m_localCoordsY;

public:

	Rectangle(vec2 start_pos, vec2 size, float mass = 0);

	void draw(sf::RenderTarget& target) override;

public: //getters

	vec2 getPosition() const;

	vec2 getVelocity() const;

	sf::FloatRect getBounds() const;
	vec2 getSize() const;

	bool getGravityIndicate() const;
	bool getCollisionIndicate() const;

	float getAngle();

	vec2 getLocalCoordsX();
	vec2 getLocalCoordsY();

public: //setters

	void setVelocity(vec2);

	void setPosition(vec2 coords);

	void setFillColor(sf::Color);

public://physics
	
	void activateGravity(bool handler);

	void activateCollision(bool handler);

	void update(float dt);

	void rotate(float angle);
};



#endif // !RECTANGLE_H
