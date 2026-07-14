#ifndef RAPTOR_TYPE_CONVERSIONS_H
#define RAPTOR_TYPE_CONVERSIONS_H

#include <string>
#include <charconv>
#include <optional>

namespace raptor {
	inline auto try_parse_bool(const std::string& value, bool def = false) -> std::optional<bool> {
		if (value == "true") return true;
		if (value == "false") return false;

		return std::nullopt;
	}


	template <typename T>
	auto try_parse_numeric(const std::string& value) -> std::optional<T> {
		static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type (integer or float).");


		T result;
		const char* first = value.data();
		const char* last = value.data() + value.size();


		std::from_chars_result res{};

		if constexpr (std::is_floating_point_v<T>)
			res = std::from_chars(first, last, result, std::chars_format::general);

		else
			res = std::from_chars(first, last, result);



		// Fail if from_chars returned an error (couldn't parse a number),
		// or if it stopped before consuming the entire string
		// (i.e. there were trailing non-numeric characters).
		if (res.ec != std::errc{} || res.ptr != value.data() + value.size()) return std::nullopt;

		return result;
	}

}

#endif //RAPTOR_TYPE_CONVERSIONS_H
