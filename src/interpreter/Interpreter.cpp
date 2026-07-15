#include "Interpreter.h"

#include <stdexcept>
#include <fstream>

#include "parser/parser.h"
#include "interpreter/modules/graphic/Graphic.h"
#include "interpreter/modules/character/Character.h"


namespace raptor::interpreter {
	namespace {
		/**
		 * @brief Reads the entire contents of a file into a std::string.
		 *
		 * @param file_name Path (as a string) of the file to read.
		 * @return The full contents of the file.
		 * @throws std::runtime_error if the file cannot be opened.
		 */
		auto read_file(const std::string& file_name) -> std::string {
			auto f = std::ifstream(file_name, std::ios::in);

			if (!f) throw std::runtime_error("Could not open file: " + file_name);

			// Determine file size by seeking to the end, then rewind.
			f.seekg(0, std::ios::end);
			auto size = f.tellg();
			f.seekg(0, std::ios::beg);

			std::string contents;
			contents.resize(size);

			f.read(contents.data(), size);


			return contents;
		}


		/**
		 * @brief Resolves a subdirectory under a given root, returning it only
		 *        if it actually exists and is a directory.
		 *
		 * @param root   The base/root directory to search under.
		 * @param subdir The subdirectory (relative path) to look for.
		 * @return The resolved path if it exists and is a directory,
		 *         otherwise an empty path.
		 */
		auto try_get_subdir(const fs::path& root, const fs::path& subdir) -> fs::path {
			auto path = root / subdir;

			if (fs::exists(path) && fs::is_directory(path)) return path;

			return "";
		}
	}



	// Class Interpreter ----------------------------------------------



	/**
	 * @brief Private implementation (PIMPL) of Interpreter.
	 */
	class Interpreter::Impl {
	private:
		/**
		 * @brief Advances the instruction pointer of the current frame until
		 *        a tag of the given type is found (inclusive), or the end of
		 *        the current program is reached.
		 *
		 * Used for error recovery.
		 *
		 * @param tag_type The tag type to stop at.
		 * @return true if a matching tag was found before the end of the
		 *         program, false otherwise.
		 */
		auto skip_until(const std::string& tag_type)-> bool {
			while (current_.ip < current_.program->size()) {
				auto tag = current_.program->at(current_.ip);
				current_.ip++;
				if (tag.type == tag_type) return true;
			}

			return false;
		}

		struct Modules {
			graphic::Graphic graphic;
			character::Character character;
		};
		Modules modules_{};

	public:
		/**
		 * @brief	Resolves the mandatory subdirectories and loads the
		 *			entry-point script as the initial execution frame.
		 *
		 * @param owner Pointer back to the owning Interpreter (used to
		 *              register commands created dynamically, i.e. macros).
		 * @param root  Root directory of the game data.
		 * @throws std::runtime_error	if one or more of the required subdirectories
		 *								does not exist under root.
		 */
		explicit Impl(Interpreter* owner, const fs::path& root) : owner_(owner) {

			// Locate the mandatory "data/scenario" directory; this is where
			// all scenario (.ks) files are expected to live.
			paths_.scenario = try_get_subdir(root, fs::path("data") / "scenario");
			if (paths_.scenario.empty())
				throw std::runtime_error(std::format(
					"Required subdirectory 'data/scenario' not found in root '{}'",
					root.string()
				));


			// Load and set the entry-point script as the initial execution frame.
			auto starting_file = paths_.scenario / "first.ks";
			current_.program = &load_program(starting_file);
			current_.file = "first.ks";
			current_.ip = 0;
		}


		auto load_modules() -> void {
			modules_.graphic.register_commands(owner_);
			modules_.character.register_commands(owner_);
		}


		/**
		 * @brief Represents a single execution context: which program (tag
		 *        list) is running, the current instruction pointer within
		 *        it, and the originating file name (for error logging).
		 */
		struct Frame {
			const std::vector<Tag>* program = nullptr;	/// Pointer to the parsed tag sequence being executed.
			size_t ip = 0;								/// Current instruction pointer

			std::string file;							/// Source file name
		};


		/**
		 * @brief Collection of resolved resource directories used by the
		 *        interpreter and submodules.
		 */
		struct Paths {
			fs::path scenario;
			fs::path image_path;
			fs::path bgm_path;
			// Continue with the list of all paths
		};



		/// Callback invoked to report execution errors.
		std::function<void(const std::string& msg, Severity severity)> log_func;


		/// Back-pointer to the owning Interpreter instance
		Interpreter* owner_;


		/// Current/last error state used by clear_error() to decide how to recover.
		Errors error_ = None;

		Paths paths_{};


