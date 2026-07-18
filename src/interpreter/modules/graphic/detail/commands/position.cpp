#include "commands.h"

#include "helper/type_conversions.h"
#include "interpreter/modules/graphic/detail/layer/helpers.h"

namespace raptor::interpreter::graphic::detail {
	namespace {
		using LayerPage = Layer::LayerPage;

		auto apply_position(
			Layer* layer,
			LayerPage page,
			const std::string& left,
			const std::string& top
		) -> void {

			if (auto x = try_parse_numeric<int>(left)) layer->set_x(static_cast<float>(*x), page);
			if (auto y = try_parse_numeric<int>(top)) layer->set_y(static_cast<float>(*y), page);
		}

		auto apply_size(
			MessageLayer* layer,
			LayerPage page,
			const std::string& width,
			const std::string& height
		) -> void {

			if (auto iwidth = try_parse_numeric<unsigned int>(width))
				layer->set_msg_width(static_cast<float>(*iwidth), page);

			if (auto iheight = try_parse_numeric<unsigned int>(height))
				layer->set_msg_height(static_cast<float>(*iheight), page);
		}

		auto apply_color(
			MessageLayer* layer,
			LayerPage page,
			const std::string& color_str
		) -> void {

			ColorRGB color{};
			if (color_str.starts_with("0x")) color = ColorRGB::from_string(
				std::string_view{
					color_str
				}.substr(2)
			);

			layer->set_msg_color(color, page);
		}

		auto apply_border_color(
			MessageLayer* layer,
			LayerPage page,
			const std::string& color_str
		) -> void {

			ColorRGB color{};
			if (color_str.starts_with("0x")) color = ColorRGB::from_string(
				std::string_view{
					color_str
				}.substr(2)
			);

			layer->set_msg_border_color(color, page);
		}

		auto apply_opacity(
			MessageLayer* layer,
			Layer::LayerPage page,
			const std::string& opacity
		) -> void {

			if (auto alpha = try_parse_numeric<uint8_t>(opacity))
				layer->set_msg_alpha(*alpha, page);
		}

			auto apply_border_size(
				MessageLayer* layer,
				Layer::LayerPage page,
				const std::string& opacity
			) -> void {

			if (auto thickness = try_parse_numeric<int>(opacity))
				layer->set_msg_border_thickness(*thickness, page);
		}

		auto apply_margin(MessageLayer* layer, LayerPage page,
			const std::string& marginl,
			const std::string& margint,
			const std::string& marginr,
			const std::string& marginb
		) -> void {

			if (auto margin_left = try_parse_numeric<unsigned int>(marginl))
				layer->set_msg_margin_left(*margin_left, page);

			if (auto margin_top = try_parse_numeric<unsigned int>(margint))
				layer->set_msg_margin_top(*margin_top, page);

			if (auto margin_right = try_parse_numeric<unsigned int>(marginr))
				layer->set_msg_margin_right(*margin_right, page);

			if (auto margin_bottom = try_parse_numeric<unsigned int>(marginb))
				layer->set_msg_margin_bottom(*margin_bottom, page);
		}


		auto apply_margin(MessageLayer* layer, LayerPage page, const std::string& margin_str) -> void {
			if (auto margin = try_parse_numeric<unsigned int>(margin_str)) {
				if (margin > 0) {
					layer->set_msg_margin_left(*margin, page);
					layer->set_msg_margin_top(*margin, page);
					layer->set_msg_margin_right(*margin, page);
					layer->set_msg_margin_bottom(*margin, page);
				}
			}
		}

		auto apply_vertical(
			MessageLayer* layer,
			LayerPage page,
			const std::string& vertical_str
		) -> void {

			if (auto vertical = try_parse_bool(vertical_str))
				layer->set_msg_vertical(*vertical, page);
		}

		auto apply_visible(
			Layer* layerObj,
			Layer::LayerPage page,
			const std::string& visible_str
		) -> void {

			if (auto visible = try_parse_bool(visible_str))
				layerObj->set_visible(*visible, page);
		}


		auto apply_generic_settings(
			const Tag& tag,
			Layer* layerObj,
			Layer::LayerPage page
		) -> void {

			apply_visible(layerObj, page, tag.get_attribute("visible"));
			apply_position(layerObj, page, tag.get_attribute("left"), tag.get_attribute("top"));
		}


		auto apply_message_options(
			const Tag& tag,
			MessageLayer* layerObj,
			Layer::LayerPage page
		) -> void {

			apply_size(layerObj, page, tag.get_attribute("width"), tag.get_attribute("height"));

			if (auto frame = tag.get_attribute("frame"); !frame.empty())
				layerObj->set_msg_frame(frame, page);

			apply_color(layerObj, page, tag.get_attribute("color"));
			apply_border_color(layerObj, page, tag.get_attribute("border_color"));

			apply_opacity(layerObj, page, tag.get_attribute("opacity"));

			apply_border_size(layerObj, page, tag.get_attribute("border_size"));

			if (tag.has_attribute("margin")) {
				apply_margin(layerObj, page, tag.get_attribute("margin"));
			}
			else {
				apply_margin(layerObj, page,
					tag.get_attribute("marginl"),
					tag.get_attribute("margint"),
					tag.get_attribute("marginr"),
					tag.get_attribute("marginb")
				);
			}

			apply_vertical(layerObj, page, tag.get_attribute("vertical"));
		}
	}



	auto position(const Tag& tag, LayerRegistry& layer_registry, const CurrentLayer& current_layer) -> void {
		Layer* layer_common;

		// Default to "message0"
		if (tag.get_attribute("layer").empty()) layer_common = layer_registry.get_or_create("message0");
		else layer_common = layer_registry.get_or_create(tag.get_attribute("layer"));

		if (layer_common == nullptr) return;	// If layer name is invalid exit


		// Parse the page or default to LayerPage::Fore
		auto page = parse_page(tag.get_attribute("page"));
		if (page == LayerPage::Auto) page = LayerPage::Fore;


		apply_generic_settings(tag, layer_common, page);

		// Apply message-layer-specific settings only if the retrieved layer is a message layer
		if (auto layer = dynamic_cast<MessageLayer*>(layer_common))
			apply_message_options(tag, layer, page);
	}
} // raptor::interpreter::graphic::detail