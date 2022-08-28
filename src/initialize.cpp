// Initialize.cpp
// Defines initializer functions.
// Uses initialize.h
#include "initialize.hpp"

// Screen parameters.
constexpr int SCR_W = 1280;
constexpr int SCR_H = 720;
constexpr int SFX_VOLUME = 64;
constexpr int NUM_BOIDS = 2250;
const std::string ASSET_DIR = "res/";

// Global pointers to sdl objects.
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

// Global game objects.
// Graphical objects.
TextureWrap* g_tex_boid = new TextureWrap();
TextureWrap* g_tex_vignette = new TextureWrap();
TextureWrap* g_tex_obstacle = new TextureWrap();
Slider* g_slider_separation;
Slider* g_slider_alignment;
Slider* g_slider_cohesion;
TextBox* g_titlebox = new TextBox();
TextBox* g_textbox = new TextBox();

// Sound objects.
Playlist* g_playlist = new Playlist();
SFX* g_click_sfx = new SFX();

// Initializes SDL using all the sub-functions.
bool _init() {
	// Initializer flag.
	bool success = true;

	// Call all the initializer functions.
	_init_video(success);
	_init_ttf(success);
	_init_mixer(success);
	_init_window(success);
	_init_renderer(success);
	_set_filter();
	_init_sdl_image(success);

	return success;
}

// Initializes SDL.
void _init_video(bool& success) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		error_msg("SDL could not initialize!", error_types::REGULAR_ERROR);
		success = false;
	}
}

void _init_ttf(bool& success) {
	if (TTF_Init() < 0) {
	    error_msg("SDL_TTF could not initialize!", error_types::TEXT_ERROR);
		success = false;
	}
}

void _init_mixer(bool& success) {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		error_msg("SDL_mixer could not initialize!", error_types::MIXER_ERROR);
		success = false;
	}
	Mix_AllocateChannels(SFX::get_max_channels());
}
// Makes an sdl window.
void _init_window(bool& success)
{
	if (success) {
		// Create an sdl window.
		g_window = SDL_CreateWindow("Chillin With the Boids",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCR_W, SCR_H, SDL_WINDOW_SHOWN);
		if (g_window == NULL) {
			error_msg("Window creation failed!", error_types::REGULAR_ERROR);
			// Set success to false.
			success = false;
		}
	}
	else {
		_criterion_not_met();
	}
}

// Initializes our renderer.
void _init_renderer(bool& success) {
	if (success) {
		g_renderer = SDL_CreateRenderer(g_window,
			-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		// Initialization failed.
		if (g_renderer == NULL) {
			error_msg("Renderer create failed!", error_types::REGULAR_ERROR);
			success = false;
		}
		else {
			// Initialize renderer color if
			// the renderer was created successfully.
			SDL_SetRenderDrawColor(
				g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
	}
	else {
		_criterion_not_met();
	}
}

// Sets texture filtering to linear.
void _set_filter() {
	if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))) {
		std::cout << "Warning: Linear texture filtering not enabled!\n";
	}
}

// Initializes SDL_image.
void _init_sdl_image(bool& success) {
	if (success) {
		int img_flags = IMG_INIT_PNG;
		// Checks if IMG_init(img_flags) and image_flags
		// are the same.
		if (!(IMG_Init(img_flags) & img_flags)) {
			// If they are not the same, throw an error.
			error_msg("SDL_image could not initialize!",
				error_types::IMAGE_ERROR);
			success = false;
		}
	}
	else {
		_criterion_not_met();
	}
}

bool try_init() {
	bool init_passed = _init();
	if (!init_passed) {
		std::cout << "Failed to initialize :(!!\n";
	}
	else {
		std::cout << "Initialization Ran Successfully :3!\n";
		init_passed = _asset_load(); // Further load assets.
	}
	return init_passed;
}

bool _asset_load() {
	bool load_passed = true;
	// Initialize objects.
	g_titlebox->configure(ASSET_DIR + "aquire.ttf", 32);
	g_textbox->configure(ASSET_DIR + "aquire.ttf", 20);

	// Set SFX volume.
	SFX::global_volume(SFX_VOLUME);

	g_click_sfx->change_channel(0);
	g_click_sfx->load_audio(ASSET_DIR + "button.wav");

	// Playlist object setup.
	g_playlist->add_song(ASSET_DIR + "bornfree.mp3");
	g_playlist->add_song(ASSET_DIR + "spanishbeach.mp3");
	g_playlist->add_song(ASSET_DIR + "acupofjoe.mp3");

    // Shit code section, no failsafes because I added this in quick
    SDL_Rect button_rect = SDL_Rect{10, 10, 16, 16};
    g_slider_cohesion = new Slider(
        SDL_Rect{SCR_W - 330, SCR_H - 30, 300, 10}, button_rect,
        0.0, 3.5f, new TextBox(ASSET_DIR + "aquire.ttf", 20));
    g_slider_alignment = new Slider(
        SDL_Rect{SCR_W - 330, SCR_H - 70, 300, 10}, button_rect,
         0.0, 3.5f, new TextBox(ASSET_DIR + "aquire.ttf", 20));
    g_slider_separation = new Slider(
        SDL_Rect{SCR_W - 330, SCR_H - 110, 300, 10}, button_rect,
         0.0, 3.5f, new TextBox(ASSET_DIR + "aquire.ttf", 20));
    // -------------------------------------------------

    // Load media
	// (this time with flags because absence of textures is not okay)
	_load_tex(load_passed, g_tex_boid, ASSET_DIR + "boid.png");
	_load_tex(load_passed, g_tex_vignette, ASSET_DIR + "vignette.png");
	_load_tex(load_passed, g_tex_obstacle, ASSET_DIR + "obstacle.png");
	_load_textbox(load_passed, g_titlebox, "A Boids Simulation");
	_load_textbox(load_passed, g_textbox,
		"M = Mute, N = Unmute, R = Remove Obstacles");
	return load_passed;
}

void _asset_destroy() {
	// Deallocate all objects memory.
	// Kinda a wall of text.
	delete g_tex_boid; g_tex_boid = nullptr;
	delete g_tex_vignette; g_tex_vignette = nullptr;
	delete g_tex_obstacle; g_tex_obstacle = nullptr;
	delete g_titlebox; g_titlebox = nullptr;
	delete g_textbox; g_textbox = nullptr;
	delete g_click_sfx; g_click_sfx = nullptr;
	delete g_playlist; g_playlist = nullptr;
    delete g_slider_cohesion; g_slider_cohesion = nullptr;
    delete g_slider_alignment; g_slider_alignment = nullptr;
    delete g_slider_separation; g_slider_separation = nullptr;
}

// Does stuff is previous initialization criterion
// were not met.
void _criterion_not_met() {
	std::cout << "Prior criteria not met!\n";
}

// Loads a texture from a path.
void _load_tex(bool& success,
	TextureWrap* p_tex, const std::string& path)
{
	// Load our image into the texture.
	if (!(p_tex->load_from_file(path, g_renderer))) {
		std::cout << "Failed to load texture at: ";
		std::cout << path << "\n";
		success = false;
	}
}

void _load_textbox(bool& success,
	TextBox* p_tbox, const std::string& msg)
{
	if (!(p_tbox->load_text(msg, g_renderer))) {
		std::cout << "Failed to load textbox with message:\n";
		std::cout << msg << "\n";
		success = false;
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
	_asset_destroy();

	// Quit SDL subsystems.
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
