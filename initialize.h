// Initialize.h
// Contains the start up helper functions for SDL

#ifndef _INITIALIZE_H_
#define _INITIALIZE_H_

// Uses SDL headers.
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// Uses string and iostream.
#include <string>
#include <iostream>

// Uses wrappers.h
// Some initializer functions request
// SDL wrapper objects.
#include "wrappers.h"
#include "tinyerror.h"

// Extern variables.
extern const int SCR_W;
extern const int SCR_H;
extern const int SFX_VOLUME;
extern const int NUM_BOIDS;

// Window and renderer.
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;

// Global game objects.
// Graphical objects.
extern TextureWrap* g_tex_boid;
extern TextureWrap* g_tex_vignette;
extern TextureWrap* g_tex_obstacle;
extern TextBox* g_titlebox;
extern TextBox* g_textbox;

// Sound objects.
extern Playlist* g_playlist;
extern SFX* g_click_sfx;

// Functions
static bool init();
static void init_video(bool* p_success);
static void init_ttf(bool* p_success);
static void init_mixer(bool* p_success);
static void init_window(bool* p_success);
static void init_renderer(bool* p_success);
static void set_filter();
static void init_sdl_image(bool* p_success);

// Startup functions
bool try_init();

// DO NOT RUN THESE BEFORE SDL HAS INITIALIZED!
// I was too lazy to figure a failsafe for these functions.
static bool asset_load();
static void asset_destroy();

// Error report message functions.
static void criterion_not_met();

// Media loader
static void load_tex(bool* p_success,
	TextureWrap* p_tex, const std::string& path);
static void load_textbox(bool* p_success,
	TextBox* p_tbox, const std::string& msg);

// Deinitializer/Program shutdown
void close();

#endif
