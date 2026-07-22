#ifndef RAPTOR_SCREENOBJECT_H
#define RAPTOR_SCREENOBJECT_H
#include <algorithm>
#include <memory>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "sprite/Sprite.h"

namespace raptor::engine::video {
	class ScreenObject {
	public:
		ScreenObject(
			std::unique_ptr<Sprite> sprite,
			uint64_t id,
			int z = 0,
			int width = Sprite::autosize, int height = Sprite::autosize
			);
		virtual ~ScreenObject() = default;


		/** @brief Returns the current screen coordinates (top-left) of the object. */
		[[nodiscard]]
		auto get_pos() const -> SDL_FPoint {return pos_;}

		/** @brief Sets the screen coordinates (top-left) of the object. */
		auto set_pos(SDL_FPoint pos) -> void {this->pos_ = pos; is_dirty_ = true;}

		/** @brief Sets the dirty flag to true. */
		auto set_dirty() -> void {is_dirty_ = true;}


		/** @brief Sets the visibility flag and updated the internal sprite accordingly. */
		auto set_visible(bool visible) -> void {
			is_visible_ = visible;
			sprite_->set_visible(visible);
		}

		auto virtual update(double dt) -> void {};

		/**
		 * @brief Renders the object at the current location.
		 *
		 * Triggers update of the internal sprite if the dirty flag is set.
		 *
		 * @param renderer Pointer to the active SDL rendering state.
		 */
		auto virtual render(SDL_Renderer* renderer) -> void;


		void set_z(int z) {z_ = z;}
		[[nodiscard]]
		auto get_z() const -> int {return z_;}

		[[nodiscard]]
		auto id() const -> uint64_t {return id_;}


		void set_alpha(float alpha) {
			alpha_ = std::clamp(alpha, 0.0f, 1.0f);
			is_dirty_ = true;
		}

		[[nodiscard]]
		auto get_alpha() const -> float {return alpha_;}


	private:
		uint64_t id_;

		bool is_dirty_ = false;
		bool is_visible_ = true;

		const float width_, height_;

		int z_ = 0;

		SDL_FPoint pos_{0, 0};

		float alpha_ = 1;

		std::unique_ptr<Sprite> sprite_;
	};
} // raptor::engine::video

#endif //RAPTOR_SCREENOBJECT_H
