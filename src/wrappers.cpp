// Wrappers.cpp
// Definitions for important SDL wrapper classes.

// Uses wrappers.h
#include "wrappers.hpp"
#include <cmath>

// Member function definitions for SDL Wrapper.
// Constructor
TextureWrap::TextureWrap() {
	_mp_texture = NULL;
	w = 0;
	h = 0;
}

// Destructor
TextureWrap::~TextureWrap() {
	// Free texture.
	free_texture();
}

// Loads and image from a file and converts it to a texture.
bool TextureWrap::load_from_file(const std::string& path,
	SDL_Renderer* p_renderer)
{
	// Get rid of preexisting texture
	free_texture();

	SDL_Surface* p_surf = IMG_Load(path.c_str());
	if (p_surf == NULL) {
		error_msg("Unable to load image!", error_types::IMAGE_ERROR);
	}
	else {
		// Create texture from surface pixels.
		_mp_texture = SDL_CreateTextureFromSurface(p_renderer, p_surf);
		if (_mp_texture == NULL) {
			error_msg("Unable to create texture from image!!!",
				error_types::REGULAR_ERROR);
		}
		else {
			w = p_surf->w;
			h = p_surf->h;
		}
		// Remove pointer to surface.
		SDL_FreeSurface(p_surf);
		p_surf = NULL;
	}
	// Return success state.
	return _mp_texture != NULL;
}

// Frees texture.
void TextureWrap::free_texture() {
	// Free texture if it exists.
	if (_mp_texture != NULL) {
		SDL_DestroyTexture(_mp_texture);
		_mp_texture = NULL;
		w = 0;
		h = 0;
	}
}

// Renders a texture at said location.
void TextureWrap::render_at(int x, int y, double rotation,
	SDL_Renderer* p_renderer, int shrink) const
{
	// Set rendering space and render to screen.
	SDL_Rect render_quad = {x, y, w/shrink, h/shrink};
	SDL_RenderCopyEx(p_renderer, _mp_texture, NULL, &render_quad,
		rotation, NULL, SDL_FLIP_NONE);
}

int TextureWrap::get_w() const {
	return w;
}

int TextureWrap::get_h() const {
	return h;
}

// Textbox member function definition area.
TextBox::TextBox(const std::string& fontname, unsigned int size):
	_m_fontsize(size),
    _m_fontname(fontname)
{
	// Nullify.
	_mp_font = NULL;
	_mp_render_box = NULL;
}

TextBox::~TextBox() {
	clear_data();
}

void TextBox::configure(const std::string& fontname, unsigned int size) {
	_m_fontname = fontname;
	_m_fontsize = size;
}

bool TextBox::load_text(const std::string& msg, SDL_Renderer* p_renderer) {
	bool success = true;
	clear_data(); // Remove old data.
	_mp_font = TTF_OpenFont(_m_fontname.c_str(), _m_fontsize);
	if (!_mp_font) {
		error_msg("Failed to load font!", error_types::TEXT_ERROR);
		success = false;
	}
	else {
		if (!create_texture(msg, p_renderer)) {
			success = false;
		}
	}
	return success;
}

bool TextBox::create_texture(const std::string& msg, SDL_Renderer* p_renderer)
{
	bool success = true;
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0x90};
	SDL_Surface* p_text_surf = TTF_RenderText_Solid(
		_mp_font, msg.c_str(), color);

	if (!p_text_surf) {
		error_msg("Failed to render text!", error_types::TEXT_ERROR);
		success = false;
	}
	else {
		// Load the surface into a texture.
		_mp_render_box = SDL_CreateTextureFromSurface(
			p_renderer, p_text_surf);

		if (_mp_render_box == NULL) {
			error_msg("Unable to create texture from image!!!",
				error_types::REGULAR_ERROR);
			success = false;
		}
		else {
			w = p_text_surf->w;
			h = p_text_surf->h;
		}
	}
	// Deallocate temporary surface memory.
	SDL_FreeSurface(p_text_surf);
	p_text_surf = NULL;
	return success;
}

void TextBox::show_text_at(int x, int y, double rotation,
	SDL_Renderer* p_renderer) const
{
	SDL_Rect render_quad = {x, y, w, h};
	SDL_RenderCopyEx(p_renderer, _mp_render_box, NULL, &render_quad,
		rotation, NULL, SDL_FLIP_NONE);
}

