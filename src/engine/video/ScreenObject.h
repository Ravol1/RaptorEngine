#ifndef RAPTOR_SCREENOBJECT_H
#define RAPTOR_SCREENOBJECT_H
#include <memory>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "sprite/Sprite.h"

namespace raptor::engine::video {
	class ScreenObject {
	public:
		using Sprite = sprite::Sprite;

		ScreenObject(std::unique_ptr<Sprite> sprite);


		/** @brief Return the current screen coordinates (top-left) of the object. */
		[[nodiscard]]
		auto get_pos() const -> SDL_FPoint {return pos_;}

		/** @brief Sets the screen coordinates (top-left) of the object. */
		auto set_pos(SDL_FPoint pos) -> void {this->pos_ = pos;}

		/** @brief Sets the dirty flag to true*/
		auto set_dirty() -> void {is_dirty_ = true;}


		/** @brief Sets the visibility flag and updated the internal sprite accordingly */
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


		virtual ~ScreenObject() = default;

	private:
		bool is_dirty_ = false;
		bool is_visible_ = true;

		SDL_FPoint pos_{0, 0};

		std::unique_ptr<Sprite> sprite_;
	};
} // raptor::engine::video

#endif //RAPTOR_SCREENOBJECT_H
