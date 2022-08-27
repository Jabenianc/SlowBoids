// Classes.cpp
// Class definitions for
// Boid and Flightspace Classes,
// as well as our SDL wrapper function.

// Uses classes.h and rng.h for random number distribution.
#include "classes.hpp"
#include "rng.hpp"
#include <iostream>

// Member function definitions for Vector2
// Operator overloads
inline Vector2 Vector2::operator+(const Vector2& a_vector) const {
	return Vector2(x+a_vector.x, y+a_vector.y);
}

inline Vector2 Vector2::operator-(const Vector2& a_vector) const {
	return Vector2(x-a_vector.x, y-a_vector.y);
}

inline Vector2 Vector2::operator*(const Vector2& a_vector) const {
	return Vector2(x*a_vector.x, y*a_vector.y);
}

// Constructor.
Vector2::Vector2(float x, float y):
	x(x), y(y)
{}

// Scales vector by a scalar
inline Vector2 Vector2::scaled(float scalar) const {
	return Vector2(x*scalar, y*scalar);
}

// Gets the distance to a vector
inline float Vector2::distance_to(const Vector2& a_vector) const {
	return (a_vector - *this).magnitude();
}

// Returns a vector that is an interpolation
// between vectors A and B.
inline Vector2 Vector2::linear_interpolate(const Vector2& a_vector, float amount) const {
	return Vector2(
        x + ((a_vector.x - x) * amount),
        y + ((a_vector.y - y) * amount)
    );
}

inline float Vector2::magnitude() const {
	return std::sqrt(x*x + y*y);
}

// Makes vector magnitude = 1
inline Vector2 Vector2::normalized() const {
	// Make sure the magnitude is not zero!!!
	float mag = magnitude();
    return (mag != 0)? Vector2(x / mag, y / mag) : *this;
}

// Boid static variable declarations.
// Boid behaviour 'strengths'
float Boid::_m_separate = 2.12;
float Boid::_m_align = 1.35;
float Boid::_m_cohede = 2.05;
float Boid::_m_avoid = 5.0;

// Boid perception radii
const int Boid::_M_PERCEPT = 20;

// Member function definitions for Boid
Boid::Boid(Vector2 position, Vector2 dir,
	float speed, float agility):
	_m_speed(speed),
    _m_agility(agility),
    _m_position(position),
    _m_dir(dir)
{}

// Boid behaviour 1: separation.
Vector2 Boid::compute(boid_hash* p_hash, obs_hash* p_obs_hash) {
	// Separation.
	Vector2 separate(0.0, 0.0);
	Vector2 s_steer(0.0, 0.0);
	// Alignment.
	Vector2 align(0.0, 0.0);
	int align_locals = 0;
	// Cohesion.
	// Uses c_vector to average locals' position, then to get the turn
	// vector to that position.
	Vector2 c_vector(0.0, 0.0);
	int cohede_locals = 0;

	// Look through our key.
	std::vector<Boid*>* local_cell = &(p_hash->at(_m_cell));

	// Traverse all boids in the same cell.
    for (auto it : *local_cell) {
		Boid* p_boid = it;
		float dist = _m_position.distance_to(p_boid->get_pos());
		if (p_boid != this && dist < _M_PERCEPT) {
			// Separation.
			s_steer = _m_position - p_boid->get_pos();
			separate = separate + s_steer.scaled((_M_PERCEPT - dist) / _M_PERCEPT);
			// Alignment.
			align = align + p_boid->get_direction();
			++align_locals;
			// Cohesion (getting center of mass)
			c_vector = c_vector + p_boid->get_pos();
			++cohede_locals;
		}
	}
	// Avoidance.
	Vector2 avoid(0.0, 0.0);
	Vector2 a_steer(0.0, 0.0);

	// Obstacle looping.
	// Check if there are obstacles in the local cell.
	if (p_obs_hash->find(_m_cell) != p_obs_hash->end()) {
		std::vector<Vector2*>* local_obs = &(p_obs_hash->at(_m_cell));
		for (auto it : *local_obs) {
			Vector2* p_obstacle = it;
			float dist = _m_position.distance_to(*p_obstacle);
			if (dist < _M_PERCEPT) {
				a_steer = _m_position - (*p_obstacle);
				avoid = avoid + a_steer.scaled((_M_PERCEPT - dist) / _M_PERCEPT);
			}
		}
	}

	// Vector processing.
	separate = separate.normalized().scaled(_m_separate);
	avoid = avoid.normalized().scaled(_m_avoid);
	if (align_locals > 0) {
		// Average
		align.x /= align_locals;
		align.y /= align_locals;
		// Normalize/Scale
		align = align.normalized().scaled(_m_align);
	}
	if (cohede_locals > 0) {
		// Average
		c_vector.x /= cohede_locals;
		c_vector.y /= cohede_locals;
		// Turn the c_vector now into the vector
		// pointing to what it used to be, the center of mass
		c_vector = (c_vector - _m_position).normalized().scaled(_m_cohede);
	}
	return separate + align + c_vector + avoid;
}