void TextBox::clear_data() {
	if (_mp_font != NULL) {
		TTF_CloseFont(_mp_font);
		_mp_font = NULL;
	}
	if (_mp_render_box != NULL) {
		SDL_DestroyTexture(_mp_render_box);
		_mp_render_box = NULL;
		w = 0;
		h = 0;
	}
}

int TextBox::get_w() const {
    return w;
}

int TextBox::get_h() const {
    return h;
}

// Music Player Member function defintion area.
MusicPlayer::MusicPlayer(const std::string& audiopath){
	_mp_music = NULL;
	if (!audiopath.empty()) {
		load_audio(audiopath);
	}
}

MusicPlayer::~MusicPlayer() {
	unload();
}

bool MusicPlayer::load_audio(const std::string& audiopath) {
	bool success = true;
	unload(); // Unload old audio.
	_mp_music = Mix_LoadMUS(audiopath.c_str());
	if (_mp_music == NULL) {
		error_msg("Failed to load music!", error_types::MIXER_ERROR);
		success = false;
	}
	return success;
}

void MusicPlayer::unload() {
	if (_mp_music != NULL) {
		if (Mix_PlayingMusic() == 1) {
			// Stop music first if it's playing.
			Mix_HaltMusic();
		}
		Mix_FreeMusic(_mp_music);
		_mp_music = NULL;
	}
}

void MusicPlayer::play(unsigned int fade, int loops) const {
	if (_mp_music != NULL) {
		if (Mix_FadeInMusic(_mp_music, loops, fade) == -1) {
			error_msg("Music couldn't play!", error_types::MIXER_ERROR);
		}
	}
	else {
		std::cout << "Music is not loaded!!!\n";
	}
}

void MusicPlayer::stop(unsigned int fade) const {
	// Only stop music if it was on.
	if (Mix_PlayingMusic() == 1) {
		Mix_FadeOutMusic(fade);
	}
}

void MusicPlayer::pause() const {
	if (Mix_PlayingMusic() == 1) {
		Mix_PauseMusic();
	}
}

void MusicPlayer::resume() const {
	if (Mix_PausedMusic() == 1) {
		Mix_ResumeMusic();
	}
}

// SFX player Member function definition area.
// Set max channels to 64.
int SFX::_max_channels = 64;

SFX::SFX(const std::string& audiopath, unsigned int channel) {
	_mp_sound = NULL;
	if (!audiopath.empty()) {
		load_audio(audiopath);
	}
	_m_channel = (channel > _max_channels)? _max_channels : channel;
}

SFX::~SFX() {
	unload();
}

bool SFX::load_audio(const std::string& audiopath) {
	bool success = true;
	unload(); // Unload old audio.
	_mp_sound = Mix_LoadWAV(audiopath.c_str());
	if (_mp_sound == NULL) {
		error_msg("Failed to load audio!", error_types::MIXER_ERROR);
		success = false;
	}
	return success;
}

void SFX::unload() {
	if (_mp_sound != NULL) {
		if (Mix_Playing(_m_channel) == 1) {
			// Stop music first if it's playing.
			Mix_HaltChannel(_m_channel);
		}
		Mix_FreeChunk(_mp_sound);
		_mp_sound = NULL;
	}
}

void SFX::play(unsigned int fade, int loops) const {
	if (_mp_sound != NULL) {
		if (Mix_FadeInChannel(_m_channel,
			_mp_sound, loops, fade) == -1)
		{
			error_msg("Sound couldn't play!", error_types::MIXER_ERROR);
		}
	}
	else {
		std::cout << "Sound is not loaded!!!\n";
	}
}


void SFX::stop(unsigned int fade) const {
	if (Mix_Playing(_m_channel) == 1) {
		Mix_FadeOutChannel(_m_channel, fade);
	}
}

void SFX::pause() const {
	if (Mix_Playing(_m_channel) == 1) {
		Mix_Pause(_m_channel);
	}
}

void SFX::resume() const {
	if (Mix_Paused(_m_channel) == 1) {
		Mix_Resume(_m_channel);
	}
}

void SFX::change_channel(unsigned int channel) {
	// We don't need to check for < 0 because channel is unsigned int
	_m_channel = (channel > _max_channels)? _max_channels : channel;
}

