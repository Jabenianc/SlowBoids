// Tiny error.cpp
// Helps with error management.

// Uses tinyerror.h
#include "tinyerror.h"

// Provides an error message.
void error_msg(const std::string issue, error_types error) {
	std::cout << "Error: -> ";
	std::cout << issue << "\nReason: ";
	switch (error) {
		case REGULAR_ERROR:
			std::cout << SDL_GetError() << "\n";
			break;
		case IMAGE_ERROR:
			std::cout << IMG_GetError() << "\n";
			break;
		case TEXT_ERROR:
			std::cout << TTF_GetError() << "\n";
			break;
		case MIXER_ERROR:
			std::cout << Mix_GetError() << "\n";
		default:
			std::cout << "Unknown Error Type!!!\n";
			break;
	}
}
