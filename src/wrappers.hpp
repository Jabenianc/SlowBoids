// Wrappers.h
// Contains some SDL wrapper classes.
#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

// Uses vector
#include <vector>

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
	// Destructor.
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

#endif
