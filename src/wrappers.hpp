// Wrappers.h
// Contains some SDL wrapper classes.
#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

// Uses vector
#include <vector>
#include <cstdint>

// Uses wrappers.h
#include "tinyerror.hpp"

// Class forward declarations.
// Graphics Wrapper Classes
class TextureWrap;
class TextBox;

// Sound Wrapper Classes
class MusicPlayer;
class SFX;
class Playlist;

// Simple SDL_Texture wrapper class.
class TextureWrap {
public:
	// Constructor.
	TextureWrap();
	// Destructor.
	~TextureWrap();
	// Loads an image from a file
	bool load_from_file(const std::string& path,
		SDL_Renderer* p_renderer);
	// Deallocates texture
	void free_texture();
	// Renders Texture at given point
	void render_at(int x, int y, double rotation,
		SDL_Renderer* p_renderer, int shrink=1) const;
	// Gets image dimensions
	int get_w() const;
	int get_h() const;
private:
	int w, h;
	SDL_Texture* _mp_texture;
};

// Text display class.
class TextBox {
public:
	TextBox(const std::string& fontname="", unsigned int size=12);
	~TextBox();

	// Member functions.
	// Helper loader functions.
	bool load_text(const std::string& msg, SDL_Renderer* p_renderer);
	void configure(const std::string& fontname, unsigned int size=12);
	void show_text_at(int x, int y,
		double rotation, SDL_Renderer* p_renderer) const;

	// Deallocator
	void clear_data();

	int get_w() const;
	int get_h() const;
private:
	// Data members.
	int w, h;
	unsigned int _m_fontsize;
	std::string _m_fontname;
	SDL_Texture* _mp_render_box;
	TTF_Font* _mp_font;

	// Private functions.
	bool create_texture(const std::string& msg,
		SDL_Renderer* p_renderer);
};

// Music player class.
class MusicPlayer {
public:
	// Destructor.
	MusicPlayer(const std::string& audiopath="");
	~MusicPlayer();
	bool load_audio(const std::string& audiopath);
	void unload();
 	void play(unsigned int fade=0, int loops=0) const;
 	void stop(unsigned int fade=0) const;

	// Pause and resume functions for when
	// you disable music.
	void pause() const;
	void resume() const;
private:
	Mix_Music* _mp_music;
};

// SFX class
class SFX {
public:
	SFX(const std::string& audiopath="", unsigned int channel=0);
	~SFX();

	// Loading and unloading functions.
	bool load_audio(const std::string& audiopath="");
	void unload();

	// Affects instance's channel.
	void play(unsigned int fade=0, int loops=0) const;
	void stop(unsigned int fade=0) const;
	void pause() const;
	void resume() const;

	// Change the instance's channel
	void change_channel(unsigned int channel);

	// Affects all SDL channels.
	static void stop_all(unsigned int fade);
	static void pause_all();
	static void resume_all();
	static void global_volume(unsigned int volume);
	static int get_max_channels();
private:
	static int _max_channels;
	unsigned int _m_channel;
	Mix_Chunk* _mp_sound;
};

class Playlist {
public:
	// Destructor and Constructor
	Playlist();
	~Playlist();

	// Member functions.
	void remove_song(std::string song);
	void add_song(std::string song);
	void next_song(bool force=false);
	void play_at(unsigned int index);
	void set_list(std::vector<std::string>& list);

	// Deallocates memory for songlist.
	void empty_songs();

	void stop_playback() const;
	void pause_playback() const;
	void resume_playback() const;
private:
	int _m_cur_song;
	std::vector<std::string>* _mp_songlist;
	MusicPlayer* _mp_player;
};

// Slider class
constexpr float range_map(float in,
    float in_start, float in_end,
    float out_start, float out_end);

constexpr float clamp(float min, float max, float val);

class Slider {
public:
    // Destructor.
    Slider(const SDL_Rect& bar_rect, const SDL_Rect& button_rect,
        float min_out, float max_out, TextBox* const p_textbox,
        const SDL_Color& bar_color=SDL_Color{0xFF, 0xFF, 0xFF, 0x90},
        const SDL_Color& button_color={0xBF, 0xBF, 0xBF, 0xFF}, float percentage=50.0f);
    ~Slider();

    enum class components { BAR, BUTTON };
    enum class button_states { HELD, INACTIVE };
    enum class mouse_inputs { CLICKED, RELEASE, NOINPUT};

    // Setters and getters
    // Color
    void set_color(const SDL_Color& color, components component);
    void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a, components component);
    SDL_Color get_color(components component) const;

    // Current value
    void set_current_value(float percentage);
    float get_current_value() const;

    // State
    void change_state(button_states new_state);
    button_states get_state() const;

    // Processes mouse clicks.
    void process_ui(int mouse_x, int mouse_y,
        mouse_inputs mstate=mouse_inputs::NOINPUT);

    // Renders parts.
    void render_parts(SDL_Renderer* const p_renderer,
        const std::string& value_name) const;

    float min_output, max_output;
private:
    void _set_button_pos(int mouse_x);
private:
    int _lpos, _rpos; // Positions of the left and right ends of the slider bar (x)
    button_states _state;
    SDL_Color _bar_color, _button_color;
    SDL_Rect _bar_rect, _button_rect;
    float _current_value;
    TextBox* _p_textbox;
};

#endif