// Apply the rules of the boids.
void Boid::apply_rules(boid_hash* p_hash, obs_hash* p_obs_hash) {
	_m_dir = _m_dir.linear_interpolate(compute(p_hash, p_obs_hash), _m_agility);
	_m_dir = _m_dir.normalized().scaled(_m_speed);
}

void Boid::move() {
	// Add the movement vector.
	_m_position = _m_position + _m_dir;
}

double Boid::get_rotation() const {
	double deg = 0.0;
	if (_m_dir.x != 0) { deg = atan(_m_dir.y/_m_dir.x) * 57.2958; }
	if (_m_dir.x < 0) { deg -= 180.0; }
	return deg;
}

Vector2 Boid::get_pos() const {
	return _m_position;
}

Vector2 Boid::get_direction() const {
	return _m_dir;
}

void Boid::set_cell(const std::string& cell) {
	_m_cell = cell;
}

void Boid::set_pos(float x, float y) {
	_m_position.x = x;
	_m_position.y = y;
}

// Setter functions.
// (Static function)
void Boid::change_behaviour(float separate,
	float align, float cohede, float avoid)
{
	_m_separate = separate;
	_m_align = align;
	_m_cohede = cohede;
	_m_avoid = avoid;
}

// Binds the boids position to set position.
void Boid::bind_position(int xmin, int xmax, int ymin, int ymax) {
	_m_position.x = (_m_position.x < xmin)? xmax : _m_position.x;
    _m_position.x = (_m_position.x > xmax)? xmin : _m_position.x;
	_m_position.y = (_m_position.y < ymin)? ymax : _m_position.y;
	_m_position.y = (_m_position.y > ymax)? ymin : _m_position.y;
}

// Member function definitions for Flightspace.
const float Flightspace::_M_CELLSIZE = 25.0f;

Flightspace::Flightspace() {
	_mp_boids = new std::vector<Boid*>();
	_mp_hash = new boid_hash();
	_mp_obs_hash = new obs_hash();
}

Flightspace::~Flightspace() {
	_mp_hash->clear(); // Clear our spatial hash
	_mp_obs_hash->clear();

    // Deletes the hash, but not the boids within the vectors.
	delete _mp_hash;
	delete _mp_obs_hash;

	// Deallocate memory for boids.
	for (auto boid : *_mp_boids) {
		delete boid; // Gets the boid pointer.
	}
	delete _mp_boids;
	// Not deleting _mp_obstacles as that is
	// supposed to be a pointer to a vector in a obstaclegroup object.
}

void Flightspace::random_populate(unsigned int size, int xmax,
	int ymax, float speed, float agility, float speed_v, float agility_v)
{
	for (int i = 0; i < size; i++) {
		Vector2 position(randint(0, xmax), randint(0, ymax));
		Vector2 dir(randint(-1, 1), randint(-1, 1));

        // Randomize speed and agility
        float variance_speed = (speed_v/speed)*100.0f; // Speed variance
        float variance_agil = (agility_v/speed)*100.0f; //

        speed += uniform(-variance_speed, variance_speed);
        agility += uniform(-variance_agil, variance_agil);

        // Normalize direction and scale the power.
		dir = dir.normalized().scaled(speed);

        // Instanciate and add the boid.
        Boid* new_boid = new Boid(position, dir,
            speed + uniform(-0.25, 0.25), agility);
		_mp_boids->push_back(new_boid);
	}
}

