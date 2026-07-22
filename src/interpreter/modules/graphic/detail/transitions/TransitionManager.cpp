#include "TransitionManager.h"

#include <ranges>


namespace raptor::interpreter::graphic::detail {
	void TransitionManager::add_transition(Transition transition, Interpreter* interpreter) {
		transitions_.emplace(transition.id,transition);

		dispatch_transition(transition, interpreter);
	}


	void TransitionManager::add_transition(bool cross, bool wait, int time, Interpreter* interpreter) {
		Transition transition;
		transition.cross = cross;
		transition.wait = wait;
		transition.time = time;


		transitions_.emplace(transition.id, transition);

		dispatch_transition(transition, interpreter);
	}

	void TransitionManager::dispatch_transition(const Transition& transition, Interpreter* interpreter) {
		auto lay = transition.target;


		auto fade_in = lay->back()
			| std::views::transform([](const auto& obj) { return obj.id; })
			| std::ranges::to<std::vector>();


		auto fade_out = lay->fore()
			| std::views::transform([](const auto& obj){return obj.id; })
			| std::ranges::to<std::vector>();



		interpreter->push_event(
			game_event::GameEventType::PerformTransition,
			game_event::PerformTransitionData{
				transition.id,
				std::move(fade_in),
				std::move(fade_out),
				transition.time
			});
	}
} // raptor::interpreter::graphic::detail
