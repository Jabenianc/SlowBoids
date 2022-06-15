// Initialize.cpp
// Defines initializer functions.
// Uses initialize.h
#include "initialize.h"

// Screen parameters.
const int SCR_W = 1280;
const int SCR_H = 720;
const int SFX_VOLUME = 64;
const int NUM_BOIDS = 1690;

// Global pointers to sdl objects.
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

// Global game objects.
// Graphical objects.
TextureWrap* g_tex_boid = new TextureWrap();
TextureWrap* g_tex_vignette = new TextureWrap();
TextureWrap* g_tex_obstacle = new TextureWrap();
TextBox* g_titlebox = new TextBox();
TextBox* g_textbox = new TextBox();

// Sound objects.
Playlist* g_playlist = new Playlist();
SFX* g_click_sfx = new SFX();

// Initializes SDL using all the sub-functions.
bool init() {
	// Initializer flag.
	bool success = true;

	// Call all the initializer functions.
	init_video(&success);
	init_ttf(&success);
	init_mixer(&success);
	init_window(&success);
	init_renderer(&success);
	set_filter();
	init_sdl_image(&success);

	return success;
}

// Initializes SDL.
void init_video(bool* p_success) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		error_msg("SDL could not initialize!", REGULAR_ERROR);
		*p_success = false;
	}
}

void init_ttf(bool* p_success) {
	if (TTF_Init() < 0) {
		error_msg("SDL_TTF could not initialize!", TEXT_ERROR);
		*p_success = false;
	}
}

void init_mixer(bool* p_success) {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		error_msg("SDL_mixer could not initialize!", MIXER_ERROR);
		*p_success = false;
	}
	Mix_AllocateChannels(SFX::get_max_channels());
}
// Makes an sdl window.
void init_window(bool* p_success)
{
	if (*p_success) {
		// Create an sdl window.
		g_window = SDL_CreateWindow("Chillin With the Boids",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCR_W, SCR_H, SDL_WINDOW_SHOWN);
		if (g_window == NULL) {
			error_msg("Window creation failed!", REGULAR_ERROR);
			// Set success to false.
			*p_success = false;
		}
	}
	else {
		criterion_not_met();
	}
}

// Initializes our renderer.
void init_renderer(bool* p_success) {
	if (*p_success) {
		g_renderer = SDL_CreateRenderer(g_window,
			-1, SDL_RENDERER_ACCELERATED);
		// Initialization failed.
		if (g_renderer == NULL) {
			error_msg("Renderer create failed!", REGULAR_ERROR);
			*p_success = false;
		}
		else {
			// Initialize renderer color if
			// the renderer was created successfully.
			SDL_SetRenderDrawColor(
				g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
	}
	else {
		criterion_not_met();
	}
}

// Sets texture filtering to linear.
void set_filter() {
	if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))) {
		std::cout << "Warning: Linear texture filtering not enabled!\n";
	}
}

// Initializes SDL_image.
void init_sdl_image(bool* p_success) {
	if (*p_success) {
		int img_flags = IMG_INIT_PNG;
		// Checks if IMG_init(img_flags) and image_flags
		// are the same.
		if (!(IMG_Init(img_flags) & img_flags)) {
			// If they are not the same, throw an error.
			error_msg("SDL_image could not initialize!",
				IMAGE_ERROR);
			*p_success = false;
		}
	}
	else {
		criterion_not_met();
	}
}

bool try_init() {
	bool init_passed = init();
	if (!init_passed) {
		std::cout << "Failed to initialize!!\n";
	}
	else {
		std::cout << "Initialization worked!\n";
		init_passed = asset_load(); // Further load assets.
	}
	return init_passed;
}

bool asset_load() {
	bool load_passed = true;
	// Initialize objects.
	g_titlebox->configure("assets/blackpast.ttf", 32);
	g_textbox->configure("assets/blackpast.ttf", 20);

	// Set SFX volume.
	SFX::global_volume(SFX_VOLUME);

	g_click_sfx->change_channel(0);
	g_click_sfx->load_audio("assets/button.wav");

	// Playlist object setup.
	g_playlist->add_song("assets/bornfree.mp3");
	g_playlist->add_song("assets/spanishbeach.mp3");
	g_playlist->add_song("assets/acupofjoe.mp3");

	// Load media
	// (this time with flags because absence of textures is not okay)
	load_tex(&load_passed, g_tex_boid, "assets/boid.png");
	load_tex(&load_passed, g_tex_vignette, "assets/vignette.png");
	load_tex(&load_passed, g_tex_obstacle, "assets/obstacle.png");
	load_textbox(&load_passed, g_titlebox, "A Boids Simulation");
	load_textbox(&load_passed, g_textbox,
		"M/N = Mute/Unmute, R = remove obstacles");
	return load_passed;
}

void asset_destroy() {
	// Deallocate all objects memory.
	// Kinda a wall of text.
	delete g_tex_boid; g_tex_boid = nullptr;
	delete g_tex_vignette; g_tex_vignette = nullptr;
	delete g_tex_obstacle; g_tex_obstacle = nullptr;
	delete g_titlebox; g_titlebox = nullptr;
	delete g_textbox; g_textbox = nullptr;
	delete g_click_sfx; g_click_sfx = nullptr;
	delete g_playlist; g_playlist = nullptr;
}

// Does stuff is previous initialization criterion
// were not met.
void criterion_not_met() {
	std::cout << "Prior criteria not met!\n";
}

// Loads a texture from a path.
void load_tex(bool* p_success,
	TextureWrap* p_tex, const std::string& path)
{
	// Load our image into the texture.
	if (!(p_tex->load_from_file(path, g_renderer))) {
		std::cout << "Failed to load texture at: ";
		std::cout << path << "\n";
		*p_success = false;
	}
}

void load_textbox(bool* p_success,
	TextBox* p_tbox, const std::string& msg)
{
	if (!(p_tbox->load_text(msg, g_renderer))) {
		std::cout << "Failed to load textbox with message:\n";
		std::cout << msg << "\n";
		*p_success = false;
	}
}

// Closes SDL
void close() {
	// Destroy resources.
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	// Prevent dangling pointers
	g_window = NULL;
	g_renderer = NULL;

	// Deallocate all objects memory.
	asset_destroy();

	// Quit SDL subsystems.
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
