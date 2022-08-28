// tiny_error.h
// Helps with error handling.
#ifndef _TINYERROR_H_
#define _TINYERROR_H_

// Uses standard output, string and SDL headers.
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// Error types enumerator.
enum class error_types {
	REGULAR_ERROR,
	IMAGE_ERROR,
	TEXT_ERROR,
	MIXER_ERROR
};

void error_msg(const std::string& issue,
    error_types error=error_types::REGULAR_ERROR);

#endif
