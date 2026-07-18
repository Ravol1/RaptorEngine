#include "LayerRegistry.h"

#include <unordered_map>
#include <array>

#include "BaseLayer.h"
#include "ForegroundLayer.h"
#include "MessageLayer.h"
#include "FixLayer.h"


namespace raptor::interpreter::graphic::detail {
	namespace {
		enum class LayerType {
			Message,
			Foreground,
			Base,
			Fix,
		};
		auto is_valid_layer_name(const std::string_view name) -> std::optional<LayerType> {
			if (name == "base") return LayerType::Base;
			if (name == "fix") return LayerType::Fix;


			auto is_numeric = [](std::string_view sv) -> bool {
				return !sv.empty() && std::ranges::all_of(sv,
					[](unsigned char c)->int { return std::isdigit(c); }
				);
			};

			// The string "message" followed by a number is a valid name
			if (name.starts_with("message")) {
				if (is_numeric(name.substr(7)))
					return LayerType::Message;

				return std::nullopt;
			}

			// Numbers are valid names
			if (is_numeric(name))
				return LayerType::Foreground;

			return std::nullopt;
		}
	};


	class LayerRegistry::Impl {
	public:
		std::unordered_map<std::string, std::unique_ptr<Layer>> layers_;

		void create_layer(const std::string& name, LayerType type) {
			std::unique_ptr<Layer> layer;
			switch (type) {
			case LayerType::Base:
				layer = std::make_unique<BaseLayer>();
				break;

			case LayerType::Fix:
				layer = std::make_unique<FixLayer>();
				break;

			case LayerType::Message:
				layer = std::make_unique<MessageLayer>();
				break;

			case LayerType::Foreground:
				layer = std::make_unique<ForegroundLayer>();
				break;
			}

			layers_.emplace(name, std::move(layer));
		}
	};

	LayerRegistry::LayerRegistry() : impl_(std::make_unique<Impl>()){}
	LayerRegistry::~LayerRegistry() = default;

	auto LayerRegistry::layer_exists(const std::string& name) -> bool {
		return impl_->layers_.contains(name);
	}


	auto LayerRegistry::get_or_create(const std::string& name) -> Layer* {
		if (auto type = is_valid_layer_name(name)) {
			if (!layer_exists(name))
				impl_->create_layer(name, *type);

			return impl_->layers_[name].get();
		}

		return nullptr;
	}
} // raptor::interpreter::graphic::layer