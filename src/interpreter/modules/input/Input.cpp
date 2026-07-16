#include "Input.h"

#include "interpreter/modules/input/detail/Status.h"
#include "interpreter/modules/input/detail/commands/commands.h"


namespace raptor::interpreter::input {
	class Input::Impl {
	public:
		using Status =  detail::Status;

		Status status{};
	};

	Input::Input() : impl_(std::make_unique<Impl>()) {}
	Input::~Input() = default;

	auto Input::register_commands(Interpreter* interpreter) -> void  {
		interpreter->register_command("start_keyconfig", [this](const Tag&, Interpreter*) {
			detail::start_keyconfig(impl_->status);
		});

		interpreter->register_command("stop_keyconfig", [this](const Tag&, Interpreter*) {
			detail::stop_keyconfig(impl_->status);
		});
	}
} // raptor::interpreter::input