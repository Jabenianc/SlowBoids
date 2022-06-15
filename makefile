boids:
	g++ -std=c++11 \
		main.cpp initialize.cpp classes.cpp wrappers.cpp tinyerror.cpp \
		-o boids \
		-I/usr/local/include -L/usr/local/lib \
		-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ltbb