		/// Cache of the parsed programs keyed by file.
		std::unordered_map<std::string, std::vector<Tag>> programs_;

		/// Registry of tag-type -> handler function used to dispatch execution.
		std::unordered_map<std::string, Handler> commands_;

		/// Registry of user-defined macros, keyed by macro name, storing the frame where macro execution starts.
		std::unordered_map<std::string, Frame> macros_;


		/// The currently executing frame.
		Frame current_{};

		/// Stack of saved frames for [call]/[return] control flow.
		std::vector<Frame> call_stack_;

		/// Stack of saved frames for macro invocation/return.
		std::vector<Frame> macro_stack_;


		/**
		 * @brief	Parses and caches the program for the given file path.
		 *			If the file has already been loaded and parsed,
		 *			the cached result is returned directly.
		 *
		 * @param path Path of the script file to load.
		 * @return A reference to the cached vector of Tag objects representing
		 *         the parsed program.
		 */
		auto load_program(const std::string& path) -> const std::vector<Tag>& {
			if (!programs_.contains(path)) {
				auto contents = read_file(path);
				auto program = parser::find_and_parse_all(contents);
				programs_[path] = parser::find_and_parse_all(contents);
			}

			auto tmp = programs_[path];
			return programs_[path];
		}



		/**
		 * @brief Begins execution of a previously registered macro by name.
		 *
		 * Pushes the current frame onto the macro stack, then switches the
		 * current execution frame to the one stored for the macro.
		 *
		 * @param name Name of the macro to run.
		 * @throws std::runtime_error if no macro with the given name is registered.
		 */
		auto run_macro(const std::string& name) -> void {
			auto found = macros_.find(name);
			if (found == macros_.end()) {
				throw std::runtime_error(std::format("Unknown macro '{}'", name));
			}

			macro_stack_.push_back(current_);
			current_ = found->second;
		}



		/**
		 * @brief Handles reaching the end of the current program.
		 *
		 * If the call stack is non-empty, this pops the top frame
		 * and resumes execution there. If the call stack is empty,
		 * execution simply ends.
		 */
		auto handle_EOF() -> void {
			if (!call_stack_.empty()) {
				current_ = call_stack_.back();
				call_stack_.pop_back();
				current_.ip++;
			}
		}



		/**
		 * @brief Executes the [call] tag: jumps into another scenario file,
		 *		  at the specified label, pushing the current frame onto the call stack.
		 *
		 * @param tag The [call] tag being executed.
		 * @throws std::runtime_error if the "storage" attribute is missing or empty.
		 */
		auto call_cmd(const Tag& tag) -> void {
			if (!tag.has_attribute("storage")) {
				error_ = Errors::MissingAttribute;
				throw std::runtime_error("Missing required attribute 'storage'");
			}

			auto path = tag.get_attribute("storage");

			if (path.empty()) {
				error_ = Errors::MissingAttribute;
				throw std::runtime_error("Required attribute 'storage' was empty");
			}

			// Save the current execution context so we can resume after the call.
			call_stack_.push_back(current_);

			// Load the new frame into the current execution frame.
			current_.program = &load_program(paths_.scenario / path);
			current_.ip = 0;
			current_.file = std::move(path);
		}


		/**
		 * @brief Executes the [return] tag: pops the call stack and resumes
		 *        execution in the caller's frame, right after the [call] tag.
		 *
		 * @param tag The [return] tag being executed (unused, kept for
		 *            handler signature consistency).
		 * @throws std::runtime_error if the call stack was empty.
		 */
		auto return_cmd(const Tag& tag) -> void {
			if (call_stack_.empty()) {
				error_ = Errors::ReturnOnEmptyStack;
				throw std::runtime_error("'return' used with an empty call stack");
			}

			current_ = call_stack_.back();
			call_stack_.pop_back();
			current_.ip++;
		}



		/**
		 * @brief Executes the [jump] tag: switches execution to another
		 *        scenario file at the specified label.
		 *
		 * @param tag The [jump] tag being executed.
		 * @throws std::runtime_error if the "storage" attribute is missing or empty.
		 */
		auto jump_cmd(const Tag& tag) -> void {
			if (!tag.has_attribute("storage")) {
				error_ = Errors::MissingAttribute;
				throw std::runtime_error("Missing required attribute 'storage'");
			}

			auto path = tag.get_attribute("storage");

			if (path.empty()) {
				error_ = Errors::MissingAttribute;
				throw std::runtime_error("Required attribute 'storage' was empty");
			}



			// Load the new frame into the current execution frame.
			current_.program = &load_program(paths_.scenario / path);
			current_.ip = 0;
			current_.file = std::move(path);
		}



