#ifndef RAPTOR_INPUT_H
#define RAPTOR_INPUT_H
#include "interpreter/modules/Module.h"

namespace raptor::interpreter::input {
	class Input : Module {
	public:
		Input();
		~Input() override;

		auto register_commands(Interpreter* interpreter) -> void override;


	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
	};
} // raptor::interpreter::input

#endif //RAPTOR_INPUT_H
