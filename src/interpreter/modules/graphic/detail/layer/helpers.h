#ifndef RAPTOR_GRAPHIC_HELPERS_H
#define RAPTOR_GRAPHIC_HELPERS_H

#include <charconv>

#include "interpreter/modules/graphic/detail/layer/Layer.h"
#include "interpreter/modules/graphic/detail/layer/LayerRegistry.h"
#include "interpreter/modules/graphic/detail/layer/CurrentLayer.h"
#include "tag/tag.h"

namespace raptor::interpreter::graphic::detail {



	
	/**
	 * @brief Converts a string containing page information into the
	 * corresponding LayerPage value.
	 *
	 * @param page	String containing the page name
	 * @return		LayerPage::Fore if page=="fore"
	 *				LayerPage::Back if page=="back"
	 *				LayerPage::Auto otherwise
	 */
	inline auto parse_page(const std::string& page) -> Layer::LayerPage {
		using LayerPage = Layer::LayerPage;

		if (page == "fore") return LayerPage::Fore;
		if (page == "back") return LayerPage::Back;
		return LayerPage::Auto;
	}




	struct ResolvedLayer {
		Layer* layer;
		Layer::LayerPage page;
	};

	/**
	 * @brief Finds the layer and page targeted by a specific setting tag (e.g. layopt, position)
	 *
	 * @param tag				Tag object containing the settings.
	 * @param layer_registry	Reference to the active layer registry.
	 * @param current_layer		Reference to the active layer tracking object.
	 *
	 * @return A ResolvedTarget containing a pointer to the resolved layer and the value of the resolved page.
	 *
	 * @note If the provided type is not in the following list, the behavior is undefines:
	 *		- layopt
	 *		- position
	 */
	inline auto resolve_target(
		const Tag& tag,
		LayerRegistry& layer_registry,
		const CurrentLayer& current_layer
	) -> ResolvedLayer {

		using LayerPage = Layer::LayerPage;


		ResolvedLayer target{};

		auto layer = tag.get_attribute("layer");
		auto page = parse_page(tag.get_attribute("page"));


		// If layer == "message", set the target as the currently active layer.
		if (layer == "message") target.layer = current_layer.layer;
		else target.layer = layer_registry.get_or_create(layer);


		// If layer == "message", the auto-deduced page will be the currently
		// active page of the currently active message layer.
		// Select the fore page otherwise.
		if (page == LayerPage::Auto) {
			if (layer == "message") target.page = current_layer.page;
			else target.page = LayerPage::Fore;
		}

		return target;
	}
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_GRAPHIC_HELPERS_H
