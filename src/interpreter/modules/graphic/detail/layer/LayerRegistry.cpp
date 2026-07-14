#include "LayerRegistry.h"
#include "interpreter/modules/graphic/detail/layer/layer.h"


namespace raptor::interpreter::graphic::detail {
	namespace {
		auto is_valid_layer_name(const std::string_view name) -> bool {
			static constexpr std::array<std::string_view, 4> whitelist = {
				"base",
				"fix",
			};


			if (std::find(whitelist.begin(), whitelist.end(), name) != whitelist.end())
				return true;


			auto is_numeric = [](std::string_view sv) -> bool {
				return !sv.empty() && std::ranges::all_of(sv, [](unsigned char c)->int { return std::isdigit(c); });
			};

			// The string "message" followed by a number is a valid name
			if (name.starts_with("message")) {
				return is_numeric(name.substr(7));
			}

			// Numbers are valid names
			return is_numeric(name);
		}
	};


	class LayerRegistry::Impl {
	public:
		std::unordered_map<std::string, Layer> layers_;
	};

	LayerRegistry::LayerRegistry() : impl_(std::make_unique<Impl>()){}
	LayerRegistry::~LayerRegistry() = default;



	auto LayerRegistry::get_or_create(const std::string& name) -> Layer* {
		if (!is_valid_layer_name(name)) return nullptr;

		if (!impl_->layers_.contains(name) && name.starts_with("message"))
			impl_->layers_[name].create_msg_component();

		return &impl_->layers_[name];
	}
} // raptor::interpreter::graphic::layer