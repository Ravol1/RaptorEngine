#ifndef RAPTOR_GRAPHIC_H
#define RAPTOR_GRAPHIC_H

#include "interpreter/Interpreter.h"
#include "interpreter/modules/Module.h"


namespace raptor::interpreter::graphic {
	class Graphic : Module {
	public:
		Graphic();
		~Graphic() override;

		auto register_commands(Interpreter* interpreter) -> void override;
	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
	};
} // raptor::interpreter::graphic

#endif //RAPTOR_GRAPHIC_H
