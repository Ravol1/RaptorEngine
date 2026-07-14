#ifndef RAPTOR_LAYERREGISTRY_H
#define RAPTOR_LAYERREGISTRY_H
#include <memory>
#include <unordered_map>
#include <array>
#include <string_view>
#include <algorithm>

#include "layer.h"


namespace raptor::interpreter::graphic::detail {
	class LayerRegistry {
	public:
		LayerRegistry();
		~LayerRegistry();
		
		auto get_or_create(const std::string& name) -> Layer*;
	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
	};
} // raptor::interpreter::graphic

#endif //RAPTOR_LAYERREGISTRY_H
