#ifndef TYRANO_PARSER_H
#define TYRANO_PARSER_H

#include <generator>
#include <string>
#include <vector>

#include "tag/tag.h"



/**
 *  Incremental (character-by-character) parser for KAG
 *
 * Completed tokens are represented as Tag objects and collected internally;
 * call Parser::drain() to retrieve and clear them.
 *
 *
 * --- Special syntax constructs ---
 *
 * The following KAG constructs are recognized and converted into structured tags.
 * Each construct is parsed from its raw textual representation into a Tag with a type
 * and associated fields.
 *
 * --- Free text ---
 *
 * Syntax:
 *   text content
 *
 * Parsed as:
 *   - Type: "text_block"
 *   - Attributes:
 *     - text: the raw text content between markers
 *
 *
 * --- JavaScript scripts ---
 *
 * Syntax:
 *   [iscript]
 *   JS_code
 *   [endscript]\n
 *
 * Parsed as:
 *   - Type: "script"
 *   - Attributes:
 *     - code: the JavaScript source code inside the block
 *
 *
 * --- Speaker labels ---
 *
 * Syntax:
 *   #Name\n
 *
 * Parsed as:
 *   - Type: "speaker"
 *   - Attributes:
 *     - text: the speaker name (Name) after '#'
 *
 *
 * --- Commands ---
 *
 * Syntax:
 *   @ command_text\n
 *
 * Parsed as:
 *   - Type: "command"
 *   - Attributes:
 *     - code: the raw command string following '@'
 *
 *
 * --- Labels ---
 *
 *	Syntax:
 *		*Label
 *
 *	Parsed as:
 *		- Type: "Label"
 *		- Attributes:
 *			- text: the text following '*'
 */
namespace raptor::parser {
	auto parse_tag(const std::string& raw_tag) -> Tag;
	auto find_and_parse(const std::string& contents) -> std::generator<Tag>;
	auto find_and_parse_all(const std::string& contents) -> std::vector<Tag>;



	/**
	 *  Character-driven, stateful parser for the script format.
	 *
	 * Feed characters one at a time via feed(); completed tokens (tags,
	 * text blocks, labels, speakers, commands, scripts) accumulate
	 * internally and can be retrieved with drain().
	 *
	 * Every TyranoScript construct which is not a tag (e.g. free text, JS scripts)
	 * is parsed as a special tag. See @c ks::parser for more details
	 *
	 * @note The parser is not thread-safe and is meant to be used for
	 * a single parsing pass over one input source.
	 */
	class Parser {
	public:
		auto feed(char c) -> void;
		auto drain() -> std::vector<Tag>;

	private:
		/**
		 * The parser's current top-level mode, controlling how plain
		 * characters are dispatched (see dispatch()).
		 */
		enum class State {
			Normal,		// Outside any tag, text block, or script region.
			Tag,		// Inside a `[...]` tag.
			Script,		// Inside an `[iscript]...[endscript]` block.
		};


		/**
		 * Tracks which single-line constructs are currently "in progress".
		 * These are orthogonal to `State` since e.g. a speaker name can be
		 * read while still logically in State::Normal.
		*/
		struct Flags {
			bool speaker = false;		// True if currently accumulating a speaker name (`#...`).
			bool label = false;			// True if currently accumulating a label (`*...`).
			bool comment = false;		// True if currently inside a KAG comment (`;...`).
			bool inline_tag = false;	// True if currently accumulating a command (`@...`).
			bool new_line = true;		// True if the previous character was a '\n'.
		};

		size_t _line = 1;

		State _state = State::Normal;
		State _previous_state = State::Normal;
		Flags _flags;


		// --- Accumulator buffers ---
		std::string _text;			// Buffer for free text
		std::string _tag_str;		// Buffer for raw content of the tag currently being read.
		std::string _speaker_name;	// Buffer for the speaker name currently being read.
		std::string _label;			// Buffer for the label currently being read.
		std::string _script;		// Buffer for JS code inside an iscript/endscript block.
		std::string _inline_tag;	// Buffer for the command currently being read.

		std::vector<Tag> _pending;	// Queue of completed tokens awaiting drain().



		auto try_start_comment(char c, bool was_new_line) -> bool;
		auto try_start_label(char c, bool was_new_line) -> bool;
		auto try_start_speaker(char c, bool was_new_line) -> bool;
		auto try_start_command(char c, bool was_new_line) -> bool;
		auto handle_new_line(char c) -> bool;
		auto try_open_tag(char c) -> bool;

		auto dispatch(char c) -> void;
		auto accumulate_or_close_tag(char c) -> void;
		auto update_state(const Tag& tag) -> void;
	};

} // raptor::parser

#endif //TYRANO_PARSER_H
