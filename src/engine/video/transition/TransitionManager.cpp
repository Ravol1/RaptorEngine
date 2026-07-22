#include "TransitionManager.h"

namespace raptor::engine::video {
	TransitionManager::TransitionManager(ScreenObjRegistry* obj_registry) : obj_registry_(obj_registry) {}


	void TransitionManager::create_transition(const game_event::PerformTransitionData* data) {
		create_transition(data->id, data->fadeIn, data->fadeOut, data->time);
	}

	void TransitionManager::create_transition(
			uint64_t id,
			const std::vector<uint64_t>& fadeIn,
			const std::vector<uint64_t>& fadeOut,
			int time_milliseconds
		) {

		transitions_.emplace_back(id,fadeIn, fadeOut, time_milliseconds, obj_registry_);
	}

	void TransitionManager::update(double dt) {
		for (auto& transition : transitions_) {
			transition.update(dt, obj_registry_);
		}
	}
} // raptor::engine::video
