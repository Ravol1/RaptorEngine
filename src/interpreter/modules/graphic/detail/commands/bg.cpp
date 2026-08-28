#include "commands.h"

#include "helper/type_conversions.h"
#include "interpreter/modules/graphic/detail/layer/BaseLayer.h"
#include "interpreter/modules/graphic/detail/transitions/TransitionManager.h"

namespace raptor::interpreter::graphic::detail {
	namespace {
		auto create_transition(const Tag& tag, Layer* target) -> Transition {
			Transition transition{};

			if (auto time = try_parse_numeric<int>(tag.get_attribute("time")))
				transition.time = *time;

			if (auto wait = try_parse_bool(tag.get_attribute("wait")))
				transition.wait = *wait;

			if (auto cross = try_parse_bool(tag.get_attribute("cross")))
				transition.cross = *cross;

			transition.target = target;

			return transition;
		}
	}

	auto bg(
		const Tag& tag,
		LayerRegistry* layer_registry,
		TransitionManager* transition_manager,
		Interpreter* interpreter
	) -> void {

		auto storage = tag.get_attribute("storage");
		if (storage.empty())
			throw std::runtime_error("Missing required argument 'storage'");

		storage = interpreter->bgimage_path() / storage;

		auto target_layer = layer_registry->get_or_create("base");

		if (auto base_layer = dynamic_cast<BaseLayer*>(target_layer)) {
			auto transition = create_transition(tag, base_layer);
			base_layer->change_bg(storage, transition, interpreter, transition_manager);
		}

	}
} // raptor::interpreter::graphic::detail