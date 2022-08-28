// Tinyerror.cpp
#include "tinyerror.hpp"

void error_msg(const std::string& issue, error_types error) {
    std::cout << "Error: -> ";
    std::cout << issue << "\nReason: ";
    switch (error) {
        case error_types::REGULAR_ERROR:
            std::cout << SDL_GetError() << "\n";
            break;
        case error_types::IMAGE_ERROR:
            std::cout << IMG_GetError() << "\n";
            break;
        case error_types::TEXT_ERROR:
            std::cout << TTF_GetError() << "\n";
            break;
        case error_types::MIXER_ERROR:
            std::cout << Mix_GetError() << "\n";
        default:
            std::cout << "Unknown Error Type!!!\n";
            break;
    }
}
