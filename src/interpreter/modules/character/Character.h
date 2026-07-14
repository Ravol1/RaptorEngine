#ifndef RAPTOR_CHARACTER_H
#define RAPTOR_CHARACTER_H

#include "interpreter/modules/Module.h"


namespace raptor::interpreter::character {
	class Character : Module {
	public:
		Character();
		~Character() override;

		auto register_commands(Interpreter* interpreter) -> void override;

	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
	};
} // raptor::interpreter::character

#endif //RAPTOR_CHARACTER_H
