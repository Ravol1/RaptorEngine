#include "Mouse.h"

#include <SDL3/SDL.h>


namespace raptor::engine::input {


	void Mouse::update() {
		SDL_MouseButtonFlags flags = SDL_GetMouseState(&x, &y);

		pre_left_state = left_state;
		left_state = flags & SDL_BUTTON_LMASK;

		pre_right_state = right_state;
		right_state = flags & SDL_BUTTON_RMASK;

		pre_middle_state = middle_state;
		middle_state = flags & SDL_BUTTON_MMASK;
	}
}// raptor::engine::input