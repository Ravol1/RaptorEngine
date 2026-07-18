#include "Graphic.h"

#include "tag/tag.h"
#include "interpreter/Interpreter.h"
#include "detail/commands/commands.h"
#include "detail/layer/CurrentLayer.h"
#include "detail/layer/FreeLayer.h"
#include "detail/layer/LayerRegistry.h"


namespace raptor::interpreter::graphic {
	using namespace detail;


	class Graphic::Impl {
	public:
		LayerRegistry layer_registry_{};
		CurrentLayer current_{};
		FreeLayer freeLayer_{};
	};

	Graphic::Graphic() : Module(), impl_(std::make_unique<Impl>()) {
		auto message0 = impl_->layer_registry_.get_or_create("message0");


		impl_->current_.layer = dynamic_cast<MessageLayer*>(message0);


		impl_->current_.layer->set_visible(true, Layer::LayerPage::Fore);
	}

	Graphic::~Graphic() = default;


	auto Graphic::register_commands(Interpreter* interpreter) -> void {
		interpreter->register_command("layopt", [this](const Tag& tag, Interpreter*) -> void {
			layopt(tag, impl_->layer_registry_, impl_->current_);
		});

		interpreter->register_command("position", [this](const Tag& tag, Interpreter*) {
			position(tag, impl_->layer_registry_, impl_->current_);
		});

		interpreter->register_command("title", [this](const Tag& tag, Interpreter* interpreter) -> void {
			title(tag, interpreter);
		});
	}
} // raptor::interpreter::graphic


