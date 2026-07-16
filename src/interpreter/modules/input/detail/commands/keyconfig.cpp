#include "commands.h"

#include "interpreter/modules/input/detail/Status.h"



namespace raptor::interpreter::input::detail {
	auto start_keyconfig(Status& status) -> void {
		status.set_keyconfig_on(true);
	}
	auto stop_keyconfig(Status& status) -> void {
		status.set_keyconfig_on(false);
	}
} // raptor::interpreter::input::detail