		/**
		 * @brief Executes the [macro] tag: defines a new macro by scanning
		 *        forward for its matching [endmacro] tag, registering the
		 *        macro's body location, and then skipping the interpreter's
		 *        instruction pointer past the macro definition.
		 *
		 * Dynamically registers a new command with the macro's name,
		 * so the macro can be invoked like any other tag/command.
		 *
		 * If a macro with the same name is already registered,
		 * silently redefines it with the new one.
		 *
		 * @param tag The [macro] tag being executed.
		 * @throws std::runtime_error if:
		 *         - the "name" attribute is missing/empty,
		 *         - the name would collide with an existing non-macro command,
		 *         - no matching "endmacro" tag is found before the end of the program.
		 */
		auto macro_cmd(const Tag& tag) -> void {
			auto name = tag.get_attribute("name");
			if (name.empty()) {
				error_ = Errors::MissingAttribute;
				throw std::runtime_error("Missing required attribute 'name'");
			}


			// Prevent a macro from silently overriding a non-macro, already
			// registered command with the same name.
			if (!macros_.contains(name) && commands_.contains(name)) {
				error_ = Errors::InvalidMacro;
				throw std::runtime_error(std::format(
					"Macro '{}' would redefine an existing command",
					name
				));
			}


			// --- Scan forward from the current tag to find the "endmacro" tag ---
			size_t mp = current_.ip;		// Pointer inside the macro
			while (++mp < current_.program->size()) {
				if (current_.program->at(mp).type == "endmacro") break;
			}

			// --- If we reached the end of the program without finding the closing tag ---
			if (mp >= current_.program->size()) {
				error_ = Errors::MacroNotEnded;
				throw std::runtime_error(std::format(
					"Macro '{}' was declared but never closed with 'endmacro'",
					name
				));
			}

			// --- Register the macro or redefine it if already existing: it begins right after the [macro] tag ---
			macros_[name] = Frame{current_.program, current_.ip+1, current_.file};
			if (!commands_.contains(name))
				owner_->register_command(name, [this, name](const Tag&, Interpreter*)
					{ this->run_macro(name); });

			// Skip past the macro's body and the closing [endmacro] in the current frame.
			current_.ip = mp + 1;

		}


		/**
		 * @brief Executes the [endmacro] tag: pops the macro stack and
		 *        resumes execution in the frame that invoked the macro.
		 *
		 * @param tag The [endmacro] tag being executed (unused, kept for
		 *            handler signature consistency).
		 * @throws std::runtime_error if the macro stack is empty (i.e.
		 *         [endmacro] was reached without a corresponding macro
		 *         invocation in progress).
		 */
		auto endmacro_cmd(const Tag& tag) -> void {
			if (macro_stack_.empty()) {
				error_ = Errors::EndmacroOnEmptyStack;
				throw std::runtime_error("'endmacro' used with an empty macro stack");
			}

			current_ = macro_stack_.back();
			macro_stack_.pop_back();
			current_.ip++;
		}


		/**
		 * @brief Executes the [iscript] tag: collects the inline script
		 *        source code from the following "script" tag(s) until the
		 *        matching closing tag and hands it off to a JS-like
		 *        parser/interpreter for execution.
		 *
		 * @param tag The [iscript] tag being executed.
		 * @throws std::runtime_error if no script content is found, or if
		 *         the closing tag '[endscript]' is never reached.
		 *
		 * @note The actual delegation to the script engine is not yet
		 *       implemented in this function (see trailing comment in the body).
		 */
		auto iscript_cmd(const Tag& tag) -> void {



			std::string script_text;


			// --- Accumulate the code from consecutive "script" tags until the closing tag is encountered ---
			auto script_ptr = current_.ip;	// Pointer inside the script'
			while (++script_ptr < current_.program->size()) {
				if (auto& new_tag = current_.program->at(script_ptr); new_tag.type == "script")
						script_text.append(new_tag.get_attribute("code"));


				if (current_.program->at(script_ptr).type == "endscript") break;
			}


			// --- If we reached the end of the program without finding the closing tag ---
			if (script_ptr >= current_.program->size()) {
				error_ = Errors::ScriptNotEnded;
				throw std::runtime_error("Missing closing tag '[endscript]' after script opening tag '[iscript]'");
			}


			if (script_text.empty()) {
				error_ = Errors::MissingScript;
				throw std::runtime_error("Missing script declaration after tag '[iscript]'");
			}


			current_.ip = script_ptr + 1;


			auto& file = current_.file;
			auto line = tag.line;


			log_func(std::format(
				"{}:{}: script detected with text\n{}",
				file, line, script_text
			), Severity::Info);
		}



