#include "ScreenObject.h"

#include <SDL3/SDL_render.h>


namespace raptor::engine::video {
	ScreenObject::ScreenObject (
		std::unique_ptr<Sprite> sprite,
		int z,
		int width, int height
	) :
	is_dirty_(true),
	width_(static_cast<float>(width)),
	height_(static_cast<float>(height)),
	z_(z),
	sprite_(std::move(sprite)){}

	auto ScreenObject::render(SDL_Renderer* renderer) -> void {
		if (is_dirty_) {
			sprite_->update(pos_, width_, height_);
			is_dirty_ = false;
		}

		sprite_->render(renderer);
	}
} // raptor::engine::video
