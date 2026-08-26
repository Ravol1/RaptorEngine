#ifndef RAPTOR_BASELAYER_H
#define RAPTOR_BASELAYER_H
#include "Layer.h"
#include "interpreter/Interpreter.h"
#include "interpreter/modules/graphic/detail/transitions/TransitionManager.h"

namespace raptor::interpreter::graphic::detail {
	class BaseLayer : public Layer {
	public:
		static constexpr int BASE = -9999;

		BaseLayer() : Layer(BASE) {}


		void change_bg(
			const std::string& storage,
			const Transition& transition,
			Interpreter* interpreter,
			TransitionManager* transition_manager
		) {

			Object obj{new_id(), autosize, autosize, 0,0, storage};
			add_object(obj, LayerPage::Back, interpreter);

			transition_manager->add_transition(transition, interpreter);
		}
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_BASELAYER_H
