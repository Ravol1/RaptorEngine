#ifndef RAPTOR_TRANSITION_MANAGER_H
#define RAPTOR_TRANSITION_MANAGER_H
#include <cstdint>
#include <unordered_map>

#include "events/game_event.h"
#include "helper/id.h"
#include "interpreter/Interpreter.h"
#include "interpreter/modules/graphic/detail/transitions/Transition.h"

namespace raptor::interpreter::graphic::detail {
	class TransitionManager {
	public:
		/** @brief Add a tra*/
		void add_transition(Transition transition, Interpreter* interpreter);
		void add_transition(bool cross = false, bool wait = true, int time = 3000, Interpreter* interpreter = nullptr);
	private:
		std::unordered_map<uint64_t, Transition> transitions_;

		static void dispatch_transition(const Transition& transition, Interpreter* interpreter);
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_TRANSITION_MANAGER_H
