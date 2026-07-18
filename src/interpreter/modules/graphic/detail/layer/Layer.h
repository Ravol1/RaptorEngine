#ifndef RAPTOR_LAYER_H
#define RAPTOR_LAYER_H
#include <cstdint>
#include <utility>


namespace raptor::interpreter::graphic::detail {
	class Layer {
	public:

		enum class LayerPage {
			Fore,
			Back,
			Auto,
		};

		struct LayerOptions {
			bool visible;
			float x;
			float y;
			uint8_t alpha;

		};

		virtual ~Layer() = default;

		auto set_visible(bool visible, LayerPage page) {
			auto& target = get_options(page);
			target.visible = visible;
		}
		auto set_x(float x, LayerPage page) {
			auto& target = get_options(page);
			target.x = x;
		}
		auto set_y(float y, LayerPage page) {
			auto& target = get_options(page);
			target.y = y;
		}
		auto set_alpha(uint8_t alpha, LayerPage page) {
			auto& target = get_options(page);
			target.alpha = alpha;
		}


		void virtual swap_pages() { std::swap(fore_, back_); }



	protected:
		auto get_options(LayerPage page) -> LayerOptions& {
			return page == LayerPage::Back ? back_ : fore_;
		}

		LayerOptions fore_{};
		LayerOptions back_{};
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_LAYER_H
