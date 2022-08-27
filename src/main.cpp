// Boids_main.cpp
// Main simulation program.
// This simulation is not optimized for speed.

// Notes.
/*
	Probably could have put more stuff into
	initialize.h
	Couple danger areas.
*/

// Uses classes.h and initialize.h
#include "classes.hpp"
#include "initialize.hpp"
#include "wrappers.hpp"

// Helper function.
void handle_events(SDL_Event* p_ev, ObstacleGroup* p_obs, Flightspace* p_flock);

int main(int argc, char* args[]) {
	// Try initializing everything.
	if (try_init()) {
		// Main loop flag.
		bool program_active = true;
		SDL_Event event;
		Flightspace my_flock;
		ObstacleGroup my_obs_group;

		// Populate the flock.
		my_flock.random_populate(NUM_BOIDS, SCR_W, SCR_H, 3.25, 0.3);
		my_flock.set_obstacles(my_obs_group.get_obstacles());

		while (program_active) {
            // Try playing next song without forcing.
			g_playlist->next_song(false);

			// Handle queued events.
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					program_active = false;
				}
				handle_events(&event, &my_obs_group, &my_flock);
			}

			// Clear screen.
			SDL_SetRenderDrawColor(g_renderer, 0x1F, 0x1F, 0x1F, 0xFF);
			SDL_RenderClear(g_renderer);

			// Update all the movement vectors prior to moving.
			my_flock.update();

			// Render boids.
			for (int i = 0; i < my_flock.get_size(); i++) {
				Boid* p_boid = my_flock.get_boid(i);
				p_boid->move();
				p_boid->bind_position(-20, SCR_W+20, -20, SCR_H+20);
				Vector2 position = p_boid->get_pos();
				g_tex_boid->render_at(position.x, position.y,
					p_boid->get_rotation(), g_renderer, 2);
			}

			// Render obstacles.
			for (int i = 0; i < my_obs_group.get_size(); i++) {
				Vector2* obstacle = my_obs_group.get_obstacles()->at(i);
				g_tex_obstacle->render_at(obstacle->x,
					obstacle->y, 0.0, g_renderer);
			}

			// Draw vignette
			g_tex_vignette->render_at(0, 0, 0.0, g_renderer);

			// Draw text
			g_titlebox->show_text_at(10, 10, 0.0, g_renderer);
			g_textbox->show_text_at(25, 35, 0.0, g_renderer);

			// Update screen.
			SDL_RenderPresent(g_renderer);
		}
	}

	// Close SDL subsystems.
	close();
	return 0;
}

// Ugly asf way to organize code lol
void handle_events(SDL_Event* p_ev, ObstacleGroup* p_obs, Flightspace* p_flock) {
	switch(p_ev->type) {
		case SDL_KEYDOWN:
			switch(p_ev->key.keysym.sym) {
				case SDLK_r:
					p_obs->clear_all();
					break;
				case SDLK_m:
					SFX::global_volume(0); // Mute
					g_playlist->pause_playback();
					break;
				case SDLK_n:
					SFX::global_volume(64); // Unmute
					g_playlist->resume_playback();
					break;
				default: break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (p_ev->button.button) {
				case SDL_BUTTON_LEFT:
					p_obs->add_obstacle(p_ev->button.x, p_ev->button.y);
					g_click_sfx->play();
					break;
				case SDL_BUTTON_RIGHT:
					p_obs->remove_obstacles(p_ev->button.x, p_ev->button.y);
					g_click_sfx->play();
					break;
				default: break;
			}
			break;
		default: break;
	}
}
