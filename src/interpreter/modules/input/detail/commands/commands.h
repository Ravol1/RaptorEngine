#ifndef RAPTOR_COMMANDS_H
#define RAPTOR_COMMANDS_H

#include "interpreter/modules/input/detail/Status.h"


namespace raptor::interpreter::input::detail {
	auto start_keyconfig(Status& status) -> void;
	auto stop_keyconfig(Status& status) -> void;
} // raptor::interpreter::input::detail

#endif //RAPTOR_COMMANDS_H
