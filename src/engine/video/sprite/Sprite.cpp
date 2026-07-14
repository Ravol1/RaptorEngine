#include "Sprite.h"

namespace raptor::engine::video::sprite {
	Sprite::Sprite(SDL_Texture* texture, const std::optional<SDL_FRect>& src) : texture_(texture), src_(src) {}



	auto Sprite::set_texture(SDL_Texture* texture) -> void {
		texture_ = texture;
	}



	auto Sprite::set_source(const std::optional<SDL_FRect>& src) -> void {
		src_ = src;
	}



	auto Sprite::update(SDL_FPoint pos, float width, float height) -> void {
		if (texture_ == nullptr) return;


		float default_w = src_ ? src_->w : texture_->w;
		float default_h = src_ ? src_->h : texture_->h;

		if (width <= 0) width = default_w;
		if (height <= 0) height = default_h;

		dst_rect_ = SDL_FRect{pos.x, pos.y, width, height};
	}



	auto Sprite::render(SDL_Renderer* renderer) -> void {
		if (texture_ == nullptr || !is_visible_) return;


		SDL_RenderTexture(renderer, texture_, src_ ? &*src_ : nullptr, &dst_rect_);
	}
} // raptor::engine::video::sprite
