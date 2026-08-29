#ifndef RAPTOR_OBJECT_H
#define RAPTOR_OBJECT_H
#include <cstdint>
#include <string>
#include <utility>

#include "helper/id.h"

namespace raptor::interpreter::graphic::detail {
	class IObject {
	public:
		enum class Type {
			Image,
			Button,
		};


		virtual ~IObject() = default;


		virtual void set_x(int value) = 0;
		virtual void set_y(int value) = 0;
		virtual void set_z(int value) = 0;
		virtual auto get_x() -> int = 0;
		virtual auto get_y() -> int = 0;
		virtual auto get_z() -> int = 0;
		virtual auto get_type() -> Type = 0;
		virtual auto get_id() -> uint64_t = 0;


		virtual void send_destroy(Interpreter* interpreter) = 0;
		virtual void send_create(Interpreter* interpreter) = 0;

	};


	class Image : public IObject {
	public:
		Image(int x, int y, int z, int width, int height, std::string& texture_path) :
		id_(new_id()), x_(x), y_(y), z_(z), width_(width), height_(height), texture_path_(texture_path) {}


		void set_x(int value) override {x_ = value;}
		void set_y(int value) override {y_ = value;}
		void set_z(int value) override {z_ = value;}
		auto get_x() -> int override {return x_;}
		auto get_y() -> int override {return y_;}
		auto get_z() -> int override {return z_;}

		auto get_type() -> Type override {return Type::Image;}

		auto get_id() -> uint64_t override {return id_;}


		void send_destroy(Interpreter* interpreter) override {
			interpreter->push_event(game_event::GameEventType::DeleteObject, game_event::DeleteObjectData(id_));
		}

		void send_create(Interpreter* interpreter) override {
			interpreter->push_event(
				game_event::GameEventType::LoadObject,
				game_event::LoadObjectData{.id = id_, .texture_path = texture_path_,
				.z = z_,
				.x = static_cast<float>(x_), .y = static_cast<float>(y_),
				.width = static_cast<float>(width_), .height = static_cast<float>(height_),
				.visible = true}
			);
		}


		uint64_t id_;
		int x_, y_;
		int z_;
		int width_, height_;

		std::string texture_path_;
	};

	class Button : public IObject {
	public:
		Button(int x, int y, int width, int height, std::string  texture_path) :
		id_(new_id()), x_(x), y_(y), width_(width), height_(height), texture_path_(std::move(texture_path)) {}


		void set_x(int value) override {x_ = value;}
		void set_y(int value) override {y_ = value;}
		auto get_x() -> int override {return x_;}
		auto get_y() -> int override {return y_;}
		auto get_type() -> Type override {return Type::Image;}


	private:
		uint64_t id_;
		int x_, y_;
		int width_, height_;

		std::string texture_path_;
	};
} // raptor::interpreter::graphic::detail

#endif //RAPTOR_OBJECT_H
