#include "Application.h"

#include <thread>
#include <print>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

#include "interpreter/Interpreter.h"
#include "engine/video/GraphicsLoop.h"


namespace raptor {
	namespace {
		using Interpreter = interpreter::Interpreter;
		auto activate_VSYNC(SDL_Renderer* renderer) -> bool {
			if (!SDL_SetRenderVSync(renderer, 1)) {
				SDL_Log("Warning: Unable to start VSYNC: %s", SDL_GetError());
				return false;
			}

			return true;
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
	}



	Application::Application() {
		if(init()) {
			graphics_loop_ = std::make_unique<GraphicsLoop>(renderer_, window_, &event_queue_, running_);
		} else {
			running_ = false;
		}
	}



	bool Application::init() {

		// Create window and renderer
		SDL_Init(SDL_INIT_VIDEO);
		window_ = SDL_CreateWindow("Raptor", WIDTH, HEIGHT, 0);
		renderer_ = SDL_CreateRenderer(window_, nullptr);

		if (!activate_VSYNC(renderer_)) return false;


		return true;
	}



	Application::~Application() {
		if (window_) {
			SDL_DestroyWindow(window_);
			window_ = nullptr;
		}

		if (renderer_) {
			SDL_DestroyRenderer(renderer_);
			renderer_ = nullptr;
		}

		SDL_Quit();
	}


	auto Application::run() -> int {
		std::thread scripting_thread(&
			Application::interpreter_thread_loop,
			this,
			&event_queue_
		);	// Launch interpreter thread

		graphics_loop_->run();	// Start the main graphics loop on this thread.

		scripting_thread.join(); // Wait for the interpreter thread to finish.
		return 0;
	}



	void Application::interpreter_thread_loop(game_event::EventQueue* event_queue) {
		Interpreter interpreter{root, event_queue};
		interpreter.set_log(interpreter_log);
		interpreter.load_modules();
		interpreter.start_execution(running_);
		//running = false;
	}
} // raptor