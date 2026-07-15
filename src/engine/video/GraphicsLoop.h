#ifndef RAPTOR_GRAPHICS_LOOP_H
#define RAPTOR_GRAPHICS_LOOP_H
#include <atomic>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include "events/game_event.h"

namespace raptor::engine {
	class GraphicsLoop {
	public:
		GraphicsLoop(
			SDL_Renderer* renderer,
			SDL_Window* window,
			game_event::EventQueue* event_queue,
			std::atomic<bool>& running
		);

		~GraphicsLoop() = default;
		GraphicsLoop(const GraphicsLoop&) = delete;
		GraphicsLoop& operator=(const GraphicsLoop&) = delete;


		void run();

	private:
		SDL_Renderer* renderer_;
		SDL_Window* window_;
		game_event::EventQueue* event_queue_;
		std::atomic<bool>& running_;

		const double frequency_ = static_cast<double>(SDL_GetPerformanceFrequency());
		uint64_t last_time = 0;


		static bool poll_sys_events();
		auto calculate_dt() -> double;
		void process_event(const game_event::GameEvent& event);
	};
} //raptor::engine

#endif //RAPTOR_GRAPHICS_LOOP_H
