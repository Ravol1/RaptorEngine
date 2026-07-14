#include "commands.h"

#include <stdexcept>

#include "helper/type_conversions.h"

namespace raptor::interpreter::character::detail {
	namespace {

		auto apply_size(CharacterInfo& info, const Tag& tag) -> void {
			if (auto width = try_parse_numeric<unsigned int>(tag.get_attribute("width"))) {
				info.width = static_cast<float>(*width);
			} else {
				info.width = autosize;
			}

			if (auto height = try_parse_numeric<unsigned int>(tag.get_attribute("height"));
				height == true) {

				info.height = static_cast<float>(*height);
			} else {
				info.height = autosize;
			}
		}

		auto apply_reflect(CharacterInfo& info, const Tag& tag) -> void {
			if (auto reflect = try_parse_bool(tag.get_attribute("reflect")))
				info.reflect = *reflect;
		}

		auto apply_color(CharacterInfo& info, const Tag& tag) -> void {
			const std::string color_str = tag.get_attribute("color");


			ColorRGB color{};
			if (color_str.starts_with("x0")) color = ColorRGB::from_string(
				std::string_view{
					color_str
				}.substr(2)
			);

			info.name_color = color;
		}

		auto apply_jname(CharacterInfo& info, const Tag& tag) {
			std::string jname = tag.get_attribute("jname");
			if (jname.empty()) jname = tag.get_attribute("name");

			info.display_name = jname;
		}


	}

	auto chara_new(const Tag& tag, CharacterList& characters) -> void {
		auto name = tag.get_attribute("name");
		auto storage = tag.get_attribute("storage");

		if (name.empty())
			throw std::runtime_error("Missing required attribute 'name'");

		if (storage.empty())
			throw std::runtime_error("Missing required attribute 'storage'");


		CharacterInfo newInfo{};

		newInfo.storage = storage;
		apply_size(newInfo, tag);
		apply_reflect(newInfo, tag);
		apply_color(newInfo, tag);
		apply_jname(newInfo, tag);

		characters[name] = newInfo;
	}

} // raptor::interpreter::character::detail