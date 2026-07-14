#include "Character.h"

#include "detail/commands/commands.h"
#include "detail/CharacterInfo.h"

namespace raptor::interpreter::character {
	class Character::Impl {
	public:
		/// Collection of the character keyed by internal name
		std::unordered_map<std::string, CharacterInfo> characters;
	};


	Character::Character() : impl_(std::make_unique<Impl>()){}
	Character::~Character() = default;


	auto Character::register_commands(Interpreter* interpreter) -> void {
		interpreter->register_command("chara_new", [this](const Tag& tag) {
			detail::chara_new(tag, impl_->characters);
		});
	}
} // raptor::interpreter::character
