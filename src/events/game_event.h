#ifndef RAPTOR_RENDER_EVENT_H
#define RAPTOR_RENDER_EVENT_H

#include <mutex>
#include <string>
#include <variant>
#include <vector>


namespace raptor::game_event {

	enum class GameEventType {
		RenderImage,
		ChangeTitle,
		LoadObject,
		DeleteObject,
		PerformTransition,
	};


	struct RenderImageData {std::string path; float x,y; float width, height;};
	struct ChangeTitleData {std::string title;};
	struct LoadObjectData {uint64_t id; std::string texture_path; int z; float x,y; float width, height; bool visible;};
	struct DeleteObjectData {uint64_t id;};
	struct PerformTransitionData {uint64_t id; std::vector<uint64_t> fadeIn; std::vector<uint64_t> fadeOut; int time;};


	using GameEventData = std::variant<
		RenderImageData,
		ChangeTitleData,
		LoadObjectData,
		DeleteObjectData,
		PerformTransitionData
	>;


	struct GameEvent {
		GameEventType type;
		GameEventData data;
	};



	class EventQueue {
	public:
		auto push(GameEvent event) -> void;
		auto flush() -> std::vector<GameEvent>;

	private:
		std::vector<GameEvent> queue_;
		std::mutex mutex_;
	};
} // raptor::engine::game_event

#endif //RAPTOR_RENDER_EVENT_H
