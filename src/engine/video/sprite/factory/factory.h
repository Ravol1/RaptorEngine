#ifndef RAPTOR_SPRITE_FACTORY_H
#define RAPTOR_SPRITE_FACTORY_H

#include <memory>

#include "../Sprite.h"

namespace raptor::engine::video::sprite_factory {

	inline auto create_static() -> std::unique_ptr<Sprite> {
		return std::make_unique<Sprite>();
	}

	inline auto create_static(SDL_Texture* texture) -> std::unique_ptr<Sprite> {
		return std::make_unique<Sprite>(texture);
	}
} // raptor::engine::video::sprite::sprite_factory

#endif //RAPTOR_SPRITE_FACTORY_H
