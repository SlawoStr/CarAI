#include "Car.h"
#define PI 3.14159265

inline double calculateDistance(sf::Vector2f firstPoint, sf::Vector2f secondPoint)
{
	return static_cast<double>(sqrt(pow((firstPoint.x - secondPoint.x), 2) + pow((firstPoint.y - secondPoint.y), 2)));
}

sf::Vector2f checkLinesIntersect(sf::Vector2f firstCoord, sf::Vector2f secondCoord, sf::Vector2f thirdCoord, sf::Vector2f fourthCoord)
{
	float A1 = secondCoord.y - firstCoord.y;
	float B1 = firstCoord.x - secondCoord.x;
	float C1 = A1 * firstCoord.x + B1 * firstCoord.y;

	float A2 = fourthCoord.y - thirdCoord.y;
	float B2 = thirdCoord.x - fourthCoord.x;
	float C2 = A2 * thirdCoord.x + B2 * thirdCoord.y;

	float x = 0.0f;
	float y = 0.0f;

	float det = A1 * B2 - A2 * B1;
	if (det == 0)
	{
		//parallel
	}
	else
	{
		x = (B2 * C1 - B1 * C2) / det;
		y = (A1 * C2 - A2 * C1) / det;
	}
	return sf::Vector2f(x, y);
}

bool onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
	int val = static_cast<int>((q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y));

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2)
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases 
}



Car::Car() : collisionLines(9)
{
	resetCar();
}

void Car::update(Movement m1, Movement m2)
{
	// Increase/decrease of speed
	if (m1 == Movement::UP)
	{
		if (speed < Settings::MAXIMUM_SPEED)
		{
			speed += Settings::SPEED_INCREASE;
		}
	}
	else if (m1 == Movement::DOWN)
	{
		speed -= Settings::SPEED_DECREASE;
		if (speed < 0.0)
			speed = 0.0;
	}
	// Turning
	if (speed != 0.0)
	{
		if (m2 == Movement::RIGHT)
		{
			angle += Settings::TURN_FORCE;
		}
		else if(m2 == Movement::LEFT)
		{
			angle -= Settings::TURN_FORCE;
		}
	}
	// Update car position

	sf::Vector2f prevPosition = position;

	position.x = static_cast<float>(position.x + speed * sin(-(angle * PI / 180.0)));
	position.y = static_cast<float>(position.y + speed * cos(-(angle * PI / 180.0)));

	double distanceOffset = calculateDistance(prevPosition, position);

	// If car didint move -> dead
	if (distanceOffset == 0.0)
	{
		alive = false;
		return;
	}
	distance += distanceOffset;

	float currentAngle = static_cast<float>(angle) - 90.0f;
	// Set up vision lines
	for (int i = 0; i < 5; i++)
	{
		float radianAngle = static_cast<float>(currentAngle * PI / 180.0f);

		collisionLines[i].first = position;
		collisionLines[i].second = sf::Vector2f(position.x + Settings::VISION_RADIUS * sin(-radianAngle), position.y + Settings::VISION_RADIUS * cos(-radianAngle));;

		currentAngle += 45.0f;
	}
	currentAngle = static_cast<float>(angle) - 45.0f;
	// Set up body lines to handle collision
	for (int i = 5; i < 9; i++)
	{
		float radiusAngle = static_cast<float>(currentAngle * PI / 180.0f);
		// distance from mid point to diagonal points (a^2/2)
		float d = static_cast<float>(Settings::BODY_SIZE.x * sqrt(2) / 2);

		collisionLines[i].first = position;
		collisionLines[i].second = sf::Vector2f(position.x + d * sin(-radiusAngle), position.y + d * cos(-radiusAngle));

		currentAngle += 90.0f;
	}
}

void Car::resetCar()
{
	alive = true;
	position = Settings::SPAWN_LOCATION;
	distance = 0.0;
	inactiveCounter = 0;
	angle = Settings::STARTING_ANGLE;
	speed = 0.0;
}

bool Car::checkCollision(const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& mapBoundaries)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < mapBoundaries.size(); j++)
		{
			// Check if map boundariues lines intersect with body and vision lines
			if (doIntersect(collisionLines[i].first, collisionLines[i].second, mapBoundaries[j].first, mapBoundaries[j].second))
			{
				// If Vision line interestect find intersect point and set it up as new line endPoint
				if (i < 5)
				{
					sf::Vector2f point = checkLinesIntersect(collisionLines[i].first, collisionLines[i].second, mapBoundaries[j].first, mapBoundaries[j].second);
					if (calculateDistance(collisionLines[i].first, collisionLines[i].second) > calculateDistance(collisionLines[i].first, point))
					{
						collisionLines[i].second = point;
					}
				}
				// If body line intersect car is considered as dead
				else
				{
					alive = false;
					return true;
				}
			}
		}
	}
	return false;
}

std::vector<double> Car::getVision()
{
	// Car neural network consist of normalized vision distances and car speed
	std::vector<double> input;
	for (int i = 0; i < 5; i++)
	{
		// Get line length and divide it by maximum possibly length
		input.push_back(calculateDistance(collisionLines[i].first, collisionLines[i].second) / Settings::VISION_RADIUS);
	}
	input.push_back(speed / Settings::MAXIMUM_SPEED);
	return input;
}
