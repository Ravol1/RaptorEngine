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

		Layer(int z_base = -2) : z_back_(z_base), z_fore(z_base + 1) {}
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


		auto fore() -> const std::vector<Object>& { return fore_; }
		auto back() -> const std::vector<Object>& { return back_; }


		void add_object(
			const Object& object,
			LayerPage page,
			Interpreter* interpreter
		){
			using namespace game_event;

			auto& objs = get_page(page);
			const auto& settings = get_options(page);

			objs.push_back(object);

			interpreter->push_event(
				GameEventType::LoadObject,
				LoadObjectData{
					object.id,
					object.texture_path,
					page == LayerPage::Back ? z_back_ : z_fore,
					static_cast<float>(object.x), static_cast<float>(object.y),
					static_cast<float>(object.width), static_cast<float>(object.height),
					settings.visible,
				}
			);
		}

		void virtual swap_pages() { std::swap(fore_settings_, back_settings_); }



	protected:
		const int z_back_ = -2;
		const int z_fore = -1;

		LayerOptions fore_settings_{};
		LayerOptions back_settings_{};

		std::vector<Object> fore_;
		std::vector<Object> back_;



		auto get_options(LayerPage page) -> LayerOptions& {
			return page == LayerPage::Back ? back_settings_ : fore_settings_;
		}

		auto get_page(LayerPage page) -> std::vector<Object>& {
			return page == LayerPage::Back ? back_ : fore_;
		}

		LayerOptions fore_{};
		LayerOptions back_{};
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_LAYER_H