		/**
		 * @brief Attempts to recover from the current error state,
		 *        adjusting the current execution frame as needed.
		 *
		 * Invoked by start_execution() right after a tag handler
		 * throws an exception, to decide whether execution can safely
		 * continue (returning true) or must stop (returning false).
		 *
		 * @return true if execution can continue after recovery, false if
		 *         the error is unrecoverable and execution must halt.
		 */
		auto clear_error() -> bool {
			switch (error_) {
			case Errors::None:
				return true;

			default:
				return false;

			case FileNotFound:
				// Skip the offending tag and continue with the next one.
				current_.ip++;
				return true;

			case MissingAttribute:
				// If the error occurred on a [macro] tag, skip the whole
				// (malformed) macro body up to its "endmacro".
				if (current_.program->at(current_.ip).type == "macro")
					return skip_until("endmacro");

				current_.ip++;
				return true;

			case ReturnOnEmptyStack:
				// Unrecoverable: there is no frame to return to.
				return false;

			case InvalidMacro:
				// Skip the conflicting macro definition entirely.
				return skip_until("endmacro");

			case MacroNotEnded:
				// No closing tag was found; there is nothing left to execute
				// in this program, so move the instruction pointer to the end.
				current_.ip = current_.program->size();
				return true;

			case UnknownTag:
				// Skip the unrecognized tag and continue with the next one.
				current_.ip++;
				return true;

			case MissingScript:
				// Skip until the closing "endscript" tag.
				return skip_until("endscript");

			case ScriptNotEnded:
				// No closing tag was found; there is nothing left to execute
				// in this program, so move the instruction pointer to the end.
				current_.ip = current_.program->size();
				return true;
			}

		}
	};



	Interpreter::Interpreter(const fs::path& root) {
		impl_ = std::make_unique<Impl>(this, root);


		if (!fs::exists(root))
			if (!fs::exists(root))
				throw std::runtime_error(std::format(
				"Path does not exist: '{}'",
				root.string()
			));

		if (!fs::is_directory(root))
			throw std::runtime_error(std::format(
				"Root path is not a directory: '{}'",
				root.string()
			));



		register_command("call", [this](const Tag& tag, Interpreter*){impl_->call_cmd(tag);});
		register_command("return", [this](const Tag& tag, Interpreter*){impl_->return_cmd(tag);});

		register_command("jump", [this](const Tag& tag, Interpreter*){impl_->jump_cmd(tag);});

		register_command("macro", [this](const Tag& tag, Interpreter*){impl_->macro_cmd(tag);});
		register_command("endmacro", [this](const Tag& tag, Interpreter*){impl_->endmacro_cmd(tag);});

		register_command("iscript", [this](const Tag& tag, Interpreter*){impl_->iscript_cmd(tag);});

		register_command("label", [](const Tag&, Interpreter*){});
	}


	Interpreter::~Interpreter() = default;


	auto Interpreter::register_command(const std::string& name, const Interpreter::Handler& handler) -> void {
		if (impl_->commands_.contains(name))
			throw std::runtime_error(std::format(
				"Command '{}' is already registered",
				name
			));

		impl_->commands_[name] = handler;
	}



	void Interpreter::execute_tag(const Tag& tag) {
		auto it = impl_->commands_.find(tag.type);
		if (it == impl_->commands_.end())
			throw std::runtime_error(std::format(
				"Unknown tag '{}'",
				tag.type
			));


		// Execute the tag
		it->second(tag, this);
	}


	auto Interpreter::set_log(std::function<void(const std::string&, Severity)> handler) -> void {
		impl_->log_func = std::move(handler);
	}


	auto Interpreter::load_modules() -> void {
		impl_->load_modules();
	}


	auto Interpreter::start_execution(const std::atomic<bool>& running) -> void {
		while (impl_->current_.ip < impl_->current_.program->size() && running) {
			auto ip = impl_->current_.ip;
			const Tag& tag = impl_->current_.program->at(ip);

			auto* previous_program = impl_->current_.program;
			auto previous_ip = impl_->current_.ip;

			try {
				execute_tag(tag);
			} catch (const std::exception& e) {
				std::string error_msg = std::format("{}:{}: {}",impl_->current_.file, tag.line, e.what());

				if (impl_->clear_error()) {
					if (impl_->log_func != nullptr)
						impl_->log_func(error_msg, Severity::Error);
				}

				else {
					if (impl_->log_func != nullptr)
						impl_->log_func(error_msg, Severity::Fatal);

					return;
				}
			}


			if (impl_->current_.program == previous_program && impl_->current_.ip == previous_ip)
				impl_->current_.ip++;


			if (impl_->current_.ip == impl_->current_.program->size())
				impl_->handle_EOF();
		}
	}
} // raptor::interpreter