#include "ScreenObjRegistry.h"

#include "engine/video/texture/TextureFactory.h"
#include "../ScreenObject.h"
#include "engine/video/sprite/factory/factory.h"


namespace raptor::engine::video {
	auto ScreenObjRegistry::create_object(
		uint64_t id,
		const std::string& texture_path,
		int width, int height,
		TextureFactory& texture_factory,
		SDL_Renderer* renderer
	) -> ScreenObject* {
		if (object_table_.contains(id)) return nullptr;


		auto texture = texture_factory.from_file(texture_path, renderer);
		auto sprite = sprite_factory::create_static(texture);
		auto new_object = std::make_unique<ScreenObject>(std::move(sprite), id, 0, width, height);


		objects_.push_back(std::move(new_object));
		object_table_[id] = objects_.back().get();


		return objects_.back().get();
	}


	auto ScreenObjRegistry::get_object(uint64_t id) const -> ScreenObject* {
		if (!object_table_.contains(id)) return nullptr;

		return object_table_.at(id);
	}

	void ScreenObjRegistry::update(double dt) {
		for (auto const& obj : objects_) {
			obj->update(dt);
		}
	}

	void ScreenObjRegistry::render(SDL_Renderer* renderer) {
		sort();
		for (auto const& obj : objects_) {
			obj->render(renderer);
		}
	}



	void ScreenObjRegistry::sort() {
		std::ranges::stable_sort(objects_, {}, &ScreenObject::get_z);
	}
} // raptor::engine::video