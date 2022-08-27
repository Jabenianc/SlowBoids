// Initialize.h
// Contains the start up helper functions for SDL

#ifndef _INITIALIZE_H_
#define _INITIALIZE_H_

// Uses wrappers.h
// Some initializer functions request
// SDL wrapper objects.
#include "wrappers.hpp"
#include "tinyerror.hpp"

// Extern variables.
extern const int SCR_W;
extern const int SCR_H;
extern const int SFX_VOLUME;
extern const int NUM_BOIDS;
extern const std::string ASSET_DIR;

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
bool _init();
void _init_video(bool& success);
void _init_ttf(bool& success);
void _init_mixer(bool& success);
void _init_window(bool& success);
void _init_renderer(bool& success);
void _set_filter();
void _init_sdl_image(bool& success);

bool try_init();

// Do not call these before initialized, I didn't implement a failsafe
bool _asset_load();
void _asset_destroy();

// Media loaders
void _load_tex(bool& success,
	TextureWrap* p_tex, const std::string& path);
void _load_textbox(bool& success, TextBox* p_tbox, const std::string& msg);

// Error report message functions.
void _criterion_not_met();


void close();

#endif
