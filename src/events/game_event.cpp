#include "game_event.h"

namespace raptor::game_event {
	auto EventQueue::push(GameEvent event) -> void {
		std::lock_guard<std::mutex> lock(mutex_);
		queue_.push_back(event);
	}

	auto EventQueue::flush() -> std::vector<GameEvent> {
		std::lock_guard<std::mutex> lock(mutex_);
		std::vector<GameEvent> localQueue;
		localQueue.swap(queue_);
		return localQueue;
	}
} // raptor::engine