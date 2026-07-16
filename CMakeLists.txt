cmake_minimum_required(VERSION 4.2)
project(raptor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRECTORIES ./)

add_executable(raptor 
		src/main.cpp
        src/parser/parser.cpp
        src/parser/parser.h
        src/tag/tag.h
		src/helper/string.h
        src/interpreter/Interpreter.cpp
        src/interpreter/Interpreter.h
        src/engine/video/ScreenObject.cpp
        src/engine/video/ScreenObject.h
        src/engine/video/sprite/Sprite.cpp
        src/engine/video/sprite/Sprite.h
        src/engine/video/sprite/factory/factory.h
        src/engine/video/texture/TextureFactory.h
        src/events/game_event.cpp
        src/events/game_event.h
		src/interpreter/modules/graphic/detail/layer/layer.h
		src/interpreter/modules/graphic/detail/layer/LayerRegistry.h
		src/interpreter/modules/graphic/detail/layer/LayerRegistry.cpp
		src/interpreter/modules/graphic/Graphic.cpp
		src/interpreter/modules/graphic/Graphic.h
		src/interpreter/modules/graphic/detail/commands/layopt.cpp
		src/interpreter/modules/graphic/detail/commands/commands.h
		src/interpreter/modules/graphic/detail/layer/CurrentLayer.h
		src/interpreter/modules/graphic/detail/layer/FreeLayer.h
		src/interpreter/modules/character/Character.cpp
		src/interpreter/modules/character/Character.h
		src/interpreter/modules/character/detail/CharacterInfo.h
		src/interpreter/modules/Module.h
		src/helper/color.h
		src/interpreter/modules/character/detail/commands/chara_new.cpp
		src/interpreter/modules/character/detail/commands/commands.h
		src/interpreter/modules/graphic/detail/commands/position.cpp
        src/interpreter/modules/graphic/detail/layer/helpers.h
		src/helper/type_conversions.h
		src/helper/color.cpp
		src/interpreter/modules/graphic/detail/commands/title.cpp
		src/Application/Application.cpp
		src/Application/Application.h
		src/engine/video/GraphicsLoop.cpp
		src/engine/video/GraphicsLoop.h
		src/engine/diagnostic/log_fps.h
		src/engine/diagnostic/log_fps.cpp
		src/interpreter/modules/input/Input.cpp
		src/interpreter/modules/input/Input.h
		src/interpreter/modules/input/detail/Status.h
		src/interpreter/modules/input/detail/commands/commands.h
		src/interpreter/modules/input/detail/commands/keyconfig.cpp
)

target_include_directories(raptor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(re2 REQUIRED)
target_link_libraries(raptor PRIVATE re2::re2)

find_package(lexbor REQUIRED)
target_link_libraries(raptor PRIVATE lexbor::lexbor)

find_package(SDL3 REQUIRED)
target_link_libraries(raptor PRIVATE SDL3::SDL3)

find_package(SDL3_image REQUIRED)
target_link_libraries(raptor PRIVATE SDL3_image::SDL3_image)
