#ifndef RAPTOR_TRANSITION_H
#define RAPTOR_TRANSITION_H
#include <vector>

#include "engine/video/objects/ScreenObjRegistry.h"

namespace raptor::engine::video {
	class Transition {
	public:
		Transition(
			uint64_t id, const std::vector<uint64_t>& fadeIn,
			const std::vector<uint64_t>& fadeOut,
			int time_milliseconds,
			const ScreenObjRegistry* obj_registry
		);

		bool update(double dt, const ScreenObjRegistry* obj_registry);

	private:
		uint64_t id_;
		std::vector<uint64_t> objects_;
		std::vector<float> alpha_slopes_;
	};
} // raptor::engine::video

#endif //RAPTOR_TRANSITION_H
