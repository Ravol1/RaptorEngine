#include "GraphicsLoop.h"

#include <SDL3/SDL_timer.h>

#include <engine/diagnostic/log_fps.h>


namespace raptor::engine {
	GraphicsLoop::GraphicsLoop(
			SDL_Renderer* renderer,
			SDL_Window* window,
			game_event::EventQueue* event_queue,
			std::atomic<bool>& running
	) :
	renderer_(renderer),
	window_(window),
	event_queue_(event_queue),
	texture_factory_{renderer_},
	transition_manager_{&obj_registry_},
	running_(running) {}



	void GraphicsLoop::run() {
		uint64_t last_time = SDL_GetPerformanceCounter();
		static constexpr SDL_Color BACK_COLOR(30, 30, 40, 255);

		// --- MAIN THREAD LOOP ---
		while (running_) {
			auto dt = calculate_dt();


			// Poll and process system events
			if (!poll_sys_events()) {
				running_ = false;
				continue;
			}


			// Process game events
			for (const auto& evt : event_queue_->flush())
				process_event(evt);



			// --- UPDATE ---
			obj_registry_.update(dt);
			transition_manager_.update(dt);

			// --- RENDER ---
			// Background
			SDL_SetRenderDrawColor(renderer_, BACK_COLOR.r, BACK_COLOR.g, BACK_COLOR.b, BACK_COLOR.a);
			SDL_RenderClear(renderer_);

			obj_registry_.render(renderer_);

			SDL_RenderPresent(renderer_);


			log_fps(dt);
		}
	}


	/**
	 * @bried poll and process all pending system events
	 *
	 * As it is now, it only processes SDL_EVENT_QUIT
	 *
	 * @return false if system sent SDL_EVENT_QUIT;
	 * true otherwise
	 */
	auto GraphicsLoop::poll_sys_events() -> bool {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				return false;
			}
		}

		return true;
	}


	/**
	 * @brief Calculates the delta time from the previous frame.
	 *
	 * @return The calculated delta time
	 */
	auto GraphicsLoop::calculate_dt() -> double {
		uint64_t current_time = SDL_GetPerformanceCounter();
		uint64_t frame_ticks = current_time - last_time;
		last_time = current_time;

		auto dt = static_cast<float>(frame_ticks) / frequency_;
		if (dt > 0.1) dt = 0.1;

		return dt;
	}


	/**
	 * @bried Processes a game event
	 * @param event		The event to process.
	 */
	void GraphicsLoop::process_event(const game_event::GameEvent& event) {
		switch (event.type) {
		default:
			break;

		case game_event::GameEventType::ChangeTitle:
			handle_change_title(event);
			break;

		case game_event::GameEventType::LoadObject:
			handle_load_object(event);
			break;

		case game_event::GameEventType::PerformTransition:
			handle_perform_transition(event);
			break;
		}
	}

	/**
	 * @brief Handles a ChangeTitle event.
	 * @param event		The event to process.
	 */
	void GraphicsLoop::handle_change_title(const game_event::GameEvent& event) {
		if (auto data = std::get_if<game_event::ChangeTitleData>(&event.data)) {
			SDL_SetWindowTitle(window_, data->title.c_str());
		}
	}

	/**
	 * @brief Handles a LoadObject event.
	 * @param event		The event to process.
	 */
	void GraphicsLoop::handle_load_object(const game_event::GameEvent& event) {
		if (auto data = std::get_if<game_event::LoadObjectData>(&event.data)) {
			auto new_obj = obj_registry_.create_object(
				data->id,
				data->texture_path,
				static_cast<int>(data->width), static_cast<int>(data->height),
				texture_factory_, renderer_);
			new_obj->set_pos({data->x, data->y});
			new_obj->set_visible(data->visible);
		}
	}

	/**
	 * @brief Handles a PerformTransition event.
	 * @param event		The event to process.
	 */
	void GraphicsLoop::handle_perform_transition(const game_event::GameEvent& event) {
		if (auto data = std::get_if<game_event::PerformTransitionData>(&event.data)) {
			transition_manager_.create_transition(data);
		}
	}
} // raptor::engine