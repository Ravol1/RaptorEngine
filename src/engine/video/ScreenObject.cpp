#include "ScreenObject.h"

#include <SDL3/SDL_render.h>


namespace raptor::engine::video {
	using Sprite = sprite::Sprite;


	ScreenObject::ScreenObject(std::unique_ptr<Sprite> sprite) : is_dirty_(true), sprite_(std::move(sprite)) {}


	auto ScreenObject::render(SDL_Renderer* renderer) -> void {
		if (is_dirty_) {
			sprite_->update(pos_, Sprite::autosize, Sprite::autosize);
			is_dirty_ = false;
		}

		sprite_->render(renderer);
	}
} // raptor::engine::video
