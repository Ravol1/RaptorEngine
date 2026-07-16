#ifndef RAPTOR_STATUS_H
#define RAPTOR_STATUS_H

namespace raptor::interpreter::input::detail {
	class Status {
	public:
		auto set_keyconfig_on(bool value) -> void { keyconfig_on_ = value; }

	private:
		bool keyconfig_on_= false;
	};
} // raptor::interpreter::input::detail

#endif //RAPTOR_STATUS_H
