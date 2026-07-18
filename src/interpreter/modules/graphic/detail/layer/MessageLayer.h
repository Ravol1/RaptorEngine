#ifndef RAPTOR_MESSAGELAYER_H
#define RAPTOR_MESSAGELAYER_H
#include <string>

#include "Layer.h"
#include "helper/color.h"


namespace raptor::interpreter::graphic::detail {
	class MessageLayer : public Layer {
	public:
		struct MessageLayerComponent {
			float width;
			float height;

			std::string frame;

			ColorRGB color;
			ColorRGB border_color;

			unsigned int border_thickness;

			std::uint8_t alpha;				/// Opacity of THE MESSAGE WINDOW, different from the one in LayerOptions.

			unsigned int margin_left;
			unsigned int margin_top;
			unsigned int margin_right;
			unsigned int margin_bottom;
			unsigned int radius;			/// Roundness of the message window corners

			bool vertical;			/// Use vertical writing mode
			// TODO: CSS GRADIENT
		};


		auto set_msg_width(float width, LayerPage page){
			with_msg_component(page, [width](auto& msg) {
				msg.width = width;
			});
		}

		auto set_msg_height(float height, LayerPage page) {
			with_msg_component(page, [height](auto& msg) {
				msg.height = height;
			});
		}

		auto set_msg_frame(const std::string& frame, LayerPage page) {
			with_msg_component(page, [frame](auto& msg) {
				msg.frame = frame;
			});
		}

		auto set_msg_color(const ColorRGB& color, LayerPage page) {
			with_msg_component(page, [color](auto& msg) {
				msg.color = color;
			});
		}

		auto set_msg_border_color(const ColorRGB& color, LayerPage page) {
			with_msg_component(page, [color](auto& msg) {
				msg.border_color = color;
			});
		}

		auto set_msg_border_thickness(unsigned int border_thickness, LayerPage page) {
			with_msg_component(page, [border_thickness](auto& msg) {
				msg.border_thickness = border_thickness;
			});
		}

		auto set_msg_alpha(uint8_t alpha, LayerPage page) {
			with_msg_component(page, [alpha](auto& msg) {
				msg.alpha = alpha;
			});
		}

		auto set_msg_margin_left(unsigned int margin_left, LayerPage page) {
			with_msg_component(page, [margin_left](auto& msg) {
				msg.margin_left = margin_left;
			});
		}

		auto set_msg_margin_top(unsigned int margin_top, LayerPage page) {
			with_msg_component(page, [margin_top](auto& msg) {
				msg.margin_top = margin_top;
			});
		}

		auto set_msg_margin_right(unsigned int margin_right, LayerPage page) {
			with_msg_component(page, [margin_right](auto& msg) {
				msg.margin_right = margin_right;
			});
		}

		auto set_msg_margin_bottom(unsigned int margin_bottom, LayerPage page) {
			with_msg_component(page, [margin_bottom](auto& msg) {
				msg.margin_bottom = margin_bottom;
			});
		}

		auto set_msg_radius(unsigned int radius, LayerPage page) {
			with_msg_component(page, [radius](auto& msg) {
				msg.radius = radius;
			});
		}

		auto set_msg_vertical(bool vertical, LayerPage page) {
			with_msg_component(page, [vertical](auto& msg) {
				msg.vertical = vertical;
			});
		}


		void swap_pages() override {
			this->Layer::swap_pages();
			std::swap(fore_msg_component_, back_msg_component_);
		}



	private:
		MessageLayerComponent fore_msg_component_{};
		MessageLayerComponent back_msg_component_{};



		auto get_msg_component(LayerPage page) -> MessageLayerComponent& {
			return page == LayerPage::Back ? back_msg_component_ : fore_msg_component_;
		}


		template <typename Func>
		void with_msg_component(LayerPage page, Func&& action) {
			auto& target = get_msg_component(page);
				action(target);

		}
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_MESSAGELAYER_H
