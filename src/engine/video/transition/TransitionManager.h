#ifndef RAPTOR_TRANSITION_MANAGER_H
#define RAPTOR_TRANSITION_MANAGER_H
#include <vector>

#include "Transition.h"
#include "events/game_event.h"

namespace raptor::engine::video {
	class TransitionManager {
	public:
		TransitionManager(ScreenObjRegistry* obj_registry);


		void create_transition(const game_event::PerformTransitionData* data);
		void create_transition(
			uint64_t id,
			const std::vector<uint64_t>& fadeIn,
			const std::vector<uint64_t>& fadeOut,
			int time_milliseconds
		);

		void update(double dt);
	private:
		std::vector<Transition> transitions_;
		ScreenObjRegistry* obj_registry_;
	};
} // raptor::engine::video

#endif //RAPTOR_TRANSITION_MANAGER_H