// Probably super inefficient lol.
void Flightspace::spatial_hash() {
	// Clear hashes
	_mp_hash->clear();
	_mp_obs_hash->clear();
	std::mutex mtx;

	tbb::parallel_for(tbb::blocked_range<int>(0, _mp_boids->size()),
	[&](tbb::blocked_range<int> r)
	{
		for (int i = r.begin(); i < r.end(); i++) {
			// Assign the boid to a cell.
			Boid* p_boid = _mp_boids->at(i);

			// Find the boid's cell.
			int x = ceil(p_boid->get_pos().x/_M_CELLSIZE);
			int y = ceil(p_boid->get_pos().y/_M_CELLSIZE);
			std::string cell = std::to_string(x) + std::to_string(y);
			p_boid->set_cell(cell);

			// Insert the boid to the multimap.
			mtx.lock();
			// Try and make a new key and value pair.
			_mp_hash->insert(hash_pair(cell, std::vector<Boid*>()));
			_mp_hash->at(cell).push_back(p_boid);
			mtx.unlock();
		}
	});

	tbb::parallel_for(tbb::blocked_range<int>(0, _mp_obstacles->size()),
	[&](tbb::blocked_range<int> r)
	{
		for (int i = r.begin(); i < r.end(); i++) {
			// Assign the obstacle to a cell.
			Vector2* p_vec = _mp_obstacles->at(i);

			// Find the obstacle's cell.
			int x = ceil(p_vec->x/_M_CELLSIZE);
			int y = ceil(p_vec->y/_M_CELLSIZE);
			std::string cell = std::to_string(x) + std::to_string(y);

			// Insert the obstacle to the multimap.
			mtx.lock();
			// Try and make a new key and value pair.
			_mp_obs_hash->insert(obs_pair(cell, std::vector<Vector2*>()));
			_mp_obs_hash->at(cell).push_back(p_vec);
			mtx.unlock();
		}
	});
}

void Flightspace::update() {
	spatial_hash(); // Hash the boids.
	// Use parallel processing for this.
	tbb::parallel_for(tbb::blocked_range<int>(0, _mp_boids->size()),
	[&](tbb::blocked_range<int> r)
	{
		for (int i = r.begin(); i < r.end(); i++) {
			// Tell each boid to apply their rules, and pass
			// the pointer to the spatial hash.
			_mp_boids->at(i)->apply_rules(_mp_hash, _mp_obs_hash);
		}
	});
}

Boid* Flightspace::get_boid(int index) const {
	return _mp_boids->at(index);
}

int Flightspace::get_size() const {
	return _mp_boids->size();
}

void Flightspace::set_obstacles(std::vector<Vector2*>* p_obstacles) {
	_mp_obstacles = p_obstacles;
}

// Member function definitions for ObstacleGroup
ObstacleGroup::ObstacleGroup(float remove_radius,
	float pack_radius, int max_obstacles):
    _m_max_obstacles(max_obstacles),
	_m_remove_radius(remove_radius),
	_m_pack_radius(pack_radius)
{}

ObstacleGroup::~ObstacleGroup() {
	for (int i = 0; i < m_obstacles.size(); i++) {
		// Calls default destructor on Vector2 pointer.
		delete m_obstacles[i];
        m_obstacles.pop_back();
	}
}

void ObstacleGroup::add_obstacle(float x, float y) {
	Vector2 add_site(x, y);
	bool allowed = true;
	for (int i = 0; i < m_obstacles.size(); i++) {
		Vector2* p_obstacle = m_obstacles[i];
		if (add_site.distance_to(*p_obstacle) < _m_pack_radius) {
			allowed = false;
		}
	}

	// If we don't break a packing limit
	// and we can still add obstacles
	if (allowed && (m_obstacles.size() < _m_max_obstacles)) {
		m_obstacles.push_back(new Vector2(x, y));
	}
}

void ObstacleGroup::remove_obstacles(float x, float y) {
	Vector2 remove_site(x, y);
	for (int i = 0; i < m_obstacles.size(); i++) {
		Vector2* p_obstacle = m_obstacles[i];
		if (remove_site.distance_to(*p_obstacle) < _m_remove_radius) {
			// Deallocate memory for the Vector2
			delete p_obstacle;
			// Remove the Vector2* itself.
			m_obstacles.erase(m_obstacles.begin() + i);
		}
	}
}

void ObstacleGroup::clear_all() {
	// Reverse iterating through the
	// obstacles and deleting them.
	for (int i = m_obstacles.size()-1; i >= 0; i--) {
		delete m_obstacles[i]; // Deallocate
		m_obstacles.pop_back(); // Remove element.
	}
}

int ObstacleGroup::get_size() const {
	return m_obstacles.size();
}

std::vector<Vector2*>* ObstacleGroup::get_obstacles() {
	return &m_obstacles;
}
