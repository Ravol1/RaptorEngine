#ifndef RAPTOR_MODULE_H
#define RAPTOR_MODULE_H
#include <memory>

#include "interpreter/Interpreter.h"

namespace raptor::interpreter {
	class Module {
	public:
		virtual ~Module() = default;
		virtual auto register_commands(Interpreter* interpreter) -> void = 0;

	private:

	};
} // raptor::interpreter

#endif //RAPTOR_MODULE_H
