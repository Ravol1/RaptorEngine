#ifndef RAPTOR_CHARA_NEW_H
#define RAPTOR_CHARA_NEW_H
#include "tag/tag.h"
#include "interpreter/modules/character/detail/CharacterInfo.h"

namespace raptor::interpreter::character::detail {
	using CharacterList = std::unordered_map<std::string, CharacterInfo>;

	auto chara_new(const Tag& tag, CharacterList& characters) -> void;
} // raptor::interpreter::character::detail


#endif //RAPTOR_CHARA_NEW_H
