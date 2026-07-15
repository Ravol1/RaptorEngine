#ifndef RAPTOR_TAG_H
#define RAPTOR_TAG_H
#include <string>
#include <unordered_map>

namespace raptor {
	/**
	 * @brief Represents a fully parsed KAG tag.
	 *
	 * A tag has a type (e.g. "image") and zero or more
	 * key-value attributes (e.g. key="value").
	 */
	struct Tag {
		std::string type;		// Tag name / identifier
		size_t line;			// Position in the file where the tag is located
		std::unordered_map<std::string, std::string> attributes;	// Parsed key-value attribute pairs

		auto has_attribute(const std::string& name) const -> bool {return attributes.contains(name);}
		auto get_attribute(const std::string& name) const -> std::string {
			if (!attributes.contains(name)) {
				return "";
			}

			return attributes.at(name);
		}
		auto set_attribute(std::string name, std::string value) {
			attributes[std::move(name)] = std::move(value);
		}
	};
} // raptor

#endif // RAPTOR_TAG_H
