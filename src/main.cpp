#include <iostream>
#include <print>
#include <thread>
#include <atomic>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include "engine/video/sprite/factory/factory.h"
#include "engine/video/texture/TextureFactory.h"
#include "events/game_event.h"

#include "interpreter/Interpreter.h"


using Interpreter = raptor::interpreter::Interpreter;

namespace game_event = raptor::game_event;



constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;


std::atomic<bool> running{true};




auto poll_events() -> bool {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			return false;
		}
	}

	return true;
}



void process_event(const game_event::GameEvent& event, SDL_Window* window) {
	switch (event.type) {
	default:
		break;

	case game_event::GameEventType::ChangeTitle:
		if (auto val = std::get_if<game_event::ChangeTitleData>(&event.data)) {
			SDL_SetWindowTitle(window, val->title.c_str());
		}

		break;
	}
}




void log_fps(double dt) {
	static double fps_accumulator = 0.0;
	static uint64_t frame_count = 0;


	fps_accumulator += dt;
	frame_count++;

	// Log average FPS each half second
	if (fps_accumulator >= 0.5) {
		// FPS = Tot frames / Tot time
		double fps = static_cast<double>(frame_count) / fps_accumulator;


		std::print("[FPS Counter] Average FPS in the last half second: {:.0f}\n", fps);


		// Reset counters
		fps_accumulator = 0.0;
		frame_count = 0;
	}
}



void graphics_loop(SDL_Renderer* renderer, SDL_Window* window, game_event::EventQueue* event_queue) {
	const auto frequency = static_cast<double>(SDL_GetPerformanceFrequency());
	uint64_t last_time = SDL_GetPerformanceCounter();

	// --- MAIN THREAD LOOP ---
	while (running) {
		if (!poll_events()) {
			running = false;
			continue;
		}

		uint64_t current_time = SDL_GetPerformanceCounter();
		uint64_t frame_ticks = current_time - last_time;
		last_time = current_time;

		auto dt = static_cast<float>(frame_ticks) / frequency;
		if (dt > 0.1) dt = 0.1;


		for (const auto& evt : event_queue->flush())
			process_event(evt, window);

		log_fps(dt);


		// Set a generic background
		SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
		SDL_RenderClear(renderer);


		SDL_RenderPresent(renderer);	// Synchronize with monitor refresh_rate
	}
}


auto interpreter_log(const std::string& msg, Interpreter::Severity severity) -> void {
	std::string sev_str;

	switch (severity) {
		case Interpreter::Severity::Info:
			sev_str = "Info"; break;

		case Interpreter::Severity::Warning:
			sev_str = "Warning"; break;

		case Interpreter::Severity::Error:
			sev_str = "Error"; break;

		case Interpreter::Severity::Fatal:
			sev_str = "Fatal"; break;
	}

	std::print("[{}] {}\n", sev_str, msg);
}



void interpreter_thread_loop(game_event::EventQueue* event_queue) {
	Interpreter interpreter{"test_data_demo/", event_queue};
	interpreter.set_log(interpreter_log);
	interpreter.load_modules();
	interpreter.start_execution(running);
	//running = false;
}


auto activate_VSYNC(SDL_Renderer* renderer) -> bool {
	if (!SDL_SetRenderVSync(renderer, 1)) {
		SDL_Log("Warning: Unable to start VSYNC: %s", SDL_GetError());
		return false;
	}

	return true;
}


auto main() -> int {
	game_event::EventQueue game_event_queue{};


	SDL_Init(SDL_INIT_VIDEO);

	// Create window and renderer
	SDL_Window* window = SDL_CreateWindow("Raptor", WIDTH, HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	if (!activate_VSYNC(renderer)) return -1;


	// Launch interpreter thread
	std::thread scripting_thread(interpreter_thread_loop, &game_event_queue);


	// Start the main graphics loop.
	graphics_loop(renderer, window, &game_event_queue);


	scripting_thread.join(); // Wait for the interpreter loop to finish.


	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();


	return 0;
}

