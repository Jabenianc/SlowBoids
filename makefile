# The name of the executable
EXEC = boids

# Folder that executable will go within
BUILDFOLDER = build

# Compiler
CXX = clang++

# Directories
INCLUDE_DIR = /opt/homebrew/include
LIB_DIR = /opt/homebrew/lib

# Compiler flags
STD = -std=c++17
# Optimize hard for this build (use -O3 if you want to go less AGRESSIVE)
OPTIMIZATION_LEVEL = -Ofast
DEBUG_LEVEL = -g0 # Don't debug
CXXFLAGS = $(STD) $(DEBUG_LEVEL) $(OPTIMIZATION_LEVEL)

# Linker related
# External libraries to link with.
LDFLAGS = -g
LDLIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ltbb

# Files
SRC_FILES = \
	src/main.cpp src/initialize.cpp \
	src/classes.cpp src/tinyerror.cpp \
	src/wrappers.cpp
HEADER_FILES = \
	src/initialize.hpp \
	src/classes.hpp src/tinyerror.hpp \
	src/wrappers.hpp src/rng.hpp

# In this case, object file filenames are just
# the source code filenames ones replaced with a '.o'
# Remove the path prefix 'src/'
OBJ_FILES = main.o initialize.o classes.o tinyerror.o wrappers.o

# Building
all: boid_sim
	@echo "Finished build :3"

# Main Build, the requirements on objects will
# build all the object files.
boid_sim: $(OBJ_FILES)
	@echo "Building executable!"
	$(CXX) $(LDFLAGS) $(OBJ_FILES) -o $(BUILDFOLDER)/$(EXEC) \
	-I$(INCLUDE_DIR) -L$(LIB_DIR) $(LDLIBS)

# Building object files
main.o: $(SRC_FILES) $(HEADER_FILES)
	@echo "building main.o"
	$(CXX) $(CXXFLAGS) -c src/main.cpp -I$(INCLUDE_DIR)

initialize.o: src/initialize.hpp src/initialize.cpp src/wrappers.cpp \
	src/wrappers.hpp src/tinyerror.hpp src/tinyerror.cpp
	@echo "building initialize.o"
	$(CXX) $(CXXFLAGS) -c src/initialize.cpp -I$(INCLUDE_DIR)

classes.o: src/classes.hpp src/classes.cpp src/rng.hpp
	@echo "building classes.o"
	$(CXX) $(CXXFLAGS) -c src/classes.cpp -I$(INCLUDE_DIR)

tinyerror.o: src/tinyerror.hpp src/tinyerror.cpp
	@echo "building tinyerror.o"
	$(CXX) $(CXXFLAGS) -c src/tinyerror.cpp -I$(INCLUDE_DIR)

wrappers.o: src/wrappers.hpp src/wrappers.hpp src/tinyerror.hpp \
	src/tinyerror.cpp
	@echo "building wrappers.o"
	$(CXX) $(CXXFLAGS) -c src/wrappers.cpp -I$(INCLUDE_DIR)


.PHONY: all clean very-clean

# Deletes everything generated
super-clean:
	rm $(OBJ_FILES)
	rm $(BUILDFOLDER)/$(EXEC)
	@echo "Super-Ultra clean! Removed executable file and objects in one swoop!"

# Deletes the object files
clean:
	rm $(OBJ_FILES)
	@echo "cleaned objects :D"

# Deletes the executable file
very-clean:
	rm $(BUILDFOLDER)/$(EXEC)
	@echo "deep clean: removed executable file $(EXEC) :D"
