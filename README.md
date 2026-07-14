# RaptorEngine
A performance-oriented modern C++ engine for visual novels made with on KAG and TyranoScript. The project is an independent and open source development that aims to be light-weight and multi-platform.

---

**Project Status: Early Development / WIP**  
RaptorEngine is currently in its early stages of developement: the core parser and interpreter are fully functional, only a minimal subset of tags is implemented at this time.

## Objectives
* **Performance:** Takes advantage of modern C++ to achieve minimal memory usage, smooth execution and fast startup times. 
* **Compatibility:** Accurate parsing and execution of KAG/TyranoScript syntax.
* **Cross-platform:** Design to support multiple platforms.

## Dependencies
To build RaptorEngine, you will need:
* **Compiler:** A C++ compiler supporting C++23 (GCC, Clang, or MSVC)
* **Build System:** CMake 4.20+**
* **Dependencies:** 
  * SDL3 (Windowing and Graphics)
  * re2
  * 

## Supported Tags
Here is the exact list of the tags currently implemented:

| Tag           | Status / Description                                        |
|:--------------|:------------------------------------------------------------|
| `[call]`      | Jumps to the specified scenario file or label.              |
| `[return]`    | Returns to the caller from the subroutine                   |
| `[jump]`      | Jumps to the specified label in a designated scenario file. |
| `[macro]`     | Starts a macro definition.                                  |
| `[endmacro]`  | Ends the macro definition.                                  |
| `[layopt]`    | Changes settings for the specified layer.                   |
| `[position]`  | Specifies various attributes for the message window.        |
| `[chara_new]` | Set the info for a new character.                           |



*Any tag not listed above will log a warning.*
