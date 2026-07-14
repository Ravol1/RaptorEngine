#ifndef RAPTOR_SPRITE_FACTORY_H
#define RAPTOR_SPRITE_FACTORY_H

#include <memory>
#include <vector>

#include "../Sprite.h"

namespace raptor::engine::video::sprite::factory {

	inline auto create_static() -> std::unique_ptr<Sprite> {
		return std::make_unique<Sprite>();
	}

	inline auto create_static(SDL_Texture* texture) -> std::unique_ptr<Sprite> {
		return std::make_unique<Sprite>(texture);
	}
} // raptor::engine::video::sprite

#endif //RAPTOR_SPRITE_FACTORY_H
