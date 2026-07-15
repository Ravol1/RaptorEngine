#ifndef TYRANO_INTERPRETER_H
#define TYRANO_INTERPRETER_H
#include <atomic>
#include <functional>
#include <string>
#include <filesystem>

#include "tag/tag.h"
#include "events/game_event.h"


namespace fs = std::filesystem;


namespace raptor::interpreter {
	class Interpreter {
	public:
		explicit Interpreter(const fs::path& root, game_event::EventQueue* event_queue);
		~Interpreter();


		using Handler = std::function<void(const Tag&, Interpreter*)>;

		enum Severity {
			Info,
			Warning,
			Error,
			Fatal,
		};
		auto set_log(std::function<void(const std::string& msg, Severity severity)> handler) -> void;

		auto load_modules() -> void;
		auto start_execution(const std::atomic<bool>& running) -> void;

		auto execute_tag(const Tag& tag) -> void;
		auto register_command(const std::string& name, const Handler& handler) -> void;

		auto push_event(game_event::GameEventType type, game_event::GameEventData data) -> void;


	private:
		class Impl;
		std::unique_ptr<Impl> impl_;


		enum Errors {
			None = 0,
			Generic,
			FileNotFound,
			MissingAttribute,
			ReturnOnEmptyStack,
			EndmacroOnEmptyStack,
			InvalidMacro,
			MacroNotEnded,
			UnknownTag,
			MissingScript,
			ScriptNotEnded,
		};
	};
} // raptor::interpreter


#endif //TYRANO_INTERPRETER_H
