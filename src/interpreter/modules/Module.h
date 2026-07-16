#ifndef RAPTOR_MODULE_H
#define RAPTOR_MODULE_H
#include <memory>

#include "interpreter/Interpreter.h"

namespace raptor::interpreter {
	class Module {
	public:
		Module() = default;
		virtual ~Module() = default;
		virtual auto register_commands(Interpreter* interpreter) -> void = 0;


		Module(const Module&) = delete;
		Module& operator=(const Module&) -> Module& = delete;


		Module(Module&&) = delete;
		Module& operator=(Module&&) -> Module& = delete;

	};
} // raptor::interpreter

#endif //RAPTOR_MODULE_H
