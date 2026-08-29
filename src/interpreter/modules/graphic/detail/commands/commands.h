#ifndef RAPTOR_LAYOPT_H
#define RAPTOR_LAYOPT_H

#include "interpreter/Interpreter.h"
#include "interpreter/modules/graphic/detail/layer/LayerRegistry.h"
#include "interpreter/modules/graphic/detail/layer/CurrentLayer.h"
#include "interpreter/modules/graphic/detail/transitions/TransitionManager.h"
#include "tag/tag.h"



namespace raptor::interpreter::graphic::detail {
	/** @brief Set layer options. */
	auto layopt(const Tag& tag, LayerRegistry& layer_registry, const CurrentLayer& current_layer) -> void;

	/** @brief	Set options for the message layer. */
	auto position(const Tag& tag, LayerRegistry& layer_registry, const CurrentLayer& current_layer) -> void;

	/** @brief Specifies the game title. */
	auto title(const Tag& tag, Interpreter* interpreter) -> void;

	/** @brief Change the background with a transition. */
	auto bg(
		const Tag& tag,
		LayerRegistry* layer_registry,
		TransitionManager* transition_manager,
		Interpreter* interpreter
	) -> void;

	/** @brief Clear all text and buttons from all message layers. */
	auto cm(LayerRegistry& layer_registry, Interpreter* interpreter) -> void;
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_LAYOPT_H
