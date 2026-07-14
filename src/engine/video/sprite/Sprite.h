#ifndef RAPTOR_SPRITE_H
#define RAPTOR_SPRITE_H
#include <optional>
#include <SDL3/SDL_render.h>


namespace raptor::engine::video::sprite {
	class Sprite {
	public:
		constexpr static float autosize = -1.0f;

		Sprite(SDL_Texture* texture = nullptr, const std::optional<SDL_FRect>& src = std::nullopt);


		auto set_visible(bool visible) -> void {is_visible_ = visible;};


		/**
		 * @brief Sets the texture that will be rendered by the sprite.
		 *
		 * @param texture Pointer to the SDL texture object. Can be nullptr.
		 */
		auto set_texture(SDL_Texture* texture) -> void;


		/**
		 * @brief Sets the source rectangle (the portion of the texture to render).
		 *
		 * @param src Optional SDL_FRect defining the source boundaries.
		 * If std::nullopt, the entire texture will be rendered.
		 */
		auto set_source(const std::optional<SDL_FRect>& src) -> void;


		/**
		 * @brief Recalculates the destination rectangle for rendering.
		 *
		 * @param pos    The screen coordinates (top-left) where the sprite will be rendered.
		 * @param width  The target width. Pass @p Sprite::autosize To automatically evaluate
		 * the width based on the source rectangle or texture.
		 * @param height The target height. Pass @p Sprite::autosize to automatically evaluate
		 * the height based on the source rectangle or texture.
		 *
		 * @note The evaluated size defaults to the size of the source rectangle (@p src_),
		 * or the full texture dimensions if no source rectangle is set.
		 */
		auto virtual update(SDL_FPoint pos, float width, float height) -> void;


		/**
		 * @brief Renders the sprite to the screen if the sprite is set to visible
		 * and has a valid texture.
		 *
		 * @param renderer Pointer to the active SDL rendering state.
		 *
		 * @note This function safely extracts the raw pointer from the std::optional source
		 * rectangle on every frame. The introduced overhead is negligible.
		 */
		auto virtual render(SDL_Renderer* renderer) -> void;


		virtual ~Sprite() = default;

	private:
		SDL_Texture* texture_ = nullptr;
		std::optional<SDL_FRect> src_;


		SDL_FRect dst_rect_{0, 0, 0, 0};

		bool is_visible_ = true;
	};
} // raptor::engine::video::sprite

#endif //RAPTOR_SPRITE_H
