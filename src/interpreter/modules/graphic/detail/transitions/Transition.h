#ifndef RAPTOR_TRANSITION_H
#define RAPTOR_TRANSITION_H

#include "interpreter/modules/graphic/detail/layer/Layer.h"

namespace raptor::interpreter::graphic::detail {
	struct Transition {
		const uint64_t id = new_id();
		bool cross = false;
		bool wait = true;
		int time = 3000;

		Layer* target = nullptr;
	};
}

#endif //RAPTOR_TRANSITION_H
