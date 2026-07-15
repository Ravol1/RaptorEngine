#ifndef RAPTOR_APPLICATION_H
#define RAPTOR_APPLICATION_H
#include <atomic>
#include <memory>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "engine/video/GraphicsLoop.h"
#include "events/game_event.h"

namespace raptor {
	constexpr int WIDTH = 1280;
	constexpr int HEIGHT = 720;


	class Application {
	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		auto run() -> int;

	private:
		static constexpr char root[] = "test_data_demo/";
		using GraphicsLoop = engine::GraphicsLoop;


		bool init();
		void interpreter_thread_loop(game_event::EventQueue* event_queue);

		SDL_Window* window_ = nullptr;
		SDL_Renderer* renderer_ = nullptr;
		game_event::EventQueue event_queue_{};
		std::atomic<bool> running_ = true;
		std::unique_ptr<engine::GraphicsLoop> graphics_loop_;
	};
} // raptor

#endif //RAPTOR_APPLICATION_H
