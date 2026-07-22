#ifndef RAPTOR_FOREGROUNDLAYER_H
#define RAPTOR_FOREGROUNDLAYER_H
#include "Layer.h"

namespace raptor::interpreter::graphic::detail {
	class ForegroundLayer : public Layer {
	public:
		static constexpr int BASE = 0;
		ForegroundLayer(int index) : Layer(BASE + 2*index) {}
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_FOREGROUNDLAYER_H
