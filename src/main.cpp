#include <iostream>
#include <print>
#include <thread>
#include <atomic>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include "engine/video/ScreenObject.h"
#include "engine/video/sprite/Sprite.h"
#include "engine/video/sprite/factory/factory.h"
#include "engine/video/texture/TextureFactory.h"
#include "engine/game_event/game_event.h"

#include "interpreter/Interpreter.h"



using Sprite = raptor::engine::video::sprite::Sprite;
using TextureFactory = raptor::engine::video::texture::TextureFactory;
using ScreenObject = raptor::engine::video::ScreenObject;
using Interpreter = raptor::interpreter::Interpreter;

namespace game_event = raptor::engine::game_event;



constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

game_event::EventQueue game_event_queue;



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



auto create_test_sprite(SDL_Renderer* renderer, TextureFactory& texture_factory)
	-> std::unique_ptr<Sprite> {
	namespace sprite_factory = raptor::engine::video::sprite::factory;


	constexpr char path[] = "test_data/data/fgimage/chara/1/aびっくり.png";


	auto texture = texture_factory.from_file(path, renderer);
	auto sprite = sprite_factory::create_static();



	sprite->set_texture(texture);

	return sprite;
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



void graphics_loop(SDL_Renderer* renderer) {

	// ------------
	// Test code
	// ------------
	TextureFactory texture_factory(renderer);

	auto sprite = create_test_sprite(renderer, texture_factory);
	ScreenObject screen_obj(std::move(sprite));

	screen_obj.set_pos(SDL_FPoint{0, 0});
	//screen_obj.set_pos({WIDTH / 2.0f, HEIGHT / 2.0f});
	screen_obj.set_visible(true);

	// ------------
	// End test
	// ------------

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


		log_fps(dt);


		// Set a generic background
		SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
		SDL_RenderClear(renderer);

		screen_obj.update(dt);
		screen_obj.render(renderer);

		SDL_RenderPresent(renderer);	// Synchronize with monitor refresh_rate
	}
}


auto interpreter_log(const std::string& msg, Interpreter::Severity severity) -> void {
	switch (severity) {
		case Interpreter::Severity::Info:
			//std::cout << msg << std::endl;
			break;

		case Interpreter::Severity::Warning:
			std::cout << msg << std::endl;
			break;

		case Interpreter::Severity::Error:
			std::cerr << msg << std::endl;
			break;

		case Interpreter::Severity::Fatal:
			std::cerr << "FATAL: " << msg << std::endl;
			break;
	}
}



void interpreter_thread_loop() {
	Interpreter interpreter{"test_data/"};
	// interpreter.set_error_handler(interpreter_log);
	interpreter.load_modules();
	interpreter.start_execution(running);
	running = false;
}


auto activate_VSYNC(SDL_Renderer* renderer) -> bool {
	if (!SDL_SetRenderVSync(renderer, 1)) {
		SDL_Log("Warning: Unable to start VSYNC: %s", SDL_GetError());
		return false;
	}

	return true;
}


auto main() -> int {
	SDL_Init(SDL_INIT_VIDEO);

	// Create window and renderer
	SDL_Window* window = SDL_CreateWindow("test", WIDTH, HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	if (!activate_VSYNC(renderer)) return -1;


	// Launch interpreter thread
	std::thread scripting_thread(interpreter_thread_loop);


	// Start the main graphics loop.
	graphics_loop(renderer);




	scripting_thread.join(); // Wait for the interpreter loop to finish.

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();


	return 0;
}

