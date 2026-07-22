#include "Transition.h"

#include "engine/video/objects/ScreenObjRegistry.h"

namespace raptor::engine::video {
	Transition::Transition(
			uint64_t id, const std::vector<uint64_t>& fadeIn,
			const std::vector<uint64_t>& fadeOut,
			int time_milliseconds,
			const ScreenObjRegistry* obj_registry
		) : id_(id){
			auto time_seconds = static_cast<float>(time_milliseconds) / 1000.0f;
			float max_slope = 1.0f / time_seconds;

			for (auto curr_id : fadeOut) {
				auto obj = obj_registry->get_object(curr_id);
				if (obj == nullptr) continue;

				float slope;
				if (obj->get_alpha() != 1.0f) {
					slope = obj->get_alpha() / time_seconds;
				} else {
					slope = max_slope;
				}

				alpha_slopes_.push_back(-slope);
				objects_.push_back(id);
			}

			for (auto curr_id : fadeIn) {
				auto obj = obj_registry->get_object(curr_id);

				obj->set_visible(true);
				obj->set_alpha(0); // TODO

				alpha_slopes_.push_back(max_slope);
				objects_.push_back(curr_id);
			}
		}



	bool Transition::update(double dt, const ScreenObjRegistry* obj_registry) {
		// Arrays must stay in sync; treat mismatch as an error state (never "done").
		if (objects_.size() != alpha_slopes_.size()) return false;

		bool all_done = true;
		for (size_t i = 0; i < objects_.size(); i++) {
			auto obj = obj_registry->get_object(objects_[i]);

			if (obj == nullptr) continue;


			auto slope = alpha_slopes_[i];
			auto new_alpha = static_cast<float>(obj->get_alpha() + slope * dt);


			// Clamp to target and detect completion
			bool done = false;
			if (slope > 0) {
				if (new_alpha >= 1.0f) {
					new_alpha = 1.0f;
					done = true;
				}
			} else {
				if (new_alpha <= 0.0f) {
					new_alpha = 0.0f;
					done = true;
				}
			}

			obj->set_alpha(static_cast<float>(new_alpha));

			if (!done) all_done = false;
		}

		return all_done;
	}
} // raptor::engine::video
