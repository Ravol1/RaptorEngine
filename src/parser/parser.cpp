#include "parser.h"

#include <generator>
#include <re2/re2.h>

#include "helper/string.h"


namespace raptor::parser {
	/**
	 *  Parses all key-value attribute pairs from a raw tag string.
	 *
	 * @param tag_str	The raw tag content inside the square brackets,
	 *					without the brackets themselves.
	 *
	 * @return			A map from attribute name to attribute value. Quotes
	 *					around quoted values are stripped.
	 *
	 * @note PERF: This uses RE2. If profiling shows this function creating
	 * a performance issue, try rewriting it doing manual lookup.
	 */
	auto parse_attributes(const std::string& tag_str) -> std::unordered_map<std::string, std::string> {
		std::unordered_map<std::string, std::string> attributes;


		// Group 1: attribute name (identifier-like: starts with a letter or
		//          underscore, followed by word chars or hyphens).
		// Group 2: the full value token (quoted or unquoted) — unused directly,
		//          kept only because RE2::FindAndConsume needs it to advance
		//          past the whole match.
		// Group 3: value with surrounding quotes stripped (used when quoted).
		// Group 4: value when NOT quoted (a run of non-whitespace chars).
		static const RE2 attr_regex(R"regex(([a-zA-Z_][\w-]*)\s*=\s*("([^"]*)"|([^\s]+)))regex");

		if (!attr_regex.ok()) return attributes;


		std::string_view input(tag_str);
		std::string_view name_piece, full_value_piece, quoted_piece, unquoted_piece;

		while (RE2::FindAndConsume(&input, attr_regex,
									&name_piece, &full_value_piece,
									&quoted_piece, &unquoted_piece)) {
			std::string name(name_piece);

			std::string value = !quoted_piece.empty() || full_value_piece.starts_with('"')
									 ? std::string(quoted_piece)
									 : std::string(unquoted_piece);
			attributes[name] = value;
									}

		return attributes;
	}



	/**
	 *  Extracts the tag type (name) from a raw tag string.
	 *
	 * Scans @p tag_str character by character, collecting characters until a
	 * space is encountered or the string ends. Leading spaces are skipped.
	 *
	 * @param tag_str	The raw content inside the square brackets, without the
	 *					brackets themselves.
	 *
	 * @return			The tag type string.
	 */
	auto get_tag_type(const std::string& tag_str) -> std::string {
		std::string type;

		type.reserve(tag_str.size());

		for (const auto c : tag_str) {
			if (c == ' ') {
				if (type.empty()) continue;		// Skip leading spaces
				break;							// Stop at the first space after the type
			}

			type += c;
		}

		return type;
	}



	/**
	 *  Fully parses a raw tag string into a Tag object.
	 *
	 * Combines get_tag_type() and parse_attributes() to build a complete
	 * Tag, extracting both the tag's type and all of its key-value
	 * attributes from @p raw_tag
	 *
	 * @param raw_tag	The raw text inside the square brackets, without the
	 *					brackets themselves.
	 *
	 * @return			A populated Tag with type and attributes filled in.
	 */
	auto parse_tag(const std::string& raw_tag) -> Tag {
		Tag tag{};
		auto& type = tag.type;
		auto& attributes = tag.attributes;

		type = get_tag_type(raw_tag);
		attributes = parse_attributes(raw_tag);

		return tag;
	}



	// class Parser -------------------------------------



	/**
	 *  Feeds a single character into the parser's state machine.
	 *
	 * This is the main entry point for incremental parsing: the caller
	 * pushes one character at a time (see find_and_parse / find_and_parse_all)
	 * and the parser updates its internal state accordingly, appending any
	 * completed tokens to its internal pending queue (retrievable via
	 * drain()).
	 *
	 * @param c  The next character of input to process.
	 */
	auto Parser::feed(char c) -> void {
		const bool was_new_line = _flags.new_line;
		_flags.new_line = false;


		if (try_start_comment(c, was_new_line)) return;
		if (try_start_label(c, was_new_line)) return;
		if (try_start_speaker(c, was_new_line)) return;
		if (try_start_command(c, was_new_line)) return;
		if (handle_new_line(c)) {
			// '\n' is part of the JS syntax: do not discard it if we're in a script.
			if (_state != State::Script)
				return;
		}


		// Skip leading whitespace on a new line.
		// This check must come after 'handle_new_line', which handles '\n' explicitly.
		if (std::isspace(c) && was_new_line) {
			_flags.new_line = true;
			return;
		}

		if (_flags.comment) return;
		if (_flags.label) {
			_label += c;
			return;
		}
		if (_flags.speaker) {
			_speaker_name += c;
			return;
		}
		if (_flags.inline_tag) {
			_inline_tag += c;
			return;
		}

		if (try_open_tag(c)) return;

		dispatch(c);
	}



	/**
	 *  Retrieves and clears all tokens produced so far.
	 *
	 * @return  A vector of all Tag objects produced since the last drain().
	 */
	auto Parser::drain() -> std::vector<Tag> {
		std::vector<Tag> out;
		std::swap(out, _pending);
		return out;
	}



	/**
	 *  Checks for and handles the start of a comment region.
	 *
	 * A comment begins with ';' as the very first character of a line, and
	 * only while the parser is in State::Normal.
	 *
	 * @param c             The character currently being processed.
	 * @param was_new_line  Whether this character is the first one on a new line.
	 *
	 * @return              True if a comment was started, false otherwise.
	 */
	auto Parser::try_start_comment(char c, bool was_new_line) -> bool {
		// --- ";" starts a comment while in normal mode ---
		if (c == ';' && was_new_line) {
			_flags.comment = true;
			return true;
		}

		return false;
	}



	/**
	 * Checks for and handles the start of a label region.
	 *
	 * A label begins with '*' as the very first character of a line, and
	 * only while the parser is in State::Normal.
	 *
	 * @param c             The character currently being processed.
	 * @param was_new_line  Whether this character is the first one on a new line.
	 *
	 * @return              True if a label was started, false otherwise.
	 */
	auto Parser::try_start_label(char c, bool was_new_line) -> bool {
		// "*" starts a label while in normal mode ---
		if (c == '*' && was_new_line) {
			_flags.label = true;
			return true;
		}

		return false;
	}



	/**
	 *  Checks for and handles the start of a speaker-name region.
	 *
	 * A speaker name begins with '#' as the very first character of a line
	 *
	 * @param c             The character currently being processed.
	 * @param was_new_line  Whether this character is the first one on a new line.
	 *
	 * @return              True if a speaker-name region was started, false otherwise.
	 */
	auto Parser::try_start_speaker(char c, bool was_new_line) -> bool {
		// --- '#' starts the speaker-name ---
		if (c == '#' && was_new_line) {
			_flags.speaker = true;
			_speaker_name.clear(); // Prepare to read a fresh speaker name
			return true;
		}

		return false;
	}



	/**
	 *  Checks for and handles the start of a command region.
	 *
	 * A command begins with '@' as the first character of a line, and only
	 * while the parser is in State::Normal.
	 *
	 * @param c             The character currently being processed.
	 * @param was_new_line  Whether this character is the first one on a new line.
	 *
	 * @return              True if a command was started, false otherwise.
	 */
	auto Parser::try_start_command(char c, bool was_new_line) -> bool {
		// "@" starts a command while in normal mode ---
		if (c == '@' && was_new_line) {
			_flags.inline_tag = true;
			return true;
		}

		return false;
	}



	/**
	 *  Handles end-of-line bookkeeping.
	 *
	 * '\n' terminates a line. Speaker names and comments only span a single
	 * line, so their "in progress" flags are cleared unconditionally. If a
	 * label, speaker name, or command was being accumulated, it is flushed
	 * as a completed token into the pending queue and its buffer reset.
	 *
	 * @param c		The character currently being processed.
	 *
	 * @return		True if @p c was a newline, false otherwise.
	 */
	auto Parser::handle_new_line(char c) -> bool {
		if (c != '\n' ) return false;

		++_line;

		// Comments end on a new line
		_flags.comment = false;


		// ---------------------------------------------------
		// Labels end on a new line: send it as a special tag,
		// clear the buffer, and reset the flag.
		// ---------------------------------------------------
		if (_flags.label) {
			_pending.push_back( {
				"label",
				_line,
				{{"text", _label}}
			});

			_flags.label = false;
			_label.clear();
		}

		// ---------------------------------------------------
		// Speaker definition end on a new line: send it as
		// a special tag, clear the buffer, and reset the flag.
		// ---------------------------------------------------
		if (_flags.speaker) {
			_pending.push_back( {
			"speaker",
		_line,
			{{"text", _speaker_name}}});

			_flags.speaker = false;
			_speaker_name.clear();
		}

		// ---------------------------------------------------
		// In-line tags end on a new line: send it as a
		// special tag, clear the buffer, and reset the flag.
		// ---------------------------------------------------
		if (_flags.inline_tag) {
			_pending.push_back(parse_tag(_inline_tag));
			_inline_tag.clear();

			_flags.inline_tag = false;
		}

		_flags.new_line = true;

		return true;
	}



	/**
	 *  Checks for and handles the start of a `[...]` tag.
	 *
	 * '[' opens a new tag region regardless of current parser state.
	 *
	 * @param c  The character currently being processed.
	 *
	 * @return   True if a tag was opened, false otherwise.
	 */
	auto Parser::try_open_tag(char c) -> bool {
		// --- '[' opens a new tag ---
		if (c != '[' || _state == State::Tag) return false;

		_previous_state = _state;
		_state = State::Tag;

		if (_previous_state == State::Normal && !_text.empty()) {
			_pending.push_back( {
				"text_block",
				_line,
				{{"text", _text}, {"speaker", _speaker_name}}
			});

			_text.clear();
		}

		return true;
	}



	/**
	 *  Accumulates characters of a `[...]` tag, or closes it on ']'.
	 *
	 * @param c  The character currently being processed.
	 */
	auto Parser::accumulate_or_close_tag(char c) -> void {

		if (c == ']') {
			/// --------------------------------------------------------------
			/// Up to this point, "[...]" is parsed as a potential tag.
			/// If we are inside a script block, it might just be JavaScript
			/// syntax. Unless it's the explicit closing tag "endscript",
			/// treat it as JS code and append it back to the script buffer.
			///
			/// Maybe it could be made safer, but it's the best I could come
			/// up with without trying to actively parse the JavaScript code.
			/// --------------------------------------------------------------
			if (_previous_state == State::Script && helper::trimmed(_tag_str) != "endscript") {
				_script += '[';
				_script += _tag_str;
				_script += ']';

				_state = State::Script;
				_tag_str.clear();
				return;
			}


			Tag tag = parse_tag(_tag_str);
			tag.line = _line;

			update_state(tag);


			_pending.push_back(tag);

			_tag_str.clear();
		}

	//	--- Tag-closing character not detected: accumulate and continue ---
		else {
			_tag_str += c;
		}
	}


	auto Parser::update_state(const Tag& tag) -> void {
		// ------------------------------------------------------
		// If we are neither entering nor exiting a text block,
		// and neither entering nor exiting a JS script,
		// then we are exiting either an inline tag or a
		// non-text-related tag. In the first case, prev_state
		// would be State::Text; in the other, State::Normal.
		// ------------------------------------------------------
		if (tag.type == "iscript")
			_state = State::Script;


		else if ( tag.type == "endscript" ) {
			_state = State::Normal;

			_pending.push_back({"script", _line, {{"code", _script}}});
			_script.clear();
		}

		else
			_state = State::Normal;
	}



	/**
	 *  Routes a character to the appropriate accumulator based on state.
	 *
	 * Called from feed() once all special-character checks (comment, label,
	 * speaker, command, newline, tag-open) have been ruled out for this
	 * character.
	 *
	 * Every TyranoScript construct that is not a tag (e.g. free text, JS scripts)
	 * is parsed as a special tag. See @c ks::parser for more details
	 *
	 * @param c		The character currently being processed.
	 */
	auto Parser::dispatch(char c) -> void {
		switch (_state) {

		case State::Tag:
			accumulate_or_close_tag(c);
			break;

		// --- Normal character inside a script region ---
		case State::Script:
			_script+=c;
			break;

		// --- Normal character inside a text-collection region ---
		case State::Normal:
			_text+=c;
			break;
		}
	}



	// --------------------------------------------------



	/**
	 *  Lazily parses @p contents and yields tags as they complete.
	 *
	 * Feeds the input one character at a time into a fresh Parser instance
	 * and, after each character, drains and yields any newly completed
	 * tokens.
	 *
	 * Every TyranoScript construct that is not a tag (e.g. free text, JS scripts)
	 * is parsed as a special tag. See @c ks::parser for more details
	 *
	 * @param contents  The full source text to parse.
	 *
	 * @return          A generator that yields each Tag as soon as it is
	 *                  recognized.
	 */
	auto find_and_parse(const std::string& contents) -> std::generator<Tag> {
		Parser parser;
		for (const auto c : contents) {
			parser.feed(c);

			for (auto& tag : parser.drain())
				co_yield tag;
		}
	}



	/**
	 *  Eagerly parses @p contents and returns all resulting tags.
	 *
	 * Feeds the entire input string through a fresh Parser instance
	 * and then drains all accumulated tokens at once.
	 * Prefer find_and_parse() if you need results incrementally.
	 *
	 * Every TyranoScript construct that is not a tag (e.g. free text, JS scripts)
	 * is parsed as a special tag. See @c ks::parser for more details
	 *
	 * @param contents  The full source text to parse.
	 *
	 * @return          All Tag objects produced while parsing the entire input.
	 */
	auto find_and_parse_all(const std::string& contents) -> std::vector<Tag> {
		Parser parser;
		for (const auto& c : contents) {
			parser.feed(c);
		}

		return parser.drain();
	}
} // raptor::parser
