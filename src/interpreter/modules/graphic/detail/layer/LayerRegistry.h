#ifndef RAPTOR_LAYERREGISTRY_H
#define RAPTOR_LAYERREGISTRY_H
#include <memory>

#include "Layer.h"
#include "MessageLayer.h"



namespace raptor::interpreter::graphic::detail {
	class LayerRegistry {
	public:

		LayerRegistry();
		~LayerRegistry();

		auto layer_exists(const std::string& name) -> bool;
		
		auto get_or_create(const std::string& name) -> Layer*;
	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
	};
} // raptor::interpreter::graphic

#endif //RAPTOR_LAYERREGISTRY_H