// Static member functions.
void SFX::stop_all(unsigned int fade) {
	// We aren't going to check for
	// every allocated channel if it is playing,
	// Mix_FadeOutChannel will only stop those that are
	// playing.
	Mix_FadeOutChannel(-1, fade);
}

void SFX::pause_all() {
	Mix_Pause(-1);
}

void SFX::resume_all() {
	Mix_Resume(-1);
}

void SFX::global_volume(unsigned int volume) {
	// SDL will take care of volumes over the limit.
	Mix_Volume(-1, volume);
}

int SFX::get_max_channels() {
	return _max_channels;
}

// Playlist Class member function definition area.
Playlist::Playlist() {
	_m_cur_song = 0;
	// Dynamically allocate memory for a new player.
	_mp_songlist = new std::vector<std::string>;
	_mp_player = new MusicPlayer();
}

Playlist::~Playlist() {
	// Free memory.
	if (_mp_player != nullptr) {
		delete _mp_player;
		_mp_player = nullptr;
	}
	// Deallocate memory for songlist.
	empty_songs();
}

void Playlist::remove_song(std::string song) {
	auto it = find(_mp_songlist->begin(), _mp_songlist->end(), song);
	if (it != _mp_songlist->end()) { // Found
		int index = it - _mp_songlist->begin();
		_mp_songlist->erase(_mp_songlist->begin() + index);
	}
}

void Playlist::add_song(std::string song) {
	_mp_songlist->push_back(song);
}

void Playlist::next_song(bool force) {
	// Is true if we aren't playing music and false if we are playing music.
	bool playnext = !(Mix_PlayingMusic() == 1);
	if (!playnext) {
		// We will only apply the force if we can't play the next song.
		// (A song is currently playing)
		// If we could play the next song and force was false then
		// we would just negate our ability to play the next song.
		playnext = force;
	}

	if (playnext) {
		// Reached end of list, or index is greater than list size.
		// I use a larger or equal to in case someone
		// just randomly deletes a bunch of songs.
		if (_m_cur_song >= _mp_songlist->size()-1) {
			_m_cur_song = 0;
		}
		else {
			++_m_cur_song;
		}
		// Play the song.
		_mp_player->load_audio(_mp_songlist->at(_m_cur_song));
		_mp_player->play(5000, 0);
	}
}

void Playlist::play_at(unsigned int index) {
	// If the index is within bounds.
	if (index < _mp_songlist->size()) {
		_m_cur_song = index;
		// Fade audio and play it with fade.
		_mp_player->stop(5000);
		_mp_player->load_audio(_mp_songlist->at(_m_cur_song));
		_mp_player->play(5000, 0);
	}
}

// Takes a reference to a songlist
// and reallocates memory for a new identical list.
void Playlist::set_list(std::vector<std::string>& list) {
	empty_songs(); // Deallocate memory.
	_mp_songlist = new std::vector<std::string>(list.begin(), list.end());
}

void Playlist::empty_songs() {
	if (_mp_songlist != nullptr) {
		delete _mp_songlist;
		_mp_songlist = nullptr;
	}
}

void Playlist::stop_playback() const {
	_mp_player->stop(5000);
}

void Playlist::pause_playback() const {
	_mp_player->pause();
}

void Playlist::resume_playback() const {
	_mp_player->resume();
}

// Slider definition area
constexpr float range_map(
    float in, float in_start, float in_end,
    float out_start, float out_end)
{
    float out = in - in_start;
    out /= (in_end - in_start);
    out *= (out_end - out_start);
    return out + out_start;
}

constexpr float clamp(float min, float max, float val) {
    return (val > max)? max : (val < min)? min : val;
}

Slider::Slider(const SDL_Rect& bar_rect, const SDL_Rect& button_rect,
    float min_out, float max_out, TextBox* const p_textbox,
    const SDL_Color& bar_color, const SDL_Color& button_color, float percentage):
    // Min and max outputs
    min_output(min_out),
    max_output(max_out),

    // Left and right positions of bar
    _lpos(bar_rect.x),
    _rpos(bar_rect.x + bar_rect.w),

    // Button state
    _state(button_states::INACTIVE),

    // Color initialization
    _bar_color(bar_color),
    _button_color(button_color),

    // Rect initialization
    _bar_rect(bar_rect),
    _button_rect(button_rect)
{
    set_current_value(percentage);
    _p_textbox = p_textbox;

    // Centers the button's y position.
    float mid_bar = _bar_rect.y + (_bar_rect.h / 2.0f);
    _button_rect.y = static_cast<int>(mid_bar - (_button_rect.h / 2.0f));
}

