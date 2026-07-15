#include <cstdint>
#include <print>


namespace raptor::engine {
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
}