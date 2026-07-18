#include "commands.h"

#include "helper/type_conversions.h"
#include "tag/tag.h"
#include "interpreter/modules/graphic/detail/layer/Layer.h"
#include "interpreter/modules/graphic/detail/layer/CurrentLayer.h"
#include "interpreter/modules/graphic/detail/layer/helpers.h"


namespace raptor::interpreter::graphic::detail {
	namespace {
		auto apply_visible(
			Layer* layerObj,
			Layer::LayerPage page,
			const std::string& visible_str
		) -> void {

			if (auto visible = try_parse_bool(visible_str))
				layerObj->set_visible(*visible, page);
		}

		auto apply_position(
			Layer* layerObj,
			Layer::LayerPage page,
			const std::string& left,
			const std::string& top
		) -> void {

			if (auto x = try_parse_numeric<int>(left)) layerObj->set_x(static_cast<float>(*x), page);
			if (auto y = try_parse_numeric<int>(top)) layerObj->set_y(static_cast<float>(*y), page);
		}

		auto apply_opacity(
			Layer* layerObj,
			Layer::LayerPage page,
			const std::string& opacity
		) -> void {

			if (auto alpha = try_parse_numeric<uint8_t>(opacity))
				layerObj->set_alpha(*alpha, page);
		}
	}


	auto layopt(const Tag& tag, LayerRegistry& layer_registry, const CurrentLayer& current_layer) -> void {
		auto [layer, page] = resolve_target(tag, layer_registry, current_layer);

		if (!layer) return;


		apply_visible(layer, page, tag.get_attribute("visible"));
		apply_position(layer, page, tag.get_attribute("left"), tag.get_attribute("top"));
		apply_opacity(layer, page, tag.get_attribute("opacity"));
	}
} // raptor::interpreter::graphic::detail