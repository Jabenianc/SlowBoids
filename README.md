# SlowBoids
A repository for my first C++ project, boids.
- Made with C++ and SDL2. Sounds and music are royalty free,
  obstacle and boid sprites are made by me.
- A decent computer should be able to run 2500 boids
- Requires SDL2, SDL2_mixer, SDL2_image, SDL2_ttf, and tbb
  (for parallel processing)

- Issues that you will see that I will have to fix in future projects
  
  ~ Header file structuring is weird, first time making a multifile project
  
  ~ Vectors should probably be done with pointers to be more efficient to prevent
    objects being copied a bunch of times.
  
  ~ More stuff should be put into initialize.cpp, less stuff for main.cpp
  
  ~ The way I managed events was a bit ugly.
  
  ~ Spatial hashing algo was slower as I converted each agents Vector2 cellid
    into a string object, instead of doing it faster with char arrays.
  
  ~ I structured the flock, boid, obstacle relationship in a weird way,
    and ended up writing two algorithms for the spatial hashing.
  
  ~ Likely some unused variables in there from editing and forgetting to delete.
