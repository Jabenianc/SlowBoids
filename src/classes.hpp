// Classes.h
// Contains all class declarations.

#ifndef _CLASSES_H_
#define _CLASSES_H_

// Uses vector.
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <tbb/parallel_for.h>
#include <mutex>

// Forward declarations of classes.
class Vector2;
class Flightspace;
class Boid;
class ObstacleGroup;

// Typedef for our "spatial hash"
using boid_hash =  std::unordered_map<std::string, std::vector<Boid*>>;
using hash_pair = std::pair<std::string, std::vector<Boid*>>;
using obs_hash = std::unordered_map<std::string, std::vector<Vector2*>>;
using obs_pair = std::pair<std::string, std::vector<Vector2*>>;

// Simple Vector2 class, we omit the cross/dot product.
class Vector2 {
public:
	// x and y variables.
	float x, y;

	// Overloads to perform vector
	// calculations.
	Vector2 operator+(const Vector2& a_vector) const;
	Vector2 operator-(const Vector2& a_vector) const;
	Vector2 operator*(const Vector2& a_vector) const;

	Vector2(float x = 0.0, float y = 0.0);

	// Scales our vector by a scalar and returns that vector
	Vector2 scaled(float scalar) const;

	// Gets the distance to another vector
	float distance_to(const Vector2& a_vector) const;

	// Returns a vector interpolated in between this one and another.
	Vector2 linear_interpolate(const Vector2& a_vector, float amount) const;

	// Returns the magnitude of the vector
	float magnitude() const;

	// Normalizes the vector.
	Vector2 normalized() const;
};

// Simple Flightspace class.
// Represents an aggregate of boid objects, and obstacles.
class Flightspace {
public:
	// Destructor.
	Flightspace();
	~Flightspace();

	// Populates the flock.
	void random_populate(unsigned int size, int xmax=100,
		int ymax=100, float speed=2.5, float agility=0.1,
        float speed_v=0.0, float agility_v=0.0);

	// Updates the flock.
	void update();

	// Gets the size of the flock.
	int get_size() const;

	// Gets a boid at said index.
	Boid* get_boid(int index = 0) const;

	// Sets the obstacle group.
	void set_obstacles(std::vector<Vector2*>* p_obstacles);
private:
	void spatial_hash();
	static const float _M_CELLSIZE;
	std::vector<Boid*>* _mp_boids;
	std::vector<Vector2*>* _mp_obstacles;

	// Spatial hashes for obstacles and boids.
	obs_hash* _mp_obs_hash;
	boid_hash* _mp_hash;
};

// Simple Boid class.
class Boid {
public:
	// Constructor (There is no default constructor for this class)
	Boid(Vector2 position, Vector2 dir,
		float speed=2.5, float agility=0.1);
	// There is no destructor for this class as
	// we only need a pointer to the flock for referencing.

	// Member functions.
	void apply_rules(boid_hash* p_hash, obs_hash* p_obs_hash);
	void move();
	double get_rotation() const;

	// Accessor functions.
	Vector2 get_pos() const;
	Vector2 get_direction() const;

	// Setter functions.
	void set_cell(const std::string& cell);
	void set_pos(float x, float y);

	// Setter functions, but it sets three data members.
	static void change_behaviour(float separate,
		float align, float cohede, float avoid);

	// Function that binds the position.
	void bind_position(int xmin, int xmax, int ymin, int ymax);

    // The "desire" for a boid to
    // do enact the three rules:
    // separation, alignment, and cohesion.
    static float m_separate;
    static float m_align;
    static float m_cohede;
    static float m_avoid;
private:
	// Applies rules to the boid.
	Vector2 compute(boid_hash* p_hash, obs_hash* p_obs_hash);

	// Constant values for our perception radii.
	static const int _M_PERCEPT;

	// Speed and agility
	// (turn speed) of the boid.
	float _m_speed;
	float _m_agility;

	// Cell identfier.
	std::string _m_cell;

	// Two vectors, one for travelling
	// direction, one for position.
	Vector2 _m_position;
	Vector2 _m_dir;
};

class ObstacleGroup {
public:
	ObstacleGroup(float remove_radius=50,
		float pack_radius=5, int max_obstacles=200);

	~ObstacleGroup(); // Deletes all dynamically allocated vector2's.

	// Adds a single obstacle at x, y
	void add_obstacle(float x, float y);
	// Removes obstacles close enough to x, y
	void remove_obstacles(float x, float y);
	// Clears all obstacles.
	void clear_all();

	int get_size() const;
	std::vector<Vector2*>* get_obstacles();
private:
	int _m_max_obstacles;
	float _m_remove_radius;
	float _m_pack_radius;
	std::vector<Vector2*> m_obstacles;
};

#endif
