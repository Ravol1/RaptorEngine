#ifndef RAPTOR_SCREEN_OBJ_REGISTRY_H
#define RAPTOR_SCREEN_OBJ_REGISTRY_H
#include <memory>
#include <vector>

#include "../ScreenObject.h"
#include "engine/video/texture/TextureFactory.h"

namespace raptor::engine::video {
	class ScreenObjRegistry {
	public:
		/**
		 * @brief Creates a new screen object.
		 *
		 * @return A pointer to the newly created object.
		 */
		auto create_object(
			uint64_t id,
			const std::string& texture_path,
			int width, int height,
			TextureFactory& texture_factory,
			SDL_Renderer* renderer
		) -> ScreenObject*;

		/** @brief updated all the stored screen objects. */
		void update(double dt);

		/** @brief sorts all the stored screen objects, then renders them. */
		void render(SDL_Renderer* renderer);


		/**
		 * @brief Retrieves a screen object by its ID.
		 *
		 * @param id The ID of the screen object to retrieve.
		 * @return The screen object if found, or nullptr if it does not exist.
		 */
		auto get_object(uint64_t id) const -> ScreenObject*;



	private:
		std::vector<std::unique_ptr<ScreenObject>> objects_;
		std::unordered_map<uint64_t, ScreenObject*> object_table_;

		void sort();
	};
} // raptor::engine::video

#endif //RAPTOR_SCREEN_OBJ_REGISTRY_H
