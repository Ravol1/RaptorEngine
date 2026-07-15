#include "commands.h"
#include "interpreter/Interpreter.h"
#include "events/game_event.h"


namespace raptor::interpreter::graphic::detail {
	auto title(const Tag& tag, Interpreter* interpreter) -> void {
		auto title = tag.get_attribute("name");

		if (title.empty()) throw std::runtime_error("Missing required attribute 'name'");

		interpreter->push_event(
			game_event::GameEventType::ChangeTitle,
			game_event::ChangeTitleData(title)
		);
	}
} // raptor::interpreter::graphic::detail