Slider::~Slider() {
    if (_p_textbox != nullptr) {
        delete _p_textbox;
        _p_textbox = nullptr;
    }
}

void Slider::set_color(const SDL_Color& color, components component) {
    switch(component) {
        case components::BAR:
            _bar_color = color;
            break;
        case components::BUTTON:
            _button_color = color;
            break;
        default:
            break;
    }
}

void Slider::set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a, components component) {
    switch(component) {
        case components::BAR:
            _bar_color.r = r;
            _bar_color.g = g;
            _bar_color.b = b;
            _bar_color.a = a;
            break;
        case components::BUTTON:
            _button_color.r = r;
            _button_color.g = g;
            _button_color.b = b;
            _button_color.a = a;
            break;
        default:
            break;
    }
}

SDL_Color Slider::get_color(components component) const {
    switch(component) {
        case components::BAR:
            return _bar_color;
            break;
        case components::BUTTON:
            return _button_color;
            break;
        default:
            break;
    }
}

void Slider::set_current_value(float percentage) {
    percentage = clamp(0, 100.0f, percentage);

    // Map the percentage to how much it corresponds
    // to the what the physical value would be.
    _current_value = range_map(percentage, 0, 100.0f, min_output, max_output);
    // Range map the value from the output range to the physical slider range
    _button_rect.x = range_map(_current_value, min_output, max_output, _lpos, _rpos)
        - (_button_rect.w / 2);
}

float Slider::get_current_value() const {
    return _current_value;
}

// Allows the user to click anywhere on the bar and the button will move
void Slider::change_state(button_states new_state) {
    _state = new_state;
}

Slider::button_states Slider::get_state() const {
    return _state;
}

void Slider::process_ui(int mouse_x, int mouse_y, mouse_inputs mstate) {
    switch (_state) {
        case button_states::HELD:
            (mstate == mouse_inputs::RELEASE)?
                change_state(button_states::INACTIVE) : _set_button_pos(mouse_x);
            break;
        case button_states::INACTIVE:
            if (mstate == mouse_inputs::CLICKED) {
                float half = _button_rect.h / 2.0f;
                float ydiff = mouse_y - (_button_rect.y + half);
                bool x_inrange = ((mouse_x >= _lpos) && (mouse_x <= _rpos));

                // Add 0.5f to make it more lenient for thin bars.
                if ((std::abs(ydiff) <= half + 0.5f) && (x_inrange)) {
                    _set_button_pos(mouse_x);
                    change_state(button_states::HELD); // Change the state to a held one.
                }
            }
            break;
        default:
            break;
    }
}

void Slider::render_parts(SDL_Renderer* const p_renderer,
    const std::string& value_name) const
{
    // Slider render
    SDL_SetRenderDrawColor(p_renderer,
        _bar_color.r, _bar_color.g, _bar_color.b, _bar_color.a);
    SDL_RenderFillRect(p_renderer, &_bar_rect);

    // Text render.
    if (_p_textbox != nullptr) {
        float rounded_val = std::ceil(_current_value * 1000.0f) / 1000.0f;
        _p_textbox->load_text(value_name + std::to_string(rounded_val), p_renderer);
        int tb_height = _p_textbox->get_h();
        _p_textbox->show_text_at(_bar_rect.x, _bar_rect.y - tb_height, 0.0, p_renderer);
    }

    // Button render
    SDL_SetRenderDrawColor(p_renderer,
        _button_color.r, _button_color.g, _button_color.b, _button_color.a);
    SDL_RenderFillRect(p_renderer, &_button_rect);
}

// Private member function
void Slider::_set_button_pos(int mouse_x) {
    mouse_x = clamp(_lpos, _rpos, mouse_x);
    _button_rect.x = mouse_x - (_button_rect.w / 2);;
    _current_value = range_map(mouse_x, _lpos, _rpos, min_output, max_output);
}
