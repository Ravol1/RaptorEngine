#ifndef RAPTOR_CURRENTLAYER_H
#define RAPTOR_CURRENTLAYER_H

#include "interpreter/modules/graphic/detail/layer/Layer.h"

namespace raptor::interpreter::graphic::detail {
	struct CurrentLayer {
		using LayerPage = Layer::LayerPage;

		Layer* layer = nullptr;
		LayerPage page = LayerPage::Fore;
	};
}

#endif //RAPTOR_CURRENTLAYER_H
