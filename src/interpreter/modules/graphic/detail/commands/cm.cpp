#include "interpreter/modules/graphic/detail/layer/LayerRegistry.h"
#include "commands.h"

namespace raptor::interpreter::graphic::detail {
	auto cm(LayerRegistry& layer_registry, Interpreter* interpreter) -> void {
		auto msg_layers = layer_registry.get_all_message();

		for (auto& layer : msg_layers) {
			layer->clear_objects(Layer::LayerPage::Fore, interpreter);
		}
	}
} // raptor::interpreter::graphic::detail