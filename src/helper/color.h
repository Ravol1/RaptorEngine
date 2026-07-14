#ifndef RAPTOR_COLOR_H
#define RAPTOR_COLOR_H
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <string_view>
#include <cctype>


namespace raptor {
	struct ColorRGB {
		uint8_t R;
		uint8_t G;
		uint8_t B;


		/**
		 * @brief Creates a ColorRGB object from the corresponding hexadecimal code
		 *
		 * @param code	The hexadecimal color code without any prefix
		 *
		 * @return		A ColorRGB object representing the input string.
		 *				If the input string is invalid, return the color back.
		 */
		static auto from_string(std::string_view code) -> ColorRGB;
	};

	struct ColorRGBA {
		uint8_t R;
		uint8_t G;
		uint8_t B;
		uint8_t A;
	};
}

#endif //RAPTOR_COLOR_H
