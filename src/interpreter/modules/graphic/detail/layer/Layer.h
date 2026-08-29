#ifndef RAPTOR_LAYER_H
#define RAPTOR_LAYER_H
#include <cstdint>
#include <utility>

#include "interpreter/Interpreter.h"
#include "interpreter/modules/graphic/detail/object/IObject.h"
#include "events/game_event.h"


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


		constexpr static int autosize = -1;


		Layer(int z_base = -2) : z_back_(z_base), z_fore(z_base + 1) {}
		virtual ~Layer() = default;


		auto set_visible(bool visible, LayerPage page) {
			set_property<&LayerOptions::visible>(visible, page);
		}
		auto set_x(float x, LayerPage page) {
			set_property<&LayerOptions::x>(x, page);
		}
		auto set_y(float y, LayerPage page) {
			set_property<&LayerOptions::y>(y, page);
		}
		auto set_alpha(uint8_t alpha, LayerPage page) {
			set_property<&LayerOptions::alpha>(alpha, page);
		}


		auto fore() -> const std::vector<std::unique_ptr<IObject>>& { return fore_; }
		auto back() -> const std::vector<std::unique_ptr<IObject>>& { return back_; }


		void add_object(
			std::unique_ptr<IObject> object,
			LayerPage page,
			Interpreter* interpreter
		){
			using namespace game_event;

			auto& objs = get_page(page);
			const auto& settings = get_options(page);

			object->send_create(interpreter);

			objs.push_back(std::move(object));
		}

		void clear_objects(LayerPage page, Interpreter* interpreter) {
			auto& objs = get_page(page);
			for (auto& obj : objs) {
				obj->send_destroy(interpreter);
			}

			objs.clear();
		}

		void virtual swap_pages() { std::swap(fore_settings_, back_settings_); }



	protected:
		const int z_back_ = -2;
		const int z_fore = -1;

		LayerOptions fore_settings_{};
		LayerOptions back_settings_{};

		std::vector<std::unique_ptr<IObject>> fore_;
		std::vector<std::unique_ptr<IObject>> back_;



		auto get_options(LayerPage page) -> LayerOptions& {
			return page == LayerPage::Back ? back_settings_ : fore_settings_;
		}

		auto get_page(LayerPage page) -> std::vector<std::unique_ptr<IObject>>& {
			return page == LayerPage::Back ? back_ : fore_;
		}


		template <auto MemberPtr, typename T>
		void set_property(T&& value, LayerPage page) {
			auto& target = get_options(page);
			target.*MemberPtr = value;
		}

	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_LAYER_H
