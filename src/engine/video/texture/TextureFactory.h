//
// Created by matteo on 7/9/26.
//

#ifndef RAPTOR_TEXTUREFACTORY_H
#define RAPTOR_TEXTUREFACTORY_H
#include <filesystem>
#include <ranges>
#include <unordered_map>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>


namespace raptor::engine::video::texture {
	class TextureFactory {
	public:
		TextureFactory(SDL_Renderer* renderer) {
			constexpr int WHOLE_SIZE = 64;
			constexpr int CHECKER_SIZE = WHOLE_SIZE/2;


			SDL_Surface *surface = SDL_CreateSurface(WHOLE_SIZE, WHOLE_SIZE, SDL_PIXELFORMAT_RGBA8888);


			auto details = SDL_GetPixelFormatDetails(surface->format);
			uint32_t magenta = SDL_MapRGBA(details, nullptr, 255, 0, 255, 255);
			uint32_t black   = SDL_MapRGBA(details, nullptr, 0, 0, 0, 255);

			SDL_Rect topLeft     = { 0,			  0,			   CHECKER_SIZE, CHECKER_SIZE };
			SDL_Rect topRight    = { CHECKER_SIZE, 0,			   CHECKER_SIZE, CHECKER_SIZE };
			SDL_Rect bottomLeft  = { 0,			  CHECKER_SIZE, CHECKER_SIZE, CHECKER_SIZE };
			SDL_Rect bottomRight = { CHECKER_SIZE, CHECKER_SIZE, CHECKER_SIZE, CHECKER_SIZE };

			SDL_FillSurfaceRect(surface, &topLeft, magenta);
			SDL_FillSurfaceRect(surface, &topRight, black);
			SDL_FillSurfaceRect(surface, &bottomLeft, black);
			SDL_FillSurfaceRect(surface, &bottomRight, magenta);


			fallback_texture_ = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_DestroySurface(surface);
		}



		auto from_file(const std::string& path, SDL_Renderer* renderer) -> SDL_Texture* {
			if (!texture_map_.contains(path)) {
				if (!std::filesystem::exists(path)) return fallback_texture_;

				SDL_Surface* surface = IMG_Load(path.data());
				if (!surface) return fallback_texture_;

				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_DestroySurface(surface);
				if (!texture) return fallback_texture_;

				texture_map_[path] = texture;
				return texture;
			}

			return texture_map_[path];
		}

		auto clear() -> void {
			for (auto& texture : texture_map_ | std::views::values) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
			}

			texture_map_.clear();
		}
	private:
		SDL_Texture* fallback_texture_ = nullptr;
		std::unordered_map<std::string, SDL_Texture*> texture_map_;
	};
}



#endif //RAPTOR_TEXTUREFACTORY_H
