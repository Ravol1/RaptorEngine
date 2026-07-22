#ifndef RAPTOR_OBJECT_H
#define RAPTOR_OBJECT_H
#include <cstdint>
#include <string>

namespace raptor::interpreter::graphic::detail {

	struct Object {
		uint64_t id;
		int width, height;
			int x, y;

		std::string texture_path;
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_OBJECT_H
