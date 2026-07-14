#include "color.h"

namespace raptor {
	auto ColorRGB::from_string(std::string_view code) -> ColorRGB {
		ColorRGB color{};


		// Is the substring after 0x composed of exactly 6 hexadecimal digits?
		if (code.empty() ||
			code.length() != 6 ||
			!std::ranges::all_of(code, [](unsigned char c) { return std::isxdigit(c); })) {

			return color;
		}



		std::string_view RStr = code.substr(0, 2);
		std::string_view GStr = code.substr(2, 2);
		std::string_view BStr = code.substr(4, 2);

		std::from_chars(RStr.data(), RStr.data() + RStr.size(), color.R, 16);
		std::from_chars(GStr.data(), GStr.data() + GStr.size(), color.G, 16);
		std::from_chars(BStr.data(), BStr.data() + BStr.size(), color.B, 16);

		return color;
	}
}