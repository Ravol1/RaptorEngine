#ifndef RAPTOR_LAYOPT_H
#define RAPTOR_LAYOPT_H

#include "interpreter/Interpreter.h"
#include "interpreter/modules/graphic/detail/layer/LayerRegistry.h"
#include "interpreter/modules/graphic/detail/layer/CurrentLayer.h"
#include "tag/tag.h"



namespace raptor::interpreter::graphic::detail {
	/** @brief Set layer options. */
	auto layopt(const Tag& tag, LayerRegistry& layer_registry, const CurrentLayer& current_layer) -> void;

	/**
	 * @brief	Set various options for the message layer.
	 *			Any parameters that are omitted will not change their attributes.
	 */
	auto position(const Tag& tag, LayerRegistry& layer_registry, const CurrentLayer& current_layer) -> void;


	/** @brief Specifies the game title*/
	auto title(const Tag& tag, Interpreter* interpreter) -> void;
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_LAYOPT_H
