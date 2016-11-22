cmake_minimum_required (VERSION 3.6)

project (display)
file(GLOB U8GLIB ../source/u8glib/*.c ./u8g_*.c)
add_executable (display main.c ${U8GLIB})

include_directories("../source/u8glib")
execute_process(COMMAND sdl-config --cflags OUTPUT_VARIABLE SDL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND sdl-config --libs OUTPUT_VARIABLE SDL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SDL_CFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SDL_LIBS}")
