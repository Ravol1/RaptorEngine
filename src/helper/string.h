#ifndef RAPTOR_STRING_H
#define RAPTOR_STRING_H
#include <string>

namespace helper {
	inline auto ltrim(std::string& str) -> void {
		while (!str.empty() && std::isspace(str.front())) {
			str.erase(0,1);
		}
	}



	inline auto rtrim(std::string& str) -> void {
		while (!str.empty() && std::isspace(str.back())) {
			str.pop_back();
		}
	}



	inline auto trim(std::string& str) -> void {
		ltrim(str);
		rtrim(str);
	}


	inline auto trimmed(const std::string& str) -> std::string {
		std::string out = str;
		trim(out);
		return out;
	}

}

#endif //RAPTOR_STRING_H
