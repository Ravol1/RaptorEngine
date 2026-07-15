#include "Application/Application.h"
#include "events/game_event.h"


namespace game_event = raptor::game_event;



auto main() -> int {
	raptor::Application application{};
	application.run();

	return 0;
}

