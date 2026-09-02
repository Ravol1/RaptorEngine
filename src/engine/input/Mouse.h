#ifndef RAPTOR_MOUSE_TEST_H
#define RAPTOR_MOUSE_TEST_H



namespace raptor::engine::input {
	class Mouse {
	public:
		[[nodiscard]]
		auto get_x() const -> float {return x;}

		[[nodiscard]]
		auto get_y() const -> float {return y;}

		[[nodiscard]] auto left_pressed() const -> bool {return !pre_left_state && left_state;}
		[[nodiscard]] auto right_pressed() const -> bool {return !pre_right_state && right_state;}
		[[nodiscard]] auto middle_pressed() const -> bool {return !pre_middle_state && middle_state;}

		[[nodiscard]] auto left_released() const -> bool {return pre_left_state && !left_state;}
		[[nodiscard]] auto right_released() const -> bool {return pre_right_state && !right_state;}
		[[nodiscard]] auto middle_released() const -> bool {return pre_middle_state && !middle_state;}


		void update();

	private:
		float x{0}, y{0};

		bool pre_left_state = false, left_state = false;
		bool pre_right_state = false, right_state = false;
		bool pre_middle_state = false, middle_state = false;
	};
}// raptor::engine::input

#endif //RAPTOR_MOUSE_TEST_H
