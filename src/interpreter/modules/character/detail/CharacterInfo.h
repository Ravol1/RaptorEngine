#ifndef RAPTOR_CHARACTERINFO_H
#define RAPTOR_CHARACTERINFO_H

#include <string>
#include "helper/color.h"

namespace raptor::interpreter::character {
	constexpr static float autosize = -1.0f;
	struct CharacterInfo {
		std::string display_name;
		std::string storage;
		float width, height;
		bool reflect;
		ColorRGB name_color;
	};
}


#endif //RAPTOR_CHARACTERINFO_H
