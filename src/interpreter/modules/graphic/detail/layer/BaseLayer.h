//
// Created by matteo on 7/16/26.
//

#ifndef RAPTOR_BASELAYER_H
#define RAPTOR_BASELAYER_H
#include "Layer.h"

namespace raptor::interpreter::graphic::detail {
	class BaseLayer : public Layer {
	public:
		static constexpr int BASE = -9999;

		BaseLayer() : Layer(BASE) {}

	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_BASELAYER_H
