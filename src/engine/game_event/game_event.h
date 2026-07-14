#ifndef RAPTOR_RENDEREVENTQUEUE_H
#define RAPTOR_RENDEREVENTQUEUE_H

#include <mutex>
#include <string>
#include <variant>
#include <vector>


namespace raptor::engine::game_event {

	enum class GameEventType {
		RenderImage
	};


	struct RenderImageData {std::string path; float x,y; float width, height;};


	struct GameEvent {
		GameEventType type;
		std::variant<RenderImageData> data;
	};



	class EventQueue {
	public:
		auto push(GameEvent& event) -> void;
		auto flush() -> std::vector<GameEvent>;

	private:
		std::vector<GameEvent> queue_;
		std::mutex mutex_;
	};
} // raptor::engine::game_event

#endif //RAPTOR_RENDEREVENTQUEUE_H
